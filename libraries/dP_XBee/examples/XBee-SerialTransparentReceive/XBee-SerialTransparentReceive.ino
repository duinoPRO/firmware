/*
 * XBee-SerialTransparentReceive.ino is part of the duinoPRO firmware. 
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
  XBee-SerialTransparentReceive

  Receives data using serial port (UART) communication in
  transparent mode with an XBee S2C ZB RF unit connected to
  the XBee module.

  Module Used: XBee
  Author: SF, KC
  Date: 19 September 2016
  */

// Include the necessary libraries
#include <duinoPRO.h>
#include <dP_XBee.h>

// Define instances of the duinoPRO and the XBee module,
// located on board position 6/7
duinoPRO myduinoPRO;
dP_XBee myXBee(6, 7);

void setup() {
  // Begin using the instance of the XBee module, with serial
  // port communication initialized at 9600 bits per second in
  // module 7 mode. Communication over the USB port is not
  // possible until this mode is changed.
  myXBee.beginSerial(9600);

  // Disable API mode, in case it is enabled, and enter
  // transparent mode.
  myXBee.apiEnable(false);
}

void loop() {
  // Wait until there is incoming data to read
  while (myXBee.serialAvailable() == 0)
  {
  }

  // Allow time for data to be received past the first byte
  delay(50);

  // Read incoming characters and write them to the serial monitor
  // while there is incoming data to read
  while (myXBee.serialAvailable() > 0)
  {
      uint8_t byteIn = myXBee.readByte();
      myduinoPRO.serialDebugMode(); // Switch the duinoPRO's serial port to debug mode
      Serial.write(byteIn);
      delay(10); // Required for serial port output before switching modes
      myduinoPRO.serialModuleMode(); // Switch the serial port back to module 7 mode
  }

  // Print a newline to the serial monitor
  myduinoPRO.serialDebugMode();
  Serial.println();
  delay(10);
  myduinoPRO.serialModuleMode();
}
