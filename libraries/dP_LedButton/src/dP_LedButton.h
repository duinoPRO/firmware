#ifndef DP_LEDBUTTON_H
#define DP_LEDBUTTON_H

#include <Module.h>

 
class dP_LedButton : public Module {
    private:
	
    protected:
	
    public:
	enum LedSetting
	{
	    OFF,
	    RED,
	    GREEN,
	    YELLOW
	};
    
	dP_LedButton(int id);
	
	bool readSwitch(uint8_t sw);
	void setLed(uint8_t led, LedSetting setting);
	void begin(void);
};

#endif /* DP_LEDBUTTON_H */