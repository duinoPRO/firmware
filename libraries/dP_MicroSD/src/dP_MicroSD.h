/*
 * dP_MicroSD.h is part of the duinoPRO firmware. 
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

#ifndef DP_MICROSD_H
#define DP_MICROSD_H


#include <dP_Module.h>
#include <dP_Pin.h>

#include <utility/dP_SD.h>

/** \file
 */

/** \brief Class for controlling a duinoPRO Micro SD module.
 *
 */
class dP_MicroSD : public dP_Module {
    private:

    protected:

    public:
		/** \brief Define the dateTimeFunction type, used for pointers to functions of the form void dateTime(uint16_t \*date, uint16_t \*time).
		*/
		typedef void (*dateTimeFunction)(uint16_t*, uint16_t*);

		/** \brief Create an instance of SDClass, used to read from and write to the inserted microSD card.
		*/
        SDClass SD;

		/** \brief Constructor for dP_MicroSD
		 *  \param id - the location of the dP_MicroSD module on the duinoPRO baseboard.
		 */
	    dP_MicroSD(int id);

		/** \brief Begin using the dP_MicroSD module.
		*/
		bool begin();
		/** \brief Enable or disable low-power mode.
		 *  \param lowpower - true to enable low-power mode, false to disable.
		 */
		void lowpower(bool lowpower);

		/** \brief Read the card detect pin.
		 *  \return - true if a microSD card is inserted in the socket, false otherwise.
		 */
		bool cardDetect();

		/** \brief Select the callback function used to set file timestamps.
		 *  \param dateTime - The callback function, of the form void dateTime(uint16_t \*date, uint16_t \*time).
		 */
		void timestampCallback(dateTimeFunction dateTime);
		/** \brief Cancel the timestamp callback function.
		 */
		void timestampCancel(void);

		/** \brief List the contents of a directory on the microSD card, including the contents of subdirectories.
		 *  \param dir - The specified directory.
		 */
		void printDirectory(File dir);
		/** \brief List the contents of a directory on the microSD card, including the contents of subdirectories,
		 *  starting with a given number of tab indents. If numTabs is greater than 0, dir is treated as a subdirectory.
		 *  \param dir - The specified directory.
		 *  \param numTabs - The initial number of tab indents.
		 */
		void printDirectory(File dir, uint8_t numTabs);

};

#endif /* DP_MICROSD_H */
