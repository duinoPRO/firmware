/*
 * dP_Sigfox.h is part of the duinoPRO firmware.
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

#ifndef DP_SIGFOX_H
#define DP_SIGFOX_H

#include <dP_Module.h>

/** \file
 */

/** \brief Class for controlling a duinoPRO Sigfox module.
 *
 */
class dP_Sigfox : public dP_Module {
    private:

    protected:

    public:
      enum NetworkModeSetting
      {
          UPONLY,	 /**< uplink only */
        	UPDOWN   /**< uplink/downlink */
      };
      enum RfFreqDomainSetting
      {
          ANZ = 3
      };
  		/** \brief Constructor for dP_Sigfox
  		 *  \param id - the 1st location of the dP_Sigfox module on the duinoPRO baseboard.
       *  \param id2 - the 2nd location of the dP_Sigfox module on the duinoPRO baseboard. (Sigfox module is a 2x1 module.)
  		 */
  		dP_Sigfox(int id, int id2);

  		/** \brief Begin using the dP_Sigfox module.
  		*/
  		void begin(void);

      void enterConfigMode(void);
      void exitConfigMode(void);

      bool waitForPrompt(int timeout);

      bool sendConfigCmd(char cmd);
      bool sendConfigCmd(char cmd, char arg);

      bool setMemoryConfigParameter(char addr, char value);

      void setNetworkMode(NetworkModeSetting networkMode);
      void setRfFreqDomain(RfFreqDomainSetting rfFreqDomain);

      int readPkt(char *rxPkt);
};

#endif /* DP_SIGFOX_H */
