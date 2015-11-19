#include <dP_SpiI2c.h>
#include <SPI.h>

// Module Pins
#define POWERDOWN_PIN 	5
#define GPIO1			1
#define GPIO2			2


dP_SpiI2c::dP_SpiI2c(int id) : Module(id) 
{

};

void dP_SpiI2c::begin()
{
    pin(POWERDOWN_PIN).mode(OUTPUT);
    pin(POWERDOWN_PIN).write(HIGH);
	spiSelect().mode(OUTPUT);
    spiSelect().write(HIGH);
}

void dP_SpiI2c::lowpower(bool lowpower)
{
    pin(POWERDOWN_PIN).write(lowpower);
}
