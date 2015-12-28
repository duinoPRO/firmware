
#include <dP_Xbee.h>
#include <SPI.h>

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

dP_Xbee::dP_Xbee(int id) : Module(id) 
{
};

void dP_Xbee::beginUart(unsigned long baud)
{
	begin();
    HardwareSerial& s = serial();
    // s.begin(baud);
    serial().begin(baud);
}


void dP_Xbee::begin()
{
	spiSelect().mode(OUTPUT);
    spiSelect().write(HIGH);
	
    pin(SLEEP_PIN).mode(OUTPUT);
    pin(SLEEP_PIN).write(LOW);
	
	pin(SPI_ATTNZ_PIN).mode(INPUT);

	rxPkt[0] = '\0';
	rxPktLen = 0;
}


void dP_Xbee::lowpower(bool lowpower)
{
    pin(SLEEP_PIN).write(lowpower);
}

HardwareSerial& dP_Xbee::serial()
{
    // In a Due version of this, different serial ports would be returned, based on id.
    return Serial;
}


void dP_Xbee::beginSpi()
{

}

uint8_t dP_Xbee::spiReadByte()
{
	uint8_t val;
	SPI.beginTransaction(SPISettings(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE));
	spiSelect().write(LOW);
	val = SPI.transfer(0xFF);
	spiSelect().write(HIGH);
	SPI.endTransaction();
	return val;
}

uint8_t dP_Xbee::spiWriteByte(uint8_t data)
{
	uint8_t val;
	SPI.beginTransaction(SPISettings(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE));
	spiSelect().write(LOW);
	val = SPI.transfer(data);
	spiSelect().write(HIGH);
	SPI.endTransaction();
	return val;
}

bool dP_Xbee::readPacket()
{
	uint8_t data;
	uint16_t byteCount = 0;
	uint16_t pktLen;
	uint16_t dataLen;
	uint8_t checksum = 0;
	
	rxPkt[0] = '\0';
	rxPktLen = 0;
	
	#ifdef XBEE_DEBUG
	// Serial.println("dP_Xbee::readPacket()");
	#endif
	
	
	while (!pin(1).read())
	{
		data = spiReadByte();
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

char *dP_Xbee::lastPacket()
{
	return rxPkt;
}

void dP_Xbee::sendPacket(char *txPkt, uint8_t txLen, const uint8_t *destAddr)
{
	uint8_t i;
	uint8_t checksum;
	
	checksum = 0;
		
	spiWriteByte(FRAME_DELIMITER);
	// LENGTH_MSB
	spiWriteByte(0x00);
	// LENGTH_LSB
	spiWriteByte(txLen + TX_FRAME_OVERHEADS);

	// FRAME_TYPE
	spiWriteByte(FRAME_TYPE_TX);
	checksum += FRAME_TYPE_TX;
	// TRANSMIT_FRAME_ID
	spiWriteByte(0x00);		// No response requested
	
	// TRANSMIT_DEST_ADDR_64
	for (i=0; i<LONG_ADDR_LEN; i++)
	{
		spiWriteByte(destAddr[i]);
		checksum += destAddr[i];
	}
	// TRANSMIT_DEST_ADDR_16
	// 0xFFFE - unknown or broadcast address
	spiWriteByte(0xFF);
	checksum +=0xFF;
	spiWriteByte(0xFE);	
	checksum += 0xFE;
	
	// TRANSMIT_BROADCAST_RADIUS
	spiWriteByte(0x00);		// maximum hops
	// TRANSMIT_OPTIONS
	spiWriteByte(0x00);		// no options	
	// TRANSMIT_DATA

	for (i=0; i<txLen; i++)
	{
		spiWriteByte(txPkt[i]);
		checksum += txPkt[i];
	}
	// Checksum
	spiWriteByte(0xFF - checksum);
}
