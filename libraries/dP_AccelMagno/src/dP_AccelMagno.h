/*
 * dP_AccelMagno.h is part of the duinoPRO firmware. 
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

#ifndef DP_ACCELMAGNO_H
#define DP_ACCELMAGNO_H

#include <dP_Module.h>

/** \file
 */

/** \brief Values for controlling accelerometer full scale range.
 */
enum AccelFullScaleEnum {
	ACCEL_2G,	/**< Full scale range of 2g */
	ACCEL_4G,	/**< Full scale range of 4g */
	ACCEL_6G,	/**< Full scale range of 6g */
	ACCEL_8G,	/**< Full scale range of 8g */
	ACCEL_16G	/**< Full scale range of 16g */
};

/** \brief Values for controlling magnetometer full scale range.
 */
enum MagnoFullScaleEnum {
	MAGNO_2GAUSS, /**< Full scale range of 2 gauss */
	MAGNO_4GAUSS, /**< Full scale range of 4 gauss */
	MAGNO_8GAUSS, /**< Full scale range of 8 gauss */
	MAGNO_12GAUSS /**< Full scale range of 12 gauss */
};

/** \brief Class for controlling a duinoPRO Accelerometer/Magnetometer module.
 *
 */
class dP_AccelMagno : public dP_Module {
    private:
		uint8_t accelFullScale;
		float accelLsb;
		uint8_t magnoFullScale;
		float magnoLsb;

    protected:
		/** \brief Read a signed 16-bit value from two registers in the Accelerometer/Magnetometer chip.
		 *  \param highAddr - Address of the register to be read containing the most significant bits.
		 *  \param lowAddr - Address of the register to be read containing the least significant bits.
		 *  \return The 16-bit value.
		 */
		int16_t read16Bit(uint8_t highAddr, uint8_t lowAddr);

    public:
		/** \brief Constructor for dP_AccelMagno
		 *  \param id - the location of the dP_AccelMagno module on the duinoPRO baseboard.
		 */
		dP_AccelMagno(int id);

		/** \brief Begin using the dP_AccelMagno module.
		*/
		void begin(void);
		/** \brief Enable or disable low-power mode.
		 *  \param lowpower - true to enable low-power mode, false to disable.
		 */
		void lowPowerMode(bool lowpower);

		/** \brief Read the value of a register in the LDM303D chip.
		 *  \param addr - Address of the register to be read.
		 *  \return The register value.
		 */
		uint8_t readRegister(uint8_t addr);
		/** \brief Write a value to a register in the LDM303D chip.
		 *  \param addr - Address of the register to be written.
		 *  \param data - The value to be written to the register.
		 */
		void writeRegister(uint8_t addr, uint8_t data);
		/** \brief Enable accelerometer.
		 */
		void enableAccel();
    /** \brief Disable accelerometer.
     */
    void disableAccel();
		/** \brief Set the full-scale range of the accelerometer.
		 *  The allowable ranges are from +/-2g to +/-16g.  Possible values are
		 *  listed in #AccelFullScaleEnum.
		 *  \param mode - Value for full-scale range, from AccelFullScaleEnum.
		 */
		void setAccelFullScale(uint8_t mode);
		/** \brief Enable magnetometer.
		 */
		void enableMagno();
    /** \brief Disable magnetometer.
     */
    void disableMagno();
		/** \brief Set the full-scale range of the magnetometer.
		 *  The allowable ranges are from +/-2 Gauss to +/-12 Gauss.  Possible values are
		 *  listed in #MagnoFullScaleEnum.
		 *  \param mode - Value for full-scale range, from MagnoFullScaleEnum.
		 */
		void setMagnoFullScale(uint8_t mode);

		/** \brief Raw X axis accelerometer reading.
		 *  \return The raw reading.
		 */
		int16_t rawAccelX();
		/** \brief Raw Y axis accelerometer reading.
		 *  \return The raw reading.
		 */
		int16_t rawAccelY();
		/** \brief Raw Z axis accelerometer reading.
		 *  \return The raw reading.
		 */
		int16_t rawAccelZ();
		/** \brief X axis accelerometer reading, expressed as a floating point number in mg.
		 *  This result is scaled correctly to allow for the accelerometer range in use.
		 *  \return The scaled reading.
		 */
		float accelX();
		/** \brief Y axis accelerometer reading, expressed as a floating point number in mg.
		 *  This result is scaled correctly to allow for the accelerometer range in use.
		 *  \return The scaled reading.
		 */
		float accelY();
		/** \brief Z axis accelerometer reading, expressed as a floating point number in mg.
		 *  This result is scaled correctly to allow for the accelerometer range in use.
		 *  \return The scaled reading.
		 */
		float accelZ();

		/** \brief X axis magnetometer reading, expressed as a floating point number in mgauss.
		 *  This result is scaled correctly to allow for the magnetometer range in use.
		 *  \return The scaled reading.
		 */
		float magnoX();
		/** \brief Y axis magnetometer reading, expressed as a floating point number in mgauss.
		 *  This result is scaled correctly to allow for the magnetometer range in use.
		 *  \return The scaled reading.
		 */
		float magnoY();
		/** \brief Z axis magnetometer reading, expressed as a floating point number in mgauss.
		 *  This result is scaled correctly to allow for the magnetometer range in use.
		 *  \return The scaled reading.
		 */
		float magnoZ();

		/** \brief Read the specified interrupt signal of the LDM303D chip.
		 *  \param intNum - Number of the interrupt signal (1 or 2) to be read.
		 *  \return The state of the interrupt signal.
		 */
		bool intRead(uint8_t intNum);
};

#endif /* dP_AccelMagno_H */
