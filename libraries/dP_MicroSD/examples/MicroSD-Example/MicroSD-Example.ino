/*
  MicroSD-Example

  Writes to a file on the inserted microSD card, and then lists
  the contents of the card. The RTC module is used to set the
  timestamps associated with the file.

  Modules Used: Micro SD, RTC
  Author: SF
  Date: 18 December 2015
  */

// Include the necessary libraries
#include <SPI.h>
#include <dP_MicroSD.h>
#include <dP_Rtc.h>

// Define instances of the Micro SD and RTC modules, located on
// board positions 2 and 6 respectively
dP_MicroSD myMicroSD(2);
dP_Rtc myRtc(6);

void setup() {
  // Begin using SPI and the instances of the Micro SD and RTC modules
  SPI.begin();
  myMicroSD.begin();
  myRtc.begin();

  // Initialize serial communication at 9600 bits per second
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
