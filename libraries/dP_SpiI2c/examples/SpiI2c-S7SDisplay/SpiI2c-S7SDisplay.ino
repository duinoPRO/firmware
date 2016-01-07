/*
  SpiI2c-S7SDisplay

  Communicates with the Sparkfun COM-11440 7-Segment Serial (S7S)
  Display using the SPI/I2C module's SPI interface.

  Module Used: SPI/I2C
  Author: SF
  Date: 15 December 2015
  */

#include <dP_SpiI2c.h>
#include <SPI.h>

dP_SpiI2c mySpiI2c(2);

void setup() {
  SPI.begin();
  mySpiI2c.begin();
  mySpiI2c.setSpiFreq(250000); // 250 kHz SPI clock. The default SPI_MODE0 is used.
  delay(100); // The S7S requires this delay.

  // Clear display
  mySpiI2c.write8Bit(0x76);
  // Display "test"
  mySpiI2c.write8Bit('t');
  mySpiI2c.write8Bit('e');
  mySpiI2c.write8Bit('s');
  mySpiI2c.write8Bit('t');
}

void loop() {
  // Flash display
  mySpiI2c.writeRegister(0x7A,255);
  delay(500);
  mySpiI2c.writeRegister(0x7A,0);
  delay(500);
}
