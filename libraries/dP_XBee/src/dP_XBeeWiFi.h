#ifndef DP_XBEEWIFI_H
#define DP_XBEEWIFI_H

#include <Module.h>

#define NO_SEC 0
#define WPA 1
#define WPA2 2
#define WEP 3
 
class dP_XBeeWiFi : public Module {
  private:
	
  protected:
	
  public:
	dP_XBeeWiFi(int id);
	
	void beginSerial(unsigned long baud);

	void lowpower(bool lowpower);
	HardwareSerial& serial();

	void sendPacketSerial(char *packet);
	void sendPacketSerialRaw(uint8_t packet);
	
	void waitForOK(void);
	
	void ATCommand(char *command, char *param);
	void ATReadCommand(char *command, char *readarray, uint8_t readarraylen);
	
	void cloudDataEnable(void);
	void cloudDataDisable(void);
	uint8_t readCloudIndicator(void);
	
	void accessPointConnect(char *SSID, uint8_t security, char *password);
	void accessPointDisconnect();
	
	void powerLevel(uint8_t plevel);
	
	void pingIP(char *IP, char *readarray, uint8_t readarraylen);
	
	void apiEnable();
	void apiDisable();
	void apiFileWrite(char *filename, char *data, bool append);
	
};

#endif /* DP_XBEEWIFI_H */