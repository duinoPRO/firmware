/*
 * dP_SpiI2c.h is part of the duinoPRO firmware.
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

#ifndef DP_SPII2C_H
#define DP_SPII2C_H

#include <dP_Module.h>

/** \file
 */

/** \brief Class for controlling a duinoPRO SPI/I2C Breakout module.
 *
 */
class dP_SpiI2c : public dP_Module {
    private:
		uint32_t spiFreq;
		uint8_t spiOrder;
		uint8_t spiMode;
		uint8_t i2cAddr;
		uint8_t spiReadMask;
		uint8_t spiWriteMask;

    protected:

    public:
		/** \brief Constructor for dP_SpiI2c
		 *  \param id - the location of the dP_SpiI2c module on the duinoPRO baseboard.
		 */
		dP_SpiI2c(int id);

		/** \brief Begin using the dP_SpiI2c module.
		*/
		void begin(void);
		/** \brief Enable or disable low-power mode.
		 *  \param lowpower - true to enable low-power mode, false to disable.
		 */
		void lowpower(bool lowpower);

		/** \brief Set the maximum SPI communication frequency.
		 *  \param freq - The maximum frequency in Hz.
		 */
		void setSpiFreq(uint32_t freq);
		/** \brief Set the data transfer order for SPI communication.
		 *  \param order - MSBFIRST or LSBFIRST.
		 */
		void setSpiOrder(uint8_t order);
		/** \brief Set the data mode for SPI communication.
		 *  \param mode - SPI_MODE0 (CPOL = 0, CPHA = 0), SPI_MODE1 (CPOL = 0, CPHA = 1), SPI_MODE2 (CPOL = 1, CPHA = 0) or SPI_MODE3 (CPOL = 1, CPHA = 1).
		 */
		void setSpiMode(uint8_t mode);
		/** \brief Set the read/write OR masks for SPI communication.
		 *  \param readMask - The OR mask used when reading a register.
		 *  \param writeMask - The OR mask used when writing to a register.
		 */
		void setSpiRWMasks(uint8_t readMask, uint8_t writeMask);
		/** \brief Set the address of a slave device for I2C communication.
		 *  \param addr - The address of the I2C slave device.
		 */
		void setI2cAddress(uint8_t addr);

		/** \brief Read the value of a register in the slave device using SPI communication.
		 *  \param addr - Address of the register to be read.
		 *  \return The register value.
		 */
		uint8_t readRegisterSpi(uint8_t addr);
		/** \brief Write a value to a register in the slave device using SPI communication.
		 *  \param addr - Address of the register to be written.
		 *  \param data - The value to be written to the register.
		 */
		void writeRegisterSpi(uint8_t addr, uint8_t data);
		/** \brief Read an unsigned 16-bit value from two registers in the slave device using SPI communication.
		 *  \param highAddr - Address of the register to be read containing the most significant bits.
		 *  \param lowAddr - Address of the register to be read containing the least significant bits.
		 *  \return The unsigned 16-bit value.
		 */
		uint16_t read16BitSpi(uint8_t highAddr, uint8_t lowAddr);
		/** \brief Read a signed 16-bit value from two registers in the slave device using SPI communication.
		 *  \param highAddr - Address of the register to be read containing the most significant bits.
		 *  \param lowAddr - Address of the register to be read containing the least significant bits.
		 *  \return The signed 16-bit value.
		 */
		int16_t read16BitSignedSpi(uint8_t highAddr, uint8_t lowAddr);
		/** \brief Send an 8-bit command to the slave device using SPI communication.
		 *  \param data - The command to be sent.
		 */
		void write8BitSpi(uint8_t data);

		/** \brief Read the value of a register in the slave device using I2C communication.
		 *  \param addr - Address of the register to be read.
		 *  \return The register value.
		 */
		uint8_t readRegisterI2c(uint8_t addr);
		/** \brief Write a value to a register in the slave device using I2C communication.
		 *  \param addr - Address of the register to be written.
		 *  \param data - The value to be written to the register.
		 */
		void writeRegisterI2c(uint8_t addr, uint8_t data);
		/** \brief Read an unsigned 16-bit value from two registers in the slave device using I2C communication.
		 *  \param highAddr - Address of the register to be read containing the most significant bits.
		 *  \param lowAddr - Address of the register to be read containing the least significant bits.
		 *  \return The unsigned 16-bit value.
		 */
		uint16_t read16BitI2c(uint8_t highAddr, uint8_t lowAddr);
		/** \brief Read a signed 16-bit value from two registers in the slave device using I2C communication.
		 *  \param highAddr - Address of the register to be read containing the most significant bits.
		 *  \param lowAddr - Address of the register to be read containing the least significant bits.
		 *  \return The signed 16-bit value.
		 */
		int16_t read16BitSignedI2c(uint8_t highAddr, uint8_t lowAddr);
		/** \brief Send an 8-bit command to the slave device using I2C communication.
		 *  \param data - The command to be sent.
		 */
		void write8BitI2c(uint8_t data);

};

#endif /* DP_SPII2C_H */
