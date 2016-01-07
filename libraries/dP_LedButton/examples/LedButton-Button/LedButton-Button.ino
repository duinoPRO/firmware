/*
  LedButton-ButtonExample

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
  // Read button 1 and turn on LED 1 if it is pressed, or
  // turn it off otherwise
  if (myLed.readSwitch(1))
  {
    myLed.setLed(1, dP_LedButton::RED);
  }
  else
  {
    myLed.setLed(1, dP_LedButton::OFF);
  }

  // Read button 2 and turn on LED 2 if it is pressed, or
  // turn it off otherwise
  if (myLed.readSwitch(2))
  {
    myLed.setLed(2, dP_LedButton::RED);
  }
  else
  {
    myLed.setLed(2, dP_LedButton::OFF);
  }
}

