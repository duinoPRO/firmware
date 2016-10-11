/*
 * dP_Sigfox.h is part of the duinoPRO firmware.
 *
 * duinoPRO is an Arduino™-compatible platform in a flat form factor with
 * surface-mount,
 * solderable modules. It is designed with commercialization of real products in
 * mind.
 * Note that we have designed duinoPRO to be compatible with the Arduino™ IDE.
 * This does
 * not imply that duinoPRO is certified, tested or endorsed by Arduino™ in any
 * way.
 *
 * For more information, contact info@duinopro.cc or visit www.duinopro.cc.
 *
 * This file is licensed under the BSD 3-Clause license
 * (see
 * https://github.com/duinoPRO/firmware/blob/master/duinoPRO_BSD_fwlicense.txt).
 *
 * Using duinoPRO core and libraries licensed under BSD for the firmware of a
 * commercial
 * product does not require you to release the source code for the firmware.
 *
*/

#ifndef DP_SIGFOX_H
#define DP_SIGFOX_H

#include <dP_Module.h>
#include <SoftwareSerial.h>

/** \file
 */

/** \brief Class for controlling a duinoPRO Sigfox module.
 *
 */
class dP_Sigfox : public dP_Module
{
    private:
        SoftwareSerial _softSerial;

        void serialBegin(unsigned long baud);
        SoftwareSerial& softSerial();
        size_t serialWrite(uint8_t data);
        size_t serialWrite(unsigned long data);
        size_t serialWrite(long data);
        size_t serialWrite(unsigned int data);
        size_t serialWrite(int data);
        size_t serialWrite(const char *str);
        size_t serialWrite(const uint8_t *buff, size_t size);
        int serialAvailable(void);
        int serialPeek(void);
        int serialRead(void);
    protected:
    public:
        enum SigfoxNetworkModeSetting {
            UPONLY, /**< uplink only */
            UPDOWN, /**< uplink/downlink */
        };
        enum SigfoxRfFreqDomainSetting {
            ANZ = 3,
        };
        enum SigfoxSleepMode {
            DISABLE = 0,
            ENABLE = 1,
            AUTO = 5,
        };
        enum SigfoxUartFlowControl {
            NONE = 0,
            CTSONLY = 1,
            CTSRTS = 2,
            RXTX = 3,
        };
        /** \brief Constructor for dP_Sigfox
        *  \param id - the 1st location of the dP_Sigfox module on the duinoPRO
        * baseboard.
        *  \param id2 - the 2nd location of the dP_Sigfox module on the duinoPRO
        * baseboard. (Sigfox module is a 2x1 module.)
        */
        dP_Sigfox(int id, int id2);

        /** \brief Begin using the dP_Sigfox module.
        */
        void begin(void);

        bool enterConfigMode(void);
        void exitConfigMode(void);

        bool waitForPrompt(int timeout);

        bool sendConfigCmd(char cmd);
        bool sendConfigCmd(char cmd, char arg);
        bool sendConfigCmd(char cmd, char *resp);
        bool sendConfigCmd(char cmd, char arg, char *resp);
        bool sendConfigCmd(char cmd, char *arg, int argc, char *ret, int retc);

        bool setMemoryConfigParameter(char addr, char value);

        // CONFIG (VOLATILE) settings methods
        bool getId(char *id);
        bool configureId(char *id);
        bool setNetworkMode(SigfoxNetworkModeSetting networkMode);
        bool getQualityIndicator(char *q);
        bool getRssi(char *rssi);
        bool getTemperature(char *temp);
        bool getBattVoltage(char *batt);
        bool getMemoryByte(char addr, char *val);
        bool sleep(void);
        void exitSleep(void);

        // CONFIG MEMORY (NON-VOLATILE) settings methods
        bool setRfFreqDomain(SigfoxRfFreqDomainSetting rfFreqDomain); // make private?
        bool setRfPower(char rfPower);                           // make private?
        bool setSleepMode(SigfoxSleepMode sleepMode);
        bool setRssiMode(bool rssiEnable);
        bool setUartTimeout(char timeout);
        bool setRetransmissionCount(char count);
        bool enablePublicKey(bool publicKeyEnable); // for test and dev purposes
        bool setTxRetransmissionDelay(char delay);
        bool saveNetworkMode(SigfoxNetworkModeSetting networkMode);
        bool setUartBaudRate(char baud);
        bool setUartFlowControl(SigfoxUartFlowControl flowControl);

        int readPkt(char *rxPkt);

        void transmitPkt(char *txPayload, char payloadLen);
        void transmitSingleBit(bool data);

        bool setOutOfBandPktPeriod(char period);
        void disableOutOfBandPkt(void);
};

#endif /* DP_SIGFOX_H */
