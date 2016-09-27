/*
 * dP_XBee.h is part of the duinoPRO firmware. 
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

#ifndef DP_XBEE_H
#define DP_XBEE_H

#include <dP_Module.h>

#define XBEE_MAX_PACKET_SIZE	(20)


/** \file
 */

 /** \brief Values for specifying communication method.
 */
enum CommEnum {
	SERIAL_COMM,	/**< serial (UART) communication */
	SPI_COMM,	/**< SPI communication */
};

/** \brief Class for controlling an XBee S2C ZB RF unit connected to a duinoPRO XBee module.
 *
 */
class dP_XBee : public dP_Module {
	private:
		char rxPkt[XBEE_MAX_PACKET_SIZE+1];
		uint8_t rxPktLen;
		uint8_t comm;
		//HardwareSerial *ser;
		//dP_Module modSpi;

	protected:

	public:
		/** \brief The 64-bit address for a ZigBee network's coordinator.
		 */
		const uint8_t coordinatorAddress[8] = { 0,0,0,0,0,0,0,0 };
		/** \brief The 64-bit broadcast address for a ZigBee network.
		 */
		const uint8_t broadcastAddress[8] = { 0,0,0,0,0,0,0xFF,0xFF };

		/** \brief Constructor for dP_XBee.
		 *  \param id - The higher location number of the dP_XBee module on the duinoPRO baseboard.
		 */
		dP_XBee(int id, int id2);

		/** \brief Begin using the dP_XBee module.
		*/
		void begin(void);
		/** \brief Begin using the dP_XBee module with serial (UART) communication.
		 *	The serial port is set to module 7 mode, so communication over the USB port is not possible until this mode is changed.
		 *  \param baud - The data rate in bits per second (baud).
		 */
		void beginSerial(unsigned long baud);
		/** \brief Begin using the dP_XBee module with SPI communication.
		*/
		void beginSpi(void);

		/** \brief Retrieve the serial port corresponding to the dP_XBee module's location on the duinoPRO baseboard.
		 *  \return The serial port.
		 */
		//HardwareSerial& serial(void);

		/** \brief Retrieve the XBee module's SPI chip select pin.
		 *  \return - The chip select pin.
		 */
		//Pin& spiSelectXBee();

		/** \brief Enable or disable low-power mode.
		 *  \param lowpower - true to enable low-power mode, false to disable.
		 */
		void lowpower(bool lowpower);

		/** \brief Configure the XBee unit's SPI pins.
		 *  UART communication in transparent mode is required.
		 */
		void setupSpi(void);

		/** \brief Read an incoming data byte using UART or SPI communication.
		 *  \return The data byte.
		 */
		uint8_t readByte(void);
		/** \brief Determine the number of bytes available for reading from the appropriate serial port.
		 *  \return The number of bytes available for reading.
		 */
		uint16_t serialAvailable(void);
		/** \brief Transmit a data byte using UART or SPI communication.
		*/
		void writeByte(uint8_t data);

		/** \brief Read an incoming ZigBee Receive Packet using UART or SPI communication.
		 *  API mode is required.
		 *  Packets are dropped if they are too long or otherwise invalid.
		 *  \return true if a valid packet has been read successfully, false otherwise.
		 */
		bool readPacket(void);
		/** \brief Retrieve the most recent valid packet read by #readPacket.
		 *  \return The most recently read valid packet.
		 */
		char *lastPacket(void);

		/** \brief Transmit a data packet using UART or SPI communication.
		 *  API mode is required.
		 *  \param txPkt - The data to be sent.
		 *  \param destAddr - The 64-bit destination address, e.g. #coordinatorAddress or #broadcastAddress.
		 */
		void sendPacket(char *txPkt, const uint8_t *destAddr);
		/** \brief Send a string over the serial port.
		 *  UART communication in transparent mode is required.
		 *  \param txStr - The string to be sent.
		 */
		void sendString(char *txStr);

		/** \brief Wait for an "OK" message from the XBee unit.
		 *  UART communication is required.
		 */
		void waitForOK(void);
		/** \brief Send an AT command (with parameter) to the XBee unit.
		 *  UART communication is required.
		 *  \param command - The command to be sent.
		 *  \param param - The command parameter to be sent.
		 */
		void ATCommand(char *command, char *param);
		/** \brief Read the XBee unit's stored parameter corresponding to the given AT command.
		 *  UART communication is required.
		 *  \param command - The AT command.
		 *  \param readArray - The XBee unit's stored parameter.
		 *  \param readArrayLen - The length of the readarray string buffer.
		 */
		void ATReadCommand(char *command, char *readArray, uint8_t readArrayLen);

		/** \brief Enable or disable API mode, and respectively disable or enable transparent mode.
		 *  Applies only to (and requires) UART communication; SPI communication uses API mode only.
		 *  \param en - true to enable API mode, false to disable.
		 */
		void apiEnable(bool en);
};

#endif /* DP_XBEE_H */
