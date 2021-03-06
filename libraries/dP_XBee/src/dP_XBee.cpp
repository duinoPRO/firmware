/*
 * dP_XBee.cpp is part of the duinoPRO firmware.
 *
 * duinoPRO is an Arduino™-compatible platform in a flat form factor with surface-mount,
 * solderable modules. It is designed with commercialization of real products in mind.
 * Note that we have designed duinoPRO to be compatible with the Arduino™ IDE.  This does
 * not imply that duinoPRO is certified, tested or endorsed by Arduino™ in any way.
 *
 * For more information, contact info@duinopro.cc or visit www.duinopro.cc.
 *
 * This file is licensed under the BSD 3-Clause license
 * (see https://github.com/duinoPRO/firmware/blob/master/duinoPRO_BSD_fwlicense.txt).
 *
 * Using duinoPRO core and libraries licensed under BSD for the firmware of a commercial
 * product does not require you to release the source code for the firmware.
 *
*/

#include <dP_XBee.h>
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

dP_XBee::dP_XBee(int id, int id2) : dP_Module(id, id2)
{
	comm = SERIAL_COMM;
};

void dP_XBee::begin()
{
    pin(SLEEP_PIN).mode(OUTPUT);
    pin(SLEEP_PIN).write(LOW);
}

void dP_XBee::beginSerial(unsigned long baud)
{
	begin();
	duinoPRO base;
	base.serialModuleMode(); // Will need to be updated to account for boards with multiple serial ports.
  //ser = &serial();
  //ser->begin(baud);
	serial().begin(baud);
}

void dP_XBee::beginSpi()
{
	begin();
	comm = SPI_COMM;
	//modSpi.spiSelect().mode(OUTPUT);
  //modSpi.spiSelect().write(HIGH);
	//modSpi.pin(SPI_ATTNZ_PIN).mode(INPUT);
	spiSelect().mode(OUTPUT);
	spiSelect().write(HIGH);
	pin(SPI_ATTNZ_PIN).mode(INPUT);

	rxPkt[0] = '\0';
	rxPktLen = 0;
}

/*HardwareSerial& dP_XBee::serial()
{
    // In a Due version of this, different serial ports would be returned, based on id.
    return Serial;
}*/

/*Pin& dP_XBee::spiSelectXBee()
{
    return modSpi.spiSelect();
};*/

void dP_XBee::lowpower(bool lowpower)
{
    pin(SLEEP_PIN).write(lowpower);
}

void dP_XBee::setupSpi()
{
	delay(1000);
	//ser->write("+++");
	serial().write("+++");
	delay(1000);
	waitForOK();

	//ser->write("ATD11\r");
	serial().write("ATD11\r");
	waitForOK();
	//ser->write("ATD21\r");
	serial().write("ATD21\r");
	waitForOK();
	//ser->write("ATD31\r");
	serial().write("ATD31\r");
	waitForOK();
	//ser->write("ATD41\r");
	serial().write("ATD41\r");
	waitForOK();
	//ser->write("ATP21\r");
	serial().write("ATP21\r");
	waitForOK();

	//ser->write("ATWR\r");
	serial().write("ATWR\r");
	waitForOK();
	//ser->write("ATAC\r");
	serial().write("ATAC\r");
	waitForOK();
	//ser->write("ATCN\r");
	serial().write("ATCN\r");
	waitForOK();
}

uint8_t dP_XBee::readByte()
{
	uint8_t val;

	if (comm == SERIAL_COMM)
	{
		//val = ser->read();
		val = serial().read();
	}
	else
	{
		spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
		//modSpi.spiSelect().write(LOW);
		spiSelect().write(LOW);
		val = spi().transfer(0xFF);
		//modSpi.spiSelect().write(HIGH);
		spiSelect().write(HIGH);
		spi().endTransaction();
	}

	return val;
}

uint16_t dP_XBee::serialAvailable()
{
	//return ser->available();
	return serial().available();
}

void dP_XBee::writeByte(uint8_t data)
{
	if (comm == SERIAL_COMM)
	{
		//ser->write(data);
		serial().write(data);
	}
	else
	{
		spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
		//modSpi.spiSelect().write(LOW);
		spiSelect().write(LOW);
		spi().transfer(data);
		//modSpi.spiSelect().write(HIGH);
		spiSelect().write(HIGH);
		spi().endTransaction();
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

	if (comm == SERIAL_COMM)
	{
		//pktAvailable = ser->available();
		pktAvailable = serial().available();
	}
	else
	{
		//pktAvailable = !modSpi.pin(SPI_ATTNZ_PIN).read();
		pktAvailable = !pin(SPI_ATTNZ_PIN).read();
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

void dP_XBee::sendPacket(char *txPkt, const uint8_t *destAddr)
{
	uint8_t i;
	uint8_t checksum;
	uint8_t txLen = strlen(txPkt);

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

//	ser->write(txStr);
	serial().write(txStr);
}

void dP_XBee::waitForOK()
{
//	while (ser->read() != 'O') {
	while(serial().read() != 'O') {
	}
//	while (ser->read() != 'K') {
	while(serial().read() != 'K') {
	}
//	while (ser->read() != '\r') {
	while(serial().read() != '\r') {
	}
}

void dP_XBee::ATCommand(char *command, char *param)
{
	delay(1000);
//	ser->write("+++");
	serial().write("+++");
	delay(1000);
	waitForOK();

//	ser->write("AT");
//	ser->write(command);
//	ser->write(param);
//	ser->write("\r");
	serial().write("AT");
	serial().write(command);
	serial().write(param);
	serial().write("\r");
	waitForOK();

	//ser->write("ATWR\r");
	serial().write("ATWR\r");
	waitForOK();
	serial().write("ATAC\r");
	//ser->write("ATAC\r");
	waitForOK();
	// Uncomment the line below if some commands don't seem to work; this is needed for the XBee S6B unit used with the dP_XBeeWiFi library.
	// delay(10);
	//ser->write("ATCN\r");
	serial().write("ATCN\r");
	waitForOK();
}

void dP_XBee::ATReadCommand(char *command, char *readArray, uint8_t readArrayLen)
{
	uint8_t len = 0;
	uint8_t data = 0;
	readArray[0] = 0;
	String str;

	delay(1000);
	//ser->write("+++");
	serial().write("+++");
	delay(1000);
	waitForOK();

	//ser->write("AT");
	//ser->write(command);
	//ser->write("\r");
	serial().write("AT");
	serial().write(command);
	serial().write("\r");
	//str = ser->readStringUntil('\r');
	str = serial().readStringUntil('\r');
	str.toCharArray(readArray, readArrayLen);

	//ser->write("ATCN\r");
	serial().write("ATCN\r");
	waitForOK();
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
