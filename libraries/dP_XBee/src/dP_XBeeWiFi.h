/*
 * dP_XBeeWiFi.h is part of the duinoPRO firmware. 
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

#ifndef DP_XBEEWIFI_H
#define DP_XBEEWIFI_H

#include <dP_Module.h>

/** \file
 */

 /** \brief Values for specifying access point security type.
 */
enum SecurityEnum {
	NO_SEC,	/**< No security */
	WPA,	/**< WPA security */
	WPA2,	/**< WPA2 security */
	WEP,	/**< WEP security */
};

/** \brief Class for controlling an XBee S6B Wi-Fi RF unit connected to a duinoPRO XBee module.
 *
 */
class dP_XBeeWiFi : public dP_Module {
	private:
	HardwareSerial *ser;

	protected:

	public:
		/** \brief Constructor for dP_XBeeWiFi
		 *  \param id - the location of the dP_XBeeWiFi module on the duinoPRO baseboard.
		 */
		dP_XBeeWiFi(int id, int id2);

		/** \brief Begin using the dP_XBeeWiFi module with UART communication.
		 *	The serial port is set to module 7 mode, so communication over the USB port is not possible until this mode is changed.
		 *  \param baud - The data rate in bits per second (baud).
		*/
		void beginUart(unsigned long baud);
		/** \brief Enable or disable low-power mode.
		 *  \param lowpower - true to enable low-power mode, false to disable.
		 */
		void lowpower(bool lowpower);
		/** \brief Retrieve the serial port corresponding to the dP_XBeeWiFi module's location on the duinoPRO baseboard.
		 *  \return The serial port.
		 */
		HardwareSerial& serial(void);

		/** \brief Send a value over the serial port. The XBee unit must be in transparent mode.
		 *  \param val - The value to be sent.
		 */
		void sendVal(uint8_t val);

		/** \brief Wait for an "OK" message from the XBee unit.
		 */
		void waitForOK(void);
		/** \brief Send an AT command (with parameter) to the XBee unit.
		 *  \param command - The command to be sent.
		 *  \param param - The command parameter to be sent.
		 */
		void ATCommand(char *command, char *param);
		/** \brief Read the XBee unit's stored parameter corresponding to the given AT command.
		 *  \param command - The AT command.
		 *  \param readArray - The XBee unit's stored parameter.
		 *  \param readArrayLen - The length of the readArray string buffer.
		 */
		void ATReadCommand(char *command, char *readArray, uint8_t readArrayLen);

		/** \brief Enable or disable sending binary data to the Digi Device Cloud.
		 *  \param en - true to enable sending binary data, false to disable.
		 */
		void cloudDataEnable(bool en);
		/** \brief Read the XBee unit's Device Cloud Indicator.
		 *  \return - The Device Cloud Indicator value.
		 */
		uint8_t readCloudIndicator(void);

		/** \brief Connect to the access point with specified SSID, security type and password.
		 *	Possible security types are listed in #SecurityEnum.
		 *  If the given timeout period elapses before a connection is made, the connection attempt
		 *  terminates and an error message is sent over the USB port.
		 *  \param SSID - The access point's SSID.
		 *  \param security - Security type, from SecurityEnum.
		 *	\param password - The access point's security key (cannot include commas), or "" when the access point has no encryption.
		 *  \param timeout - Number of seconds allowed for the connection attempt.
		 */
		void accessPointConnect(char *SSID, uint8_t security, char *password, uint8_t timeout);
		/** \brief Disconnect from the connected access point.
		*/
		void accessPointDisconnect(void);

		/** \brief Set the XBee unit's power level.
		 *	This is unrelated to the dP_XBeeWiFi module's low-power mode.
		 *	\param plevel - The power level to be set, from 0 (lowest) to 4 (highest).
		 */
		void powerLevel(uint8_t plevel);

		/** \brief Ping an IP address.
		 *	\param IP - The IP address to be pinged.
		 *  \param readArray - The response time, or an error message in the case of no reply (requires a string buffer of at least 9 characters).
		 */
		void pingIP(char *IP, char *readArray);

		/** \brief Enable or disable API mode, and respectively disable or enable transparent mode.
		 *  \param en - true to enable API mode, false to disable.
		 */
		void apiEnable(bool en);
		/** \brief Send a plain text file to the Digi Device Cloud, either overwriting or appending if the file already exists.
		 *  The XBee unit must be in API mode.
		 *  \param filename - The filename of the file to be sent (up to 255 characters long, including the extension).
		 *  \param data - The contents of the file to be sent (up to 1400 characters long).
		 *  \param apppend - true to append to an existing file, false to overwrite.
		 */
		void apiFileWrite(char *filename, char *data, bool append);

};

#endif /* DP_XBEEWIFI_H */
