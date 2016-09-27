/*
 * Xbee-SpiTransmit.ino is part of the duinoPRO firmware. 
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
  XBee-SpiTransmit

  Transmits data using SPI communication with an
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

// This 64-bit address is used to broadcast transmissions to
// all devices. To transmit to a specific device, use that
// device's 64-bit address instead.
uint8_t addr[] = {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF};

void setup() {
  // Begin using the instance of the XBee module, using
  // SPI communication.
  myXBee.beginSpi();
}

void loop() {
  // Transmit a packet consisting of the text "Test" to all devices
  // using SPI communication in API mode.
  myXBee.sendPacket("Test",addr);

  // Wait for a second before sending the packet again.
  delay(1000);
}
