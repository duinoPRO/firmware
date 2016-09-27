/*
 * MicroSD-Example.ino is part of the duinoPRO firmware.
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
  MicroSD-Example

  Writes to a file on the inserted microSD card, and then lists
  the contents of the card over the debug serial port (Serial Monitor).
  The duinoPRO RTC module is used to set the timestamps associated
  with the file.

  Modules Used: Micro SD, RTC
  Author: SF, KC
  Date: 18 December 2015
  */

// Include the necessary libraries
#include <dP_MicroSD.h>
#include <dP_Rtc.h>

// Define instances of the Micro SD and RTC modules, located on
// board positions 2 and 6 respectively
dP_MicroSD myMicroSD(2);
dP_Rtc myRtc(5);

void setup() {
  // Begin using the instances of the Micro SD and RTC modules
  myMicroSD.begin();
  myRtc.begin();

  // Initialize serial communication at 9600 bits per second (DEBUG)
  Serial.begin(9600);

  // Check that a microSD card is inserted into the module and
  // terminate setup() otherwise
  if (myMicroSD.cardDetect() == false)
  {
    Serial.println("microSD card not detected.");
    return;
  }

  // Select the callback function used to set file timestamps
  myMicroSD.timestampCallback(dateTime);

  // Open the file "EXAMPLE.TXT" on the inserted microSD card
  // in write mode. Note: The filename and extension here can be
  // at most 8 and 3 characters long respectively. If the file
  // does not exist, it will be created with a filename in all
  // caps.
  File f = myMicroSD.SD.open("EXAMPLE.TXT", FILE_WRITE);

  // Check that the file was opened successfully
  if (f) {
    // Write to the opened file
    f.println("Example text");
    // Close the opened file
    f.close();
    Serial.println("File written.");
  }
  else {
    Serial.println("Problem opening file on microSD card.");
  }

  // List the contents of the inserted microSD card.
  File root = myMicroSD.SD.open("/");
  if (root) {
    myMicroSD.printDirectory(root);
  }
  else {
    Serial.println("Problem opening microSD card's root directory.");
  }
}

void loop() {
  // Nothing happens after setup() is complete
}

// Function called by timestampCallback() to read the current
// date and time
void dateTime(uint16_t *date, uint16_t *time) {
  *date = myRtc.readFatDate();
  *time = myRtc.readFatTime();
}
