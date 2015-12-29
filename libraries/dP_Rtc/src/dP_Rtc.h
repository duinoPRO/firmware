#ifndef DP_RTC_H
#define DP_RTC_H

#include <Module.h>


 
class dP_Rtc : public Module {
    private:
	
    protected:
	
    public:

	dP_Rtc(int id);
	
	void begin(void);
	uint8_t readRegister(uint8_t addr);
	void writeRegister(uint8_t addr, uint8_t data);
	
	void setDate(uint16_t year, uint8_t month, uint8_t day);
	void setDayWeek(uint8_t dayofweek);
	void setTime(uint8_t hour, uint8_t minute, uint8_t second);
	void readDate(uint8_t *year, uint8_t *month, uint8_t *day);
	void readDayWeek(uint8_t *dayofweek);
	void readTime(uint8_t *hour, uint8_t *minute, uint8_t *second);
	
	void readYear(uint8_t *year);
	
	uint16_t readFatDate(void);
	uint16_t readFatTime(void);
	
	void timeString(char *str);
	void timeString(char *str, char separator);
	void dateString(char *str);
	void dateString(char *str, char separator);
	
	void alarmSet(uint8_t alarmnum, uint8_t alarmmode, uint8_t second, uint8_t minute, uint8_t hour, uint8_t day);
	void alarmEnable(uint8_t alarmnum);
	void alarmDisable(uint8_t alarmnum);
	void alarmIntDisable(void);
	bool alarmIntRead(void);
	void alarmIntDeactivate(void);
	void alarmStop(uint8_t alarmnum);
	void sqwFreq(uint8_t mode);
	bool sqwRead();
	
};

#endif /* DP_RTC_H */