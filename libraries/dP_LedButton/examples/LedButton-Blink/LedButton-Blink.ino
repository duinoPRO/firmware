/*
 * LEDButton-Blink.ino is part of the duinoPRO firmware.
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
  LEDButton-Blink

  Turns on each LED for 1 second changing between green and red

  Module Used: LED/Button
  Author: TD
  Date:   8 October 2015
 */


// Include the required library
#include <dP_LedButton.h>


// Define an instance of the LED/Button module, located on board
// position 1
dP_LedButton myLed(1);


void setup() {
  myLed.begin();  //  Begin using this instance of myLed
}

// Loop indefinitely
void loop() {
  myLed.setLed(1, dP_LedButton::GREEN);  // Set LED 1 to green
  delay(1000);                        // Wait 1 second
  myLed.setLed(1, dP_LedButton::RED);    // Set LED 1 to red
  delay(1000);                        // Wait 1 second
  myLed.setLed(1, dP_LedButton::OFF);    // Turn LED 1 off
  delay(1000);                        // Wait 1 second
  myLed.setLed(2, dP_LedButton::RED);    // Set LED 2 to red
  delay(1000);                        // Wait 1 second
  myLed.setLed(2, dP_LedButton::GREEN);  // Set LED 2 to green
  delay(1000);                        // Wait 1 second
  myLed.setLed(2, dP_LedButton::OFF);    // Turn LED 2 off
  delay(1000);                        // Wait 1 second
}
