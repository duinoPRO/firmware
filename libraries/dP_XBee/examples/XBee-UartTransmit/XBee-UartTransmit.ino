/*
  XBee-UartTransmitExample

  Transmits data using serial port (UART) communication with
  an XBee S2C ZB RF unit connected to the XBee module.

  Module Used: XBee
  Author: SF
  Date: 11 January 2016
  */

// Include the necessary libraries
#include <dP_XBee.h>

// Define an instance of the XBee module, located on
// board position 6/7 (the higher number is used as
// the argument)
dP_XBee myXBee(7);

// This 64-bit address is used to broadcast transmissions to
// all devices. To transmit to a specific device, use that
// device's 64-bit address instead.
uint8_t addr[] = {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF};

void setup() {
  // Begin using the instance of the XBee module, with serial
  // port communication initialized at 9600 bits per second in
  // module 7 mode. Communication over the USB port is not
  // possible until this mode is changed.
  myXBee.beginUart(9600);

  // Disable API mode, in case it is enabled, and enter
  // transparent mode
  myXBee.apiEnable(false);

  // Send the string "Test1" over the serial port in
  // transparent mode, once per second for 20 seconds
  for (int i = 1; i <= 20; i++) {
    myXBee.sendString("Test1");
    delay(1000);
  }

  // Enable API mode
  myXBee.apiEnable(true);

  // Send a packet consisting of the text "Test2" to all
  // devices over the serial port in API mode, once per second
  // for 20 seconds.
  for (int i = 1; i <= 20; i++) {
    myXBee.sendPacket("Test2",addr);
    delay(1000);
  }

  // Disable API mode again
  myXBee.apiEnable(false);
}

void loop() { 
  // Nothing happens after setup() is complete
}
