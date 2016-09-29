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


#define RSSI_ENABLE_ADDR        0x05

#define TESTMODE0_CMD           0x30


// Define an instance of the Sigfox module, located on
// board position 6/7
dP_Sigfox mySigfox(6,7);


char rssi, temp;
char id[12];

void setup() {
  // Begin using the instance of the Sigfox module
  mySigfox.begin();

  delay(500);

  //empty buffer
  while(mySigfox.serial().available())
  {
    mySigfox.serial().read();
  }

  mySigfox.enterConfigMode(); // you must explictly enter config mode before setting config parameters

  // enable RSSI mode - append RSSI to received data
  mySigfox.setMemoryConfigParameter(RSSI_ENABLE_ADDR, 1);
  // set network mode to uplink/downlink
  mySigfox.setNetworkMode(dP_Sigfox::UPDOWN);

  //print config parameters again
  mySigfox.sendConfigCmd(TESTMODE0_CMD);

  // read ID
  mySigfox.getId(id);

  // get RSSI
  mySigfox.getRssi(&rssi);
  // get temperature
  mySigfox.getTemperature(&temp);

  mySigfox.exitConfigMode();  // you must explicitly exit config mode to return to idle mode
}


void loop() {
  char rxPkt[20];
  int len;

  if(len = mySigfox.readPkt(rxPkt))
  {
    // packet received
  }
}
