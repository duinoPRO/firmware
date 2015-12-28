#ifndef DP_MICROSD_H
#define DP_MICROSD_H

#include <Spi.h>
#include <Module.h>
#include <pin.h>

#include <utility/dP_SD.h>

 
class dP_MicroSD : public Module {
    private:
	
    protected:
	
    public:
        SDClass SD;
    
	    dP_MicroSD(int id);
	
		bool begin();
	
		// Returns true if there is a dP_MicroSD card inserted in the socket, false otherwise.
		bool cardDetect();
		void lowpower(bool lowpower);
		
		void timestampCallback(void (*dateTime)(uint16_t* date, uint16_t* time));
		void timestampCancel(void);
		
		void printDirectory(File dir, int numTabs);
};

#endif /* DP_MICROSD_H */