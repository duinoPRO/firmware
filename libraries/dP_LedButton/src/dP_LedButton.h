#ifndef DP_LEDBUTTON_H
#define DP_LEDBUTTON_H

#include <Module.h>

/** \file
 */

/** \brief Class for controlling a duinoPRO LED Button module.
 *  
 */  
class dP_LedButton : public Module {
    private:
	
    protected:
	
    public:
	/** \brief Values for controlling LED setting.
	 */
	enum LedSetting
	{
	    OFF,	/**< LED off */
	    RED,	/**< LED red */
	    GREEN,	/**< LED green */
	    YELLOW	/**< LED yellow */
	};
	
	/** \brief Constructor for dP_LedButton
	 *  \param id - the location of the dP_LedButton module on the duinoPRO baseboard.
	 */
	dP_LedButton(int id);
    
	/** \brief Begin using the dP_LedButton module.
	*/
	void begin(void);
	
	/** \brief Read the state of the specified button.
	 *  \param sw - Number of the button to be read.
	 *  \return true if the button is pressed, false otherwise.
	 */
	bool readSwitch(uint8_t sw);
	/** \brief Control the specified LED's setting. Possible settings are listed in #LedSetting.
	 *  \param led - Number of the LED to be set.
	 *  \param setting - LED setting, from LedSetting.
	 */
	void setLed(uint8_t led, LedSetting setting);
};

#endif /* DP_LEDBUTTON_H */