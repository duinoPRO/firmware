/*
 * dP_SpiI2c.cpp is part of the duinoPRO firmware.
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

#include <dP_SpiI2c.h>


// Module Pins
#define POWERDOWN_PIN 	5
#define GPIO1			1
#define GPIO2			2


dP_SpiI2c::dP_SpiI2c(int id) : dP_Module(id)
{
	spiFreq = 4000000;
	spiOrder = MSBFIRST;
	spiMode = SPI_MODE0;
	spiReadMask = 0x00;
	spiWriteMask = 0x00;
	i2cAddr = 0x00;
};

void dP_SpiI2c::begin()
{
    pin(POWERDOWN_PIN).mode(OUTPUT);
    pin(POWERDOWN_PIN).write(LOW);
		spiSelect().mode(OUTPUT);
    spiSelect().write(HIGH);
		spi().begin();
}

void dP_SpiI2c::lowpower(bool lowpower)
{
    pin(POWERDOWN_PIN).write(lowpower);
}

void dP_SpiI2c::setSpiFreq(uint32_t freq)
{
	spiFreq = freq;
}

void dP_SpiI2c::setSpiOrder(uint8_t order)
{
	spiOrder = order;
}

void dP_SpiI2c::setSpiMode(uint8_t mode)
{
	spiMode = mode;
}

void dP_SpiI2c::setSpiRWMasks(uint8_t readMask, uint8_t writeMask)
{
	spiReadMask = readMask;
	spiWriteMask = writeMask;
}

void dP_SpiI2c::setI2cAddress(uint8_t addr)
{
	i2cAddr = addr;
}

uint8_t dP_SpiI2c::readRegisterSpi(uint8_t addr)
{
	uint8_t val;
	spi().beginTransaction(spiFreq, spiOrder, spiMode);
	spiSelect().write(LOW);
	val = spi().transfer(addr | spiReadMask);
	val = spi().transfer(0x00);
	spiSelect().write(HIGH);
	spi().endTransaction();
	return val;
}

void dP_SpiI2c::writeRegisterSpi(uint8_t addr, uint8_t data)
{
	spi().beginTransaction(spiFreq, spiOrder, spiMode);
	spiSelect().write(LOW);
	spi().transfer(addr | spiWriteMask);
	spi().transfer(data);
	spiSelect().write(HIGH);
	spi().endTransaction();
}

uint16_t dP_SpiI2c::read16BitSpi(uint8_t highAddr, uint8_t lowAddr)
{
	uint16_t val = 0;
	val = readRegisterSpi(highAddr);
	val <<= 8;
	val |= readRegisterSpi(lowAddr);
	return val;
}

int16_t dP_SpiI2c::read16BitSignedSpi(uint8_t highAddr, uint8_t lowAddr)
{
	int16_t val = 0;
	val = readRegisterSpi(highAddr);
	val <<= 8;
	val |= readRegisterSpi(lowAddr);
	return val;
}

void dP_SpiI2c::write8BitSpi(uint8_t data)
{
	spi().beginTransaction(spiFreq, spiOrder, spiMode);
	spiSelect().write(LOW);
	spi().transfer(data);
	spiSelect().write(HIGH);
	spi().endTransaction();
}

// If this method doesn't work with a connected module, try replacing
// "Wire.endTransmission();" with "Wire.endTransmission(false);" to keep
// the connection active
uint8_t dP_SpiI2c::readRegisterI2c(uint8_t addr)
{
	uint8_t val;
	wire().beginTransmission(i2cAddr);
	wire().write(addr);
	wire().endTransmission();
	wire().requestFrom(i2cAddr, 1);
	val = wire().read();
	return val;
}

void dP_SpiI2c::writeRegisterI2c(uint8_t addr, uint8_t data)
{
	wire().beginTransmission(i2cAddr);
	wire().write(addr);
	wire().write(data);
	wire().endTransmission();
}

uint16_t dP_SpiI2c::read16BitI2c(uint8_t highAddr, uint8_t lowAddr)
{
	uint16_t val = 0;
	val = readRegisterI2c(highAddr);
	val <<= 8;
	val |= readRegisterI2c(lowAddr);
	return val;
}

int16_t dP_SpiI2c::read16BitSignedI2c(uint8_t highAddr, uint8_t lowAddr)
{
	int16_t val = 0;
	val = readRegisterI2c(highAddr);
	val <<= 8;
	val |= readRegisterI2c(lowAddr);
	return val;
}

void dP_SpiI2c::write8BitI2c(uint8_t data)
{
	wire().beginTransmission(i2cAddr);
	wire().write(data);
	wire().endTransmission();
}
