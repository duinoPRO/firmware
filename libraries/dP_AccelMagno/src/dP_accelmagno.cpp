
#include <dP_AccelMagno.h>
#include <SPI.h>

// Module Pins
#define POWERDOWN_PIN 5

// Register Address Definitions
#define TEMP_OUT_L		0x05
#define TEMP_OUT_H		0x06
#define STATUS_M		0x07
#define OUT_X_L_M		0x08
#define OUT_X_H_M		0x09
#define OUT_Y_L_M		0x0A
#define OUT_Y_H_M		0x0B
#define OUT_Z_L_M		0x0C
#define OUT_Z_H_M		0x0D
#define WHO_AM_I		0x0F
#define INT_CTRL_M		0x12
#define INT_SRC_M		0x13
#define INT_THS_L_M		0x14
#define INT_THS_H_M		0x15
#define OFFSET_X_L_M	0x16
#define OFFSET_X_H_M	0x17
#define OFFSET_Y_L_M	0x18
#define OFFSET_Y_H_M	0x19
#define OFFSET_Z_L_M	0x1A
#define OFFSET_Z_H_M	0x1B
#define REFERENCE_X		0x1C
#define REFERENCE_Y		0x1D
#define REFERENCE_Z		0x1E
#define CTRL0			0x1F
#define CTRL1			0x20
#define CTRL2			0x21
#define CTRL3			0x22
#define CTRL4			0x23
#define CTRL5			0x24
#define CTRL6			0x25
#define CTRL7			0x26
#define STATUS_A		0x27
#define OUT_X_L_A		0x28
#define OUT_X_H_A		0x29
#define OUT_Y_L_A		0x2A
#define OUT_Y_H_A		0x2B
#define OUT_Z_L_A		0x2C
#define OUT_Z_H_A		0x2D
#define FIFO_CTRL		0x2E
#define FIFO_SRC		0x2F
#define IG_CFG1			0x30
#define IG_SRC1			0x31
#define IG_THS1			0x32
#define IG_DUR1			0x33
#define IG_CFG2			0x34
#define IG_SRC2			0x35
#define IG_THS2			0x36
#define IG_DUR2			0x37
#define CLICK_CFG		0x38
#define CLICK_SRC		0x39
#define CLICK_THS		0x3A
#define TIME_LIMIT		0x3B
#define TIME_LATENCY	0x3C
#define TIME_WINDOW		0x3D
#define ACT_THS			0x3E
#define ACT_DUR			0x3F


// Accelerometer LSB values, in mg.
#define ACCEL_2G_LSB (0.061)
#define ACCEL_4G_LSB (0.122)
#define ACCEL_6G_LSB (0.183)
#define ACCEL_8G_LSB (0.244)
#define ACCEL_16G_LSB (0.732)

// Magnetometer LSB values, in mgauss.
#define MAGNO_2G_LSB (0.080)
#define MAGNO_4G_LSB (0.160)
#define MAGNO_8G_LSB (0.320)
#define MAGNO_12G_LSB (0.479)


// SPI Port Characteristics
#define MY_SPEED_MAX 	(10000000)
#define MY_DATA_ORDER  	(MSBFIRST)
#define MY_DATA_MODE	(SPI_MODE3)

dP_AccelMagno::dP_AccelMagno(int id) : Module(id) 
{
	accelFullScale = ACCEL_2G;
	accelLsb = ACCEL_2G_LSB;
	magnoFullScale = MAGNO_4GAUSS;
	magnoLsb = MAGNO_4G_LSB;
};

void dP_AccelMagno::begin()
{
    pin(POWERDOWN_PIN).mode(OUTPUT);
    pin(POWERDOWN_PIN).write(HIGH);
	spiSelect().mode(OUTPUT);
    spiSelect().write(HIGH);
}

void dP_AccelMagno::lowpower(bool lowpower)
{
    pin(POWERDOWN_PIN).write(lowpower);
}

uint8_t dP_AccelMagno::readregister(uint8_t addr)
{
	uint8_t val;
	SPI.beginTransaction(SPISettings(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE));
	spiSelect().write(LOW);
	val = SPI.transfer((addr & 0x3F) | 0x80);
	val = SPI.transfer(0x00);
	spiSelect().write(HIGH);
	SPI.endTransaction();
	return val;
}

uint8_t dP_AccelMagno::writeregister(uint8_t addr, uint8_t data)
{
	uint8_t val;
	SPI.beginTransaction(SPISettings(MY_SPEED_MAX, MY_DATA_ORDER, MY_DATA_MODE));
	spiSelect().write(LOW);
	val = SPI.transfer(addr & 0x3F);
	val = SPI.transfer(data);
	spiSelect().write(HIGH);
	SPI.endTransaction();
	return val;
}

int16_t dP_AccelMagno::read16bit(uint8_t highAddr, uint8_t lowAddr)
{
	int16_t val=0;
	
	val = readregister(highAddr);
	val <<= 8;
	val |= readregister(lowAddr);
	return val;
}
	
int16_t dP_AccelMagno::read12bit(uint8_t highAddr, uint8_t lowAddr)
{
}


void dP_AccelMagno::enableAccel(bool en)
{
	if (en)
	{
		writeregister(CTRL1, 0x67);   // 100Hz, XYZ on
	}
	else
	{
		writeregister(CTRL1, 0x00);
	}
}

void dP_AccelMagno::setAccelFullScale(uint8_t mode)
{
	uint8_t val;
	
	switch (mode) {
		case ACCEL_2G:
			accelFullScale = ACCEL_2G;
			accelLsb = ACCEL_2G_LSB;
			break;
		case ACCEL_4G:
			accelFullScale = ACCEL_4G;
			accelLsb = ACCEL_4G_LSB;
			break;
		case ACCEL_6G:
			accelFullScale = ACCEL_6G;
			accelLsb = ACCEL_6G_LSB;
			break;
		case ACCEL_8G:
			accelFullScale = ACCEL_8G;
			accelLsb = ACCEL_8G_LSB;
			break;
		case ACCEL_16G:
			accelFullScale = ACCEL_16G;
			accelLsb = ACCEL_16G_LSB;
			break;
		default:
			accelFullScale = ACCEL_2G;
			accelLsb = ACCEL_2G_LSB;
			break;
	}
	val = readregister(CTRL2);
	val &= 0xC7;
	val |= (accelFullScale << 3);
	writeregister(CTRL2, val);
}

void dP_AccelMagno::enableMagno(bool en)
{
	uint8_t val;
	
	if (en)
	{
		val = readregister(CTRL5);
		writeregister(CTRL5, 0xF0);
		// writeregister(CTRL5, (val & 0xE3) | 0x10);   // 50Hz
		val = readregister(CTRL7);
		writeregister(CTRL7, (val & 0xF8) | 0x00);   // 
	}
	else
	{
		val = readregister(CTRL7);
		writeregister(CTRL7, (val & 0xF8) | 0x07);   // 
	}
}


void dP_AccelMagno::setMagnoFullScale(uint8_t mode)
{
	uint8_t val;
	
	switch (mode) {
		case MAGNO_2GAUSS:
			magnoFullScale = MAGNO_2GAUSS;
			magnoLsb = MAGNO_2G_LSB;
			break;
		case MAGNO_4GAUSS:
			magnoFullScale = MAGNO_4GAUSS;
			magnoLsb = MAGNO_4G_LSB;
			break;
		case MAGNO_8GAUSS:
			magnoFullScale = MAGNO_8GAUSS;
			magnoLsb = MAGNO_8G_LSB;
			break;
		case MAGNO_12GAUSS:
			magnoFullScale = MAGNO_12GAUSS;
			magnoLsb = MAGNO_12G_LSB;
			break;
		default:
			magnoFullScale = MAGNO_2GAUSS;
			magnoLsb = MAGNO_2G_LSB;
			break;
	}

	val |= (magnoFullScale << 5);
	writeregister(CTRL6, val);
}




int16_t dP_AccelMagno::rawAccelX()
{
	return read16bit(OUT_X_H_A, OUT_X_L_A);
}

int16_t dP_AccelMagno::rawAccelY()
{
	return read16bit(OUT_Y_H_A, OUT_Y_L_A);
}

int16_t dP_AccelMagno::rawAccelZ()
{
	return read16bit(OUT_Z_H_A, OUT_Z_L_A);
}

float dP_AccelMagno::accelX()
{
	return read16bit(OUT_X_H_A, OUT_X_L_A) * accelLsb;
}

float dP_AccelMagno::accelY()
{
	return read16bit(OUT_Y_H_A, OUT_Y_L_A) * accelLsb;
}

float dP_AccelMagno::accelZ()
{
	return read16bit(OUT_Z_H_A, OUT_Z_L_A) * accelLsb;  
}

float dP_AccelMagno::magnoX()
{
	return read16bit(OUT_X_H_M, OUT_X_L_M) * magnoLsb;
}

float dP_AccelMagno::magnoY()
{
	return read16bit(OUT_Y_H_M, OUT_Y_L_M) * magnoLsb;
}

float dP_AccelMagno::magnoZ()
{
	return read16bit(OUT_Z_H_M, OUT_Z_L_M) * magnoLsb;  
}

