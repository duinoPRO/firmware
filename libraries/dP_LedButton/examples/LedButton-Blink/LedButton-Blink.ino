/*
  Blink - Colour

  Turns on each LED for 1 second changing between green and red

  Module Used: LED Button
  Author: TD
  Date:   8 October 2015
 */

// We are controlling the board and the ledswitch module so we need both libraries
#include <dP_LedButton.h>
#include <duinoPRO.h>
// define instances of the duinoPRO and the LedButton module
duinoPRO myduinoPRO;
dP_LedButton myLed(1); //LedSwitch module is located on board position 1


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
