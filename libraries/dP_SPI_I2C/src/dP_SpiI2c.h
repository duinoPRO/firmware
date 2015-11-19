#ifndef DP_SPII2C_H
#define DP_SPII2C_H

#include <Module.h>

/** \file
 */
 


/** \brief Class for controlling an dP_SpiI2c (SPI/I2C Breakout) module.
 *  
 */
class dP_SpiI2c : public Module {
    private:
		
    protected:
		
    public:
		/** \brief Constructor for dP_SpiI2c
		 *  \param id - the location of the dP_SpiI2c module on the duinoPRO baseboard.
		 */
		dP_SpiI2c(int id);
	
		void begin(void);
		void lowpower(bool lowpower);
		
};

#endif /* DP_SPII2C_H */