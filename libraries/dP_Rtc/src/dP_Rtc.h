#ifndef DP_RTC_H
#define DP_RTC_H

#include <Module.h>

/** \file
 */

/** \brief Class for controlling a duinoPRO RTC module.
 *  
 */ 
class dP_Rtc : public Module {
    private:
	
    protected:
	
    public:
		/** \brief Constructor for dP_Rtc
		 *  \param id - the location of the dP_Rtc module on the duinoPRO baseboard.
		 */
		dP_Rtc(int id);
		
		/** \brief Begin using the dP_Rtc module.
		*/
		void begin(void);
		
		/** \brief Read the value of a register in the RTC chip.
		 *  \param addr - Address of the register to be read.
		 *  \return The register value.
		 */
		uint8_t readRegister(uint8_t addr);
		/** \brief Write a value to a register in the RTC chip.
		 *  \param addr - Address of the register to be written.
		 *  \param data - The value to be written to the register.
		 */
		void writeRegister(uint8_t addr, uint8_t data);
		
		/** \brief Set the RTC's date (year, month and day).
		 *  \param year - The year value (2000-2099) to be set.
		 *  \param month - The month value (01-12) to be set.
		 *  \param day - The day value (01-31) to be set.
		 */
		void setDate(uint16_t year, uint8_t month, uint8_t day);
		/** \brief Set the RTC's day of the week.
		 *  \param dayOfWeek - The user-defined day of the week value (1-7) to be set.
		 */
		void setDayWeek(uint8_t dayOfWeek);
		/** \brief Set the RTC's time (hour, minute and second).
		 *  \param hour - The hour value (00-23) to be set, specified in 24-hour format.
		 *  \param minute - The minute value (00-59) to be set.
		 *  \param second - The second value (00-59) to be set.
		 */
		void setTime(uint8_t hour, uint8_t minute, uint8_t second);
		/** \brief Read the RTC's date (year, month and day).
		 *  \param year - The RTC's year value (2000-2099).
		 *  \param month - The RTC's month value (01-12).
		 *  \param day - The RTC's day value (01-31).
		 */
		void readDate(uint8_t *year, uint8_t *month, uint8_t *day);
		/** \brief Read the RTC's day of the week.
		 *  \param dayofWeek - The RTC's day of the week value (1-7).
		 */
		void readDayWeek(uint8_t *dayOfWeek);
		/** \brief Read the RTC's time (hour, minute and second).
		 *  \param hour - The RTC's hour value, in 24-hour format (00-23).
		 *  \param minute - The RTC's minute value (00-59).
		 *  \param second - The RTC's second value (00-59).
		 */
		void readTime(uint8_t *hour, uint8_t *minute, uint8_t *second);
		
		/** \brief Read the RTC's date (year, month and day) in FAT format.
		 *  \return The RTC's FAT date.
		 */
		uint16_t readFatDate(void);
		/** \brief Read the RTC's date (hour, minute and second) in FAT format.
		 *  \return The RTC's FAT time.
		 */
		uint16_t readFatTime(void);
		
		/** \brief Read the RTC's date (year, month and day) in string format using the '-' separator.
		 *  \param str - The RTC's date in string format (requires a string buffer of at least 11 characters).
		 */
		void dateString(char *str);
		/** \brief Read the RTC's date (year, month and day) in string format using a specified separator.
		 *  \param str - The RTC's date in string format (requires a string buffer of at least 11 characters).
		 *  \param separator - The separator to be used in the string.
		 */
		void dateString(char *str, char separator);
		/** \brief Read the RTC's time (hour, minute and second) in string format using the ':' separator.
		 *  \param str - The RTC's time in string format (requires a string buffer of at least 9 characters).
		 */
		void timeString(char *str);
		/** \brief Read the RTC's time (hour, minute and second) in string format using a specified separator.
		 *  \param str - The RTC's time in string format (requires a string buffer of at least 9 characters).
		 *  \param separator - The separator to be used in the string.
		 */
		void timeString(char *str, char separator);
				
		/** \brief Set the specified alarm of the RTC chip using the specified mode and date/time values.
		 *	Note that this does not enable the specified alarm.
		 *	Date/time parameters that are irrelevant to the chosen alarm mode can be set to any allowable values.
		 *  \param alarmnum - Number of the alarm (1 or 2) to be set.
		 *  \param alarmmode - The alarm mode to be set. Possible modes are listed on p13 of the RTC chip's datasheet (Maxim Integrated Products DS3234).
		 *  E.g. alarm 1 activates when minutes and seconds match when alarmnum = 1 and alarmmode = 0b01100 (or 0b11100).
		 * 	When setting alarm 2, the LSB of alarmmode is irrelevant and can be 0 or 1.
		 *	\param day - The day value (01-31) or day of the week value (1-7) to be set, depending on the chosen alarm mode.
		 *	\param hour - The hour value (00-23) to be set.
		 *	\param minute - The minute value (00-59) to be set.
		 *	\param second - The second value (00-59) to be set.
		 */
		void alarmSet(uint8_t alarmnum, uint8_t alarmmode, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
		/** \brief Enable the specified alarm of the RTC chip.
		 *	\param alarmnum - Number of the alarm (1 or 2) to be enabled.
		 */
		void alarmEnable(uint8_t alarmnum);
		/** \brief Disable the specified alarm of the RTC chip and stop the alarm by turning its alarm interrupt flag off.
		 *	\param alarmnum - Number of the alarm (1 or 2) to be disabled.
		 */
		void alarmDisable(uint8_t alarmnum);
		/** \brief Stop the specified alarm of the RTC chip by turning its alarm interrupt flag off, without disabling the alarm.
		 *	\param alarmnum - Number of the alarm (1 or 2) to be stopped.
		 */
		void alarmStop(uint8_t alarmnum);
		/** \brief Read the alarm interrupt signal of the RTC chip.
		 *  \return The state of the alarm interrupt signal.
		 */
		bool alarmIntRead(void);
		/** \brief Disable and stop both of the RTC chip's alarms and enables the square wave output from the GPIO1/INT pin (pin 1).
		 */
		void alarmIntDisable(void);
		/** \brief Set the square wave output frequency.
		 *  \param mode - 0 (1 Hz), 1 (1.024 kHz), 2 (4.096 kHz) or 3 (8.192 kHz).
		 */
		void sqwFreq(uint8_t mode);
	
};

#endif /* DP_RTC_H */