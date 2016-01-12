#ifndef DP_XBEE_H
#define DP_XBEE_H

#include <Module.h>

#define XBEE_MAX_PACKET_SIZE	(20)

enum CommEnum {
	UART_COMM,
	SPI_COMM,
};
 
class dP_XBee : public Module {
  private:
	char rxPkt[XBEE_MAX_PACKET_SIZE+1];
	uint8_t rxPktLen;
	uint8_t comm;
	uint8_t modSpiID;
	HardwareSerial *s;
	Module *modSpi;
	
  protected:
	
  public:
    const uint8_t coordinatorAddress[8] = { 0,0,0,0,0,0,0,0 };
    const uint8_t broadcastAddress[8] = { 0,0,0,0,0,0,0xFF,0xFF }; 
  
	dP_XBee(int id);
	
	void begin(void);
	void beginUart(unsigned long baud);
	void beginSpi(void);
	
	HardwareSerial& serial(void);
	Module& getModSpi(void);

	void lowpower(bool lowpower);
	
	void setupSpi(void);
	
	uint8_t readByte(void);
	uint16_t serialAvailable(void);
	void writeByte(uint8_t data);
	
	bool readPacket(void);
	char *lastPacket(void);
	
	void sendPacket(char *txPkt, uint8_t txLen, const uint8_t *destAddr);
	void sendString(char *txPkt);
	
	/** \brief Wait for an "OK" message from the XBee unit.
	 */
	void waitForOK(void);		
	/** \brief Send an AT command (with parameter) to the XBee unit.
	 *  \param command - The command to be sent.
	 *  \param param - The command parameter to be sent.
	 */
	void ATCommand(char *command, char *param);
	/** \brief Read the XBee unit's stored parameter corresponding to the given AT command.
	 *  \param command - The AT command.
	 *  \param readarray - The XBee unit's stored parameter.
	 *  \param readarraylen - The length of the readarray string buffer.
	 */
	void ATReadCommand(char *command, char *readarray, uint8_t readarraylen);
	
	/** \brief Enable or disable API mode, and respectively disable or enable transparent mode.
	 *  Applies only to UART communication; SPI communication uses API mode only.
	 *  \param en - true to enable API mode, false to disable.
	 */
	void apiEnable(bool en);
};

#endif /* DP_XBEE_H */