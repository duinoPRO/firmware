/*
 * Sigfox-DemoExample.ino is part of the duinoPRO firmware.
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
  Sigfox-DemoExample

  Module Used: Sigfox
  Author: KC
  Date: 16 September 2016
  */

// Include the necessary libraries
#include <dP_Sigfox.h>
//FOR DEBUG
#include <duinoPRO.h>


#define RSSI_ENABLE_ADDR        0x05

#define TESTMODE0_CMD           0x30


// Define an instance of the Sigfox module, located on
// board position 6/7
dP_Sigfox mySigfox(6,7);

duinoPRO baseboard;

char rssi, temp;
char id[12];

char myTestMsg[]="duinoPRO-Sfx";


void setup() {
    // Begin using the instance of the Sigfox module
    mySigfox.begin();

    //DEBUG
    Serial.begin(19200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    baseboard.serialDebugMode();
    Serial.print("K");

    delay(500);

    //empty buffer
    while(mySigfox.serial().available())
    {
        mySigfox.serial().read();
    }

    mySigfox.enterConfigMode(); // you must explictly enter config mode before setting config parameters

    // set network mode to uplink/downlink
    mySigfox.setNetworkMode(dP_Sigfox::UPDOWN);

    // read ID
    mySigfox.getId(id);

    // get RSSI
    mySigfox.getRssi(&rssi);
    // get temperature
    mySigfox.getTemperature(&temp);

    mySigfox.exitConfigMode();  // you must explicitly exit config mode to return to idle mode

    // send a test message
    mySigfox.transmitPkt(myTestMsg,strlen(myTestMsg));
}


void loop() {
    char rxPkt[20];
    int len;

    if(len = mySigfox.readPkt(rxPkt))
    {
        // packet received
    }
}
