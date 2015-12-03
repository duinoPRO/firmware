#ifndef DP_XBEE_H
#define DP_XBEE_H

#include <Module.h>

#define XBEE_MAX_PACKET_SIZE	(20)
 
class dP_XBee : public Module {
  private:
	char rxPkt[XBEE_MAX_PACKET_SIZE+1];
	uint8_t rxPktLen;
	
  protected:
	
  public:
    const uint8_t coordinatorAddress[8] = { 0,0,0,0,0,0,0,0 };
    const uint8_t broadcastAddress[8] = { 0,0,0,0,0,0,0xFF,0xFF }; 
  
	dP_XBee(int id);
	
	void beginUart(unsigned long baud);
	void begin(void);

	void lowpower(bool lowpower);
	HardwareSerial& serial();
	
	void beginSpi();	
	uint8_t spiReadByte();
	
	uint8_t spiWriteByte(uint8_t data);
	
	bool readPacket();
	char *lastPacket();
	
	void sendPacket(char *txPkt, uint8_t txLen, const uint8_t *destAddr);
};

#endif /* DP_XBEE_H */
