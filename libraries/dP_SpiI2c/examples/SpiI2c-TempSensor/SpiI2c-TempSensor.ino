/*
 * SpiI2c-TempSensor.ino is part of the duinoPRO firmware.
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
  SpiI2c-TempSensor

  Communicates with the TI TMP102 digital temperature sensor using
  the SPI/I2C module's I2C interface to read its outputs.

  Module Used: SPI/I2C
  Author: KC
  Date: 29  August 2016
  */

// Include the required library/s
#include <dP_SpiI2c.h>


#define TMP102_I2C_ADDRESS  0x48 // GND ADD0 pin on TMP102 for this address (tie to 3.3V for address 0x49)


// Define an instance of the SPI/I2C module, located on board
// position 6
dP_SpiI2c myTempSensor(6);


// Declare the temperature sensor output variable
float temp;


void setup() {
  // Begin using the instance of the SPI/I2C module
  myTempSensor.begin();

  // FOR DEBUG
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Define the temperature sensor's I2C address
  myTempSensor.setI2cAddress(TMP102_I2C_ADDRESS);
}


void loop() {
  // Read and print the temperature sensor output in units of degrees celcius
  //temp = myTempSensor.read16BitSignedI2c(0x00, 0x01)*0.0625;

 /* Wire.requestFrom(TMP102_I2C_ADDRESS, 2);

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; */
  byte MSB = myTempSensor.readRegisterI2c(TMP102_I2C_ADDRESS);
  byte LSB = myTempSensor.readRegisterI2c(TMP102_I2C_ADDRESS + 1);

  temp = (((MSB << 8) | LSB) >> 4)*0.0625;

  // DEBUG
  Serial.print("\nTemperature: ");
  Serial.print(temp);

  // Wait 2s before reading and printing the output again
  delay(2000);
}
