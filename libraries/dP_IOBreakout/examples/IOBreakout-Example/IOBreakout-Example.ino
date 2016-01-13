/*
  IOBreakout-Example

  Reads the state one of the IO Breakout module's digital IO
  pins and outputs the inverted state using a second IO pin.

  Module Used: IO Breakout
  Author: SF
  Date: 13 January 2016
  */
  
// Include the required Module library
#include <Module.h>

// Define an instance of the IO Breakout module, located on
// board position 2
Module myIO(2);

// Declare the variable used to store the input state
bool readIn;

// Declare a variable used to refer to the IO Breakout module's
// IO pin 2. While not done here, a variable can be declared
// similarly for IO pin 1.
Pin& pinOut = myIO.pin(2);

void setup() {
  // Configure the IO Breakout module's IO pin 1 as an input and
  // its IO pin 2 as an output
  myIO.pin(1).mode(INPUT);
  pinOut.mode(OUTPUT);
}

void loop() {
  // Read the state of the input pin
  readIn = myIO.pin(1).read();

  // Output the inverted input state via the output pin
  if (readIn)
  {
    pinOut.write(LOW);    
  }
  else
  {
    pinOut.write(HIGH);
  }
}
