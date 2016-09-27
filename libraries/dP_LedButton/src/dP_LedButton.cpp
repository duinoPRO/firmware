/*
 * dP_LedButton.cpp is part of the duinoPRO firmware.
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

/*! \file dP_LedButton.cpp
    \brief duinoPRO LED/Button Module library.
*/

#include <dP_LedButton.h>

dP_LedButton::dP_LedButton(int id) :dP_Module(id)
{
};

void dP_LedButton::begin()
{
    pin(1).mode(INPUT);
    pin(2).mode(INPUT);
    pin(3).mode(OUTPUT);
    pin(4).mode(OUTPUT);
    pin(5).mode(OUTPUT);
    pin(6).mode(OUTPUT);
    pin(3).write(LOW);
    pin(4).write(LOW);
    pin(5).write(LOW);
    pin(6).write(LOW);
}

bool dP_LedButton::readButton(uint8_t button)
{
    if (button == 1)
    {
	       return !pin(1).read();
    }
    else
    {
	       return !pin(2).read();
    }
}

void dP_LedButton::setLed(uint8_t led, LedSetting setting)
{
    uint8_t green, red;
    if (led==1)
    {
		    green = 3;
		    red = 4;
    }
    else
    {
		    green = 5;
		    red = 6;
    }

    switch(setting)
    {
        case OFF:
            pin(green).write(LOW);
		        pin(red).write(LOW);
		        break;
        case RED:
            pin(green).write(LOW);
		        pin(red).write(HIGH);
		        break;
        case GREEN:
            pin(green).write(HIGH);
		        pin(red).write(LOW);
		        break;
        case YELLOW:
            pin(green).write(HIGH);
		        pin(red).write(HIGH);
		        break;
    }
}
