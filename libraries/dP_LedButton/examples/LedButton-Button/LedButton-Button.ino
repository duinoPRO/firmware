/*
 * LEDButton-Button is part of the duinoPRO firmware.
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
  LedButton-Button

  Turns on each LED while the corresponding button is pressed.

  Module Used: LED Button
  Author: SF
  Date: 5 January 2016
  */

// Include the necessary libraries
#include <dP_LedButton.h>

// Define an instance of the LED Button module, located on board
// position 2
dP_LedButton myLed(2);

void setup() {
  // Begin using the instance of the LED Button module
  myLed.begin();
}

void loop() {
  // Read button 1 and turn on LED 1 (red) if it is pressed, or
  // turn it off otherwise
  if (myLed.readButton(1))
  {
    myLed.setLed(1, dP_LedButton::RED);
  }
  else
  {
    myLed.setLed(1, dP_LedButton::OFF);
  }

  // Read button 2 and turn on LED 2 (red) if it is pressed, or
  // turn it off otherwise
  if (myLed.readButton(2))
  {
    myLed.setLed(2, dP_LedButton::RED);
  }
  else
  {
    myLed.setLed(2, dP_LedButton::OFF);
  }
}
