#include <dP_SpiI2c.h>
#include <SPI.h>
#include <Wire.h>

// Module Pins
#define POWERDOWN_PIN 	5
#define GPIO1			1
#define GPIO2			2

dP_SpiI2c::dP_SpiI2c(int id) : Module(id) 
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
}

void dP_SpiI2c::lowpower(bool lowpower)
{
    pin(POWERDOWN_PIN).write(lowpower);
}

// freq is in Hz
void dP_SpiI2c::setSpiFreq(uint32_t freq)
{
	spiFreq = freq;
}

// order is LSBFIRST or MSBFIRST
void dP_SpiI2c::setSpiOrder(uint8_t order)
{
	spiOrder = order;
}

// mode is SPI_MODE0, SPI_MODE1, SPI_MODE2 or SPI_MODE3
void dP_SpiI2c::setSpiMode(uint8_t mode)
{
	spiMode = mode;
}

// Set OR masks for read/write addresses
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
	SPI.beginTransaction(SPISettings(spiFreq, spiOrder, spiMode));
	spiSelect().write(LOW);
	val = SPI.transfer(addr | spiReadMask);
	val = SPI.transfer(0x00);
	spiSelect().write(HIGH);
	SPI.endTransaction();
	return val;
}

void dP_SpiI2c::writeRegisterSpi(uint8_t addr, uint8_t data)
{
	SPI.beginTransaction(SPISettings(spiFreq, spiOrder, spiMode));
	spiSelect().write(LOW);
	SPI.transfer(addr | spiWriteMask);
	SPI.transfer(data);
	spiSelect().write(HIGH);
	SPI.endTransaction();
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
	SPI.beginTransaction(SPISettings(spiFreq, spiOrder, spiMode));
	spiSelect().write(LOW);
	SPI.transfer(data);
	spiSelect().write(HIGH);
	SPI.endTransaction();
}

// If this method doesn't work with a connected module, try replacing
// "Wire.endTransmission();" with "Wire.endTransmission(false);" to keep
// the connection active
uint8_t dP_SpiI2c::readRegisterI2c(uint8_t addr)
{
	uint8_t val;
	Wire.beginTransmission(i2cAddr);
	Wire.write(addr);
	Wire.endTransmission();
	Wire.requestFrom(i2cAddr, 1);
	val = Wire.read();
	return val;
}

void dP_SpiI2c::writeRegisterI2c(uint8_t addr, uint8_t data)
{
	Wire.beginTransmission(i2cAddr);
	Wire.write(addr);
	Wire.write(data);
	Wire.endTransmission();
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
	Wire.beginTransmission(i2cAddr);
	Wire.write(data);
	Wire.endTransmission();
}