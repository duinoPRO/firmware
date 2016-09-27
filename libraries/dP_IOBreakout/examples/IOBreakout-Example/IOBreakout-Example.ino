/*
 * IOBreakout-Example.ino is part of the duinoPRO firmware. 
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
  IOBreakout-Example

  Reads the state one of the IO Breakout module's digital IO
  pins and outputs the inverted state using a second IO pin.

  Module Used: IO Breakout
  Author: SF
  Date: 13 January 2016
  */

// Include the required dP_Module library
#include <dP_Module.h>

// Define an instance of the IO Breakout module, located on
// board position 2
dP_Module myIO(2);

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
