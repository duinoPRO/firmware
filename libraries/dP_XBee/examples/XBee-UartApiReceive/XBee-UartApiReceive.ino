/*
  XBee-UartReceiveExample

  Receives data using serial port (UART) communication in API
  mode with an XBee S2C ZB RF unit connected to the XBee module.

  Module Used: XBee
  Author: SF
  Date: 11 January 2016
  */

// Include the necessary libraries
#include <duinoPRO.h>
#include <dP_XBee.h>

// Define instances of the duinoPRO and the XBee module,
// located on board position 6/7 (the higher number is used
// as the argument)
duinoPRO myduinoPRO;
dP_XBee myXBee(7);

void setup() {
  // Begin using the instance of the XBee module, with serial
  // port communication initialized at 9600 bits per second in
  // module 7 mode. Communication over the USB port is not
  // possible until this mode is changed.
  myXBee.beginUart(9600);

  // Enable API mode and disable transparent mode.
  myXBee.apiEnable(true);
}

void loop() {
  // Read the last received packet in API mode. The returned
  // boolean value is true if and only if the packet is valid.
  bool valPacket = myXBee.readPacket();

  // If the packet is valid, switch the duinoPRO's
  // serial port to USB mode, print the most recent received packet
  // to the serial monitor, then switch the duinoPRO's serial port
  // back to module 7 mode.
  if (valPacket)
  {
    myduinoPRO.uartUsbMode();
    Serial.println(myXBee.lastPacket());
    delay(10); // Required for correct serial port output before switching modes
    myduinoPRO.uartModule7Mode();
  }

  // Wait for a second before receiving another packet
  delay(1000);
}
