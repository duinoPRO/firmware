/*
 * XBee-SpiReceive.ino is part of the duinoPRO firmware.
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
  XBee-SpiReceive

  Receives data using SPI communication with an
  XBee S2C ZB RF unit connected to the XBee module. The
  XBee unit's SPI pins must be configured, e.g. by running
  the XBee-SetupSpiExample sketch.

  Module Used: XBee
  Author: SF, KC
  Date: 11 January 2016
  */

// Include the necessary libraries
#include <dP_XBee.h>


// Define an instance of the XBee module, located on
// board position 1/2 (the higher number is used as
// the argument)
dP_XBee myXBee(2);


void setup() {
  // Begin using the instance of the XBee module, using
  // SPI communication.
  myXBee.beginSpi();

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

void loop() {
  // Read the last received packet in API mode, and print it to
  // the serial monitor if it is a valid packet
  bool valPacket = myXBee.readPacket();
  if (valPacket)
  {
    Serial.println(myXBee.lastPacket());
  }

  // Wait for a second before receiving another packet
  delay(1000);
}
