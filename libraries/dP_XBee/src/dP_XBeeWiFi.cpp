
#include <dP_XBeeWiFi.h>
#include <SPI.h>
#include <duinoPRO.h>

// Module Pins
#define SLEEP_PIN 6

dP_XBeeWiFi::dP_XBeeWiFi(int id) : Module(id) 
{
};

// Could rename this method to begin if SPI is not implemented.
void dP_XBeeWiFi::beginUart(unsigned long baud)
{
	duinoPRO base;
	//base.uartModule7Mode(); // Will need to be updated to account for boards with multiple serial ports.
	ser = &serial();
	ser->begin(baud);
	ser->write("test2");
	serial().write("test");
	pin(SLEEP_PIN).mode(OUTPUT);
    pin(SLEEP_PIN).write(LOW);
}

void dP_XBeeWiFi::lowpower(bool lowpower)
{
    pin(SLEEP_PIN).write(lowpower);
}

HardwareSerial& dP_XBeeWiFi::serial()
{
    // In a Due version of this, different serial ports would be returned, based on id.
    return Serial;
}

void dP_XBeeWiFi::sendVal(uint8_t val)
{
	ser->write(val);
	delay(500);
}

void dP_XBeeWiFi::waitForOK()
{
	while (ser->read() != 'O') {
	}
	while (ser->read() != 'K') {
	}
	while (ser->read() != '\r') {
	}
}

void dP_XBeeWiFi::ATCommand(char *command, char *param)
{
	delay(1000);
	ser->write("+++");
	delay(1000);
	waitForOK();
	
	ser->write("AT");
	ser->write(command);
	ser->write(param);
	ser->write("\r");
	waitForOK();
	
	ser->write("ATWR\r");
	waitForOK();
	ser->write("ATAC\r");
	waitForOK();
	delay(10);	// Seems not to work properly for some commands if this isn't here.
	ser->write("ATCN\r");
	waitForOK();
}

void dP_XBeeWiFi::ATReadCommand(char *command, char *readarray, uint8_t readarraylen)
{
	uint8_t len = 0;
	uint8_t data = 0;
	readarray[0] = 0;

	delay(1000);
	ser->write("+++");
	delay(1000);
	waitForOK();
	
	ser->write("AT");
	ser->write(command);
	ser->write("\r");
	while (data != '\r' && len < readarraylen - 1) {
		while (ser->available() == 0) {
		}
		data = ser->read();
		if (data != '\r') {
			readarray[len] = (char)data;
			len++;
		}
	}
	readarray[len] = 0;
}

void dP_XBeeWiFi::cloudDataEnable(bool en)
{
	if (en)
	{
		ATCommand("DO","15");
	}
	else
	{
		ATCommand("DO","1");
	}
}

uint8_t dP_XBeeWiFi::readCloudIndicator(void)
{
	delay(1000);
	ser->write("+++");
	delay(1000);
	waitForOK();
	
	ser->write("ATDI\r");
	while (ser->available() == 0) {
	}
	uint8_t val = ser->read();
	while (ser->read() != '\r') {
	}
	ser->write("ATCN\r");
	waitForOK();
	return val;
}

// password cannot contain any commas.
void dP_XBeeWiFi::accessPointConnect(char *SSID, uint8_t security, char *password, uint8_t timeout)
{
	delay(1000);
	ser->write("+++");
	delay(1000);
	waitForOK();
	
	ser->write("ATDO\r"); // Enables Device Cloud if it is disabled.
	while (ser->available() == 0) {
	}
	if (ser->read() == '0') {
		while (ser->read() != '\r') {
		}
		ser->write("ATDO1\r");
		waitForOK();
	}
	else {
		while (ser->read() != '\r') {
		}
	}
	
	ser->write("ATID");
	ser->write(SSID);
	ser->write("\r");
	waitForOK();
	
	ser->write("ATEE");
	switch (security) {
		case NO_SEC:
			ser->write("0");
			break;
		case WPA:
			ser->write("1");
			break;
		case WPA2:
			ser->write("2");
			break;
		case WEP:
			ser->write("3");
			break;
		default:
			ser->write("0");
	}
	ser->write("\r");
	waitForOK();
	
	ser->write("ATPK");
	ser->write(password);
	ser->write("\r");
	waitForOK();
	
	ser->write("ATWR\r");
	waitForOK();
	ser->write("ATAC\r");
	waitForOK();
	ser->write("ATCN\r");
	waitForOK();	
	
	unsigned long starttime = millis();
	while (readCloudIndicator() != '0') { // Waits for connection to Device Cloud.
		if (millis() > starttime + timeout*1000) {
			duinoPRO base;
			delay(10);
			Serial.println("Error: Connection timeout.");
			delay(100);
			base.uartModule7Mode(); // Will need to be updated to account for boards with multiple serial ports.
			accessPointDisconnect(); // Ensures that a connection isn't made after the timeout error.
			break;
		}
	}
}

void dP_XBeeWiFi::accessPointDisconnect()
{
	delay(1000);
	ser->write("+++");
	delay(1000);
	waitForOK();
	
	ser->write("ATID");
	ser->write(NULL);
	ser->write("\r");
	waitForOK();
	
	ser->write("ATEE0\r");
	waitForOK();
	
	ser->write("ATPK");
	ser->write(NULL);
	ser->write("\r");
	waitForOK();
	
	ser->write("ATWR\r");
	waitForOK();
	ser->write("ATAC\r");
	waitForOK();
	ser->write("ATCN\r");
	waitForOK();	
}

// If sketch uploading fails with the XBee connected to the duinoPRO, or if the board keeps restarting with the XBee connected,
// try uploading a sketch that calls this method with an argument of 1 with the XBee disconnected, then connect the XBee.
void dP_XBeeWiFi::powerLevel(uint8_t plevel)
{
	if (plevel < 0 || plevel > 4) {
		plevel = 0;
	}
	char plevelchar[1] = {String(plevel)[0]};
	ATCommand("PL",plevelchar);
}

// Need a string buffer of at least 9 characters
void dP_XBeeWiFi::pingIP(char *IP, char *readarray)
{
	char command[strlen(IP)+2];
	command[0] = 'P';
	command[1] = 'G';
	for (int i = 2; i < strlen(IP)+2; i++) {
		command[i] = IP[i-2];
	}
	ATReadCommand(command,readarray,9);
}

void dP_XBeeWiFi::apiEnable(bool en)
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

void dP_XBeeWiFi::apiFileWrite(char *filename, char *data, bool append)
{
	uint8_t i = 0;
	for (int j = 0; j < strlen(filename); j++) {
		i = (i + (int)filename[j]) % 256;
	}
	for (int j = 0; j < strlen(data); j++) {
		i = (i + (int)data[j]) % 256;
	}	
	
	ser->write(0x7E);
	delay(100);
	ser->write(((16+strlen(filename)+strlen(data)) & 0xFF00) >> 2);
	delay(100);
	ser->write((16+strlen(filename)+strlen(data)) % 256);
	delay(100);
	ser->write(0x28);
	delay(100);
	ser->write(0x00);
	delay(100);
	ser->write(strlen(filename));
	delay(100);
	ser->write(filename);
	delay(100);
	ser->write(0x0A);
	delay(100);
	ser->write(0x74);
	delay(100);
	ser->write(0x65);
	delay(100);
	ser->write(0x78);
	delay(100);
	ser->write(0x74);
	delay(100);
	ser->write(0x2F);
	delay(100);
	ser->write(0x70);
	delay(100);
	ser->write(0x6C);
	delay(100);
	ser->write(0x61);
	delay(100);
	ser->write(0x69);
	delay(100);
	ser->write(0x6E);
	delay(100);
	ser->write(0x00);
	delay(100);
	if (append == 1) {
		ser->write(0x02);
	}
	else {
		ser->write(0x00);
	}
	delay(100);
	ser->write(data);
	delay(100);
	if (append == 1) {
		ser->write(0xFF-((0x3C+i+strlen(filename)) % 256));
	}
	else {
		ser->write(0xFF-((0x3A+i+strlen(filename)) % 256));
	}
	delay(100);
}