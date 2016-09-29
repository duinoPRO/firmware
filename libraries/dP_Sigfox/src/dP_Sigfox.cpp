/*
 * dP_Sigfox.cpp is part of the duinoPRO firmware.
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

#include <duinoPRO.h>
#include <dP_Sigfox.h>


// Module Pins
#define CONFIG_PIN                        1
#define RESET_PIN                         6

#define RC232BAUD                         19200
#define DEFAULT_RFPOWER                   100     // 0-255.  PA step down value from the maximum power. Default setting (0x05) gives +25 dBm conducted power.

#define WAITFORPROMPT_TIMEOUT_INIT        500
#define WAITFORPROMPT_TIMEOUT             200
#define RESPONSE_TIMEOUT                  200

// CONFIG commands
#define ENTER_CONFIG_CMD                  0x00
#define EXIT_CONFIG_CMD                   0x58
#define MEMORY_CONFIG_CMD                 0x4D
#define END_MEMORY_CONFIG_CMD             0xFF

#define READ_ID_CMD                       0x39
#define CONFIGURE_ID_CMD                  0x41
#define NETWORK_MODE_CMD                  0x46
#define GET_QUALITY_CMD                   0x51
#define GET_RSSI_CMD                      0x53
#define GET_TEMP_CMD                      0x55
#define GET_BATT_VOLTAGE_CMD              0x56
#define GET_MEMORY_BYTE_CMD               0x59
#define SLEEP_MODE_CMD                    0x5A
#define EXIT_SLEEP_CMD                    0xFF
#define OOB_PKT_PERIOD_CMD                0x2F

// CONFIG MEMORY addresses
#define RFDOMAIN_CONFIG_MEMORY_ADDR       0x00
#define RFPOWER_CONFIG_MEMORY_ADDR        0x01
#define SLEEPMODE_CONFIG_MEMORY_ADDR			0x04
#define RSSIMODE_CONFIG_MEMORY_ADDR				0x05
#define TIMEOUT_CONFIG_MEMORY_ADDR				0x10
#define EOSCHAR_CONFIG_MEMORY_ADDR				0x36
#define RETRANSMIT_CONFIG_MEMORY_ADDR			0x27
#define PUBLICKEY_CONFIG_MEMORY_ADDR			0x28
#define TXDELAY_CONFIG_MEMORY_ADDR				0x2E
#define NETWORKMODE_CONFIG_MEMORY_ADDR		0x3B
#define UARTBAUD_CONFIG_MEMORY_ADDR				0x30
#define UARTFLOWCTRL_CONFIG_MEMORY_ADDR   0x35


dP_Sigfox::dP_Sigfox(int id, int id2) : dP_Module(id, id2)
{
};


void dP_Sigfox::begin()
{
	duinoPRO base;
	base.serialModuleMode();

  //disable config mode; CONFIG_PIN is active low
  pin(CONFIG_PIN).mode(OUTPUT);
  pin(CONFIG_PIN).write(HIGH);

  // set frequency to AU/NZ settings
  setRfFreqDomain(dP_Sigfox::ANZ);
	// set RF power
  setRfPower(DEFAULT_RFPOWER);

	serial().begin(RC232BAUD);
}


bool dP_Sigfox::enterConfigMode()
{
  serial().write(ENTER_CONFIG_CMD);
  if(!waitForPrompt(WAITFORPROMPT_TIMEOUT_INIT)) { return false; }

  return true;
}


void dP_Sigfox::exitConfigMode()
{
  serial().write(EXIT_CONFIG_CMD);
}


bool dP_Sigfox::waitForPrompt(int timeout)  //timeout in ~ms
{
  int timeoutCount = 0;

  while(timeoutCount < timeout)
  {
    if(serial().available())
    {
        if((char)serial().read() == '>')
        {
          return true;
        }
    }
    else
    {
      delay(1);
      timeoutCount++;
    }
  }
  return false;   // wait has timed out
}


bool dP_Sigfox::sendConfigCmd(char cmd)
{
  return sendConfigCmd(cmd, NULL, 0, NULL, 0);
}

bool dP_Sigfox::sendConfigCmd(char cmd, char arg)
{
  return sendConfigCmd(cmd, &arg, 1, NULL, 0);
}

bool dP_Sigfox::sendConfigCmd(char cmd, char *resp)
{
  return sendConfigCmd(cmd, NULL, 0, resp, 1);
}

bool dP_Sigfox::sendConfigCmd(char cmd, char arg, char *resp)
{
  return sendConfigCmd(cmd, &arg, 1, resp, 1);
}

bool dP_Sigfox::sendConfigCmd(char cmd, char *arg, int argc, char *ret, int retc)
{
  serial().write(cmd);
  serial().write((uint8_t*)arg, argc);

  int i = 0;
  for (int timeout = 0; (timeout < 300) && (i < retc);)
  {
    if(serial().available())
    {
      ret[i++] = serial().read();
      timeout = 0;
    }
    else
    {
      delay(1);
      timeout++;
    }
  }
  if (i < retc)
  {
    return false;
  }
  if (!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { return false; }
  return true;
}


bool dP_Sigfox::setMemoryConfigParameter(char addr, char value)
{
  //serial().write(MEMORY_CONFIG_CMD);
  //if(!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { return false; }
  sendConfigCmd(MEMORY_CONFIG_CMD);
  serial().write(addr);
  serial().write(value);
  sendConfigCmd(END_MEMORY_CONFIG_CMD);
  //serial().write(END_MEMORY_CONFIG_CMD);

  //if(!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { return false; }

  return true;
}


bool dP_Sigfox::getId(char *id)
{
  return sendConfigCmd(READ_ID_CMD, NULL, 0, id, 12);
}

bool dP_Sigfox::configureId(char *id)
{
  return sendConfigCmd(CONFIGURE_ID_CMD, id, 28, NULL, 0);
}

bool dP_Sigfox::setNetworkMode(SigfoxNetworkModeSetting networkMode)  // setting stored in volatile memory only
{
  return sendConfigCmd(NETWORK_MODE_CMD, networkMode);
}

bool dP_Sigfox::getQualityIndicator(char *q)
{
  return sendConfigCmd(GET_QUALITY_CMD, q);
}

bool dP_Sigfox::getRssi(char *rssi)
{
  return sendConfigCmd(GET_RSSI_CMD, rssi);
}

bool dP_Sigfox::getTemperature(char *temp)
{
  return sendConfigCmd(GET_TEMP_CMD, temp);
}

bool dP_Sigfox::getBattVoltage(char *batt)
{
  return sendConfigCmd(GET_BATT_VOLTAGE_CMD, batt);
}

bool dP_Sigfox::getMemoryByte(char addr, char *val)
{
  if((addr >= 0x00) && (addr <= 0x7F))
  {
    return sendConfigCmd(GET_MEMORY_BYTE_CMD, addr, val);
  }
  else
  {
    return false;   // addr outside of configuration memory address range
  }
}

bool dP_Sigfox::sleep()
{
  return sendConfigCmd(SLEEP_MODE_CMD);
}

void dP_Sigfox::exitSleep()
{
    serial().write(EXIT_SLEEP_CMD);
}


bool dP_Sigfox::setRfFreqDomain(SigfoxRfFreqDomainSetting rfFreqDomain)
{
  return setMemoryConfigParameter(RFDOMAIN_CONFIG_MEMORY_ADDR, rfFreqDomain);
}

bool dP_Sigfox::setRfPower(char rfPower)
{
  return setMemoryConfigParameter(RFPOWER_CONFIG_MEMORY_ADDR, rfPower);
}

bool dP_Sigfox::setSleepMode(SigfoxSleepMode sleepMode)
{
	return setMemoryConfigParameter(SLEEPMODE_CONFIG_MEMORY_ADDR, sleepMode);
}

bool dP_Sigfox::setRssiMode(bool rssiEnable)		// append RSSI to received data
{
	return setMemoryConfigParameter(RSSIMODE_CONFIG_MEMORY_ADDR, rssiEnable);
}

bool dP_Sigfox::setUartTimeout(char timeout)	// time to wait to complete a UART message
{
	return setMemoryConfigParameter(TIMEOUT_CONFIG_MEMORY_ADDR, timeout);	//check arg??
}

bool dP_Sigfox::setRetransmissionCount(char count)
{
	if((count >= 0) && (count <= 2))
	{
		return setMemoryConfigParameter(RETRANSMIT_CONFIG_MEMORY_ADDR, count);
	}
	else
	{
		return false;		// retransmission count argument must be between 0 and 2 inclusive
	}
}

// for test and dev purposes
bool dP_Sigfox::enablePublicKey(bool publicKeyEnable)
{
	return setMemoryConfigParameter(PUBLICKEY_CONFIG_MEMORY_ADDR, publicKeyEnable);		// 0: unique ID+KEY, 1: public ID+KEY
}

bool dP_Sigfox::setTxRetransmissionDelay(char delay)		// delay in 10ms
{
	if((delay >= 0) && (delay <= 200))
	{
		return setMemoryConfigParameter(TXDELAY_CONFIG_MEMORY_ADDR, delay);
	}
	else
	{
		return false;		// delay argument outside of allowed range
	}
}

bool dP_Sigfox::saveNetworkMode(SigfoxNetworkModeSetting networkMode)
{
  return setMemoryConfigParameter(NETWORKMODE_CONFIG_MEMORY_ADDR, networkMode);
}

bool dP_Sigfox::setUartBaudRate(char baud)
{
/* Baud rate settings:
	0x01: 2400
	0x02: 4800
	0x03: 9600
	0x04: 14400
	0x05: 19200
	0x06: 28800
	0x07: 38400
	0x08: 57600
	0x09: 76800
	0x0A: 115200
	0x0B: 230400  */
	if((baud >= 0x01) && (baud <= 0x0B))
	{
		return setMemoryConfigParameter(UARTBAUD_CONFIG_MEMORY_ADDR, baud);
	}
	else
	{
		return false;		// baud rate argument outside of permitted range
	}

}

bool dP_Sigfox::setUartFlowControl(SigfoxUartFlowControl flowControl)
{
	return setMemoryConfigParameter(UARTFLOWCTRL_CONFIG_MEMORY_ADDR, flowControl);
}


int dP_Sigfox::readPkt(char *rxPkt)
{
  if(serial().available())
  {
    int len = serial().peek();
    if(serial().available() > len)
    {
      serial().read();  //discard length byte
      for(int i = 0; i < len; i++)
      {
        rxPkt[i] = serial().read();
      }
      return len;
    }
  }
  return 0;
}


void dP_Sigfox::transmitPkt(char *txPayload, char payloadLen)
{
  //send length byte
  serial().write(payloadLen);
  //send payload
  for(int i = 0; i < payloadLen; i++)
  {
    serial().write(txPayload[i]);
  }
}

void dP_Sigfox::transmitSingleBit(bool data)
{
  //send length byte
  serial().write(0x10);
  //send payload
  serial().write(data ? 0x01 : 0x00); // data is one byte where only the LSB is relevant
}


bool dP_Sigfox::setOutOfBandPktPeriod(char period)
{
  if((period >= 0) && (period <= 15))
  {
    return setMemoryConfigParameter(OOB_PKT_PERIOD_CMD, period);     // 0: disabled, 1-15: hours
  }
  else
  {
    return false;   // period argument outside of allowed range
  }
}

void dP_Sigfox::disableOutOfBandPkt()
{
  setOutOfBandPktPeriod(0);
}
