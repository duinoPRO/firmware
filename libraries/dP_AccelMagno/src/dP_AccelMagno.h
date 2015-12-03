#ifndef DP_ACCELMAGNO_H
#define DP_ACCELMAGNO_H

#include <Module.h>

/** \file
 */
 
/** \brief Values for controlling accelerometer full scale range.
 */
enum AccelFullScaleEnum {
	ACCEL_2G,	/**< Full scale range of 2G */
	ACCEL_4G,	/**< Full scale range of 4G */
	ACCEL_6G,	/**< Full scale range of 6G */
	ACCEL_8G,	/**< Full scale range of 8G */
	ACCEL_16G	/**< Full scale range of 16G */
};

enum MagnoFullScaleEnum {
	MAGNO_2GAUSS,
	MAGNO_4GAUSS,
	MAGNO_8GAUSS,
	MAGNO_12GAUSS
};

/** \brief Class for controlling an dP_AccelMagno (Accelerometer/Magnetometer) module.
 *  
 */
class dP_AccelMagno : public Module {
    private:
		uint8_t accelFullScale;
		float accelLsb;
		uint8_t magnoFullScale;
		float magnoLsb;
		
    protected:
		int16_t read16bit(uint8_t highAddr, uint8_t lowAddr);
		int16_t read12bit(uint8_t highAddr, uint8_t lowAddr);	
		
    public:
		/** \brief Constructor for dP_AccelMagno
		 *  \param id - the location of the dP_AccelMagno module on the duinoPRO baseboard.
		 */
		dP_AccelMagno(int id);
	
		void begin(void);
		void lowpower(bool lowpower);
		
		/** \brief Read the value of a register in the Accelerometer/Magnetometer chip.
		 *  \param addr - Address of the register to be read.
		 *  \return The register value.
		 */
		uint8_t readregister(uint8_t addr);
		/** \brief Write a value to a register in the Accelerometer/Magnetometer chip.
		 *  \param addr - Address of the register to be written.
		 *  \param data - The value to be written to the register.
		 *  \return ??? I have no idea
		 */
		uint8_t writeregister(uint8_t addr, uint8_t data);
		/** \brief Enable or disable accelerometer.
		 *  \param en - TRUE to enable accelerometer, FALSE to disable.
		 */
		void enableAccel(bool en);
		/** \brief Set the full-scale range of the accelerometer.
		 *  The allowable ranges are from 2G to 16G.  Possible values are 
		 *  listed in #AccelFullScaleEnum.
		 *  \param mode - Value for full-scale range, from AccelFullScaleEnum.
		 */
		void setAccelFullScale(uint8_t mode);
		/** \brief Enable or disable magnetometer.
		 *  \param en - TRUE to enable magnetometer, FALSE to disable.
		 */
		void enableMagno(bool en);
		/** \brief Set the full-scale range of the magnetometer.
		 *  The allowable ranges are from 2 Gauss to 12 Gauss.  Possible values are 
		 *  listed in #MagnoFullScaleEnum.
		 *  \param mode - Value for full-scale range, from MagnoFullScaleEnum.
		 */
		void setMagnoFullScale(uint8_t mode);	
		
		int16_t rawAccelX();
		int16_t rawAccelY();
		int16_t rawAccelZ();
		/** \brief X axis accelerometer reading, expressed as a floating point number in g. 
		 *  This result is scaled correctly to allow for the accelerometer range in use.
		 */
		float accelX();
		/** \brief Y axis accelerometer reading, expressed as a floating point number in g. 
		 *  This result is scaled correctly to allow for the accelerometer range in use.
		 */
		float accelY();
		/** \brief X axis accelerometer reading, expressed as a floating point number in g. 
		 *  This result is scaled correctly to allow for the accelerometer range in use.
		 */
		float accelZ();
		
		float magnoX();
		float magnoY();
		float magnoZ();
};

#endif /* dP_AccelMagno_H */
