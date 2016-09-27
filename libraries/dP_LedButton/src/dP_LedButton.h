/*
 * dP_LedButton.h is part of the duinoPRO firmware.
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

#ifndef DP_LEDBUTTON_H
#define DP_LEDBUTTON_H

#include <dP_Module.h>

/** \file
 */

/** \brief Class for controlling a duinoPRO LED Button module.
 *
 */
class dP_LedButton : public dP_Module {
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
        *   \param void
        *   \return void
      	*/
      	void begin(void);

      	/** \brief Read the state of the specified button.
      	 *  \param button - Number of the button to be read.
      	 *  \return true if the button is pressed, false otherwise.
         *
         *  <B>Example</B>
         *  <p>\code
         *      // Read button 1 and turn on LED 1 if it is pressed, or
                // turn it off otherwise
                if (myLed.readButton(1))
                {
                     myLed.setLed(1, dP_LedButton::RED);
                }
            \endcode</p>
      	 */
      	bool readButton(uint8_t button);
      	/** \brief Control the specified LED's setting. Possible settings are listed in #LedSetting.
      	 *  \param led - Number of the LED to be set.
      	 *  \param setting - LED setting, from LedSetting.
      	 */
      	void setLed(uint8_t led, LedSetting setting);
};

#endif /* DP_LEDBUTTON_H */
