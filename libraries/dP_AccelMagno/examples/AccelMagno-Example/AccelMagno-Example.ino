/*
 * AccelMagno-Example.ino is part of the duinoPRO firmware.
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
  AccelMagno-Example

  Reads the accelerometer and magnetometer outputs from the
  Accel/Magno module.

  Module Used: Accel/Magno
  Author: SF
  Date: 17 December 2015
*/

// Include the necessary libraries
#include <dP_AccelMagno.h>

// Define an instance of the Accel/Magno module, located on
// board position 2
dP_AccelMagno myAccMag(2);

// Declare the accelerometer and magnetometer output variables
float accX, accY, accZ, magX, magY, magZ;

void setup() {
  // Begin using the instance of the Accel/Magno module
  myAccMag.begin();

  // Initialize serial communication at 9600 bits per second for DEBUG
  Serial.begin(9600);

  // Set the accelerometer and magnetometer full scales
  myAccMag.setAccelFullScale(ACCEL_2G);
  myAccMag.setMagnoFullScale(MAGNO_2GAUSS);

  // Enable the accelerometer and magnetometer
  myAccMag.enableAccel();
  myAccMag.enableMagno();
}

void loop() {
    // Read and print the accelerometer and magnetometer outputs in
    // units of mg (1/1000 of Earth's gravity) and mgauss respectively
    accX = myAccMag.accelX();
    accY = myAccMag.accelY();
    accZ = myAccMag.accelZ();
    magX = myAccMag.magnoX();
    magY = myAccMag.magnoY();
    magZ = myAccMag.magnoZ();

    Serial.print("Accel X: ");
    Serial.print(accX);
    Serial.print(", Accel Y: ");
    Serial.print(accY);
    Serial.print(", Accel Z: ");
    Serial.println(accZ);
    Serial.print("Magno X: ");
    Serial.print(magX);
    Serial.print(", Magno Y: ");
    Serial.print(magY);
    Serial.print(", Magno Z: ");
    Serial.print(magZ);
    Serial.print("\n\n");

    // Wait 200 ms before reading and printing the outputs again
    delay(200);
}
