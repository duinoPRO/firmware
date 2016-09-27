/*
 * XBeeWiFi-Example.ino is part of the duinoPRO firmware. 
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

/*
  XBeeWiFi-Example

  Sends binary data (in transparent mode) and a file (in API mode)
  to the Digi Device Cloud using serial port (UART) communication
  with an XBee S6B Wi-Fi RF unit connected to the XBee module.

  Module Used: XBee
  Author: SF
  Date: 18 December 2015
  */

// Include the necessary libraries
#include <duinoPRO.h>
#include <dP_XBeeWiFi.h>

// Define instances of the duinoPRO and the XBee module,
// located on board position 6/7 (the higher number is used as
// the argument)
duinoPRO myduinoPRO;
dP_XBeeWiFi myXBee(7);

void setup() {
  // Begin using the instance of the XBee module, with serial
  // port communication initialized at 9600 bits per second in
  // module 7 mode. Communication over the USB port is not
  // possible until this mode is changed, as done here at the
  // end of setup().
  myXBee.beginUart(9600);

  // Set the XBee unit's power level to 1 (its second lowest
  // level). Note: this setting is unrelated to the XBee
  // module's low-power mode.
  myXBee.powerLevel(1);

  // Disable API mode, in case it is enabled, and enter
  // transparent mode
  myXBee.apiEnable(false);

  // Connect to an access point by specifying its SSID below. If
  // the access point uses encryption, replace NO_SEC with WEP,
  // WPA or WPA2 as appropriate, and enter the security key as the
  // 3rd argument. Keys cannot include commas. If the connection
  // attempt takes longer than the number of seconds specified by
  // the 4th argument, the attempt ends and an error message is
  // displayed.
  myXBee.accessPointConnect("SSID",NO_SEC,"",20);

  // Reads the Device Cloud indicator, with the result to be
  // printed later. A result of 0 means that we are connected to
  // the Device Cloud.
  uint8_t cloudVal=myXBee.readCloudIndicator();

  // Pings an IP address, with the result to be printed later
  char pingBuffer[9];
  myXBee.pingIP("8.8.8.8",pingBuffer);

  // Enable sending binary data to the Digi Device Cloud
  myXBee.cloudDataEnable(true);

  // Send the numbers 1 to 20 to the Digi Device Cloud as
  // binary data.
  for (int i = 1; i <= 20; i++) {
    myXBee.sendVal(i);
  }

  // Disable sending binary data to the Digi Device Cloud
  myXBee.cloudDataEnable(false);

  // Enable API mode and write the plain text file "Example.txt",
  // containing the text "Example text" to the Digi Device Cloud,
  // overwriting the file if it already exists. Set the 3rd
  // argument of apiFileWrite() to true to append to an existing file
  // instead.
  myXBee.apiEnable(true);
  myXBee.apiFileWrite("Example.txt","Example text",false);

  // Disable API mode and disconnect from the access point
  myXBee.apiEnable(false);
  myXBee.accessPointDisconnect();

  // Switch the duinoPRO's serial port to debug mode
  myduinoPRO.serialDebugMode();
  // Print the result of the previous Device Cloud indicator read
  // and IP ping. cloudVal is an ASCII encoded digit, so it must be
  // displayed using Serial.write().
  Serial.println("Device Cloud indicator:");
  Serial.write(cloudVal);
  Serial.print("\n");
  Serial.println("IP ping response time:");
  Serial.println(pingBuffer);
}

void loop() {
  // Nothing happens after setup() is complete
}
