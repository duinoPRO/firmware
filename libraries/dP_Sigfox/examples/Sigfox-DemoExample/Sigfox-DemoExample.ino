/*
  Sigfox-DemoExample

  Module Used: Sigfox
  Author: KC
  Date: 16 September 2016
  */

// Include the necessary libraries
#include <dP_Sigfox.h>
// FOR DEBUG
#include <dP_LedButton.h>


#define MYID 1
#define DATATYPE 1

char txPkt[50];

// Define an instance of the Sigfox module, located on
// board position 6/7
dP_Sigfox mySigfox(6,7);


#define WAITFORPROMPT_TIMEOUT     5000   // set wait for prompt to timeout after 1s


void setup() {
  Serial.begin(9600);
  delay(2000);
  // Begin using the instance of the Sigfox module
  mySigfox.begin();

  delay(500);

  //empty buffer
  while(mySigfox.serial().available())
  {
    mySigfox.serial().read();
  }

  mySigfox.enterConfigMode(); // you must explictly enter config mode before setting config parameters

  //set frequency to AU/NZ settings
  mySigfox.setMemoryConfigParameter(0x00, 0x03);
  // enable RSSI mode - append RSSI to received data
  mySigfox.setMemoryConfigParameter(0x05, 0x01);
  // set network mode to uplink/downlink
  mySigfox.setNetworkMode(dP_Sigfox::UPONLY);
  //mySigfox.setMemoryConfigParameter(0x3B, 0x01);

  //print config parameters again
  mySigfox.serial().write(0x30);

  mySigfox.exitConfigMode();  // you must explicitly exit config mode to return to idle mode
}


void loop() {
  int i;
  char val;

  delay(1000);

/*  for (i=0; i<30; i++)
  {
    txPkt[i] = mySigfox.serial().read();
  }

  txPkt[i] = '\0';*/
}
