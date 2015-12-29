/*
  RTC_Example

  Sets the RTC's date and time if necessary, then reads its
  date and time, followed by setting an alarm and reading 
  the alarm interrupt status

  Module Used: RTC
  Author: SF
  Date: 17 December 2015
  */

// Include the necessary libraries
#include <dP_Rtc.h> 
#include <SPI.h>

// Define an instance of the RTC module
dP_Rtc myRtc(2);

void setup() {
  // Begin using SPI and the instance of the RTC module
  SPI.begin();
  myRtc.begin();

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Uncomment the code block below and change the arguments
  // appropriately to set the RTC's date and time
  /*
    myRtc.setDate(2015,12,17);
    myRtc.setTime(14,27,2);
    myRtc.setDayWeek(5);
  */  

  // Read the RTC's date and time
  uint8_t year, month, day, hour, minute, second, dayofweek;
  myRtc.readDate(&year, &month, &day);
  myRtc.readDayWeek(&dayofweek);
  myRtc.readTime(&hour, &minute, &second);

  // Print the RTC's date and time to the serial monitor
  Serial.print("Year: ");
  Serial.println(year);
  Serial.print("Month: ");
  Serial.println(month);
  Serial.print("Day: ");
  Serial.println(day);
  Serial.print("Day of the week: ");
  Serial.println(dayofweek);
  Serial.print("Hour: ");
  Serial.println(hour);
  Serial.print("Minute: ");
  Serial.println(minute);
  Serial.print("Second: ");
  Serial.println(second);

  // The RTC's date and time can also be read and then displayed in string format
  // with specified separators
  char timeBuffer[9];
  char dateBuffer[11];
  myRtc.timeString(timeBuffer, ':'); // The second argument defaults to ':' when excluded
  myRtc.dateString(dateBuffer, '-'); // The second argument defaults to '-' when excluded
  Serial.println("Date and time:");
  Serial.print(dateBuffer);
  Serial.print(" ");
  Serial.println(timeBuffer);

  // Disable the RTC's alarm 2 in case it has previously been
  // enabled, enable alarm 1, and set alarm 1 to activate every
  // day (mode 01000) at 18:30:45. For this alarm mode, the
  // 3rd (day) argument of alarmSet() is irrelevant.
  myRtc.alarmDisable(2);
  myRtc.alarmEnable(1);
  myRtc.alarmSet(1, 0b01000, 1, 18, 30, 45);
}

void loop() {
  // Continuously read the RTC's alarm interrupt status to determine if an alarm has been activated
  if (myRtc.alarmIntRead() == 1) {
    Serial.println("It's 18:30:45 now!");

    // Stops the alarm until it is activated again at 18:30:45 the next day
    myRtc.alarmStop(1);
  }
}
