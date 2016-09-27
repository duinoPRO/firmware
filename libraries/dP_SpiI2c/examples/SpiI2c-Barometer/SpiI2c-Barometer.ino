/*
 * SpiI2c-Barometer.ino is part of the duinoPRO firmware.
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
  SpiI2c-Barometer

  Communicates with the Freescale Semiconductor MPL115A1 Miniature
  SPI Digital Barometer barometer using the SPI/I2C module's SPI
  interface to read its outputs.

  Module Used: SPI/I2C
  Author: SF, KC
  Date: 15 December 2015
  */

#include <dP_SpiI2c.h>
#include <math.h>


dP_SpiI2c mySpiI2c(4);


void setup() {
  mySpiI2c.begin();
  mySpiI2c.setSpiFreq(6000000); // 6 MHz SPI clock (the duinoPRO UNO's maximum speed; the barometer's maximum speed is higher). The default SPI_MODE0 is used.
  mySpiI2c.setSpiRWMasks(0x80,0x00); // Read/write bit is high in read mode.

  Serial.begin(9600);
}

void loop() {
  // The addresses here occupy the upper 7 bits, i.e. the effective address is multiplied by 2 compared with the addresses given in the datasheet.
  mySpiI2c.writeRegister(0x24,0x00);
  delay(3); // Barometer conversion time

  int16_t Padc = mySpiI2c.read16Bit(0x00,0x02) >> 6;
  int16_t Tadc = mySpiI2c.read16Bit(0x04,0x06) >> 6;
  int16_t a0i = mySpiI2c.read16BitSigned(0x08,0x0A);
  int16_t b1i = mySpiI2c.read16BitSigned(0x0C,0x0E);
  int16_t b2i = mySpiI2c.read16BitSigned(0x10,0x12);
  int16_t c12i = mySpiI2c.read16BitSigned(0x14,0x16);

  float a0 = a0i/pow(2,3);
  float b1 = b1i/pow(2,13);
  float b2 = b2i/pow(2,14);
  float c12 = c12i/pow(2,24);

  float Pcomp = a0 + (b1 + c12 * Tadc) * Padc + b2 * Tadc;
  float pressure = Pcomp*((115.0-50.0)/1023.0)+50.0;

  Serial.print("Pressure = ");
  Serial.print(pressure,10);
  Serial.println(" kPa");
}
