/*
  XBee-UartReceiveExample

  Receives data using serial port (UART) communication in
  transparent mode with an XBee S2C ZB RF unit connected to
  the XBee module.

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

  // Disable API mode, in case it is enabled, and enter
  // transparent mode.
  myXBee.apiEnable(false);
}

void loop() {
  // Wait until there is incoming data to read
  while (myXBee.serialAvailable() == 0)
  {
  }

  // Allow time for data to be received past the first byte
  delay(50); 

  // Read incoming characters and write them to the serial monitor
  // while there is incoming data to read
  while (myXBee.serialAvailable() > 0)
  {
      uint8_t byteIn = myXBee.readByte();
      myduinoPRO.uartUsbMode(); // Switch the duinoPRO's serial port to USB mode
      Serial.write(byteIn);
      delay(10); // Required for serial port output before switching modes
      myduinoPRO.uartModule7Mode(); // Switch the serial port back to module 7 mode
  }

  // Print a newline to the serial monitor
  myduinoPRO.uartUsbMode();
  Serial.println();
  delay(10);
  myduinoPRO.uartModule7Mode();
}
