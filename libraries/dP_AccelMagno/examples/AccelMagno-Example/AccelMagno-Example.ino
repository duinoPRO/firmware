/*
  AccelMagno_Example

  Reads the accelerometer and magnetometer outputs from the
  Accel/Magno module.

  Module Used: Accel/Magno
  Author: SF
  Date: 17 December 2015
  */
  
// Include the necessary libraries
#include <dP_AccelMagno.h>
#include <SPI.h>

// Define an instance of the Accel/Magno module
dP_AccelMagno myAccMag(2);

// Declare the accelerometer and magnetometer output variables
float accX, accY, accZ, magX, magY, magZ;

void setup() {
  // Begin using SPI and the instance of the Accel/Magno module
  SPI.begin();
  myAccMag.begin();

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Set the accelerometer and magnetometer full scales
  myAccMag.setAccelFullScale(ACCEL_2G);
  myAccMag.setMagnoFullScale(MAGNO_2GAUSS);

  // Enable the accelerometer and magnetometer
  myAccMag.enableAccel(1);
  myAccMag.enableMagno(1);
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
