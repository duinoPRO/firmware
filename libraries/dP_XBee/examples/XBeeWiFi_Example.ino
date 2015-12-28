/*
  XBeeWiFi_Example

  Sends binary data (in transparent mode) and a file (in API mode)
  to the Digi Device Cloud using the XBee S6B Wi-Fi RF module and
  the duinoPRO XBee module.

  Module Used: XBee
  Author: SF
  Date: 18 December 2015
  */

// Include the necessary libraries
#include <duinoPRO.h>
#include <dP_XBeeWiFi.h>

// Define instances of the XBee module and the duinoPRO
dP_XBeeWiFi MyXBee(7);
duinoPRO myduinoPRO;

void setup() {
  // Begin using the instance of the XBee module, with serial
  // port communication initialized at 9600 bits per second in
  // module 7 mode. Communication over the USB port is not
  // possible until this mode is changed, as done here at the
  // end of setup().
  MyXBee.beginSerial(9600);

  // Set the XBee's internal power level to 1 (its second lowest
  // level). Note: this setting is unrelated to the duinoPRO
  // module's low-power mode.
  MyXBee.powerLevel(1);

  // Disable API mode, in case it is enabled, and enter
  // transparent mode
  MyXBee.apiDisable();

  // Connect to an access point by specifying its SSID below. If
  // the access point uses encryption, replace NO_SEC with WEP,
  // WPA or WPA2 as appropriate, and enter the security key as the
  // 3rd argument. Keys cannot include commas, and ASCII keys should
  // be placed within double quotation marks. If the connection
  // attempt takes longer than the number of seconds specified by
  // the 4th argument, the attempt ends and an error message is
  // displayed.
  MyXBee.accessPointConnect("SSID",NO_SEC,"",20);

  // Reads the Device Cloud indicator, with the result to be
  // printed later. A result of 0 means that we are connected to
  // the Device Cloud.
  uint8_t cloudVal=MyXBee.readCloudIndicator();
  
  // Pings an IP address, with the result to be printed later
  char pingbuffer[9];
  MyXBee.pingIP("8.8.8.8",pingbuffer);

  // Enable sending binary data to the Digi Device Cloud
  MyXBee.cloudDataEnable();

  // Send the numbers 1 to 20 to the Digi Device Cloud as
  // binary data.
  for (int i = 1; i <= 20; i++) {
    MyXBee.sendPacketSerial(i);
  }

  // Disable sending binary data to the Digi Device Cloud
  MyXBee.cloudDataDisable();

  // Enable API mode and write the plain text file "Example.txt",
  // containing the text "Example text" to the Digi Device Cloud,
  // overwriting the file if it already exists. Set the 3rd
  // argument of apiFileWrite() to 1 to append to an existing file
  // instead.
  MyXBee.apiEnable();
  MyXBee.apiFileWrite("Example.txt","Example text",0);

  // Disable API mode and disconnect from the access point
  MyXBee.apiDisable();

  // Disconnect from the access point
  MyXBee.accessPointDisconnect();

  // Switch the duinoPRO's serial port to USB mode
  myduinoPRO.uartUsbMode();
  // Print the result of the previous Device Cloud indicator read
  // and IP ping. cloudVal is an ASCII encoded digit, so it must be
  // displayed using Serial.write().
  Serial.println("Device Cloud indicator:");
  Serial.write(cloudVal);
  Serial.print("\n");
  Serial.println("IP ping response time:");
  Serial.println(pingbuffer);  
}

void loop() {
  // Nothing happens after setup() is complete
}

