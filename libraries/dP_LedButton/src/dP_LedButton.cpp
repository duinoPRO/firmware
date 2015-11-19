
#include <dP_ledbutton.h>

dP_LedButton::dP_LedButton(int id) : Module(id) 
{
};

void dP_LedButton::begin()
{
    pin(3).mode(OUTPUT);
    pin(4).mode(OUTPUT);
    pin(5).mode(OUTPUT);
    pin(6).mode(OUTPUT);
	pin(3).write(LOW);
    pin(4).write(LOW);
    pin(5).write(LOW);
    pin(6).write(LOW);
}

bool dP_LedButton::readSwitch(uint8_t sw)
{
    if (sw==1)
    {
        return pin(1).read();
    }
    else
    {
        return pin(2).read();
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

