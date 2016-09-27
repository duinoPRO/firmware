/*
 * SpiI2c-S7SDisplay.ino is part of the duinoPRO firmware.
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
  SpiI2c-S7SDisplay

  Communicates with the Sparkfun COM-11440 7-Segment Serial (S7S)
  Display using the SPI/I2C module's SPI interface.

  Module Used: SPI/I2C
  Author: SF, KC
  Date: 8 September 2016
  */

#include <dP_SpiI2c.h>


dP_SpiI2c mySpiI2c(4);


void setup() {
  mySpiI2c.begin();
  mySpiI2c.setSpiFreq(250000); // 250 kHz SPI clock. The default SPI_MODE0 is used.
  delay(100); // The S7S requires this delay.

  // Clear display
  mySpiI2c.write8BitSpi(0x76);
  // Display "1234"
  mySpiI2c.write8BitSpi('1');
  mySpiI2c.write8BitSpi('2');
  mySpiI2c.write8BitSpi('3');
  mySpiI2c.write8BitSpi('4');
}

void loop() {
  // Flash display
  mySpiI2c.writeRegisterSpi(0x7A,255);
  delay(500);
  mySpiI2c.writeRegisterSpi(0x7A,0);
  delay(500);
}
