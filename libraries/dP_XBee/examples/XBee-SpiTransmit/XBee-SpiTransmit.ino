/*
  XBee-SpiTransmitExample

  Transmits data using SPI communication with an
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
// board position 6/7 (the higher number is used as
// the argument)
dP_XBee myXBee(7);

// This 64-bit address is used to broadcast transmissions to
// all devices. To transmit to a specific device, use that
// device's 64-bit address instead.
uint8_t addr[] = {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF};
 
void setup() {
  // Begin using SPI and the instance of the XBee module, using
  // SPI communication.
  SPI.begin();
  myXBee.beginSpi();
}

void loop() {
  // Transmit a packet consisting of the text "Test" to all devices
  // using SPI communication in API mode. The second argument
  // specifies the length of the packet.
  myXBee.sendPacket("Test",4,addr);

  // Wait for a second before sending the packet again.
  delay(1000);
}
