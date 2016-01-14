
#include <dP_XBee.h>
#include <SPI.h>
#include <duinoPRO.h>

// #define XBEE_DEBUG 1

// Module Pins
#define SPI_ATTNZ_PIN 1
#define SLEEP_PIN 6

// SPI Port Characteristics
#define MY_SPEED_MAX 	(4000000)
#define MY_DATA_ORDER  	(MSBFIRST)
#define MY_DATA_MODE	(SPI_MODE0)


#define FRAME_DELIMITER  (0x7E)

#define LONG_ADDR_LEN	(8)
#define SHORT_ADDR_LEN	(2)

#define FRAME_TYPE_TX	(0x10)
#define FRAME_TYPE_RX	(0x90)

// Field locations in API packets
#define LENGTH_MSB		 (1)
#define LENGTH_LSB		 (2)
#define FRAME_TYPE		 (3)

// Field locations in receive API packets
#define RECEIVE_SOURCE_ADDR_64	 (4)
#define RECEIVE_SOURCE_ADDR_16	 (12)
#define RECEIVE_OPTIONS	 		 (14)
#define RECEIVE_DATA	 		 (15)	

#define RX_FRAME_OVERHEADS (RECEIVE_DATA - FRAME_TYPE)


// Field locations in transmit API packets
#define TRANSMIT_FRAME_ID		 	(4)
#define TRANSMIT_DEST_ADDR_64	 	(5)
#define TRANSMIT_DEST_ADDR_16	 	(13)
#define TRANSMIT_BROADCAST_RADIUS	(15)
#define TRANSMIT_OPTIONS 		 	(16)	
#define TRANSMIT_DATA		 		(17)

#define TX_FRAME_OVERHEADS (TRANSMIT_DATA - FRAME_TYPE)

dP_XBee::dP_XBee(int id) : Module(id) 
{
	comm = UART_COMM;
	modSpiID = max(1,id-1); // Ensures that modIDSpi is positive
};

void dP_XBee::begin()
{
    pin(SLEEP_PIN).mode(OUTPUT);
    pin(SLEEP_PIN).write(LOW);
}

void dP_XBee::beginUart(unsigned long baud)
{
	begin();
	duinoPRO base;
	base.uartModule7Mode(); // Will need to be updated to account for boards with multiple serial ports.
    s = &serial();
    s->begin(baud);
}

void dP_XBee::beginSpi()
{
	begin();
	modSpi = &getModSpi(); // modSpi is used for the SPI_CSZ and SPI_ATTNZ pins
	comm = SPI_COMM;
	modSpi->spiSelect().mode(OUTPUT);
    modSpi->spiSelect().write(HIGH);
	modSpi->pin(SPI_ATTNZ_PIN).mode(INPUT);
	
	rxPkt[0] = '\0';
	rxPktLen = 0;
}

HardwareSerial& dP_XBee::serial()
{
    // In a Due version of this, different serial ports would be returned, based on id.
    return Serial;
}

Module& dP_XBee::getModSpi()
{
	Module modSpi(modSpiID);
    return modSpi;
}

void dP_XBee::lowpower(bool lowpower)
{
    pin(SLEEP_PIN).write(lowpower);
}

void dP_XBee::setupSpi()
{
	delay(1000);
	s->write("+++");
	delay(1000);
	waitForOK();
	
	s->write("ATD11\r");
	waitForOK();
	s->write("ATD21\r");
	waitForOK();
	s->write("ATD31\r");
	waitForOK();
	s->write("ATD41\r");
	waitForOK();
	s->write("ATP21\r");
	waitForOK();
	
	s->write("ATWR\r");
	waitForOK();
	s->write("ATAC\r");
	waitForOK();
	s->write("ATCN\r");
	waitForOK();
}

uint8_t dP_XBee::readByte()
{
	uint8_t val;
	
	if (comm == UART_COMM)
	{
		val = s->read();
	}
	else
	{
		SPI.beginTransaction(SPISettings(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE));
		modSpi->spiSelect().write(LOW);
		val = SPI.transfer(0xFF);
		modSpi->spiSelect().write(HIGH);
		SPI.endTransaction();
	}
	
	return val;
}
	
uint16_t dP_XBee::serialAvailable()
{
	return s->available();
}

void dP_XBee::writeByte(uint8_t data)
{
	if (comm == UART_COMM)
	{
		s->write(data);
	}
	else
	{
		SPI.beginTransaction(SPISettings(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE));
		modSpi->spiSelect().write(LOW);
		SPI.transfer(data);
		modSpi->spiSelect().write(HIGH);
		SPI.endTransaction();
	}
}

bool dP_XBee::readPacket()
{
	uint8_t data;
	uint16_t byteCount = 0;
	uint16_t pktLen;
	uint16_t dataLen;
	uint8_t checksum = 0;
	bool pktAvailable;
	
	rxPkt[0] = '\0';
	rxPktLen = 0;
	
	#ifdef XBEE_DEBUG
	// Serial.println("dP_XBee::readPacket()");
	#endif
	
	if (comm == UART_COMM)
	{
		pktAvailable = s->available();
	}
	else
	{
		pktAvailable = !modSpi->pin(SPI_ATTNZ_PIN).read();
	}
	
	while (pktAvailable)
	{
		data = readByte();
		#ifdef XBEE_DEBUG
		Serial.print("data: ");
		Serial.println(data, HEX);
		#endif

		if ((byteCount == 0) && (data != FRAME_DELIMITER))
		{
			continue;
		}

		if (byteCount == LENGTH_MSB)
		{
			pktLen = (data << 8);
		}
		if (byteCount == LENGTH_LSB)
		{
			pktLen += data;
			dataLen = pktLen - RX_FRAME_OVERHEADS;
			#ifdef XBEE_DEBUG
			Serial.print("packet length: ");
			Serial.println(pktLen);
			#endif
			if (dataLen > XBEE_MAX_PACKET_SIZE)
			{
				// This is a larger packet than we're prepared to handle, so drop it
				#ifdef XBEE_DEBUG
				Serial.println("Packet too long");
				#endif
				return false;
			}
		}
		if( byteCount > LENGTH_LSB)
		{
			checksum += data;
		}
		if (byteCount == FRAME_TYPE)
		{
			if (data != FRAME_TYPE_RX)
			{
				#ifdef XBEE_DEBUG
				Serial.println("Incorrect Frame Type");
				#endif
				return false;		// We are not handling packet types other than zigbee receive frames
			}
		}
		if (byteCount >= RECEIVE_DATA)
		{
			#ifdef XBEE_DEBUG
			Serial.print("byteCount: ");
			Serial.println(byteCount);
			#endif		
			if (rxPktLen == dataLen)
			{
				rxPkt[rxPktLen] = '\0';
				// check the checksum
				if (checksum == 0xFF)
				{
					#ifdef XBEE_DEBUG
					Serial.println("Valid packet received");
					#endif
					return true;
				}
				else
				{
					#ifdef XBEE_DEBUG
					Serial.print("Incorrect Checksum - calculated: ");
					Serial.println(checksum, HEX);
					#endif
					return false;
				}
			}
			else
			{
				rxPkt[rxPktLen] = data;
				rxPktLen++;
			}
		}
		
		byteCount++;
	}
	return false;
}

char *dP_XBee::lastPacket()
{
	return rxPkt;
}

void dP_XBee::sendPacket(char *txPkt, uint8_t txLen, const uint8_t *destAddr)
{
	uint8_t i;
	uint8_t checksum;
	
	checksum = 0;
		
	writeByte(FRAME_DELIMITER);
	// LENGTH_MSB
	writeByte(0x00);
	// LENGTH_LSB
	writeByte(txLen + TX_FRAME_OVERHEADS);

	// FRAME_TYPE
	writeByte(FRAME_TYPE_TX);
	checksum += FRAME_TYPE_TX;
	// TRANSMIT_FRAME_ID
	writeByte(0x00);		// No response requested
	
	// TRANSMIT_DEST_ADDR_64
	for (i=0; i<LONG_ADDR_LEN; i++)
	{
		writeByte(destAddr[i]);
		checksum += destAddr[i];
	}
	// TRANSMIT_DEST_ADDR_16
	// 0xFFFE - unknown or broadcast address
	writeByte(0xFF);
	checksum +=0xFF;
	writeByte(0xFE);	
	checksum += 0xFE;
	
	// TRANSMIT_BROADCAST_RADIUS
	writeByte(0x00);		// maximum hops
	// TRANSMIT_OPTIONS
	writeByte(0x00);		// no options	
	// TRANSMIT_DATA

	for (i=0; i<txLen; i++)
	{
		writeByte(txPkt[i]);
		checksum += txPkt[i];
	}
	// Checksum
	writeByte(0xFF - checksum);
}

void dP_XBee::sendString(char *txStr)
{
	uint8_t i;
	
	s->write(txStr);
}

void dP_XBee::waitForOK()
{
	while (s->read() != 0x4F) {
	}
	while (s->read() != 0x4B) {
	}
	while (s->read() != 0x0D) {
	}
}

void dP_XBee::ATCommand(char *command, char *param)
{
	delay(1000);
	s->write("+++");
	delay(1000);
	waitForOK();
	
	s->write("AT");
	s->write(command);
	s->write(param);
	s->write("\r");
	waitForOK();
	
	s->write("ATWR\r");
	waitForOK();
	s->write("ATAC\r");
	waitForOK();
	// Uncomment the line below if some commands don't seem to work; this is needed for the XBee S6B unit.
	// delay(10);
	s->write("ATCN\r");
	waitForOK();
}

void dP_XBee::ATReadCommand(char *command, char *readarray, uint8_t readarraylen)
{
	uint8_t len = 0;
	uint8_t data = 0;
	readarray[0] = 0;

	delay(1000);
	s->write("+++");
	delay(1000);
	waitForOK();
	
	s->write("AT");
	s->write(command);
	s->write("\r");
	while (data != 0x0D && len < readarraylen - 1) {
		while (s->available() == 0) {
		}
		data = s->read();
		if (data != 0x0D) {
			readarray[len] = (char)data;
			len++;
		}
	}
	readarray[len] = 0;
}

void dP_XBee::apiEnable(bool en)
{
	if (en)
	{
		ATCommand("AP","1");
	}
	else
	{
		ATCommand("AP","0");
	}
}