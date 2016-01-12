/*
  XBee-SpiReceiveExample

  Receives data using SPI communication with an
  XBee S2C ZB RF unit connected to the XBee module. The
  XBee unit's SPI pins must be configured, e.g. by running
  the XBee-SetupSpiExample sketch.

  Module Used: XBee
  Author: SF
  Date: 11 January 2016
  */

// Include the necessary libraries
#include <dP_XBee.h>
#include <SPI.h>

// Define an instance of the XBee module, located on
// board position 1/2 (the higher number is used as
// the argument)
dP_XBee myXBee(2);

void setup() {
  // Begin using SPI and the instance of the XBee module, using
  // SPI communication.
  SPI.begin();
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
