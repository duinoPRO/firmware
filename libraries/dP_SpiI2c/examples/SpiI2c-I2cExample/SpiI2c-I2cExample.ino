/*
 * SpiI2c-I2cExample.ino is part of the duinoPRO firmware.
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
  SpiI2c-I2cExample

  Communicates with the ST L3GD20 3-axis digital gyroscope using
  the SPI/I2C module's I2C interface to read its outputs.

  Module Used: SPI/I2C
  Author: SF
  Date: 17 December 2015
  */

// Include the required library
#include <dP_SpiI2c.h>

// Define an instance of the SPI/I2C module, located on board
// position 4
dP_SpiI2c mySpiI2c(4);

// Declare the gyroscope output variables
float gyroX, gyroY, gyroZ;

void setup() {
  // Begin using the instance of the SPI/I2C module
  mySpiI2c.begin();

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Define the gyroscope's I2C address
  mySpiI2c.setI2cAddress(0x6B);

  // Enable the gyroscope by writing the appropriate byte (0x0F) to the
  // appropriate register (address 0x20)
  mySpiI2c.writeRegisterI2c(0x20,0x0F);

  // Read the register with address 0x20
  uint8_t regByte = mySpiI2c.readRegisterI2c(0x20);
  Serial.print("Register byte: ");
  Serial.println(regByte,HEX);

  // Wait 1 second before continuing
  delay(1000);
}

void loop() {
  // Read and print the gyroscope outputs in units of dps
  gyroX = mySpiI2c.read16BitSignedI2c(0x29,0x28)*0.00875;
  gyroY = mySpiI2c.read16BitSignedI2c(0x2B,0x2A)*0.00875;
  gyroZ = mySpiI2c.read16BitSignedI2c(0x2D,0x2C)*0.00875;

  Serial.print("X: ");
  Serial.print(gyroX);
  Serial.print(" Y: ");
  Serial.print(gyroY);
  Serial.print(" Z: ");
  Serial.println(gyroZ);

  // Wait 200 ms before reading and printing the outputs again
  delay(200);
}
