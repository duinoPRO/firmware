/*
 * dP_Rtc.cpp is part of the duinoPRO firmware.
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

#include <dP_Rtc.h>


// Module Pins
#define INT_PIN 1
#define RESET_PIN 3
#define POWERDOWN_PIN 5 // This doesn't seem to apply to this module.

dP_Rtc::dP_Rtc(int id) : dP_Module(id)
{
};


// SPI Port Characteristics
#define MY_SPEED_MAX 	(2000000)
#define MY_DATA_ORDER  	(MSBFIRST)
#define MY_DATA_MODE	(SPI_MODE3)

#define SECOND_REG				0x00
#define MINUTE_REG				0x01
#define HOUR_REG				0x02
#define DAY_REG					0x03
#define DATE_REG				0x04
#define MONTH_REG				0x05
#define YEAR_REG				0x06
#define ALARM_1_SECOND_REG		0x07
#define ALARM_1_MINUTE_REG		0x08
#define ALARM_1_HOUR_REG		0x09
#define ALARM_1_DATE_REG		0x0A
#define ALARM_2_MINUTE_REG		0x0B
#define ALARM_2_HOUR_REG		0x0C
#define ALARM_2_DATE_REG		0x0D
#define CONTROL_REG				0x0E
#define STATUS_REG				0x0F

#define REPACK_TO_BCD(n) 	((((n) / 10)  << 4) | (n % 10))
#define UNPACK_FROM_BCD(n)  ((((n) & 0xF0) >> 4) * 10 + ((n) & 0x0F)) // Can't apply this to SPI.transfer(0x00) or it will execute twice.
#define TOP_NIBBLE(n)		(((n) & 0xF0) >> 4)
#define BOTTOM_NIBBLE(n)	((n) & 0x0F)

void dP_Rtc::begin()
{
	spiSelect().mode(OUTPUT);
  spiSelect().write(HIGH);
	// Enables 32 kHz output, but disables battery-backed 32 kHz output, while maintaining the oscillator stop flag.
	uint8_t val = readRegister(0x0F);
	writeRegister(0x0F,(val | 0x08) & 0x88);
	spi().begin();
}

uint8_t dP_Rtc::readRegister(uint8_t addr)
{
	uint8_t val;
	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	val = spi().transfer(addr & 0x3F);
	val = spi().transfer(0x00);
	spiSelect().write(HIGH);
	spi().endTransaction();
	return val;
}

void dP_Rtc::writeRegister(uint8_t addr, uint8_t data)
{
	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	spi().transfer((addr & 0x3F) | 0x80);
	spi().transfer(data);
	spiSelect().write(HIGH);
	spi().endTransaction();
}

void dP_Rtc::setDate(uint16_t year, uint8_t month, uint8_t day)
{
	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	spi().transfer(DATE_REG | 0x80);
	spi().transfer(REPACK_TO_BCD(day));
	spi().transfer(REPACK_TO_BCD(month));
	spi().transfer(REPACK_TO_BCD(year % 100));
	spiSelect().write(HIGH);
	spi().endTransaction();
}

void dP_Rtc::setDayWeek(uint8_t dayOfWeek)
{
	writeRegister(DAY_REG, dayOfWeek);
}

void dP_Rtc::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	spi().transfer(SECOND_REG | 0x80);
	spi().transfer(REPACK_TO_BCD(second));
	spi().transfer(REPACK_TO_BCD(minute));
	spi().transfer(REPACK_TO_BCD(hour));
	spiSelect().write(HIGH);
	spi().endTransaction();
}

void dP_Rtc::readDate(uint8_t *year, uint8_t *month, uint8_t *day)
{
	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	spi().transfer(DATE_REG);
	*day = spi().transfer(0x00);
	*day = UNPACK_FROM_BCD(*day);
	*month = spi().transfer(0x00) & 0x1F; // Ignores month register's century bit.
	*month = UNPACK_FROM_BCD(*month);
	*year = spi().transfer(0x00);
	*year = UNPACK_FROM_BCD(*year);
	spiSelect().write(HIGH);
	spi().endTransaction();
}

void dP_Rtc::readDayWeek(uint8_t *dayOfWeek)
{
	*dayOfWeek = readRegister(DAY_REG);
}

// Accounts for the case where the RTC is operating in 12 hour mode.
void dP_Rtc::readTime(uint8_t *hour, uint8_t *minute, uint8_t *second)
{
	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	spi().transfer(SECOND_REG);
	*second = spi().transfer(0x00);
	*second = UNPACK_FROM_BCD(*second);
	*minute = spi().transfer(0x00);
	*minute = UNPACK_FROM_BCD(*minute);
	*hour = spi().transfer(0x00);
	if ((*hour & 0x40) == 0x40) {
		*hour = UNPACK_FROM_BCD(*hour & 0x1F) + (12 * ((*hour & 0x20) >> 5));
	}
	else {
		*hour = UNPACK_FROM_BCD(*hour);
	}
	spiSelect().write(HIGH);
	spi().endTransaction();
}

uint16_t dP_Rtc::readFatDate()
{
	uint8_t year, month, day;
	readDate(&year,&month,&day);
	return (year + 20) << 9 | month << 5 | day; // RTC year 0 is 2000, FAT year 0 is 1980
}

uint16_t dP_Rtc::readFatTime()
{
	uint8_t hour, minute, second;
	readTime(&hour,&minute,&second);
	return hour << 11 | minute << 5 | second >> 1;
}

// Need a string buffer of at least 11 characters
void dP_Rtc::dateString(char *str)
{
	dateString(str, '-');
}

// Need a string buffer of at least 11 characters
void dP_Rtc::dateString(char *str, char separator)
{
	uint8_t day, month, year;

	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	spi().transfer(DATE_REG);
	day = spi().transfer(0x00);
	month = spi().transfer(0x00) & 0x1F;
	year = spi().transfer(0x00);
	spiSelect().write(HIGH);
	spi().endTransaction();

	str[0] = '2';
	str[1] = '0';
	str[2] = TOP_NIBBLE(year) + '0';
	str[3] = BOTTOM_NIBBLE(year) + '0';
	str[4] = separator;
	str[5] = TOP_NIBBLE(month) + '0';
	str[6] = BOTTOM_NIBBLE(month) + '0';
	str[7] = separator;
	str[8] = TOP_NIBBLE(day) + '0';
	str[9] = BOTTOM_NIBBLE(day) + '0';
	str[10] = 0;
}

// Need a string buffer of at least 9 characters
void dP_Rtc::timeString(char *str)
{
	timeString(str, ':');
}

// Need a string buffer of at least 9 characters
void dP_Rtc::timeString(char *str, char separator)
{
	uint8_t hour, minute, second;

	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	spi().transfer(SECOND_REG);
	second = spi().transfer(0x00);
	minute = spi().transfer(0x00);
	hour = spi().transfer(0x00);
	spiSelect().write(HIGH);
	spi().endTransaction();

	str[0] = TOP_NIBBLE(hour) + '0';
	str[1] = BOTTOM_NIBBLE(hour) + '0';
	str[2] = separator;
	str[3] = TOP_NIBBLE(minute) + '0';
	str[4] = BOTTOM_NIBBLE(minute) + '0';
	str[5] = separator;
	str[6] = TOP_NIBBLE(second) + '0';
	str[7] = BOTTOM_NIBBLE(second) + '0';
	str[8] = 0;
}

// dayOfWeek replaces day for appropriate alarm modes
void dP_Rtc::alarmSet(uint8_t alarmnum, uint8_t alarmmode, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	spi().beginTransaction(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE);
	spiSelect().write(LOW);
	if (alarmnum == 1) {
		spi().transfer(ALARM_1_SECOND_REG | 0x80);
		spi().transfer((alarmmode & 0x01) << 7 | REPACK_TO_BCD(second));
	}
	else {
		spi().transfer(ALARM_2_MINUTE_REG | 0x80);
	}
	spi().transfer((alarmmode & 0x02) << 6 | REPACK_TO_BCD(minute));
	spi().transfer((alarmmode & 0x04) << 5 | REPACK_TO_BCD(hour));
	spi().transfer((alarmmode & 0x08) << 4 | (alarmmode & 0x10) << 2 | REPACK_TO_BCD(day));
	spiSelect().write(HIGH);
	spi().endTransaction();
}

void dP_Rtc::alarmEnable(uint8_t alarmnum)
{
	uint8_t val = readRegister(CONTROL_REG);
	if (alarmnum == 1) {
		writeRegister(CONTROL_REG, val | 0x05);
	}
	else {
		writeRegister(CONTROL_REG, val | 0x06);
	}
}

void dP_Rtc::alarmDisable(uint8_t alarmnum)
{
	uint8_t val = readRegister(CONTROL_REG);
	if (alarmnum == 1) {
		writeRegister(CONTROL_REG, val & 0xFE);
	}
	else {
		writeRegister(CONTROL_REG, val & 0xFD);
	}
	val = readRegister(STATUS_REG);
	if (alarmnum == 1) {
		writeRegister(STATUS_REG, val & 0xFE);
	}
	else {
		writeRegister(STATUS_REG, val & 0xFD);
	}

}

void dP_Rtc::alarmStop(uint8_t alarmnum)
{
	uint8_t val = readRegister(STATUS_REG);
	if (alarmnum == 1) {
		writeRegister(STATUS_REG, val & 0xFE);
	}
	else {
		writeRegister(STATUS_REG, val & 0xFD);
	}
}

bool dP_Rtc::alarmIntRead()
{
	return !pin(INT_PIN).read();
}

void dP_Rtc::alarmIntDisable()
{
	uint8_t val = readRegister(CONTROL_REG);
	writeRegister(CONTROL_REG, val & 0xF8);
	val = readRegister(STATUS_REG);
	writeRegister(STATUS_REG, val & 0xFC);
}

void dP_Rtc::sqwFreq(uint8_t mode)
{
	uint8_t val = readRegister(CONTROL_REG);
	if (mode == 0) { // 1 Hz
		writeRegister(CONTROL_REG, val & 0xE7);
	}
	else if (mode == 1) { // 1.024 kHz
		writeRegister(CONTROL_REG, (val | 0x08) & 0xEF);
	}
	else if (mode == 2) { // 4.096 kHz
		writeRegister(CONTROL_REG, (val | 0x10) & 0xF7);
	}
	else { // 8.192 kHz
		writeRegister(CONTROL_REG, val | 0x18);
	}
}
