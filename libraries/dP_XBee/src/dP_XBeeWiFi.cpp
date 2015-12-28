
#include <dP_XBeeWiFi.h>
#include <SPI.h>
#include <duinoPRO.h>

// Module Pins
#define SLEEP_PIN 6

dP_XBeeWiFi::dP_XBeeWiFi(int id) : Module(id) 
{
};

// Could rename this method to begin if SPI is not implemented.
void dP_XBeeWiFi::beginSerial(unsigned long baud)
{
	duinoPRO base;
	base.uartModule7Mode(); // Will need to be updated to account for boards with multiple serial ports.
	HardwareSerial& s = serial();
	serial().begin(baud);
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

void dP_XBeeWiFi::sendPacketSerial(char *packet)
{
	serial().write(packet);
	delay(500);
}

void dP_XBeeWiFi::sendPacketSerialRaw(uint8_t packet)
{
	serial().print(packet);
	delay(500);
}

void dP_XBeeWiFi::waitForOK()
{
	while (serial().read() != 0x4F) {
	}
	while (serial().read() != 0x4B) {
	}
	while (serial().read() != 0x0D) {
	}
}

void dP_XBeeWiFi::ATCommand(char *command, char *param)
{
	delay(1000);
	serial().write("+++");
	delay(1000);
	waitForOK();
	
	serial().write("AT");
	serial().write(command);
	serial().write(param);
	serial().write("\r");
	waitForOK();
	
	serial().write("ATWR\r");
	waitForOK();
	serial().write("ATAC\r");
	waitForOK();
	delay(10);	// Seems not to work properly for some commands if this isn't here.
	serial().write("ATCN\r");
	waitForOK();
}

void dP_XBeeWiFi::ATReadCommand(char *command, char *readarray, uint8_t readarraylen)
{
	uint8_t len = 0;
	uint8_t data = 0;
	char temp[1] = {""};
	*readarray = temp[0];

	delay(1000);
	serial().write("+++");
	delay(1000);
	waitForOK();
	
	serial().write("AT");
	serial().write(command);
	serial().write("\r");
	while (data != 0x0D && len < readarraylen - 1) {
		while (serial().available() == 0) {
		}
		data = serial().read();
		if (data != 0x0D) {
			readarray[len] = (char)data;
			len++;
		}
	}
	readarray[len] = '\0';
}

void dP_XBeeWiFi::cloudDataEnable()
{
	ATCommand("DO","15");
}

void dP_XBeeWiFi::cloudDataDisable()
{
	ATCommand("DO","1");
}

uint8_t dP_XBeeWiFi::readCloudIndicator(void)
{
	delay(1000);
	serial().write("+++");
	delay(1000);
	waitForOK();
	
	serial().write("ATDI\r");
	while (serial().available() == 0) {
	}
	uint8_t val = serial().read();
	while (serial().read() != 0x0D) {
	}
	serial().write("ATCN\r");
	waitForOK();
	return val;
}

// password cannot contain any commas.
void dP_XBeeWiFi::accessPointConnect(char *SSID, uint8_t security, char *password)
{
	delay(1000);
	serial().write("+++");
	delay(1000);
	waitForOK();
	
	serial().write("ATDO\r"); // Enables Device Cloud if it is disabled.
	while (serial().available() == 0) {
	}
	if (serial().read() == 0x30) {
		while (serial().read() != 0x0D) {
		}
		serial().write("ATDO1\r");
		waitForOK();
	}
	else {
		while (serial().read() != 0x0D) {
		}
	}
	
	serial().write("ATID");
	serial().write(SSID);
	serial().write("\r");
	waitForOK();
	
	serial().write("ATEE");
	switch (security) {
		case WPA:
			serial().write("1");
			break;
		case WPA2:
			serial().write("2");
			break;
		case WEP:
			serial().write("3");
			break;
		default:
			serial().write("0");
	}
	serial().write("\r");
	waitForOK();
	
	serial().write("ATPK");
	serial().write(password);
	serial().write("\r");
	waitForOK();
	
	serial().write("ATWR\r");
	waitForOK();
	serial().write("ATAC\r");
	waitForOK();
	serial().write("ATCN\r");
	waitForOK();	
	
	unsigned long starttime = millis();
	while (readCloudIndicator() != 0x30) { // Waits for connection to Device Cloud.
		if (millis() > starttime + 20000) {
			duinoPRO base;
			delay(10);
			Serial.println("Error: Connection timeout.");
			delay(100);
			base.uartModule7Mode();
			break;
		}
	}
}

// Is there a better way to effectively disconnect from all access points?
void dP_XBeeWiFi::accessPointDisconnect()
{
	delay(1000);
	serial().write("+++");
	delay(1000);
	waitForOK();
	
	serial().write("ATID");
	serial().write(NULL);
	serial().write("\r");
	waitForOK();
	
	serial().write("ATEE0\r");
	waitForOK();
	
	serial().write("ATPK");
	serial().write(NULL);
	serial().write("\r");
	waitForOK();
	
	serial().write("ATWR\r");
	waitForOK();
	serial().write("ATAC\r");
	waitForOK();
	serial().write("ATCN\r");
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

void dP_XBeeWiFi::pingIP(char *IP, char *readarray, uint8_t readarraylen)
{
	char command[strlen(IP)+2];
	command[0] = 'P';
	command[1] = 'G';
	for (int i = 2; i < strlen(IP)+2; i++) {
		command[i] = IP[i-2];
	}
	ATReadCommand(command,readarray,readarraylen);
}

void dP_XBeeWiFi::apiEnable()
{
	ATCommand("AP","1");
}

void dP_XBeeWiFi::apiDisable()
{
	ATCommand("AP","0");
}

// Creates plain text file, either overwriting or appending if file already exists. filename and data can be up to 255 and 1400 bytes long respectively.
void dP_XBeeWiFi::apiFileWrite(char *filename, char *data, bool append)
{
	uint8_t i = 0;
	for (int j = 0; j < strlen(filename); j++) {
		i = (i + (int)filename[j]) % 256;
	}
	for (int j = 0; j < strlen(data); j++) {
		i = (i + (int)data[j]) % 256;
	}	
	
	serial().write(0x7E);
	delay(100);
	serial().write(((16+strlen(filename)+strlen(data)) & 0xFF00) >> 2);
	delay(100);
	serial().write((16+strlen(filename)+strlen(data)) % 256);
	delay(100);
	serial().write(0x28);
	delay(100);
	serial().write(0x00);
	delay(100);
	serial().write(strlen(filename));
	delay(100);
	serial().write(filename);
	delay(100);
	serial().write(0x0A);
	delay(100);
	serial().write(0x74);
	delay(100);
	serial().write(0x65);
	delay(100);
	serial().write(0x78);
	delay(100);
	serial().write(0x74);
	delay(100);
	serial().write(0x2F);
	delay(100);
	serial().write(0x70);
	delay(100);
	serial().write(0x6C);
	delay(100);
	serial().write(0x61);
	delay(100);
	serial().write(0x69);
	delay(100);
	serial().write(0x6E);
	delay(100);
	serial().write(0x00);
	delay(100);
	if (append == 1) {
		serial().write(0x02);
	}
	else {
		serial().write(0x00);
	}
	delay(100);
	serial().write(data);
	delay(100);
	if (append == 1) {
		serial().write(0xFF-((0x3C+i+strlen(filename)) % 256));
	}
	else {
		serial().write(0xFF-((0x3A+i+strlen(filename)) % 256));
	}
	delay(100);
}