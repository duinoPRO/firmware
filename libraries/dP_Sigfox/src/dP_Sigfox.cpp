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
#define CONFIG_PIN                    1
#define RESET_PIN                     6

#define RC232BAUD                     19200
#define DEFAULT_RFPOWER               100     // 0-255.  PA step down value from the maximum power. Default setting (0x05) gives +25 dBm conducted power.

#define WAITFORPROMPT_TIMEOUT_INIT    500
#define WAITFORPROMPT_TIMEOUT         200

// CONFIG parameters
#define ENTER_CONFIG_CMD              0x00
#define EXIT_CONFIG_CMD               0x58
#define MEMORY_CONFIG_CMD             0x4D
#define END_MEMORY_CONFIG_CMD         0xFF


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

  //set frequency to AU/NZ settings
  setRfFreqDomain(dP_Sigfox::ANZ);
  setRfPower(DEFAULT_RFPOWER);

	serial().begin(RC232BAUD);
}


void dP_Sigfox::enterConfigMode()
{
  serial().write(ENTER_CONFIG_CMD);
  if(!waitForPrompt(WAITFORPROMPT_TIMEOUT_INIT)) { for(;;); }
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
  serial().write(cmd);
  if(!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { for(;;); }
}

bool dP_Sigfox::sendConfigCmd(char cmd, char arg)
{
  serial().write(cmd);
  if(!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { for(;;); }
  serial().write(arg);
  if(!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { for(;;); }
}


bool dP_Sigfox::setMemoryConfigParameter(char addr, char value)
{
  serial().write(MEMORY_CONFIG_CMD);
  if(!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { for(;;); }
  serial().write(addr);
  serial().write(value);
  serial().write(END_MEMORY_CONFIG_CMD);

  if(!waitForPrompt(WAITFORPROMPT_TIMEOUT)) { for(;;); }
}


void dP_Sigfox::setNetworkMode(NetworkModeSetting networkMode)
{
  setMemoryConfigParameter(0x3B, networkMode);
}

 void dP_Sigfox::setRfFreqDomain(RfFreqDomainSetting rfFreqDomain)
 {
    setMemoryConfigParameter(0x00, rfFreqDomain);
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


void dP_Sigfox::setRfPower(char rfPower)
{
  setMemoryConfigParameter(0x01, rfPower);

void dP_Sigfox::transmitPkt(char *txPayload, char payloadLen)
{
  char txPkt[payloadLen];

  txPkt[0] = payloadLen;
  for(int i = 0; i < payloadLen; i++)
  {
    txPkt[i+1] = txPayload[i];
  }

  serial().write(txPkt, payloadLen + 1);
}


void dP_Sigfox::transmitSingleBit(char data)
{
  char txPkt[2];

  txPkt[0] = 0x10;
  txPkt[1] = data;   // data is one byte where only the LSB is relevant

  serial().write(txPkt, 2);
}


/*void dP_Sigfox::enableOutOfBandPkt()
{

}


void dP_Sigfox::setOutOfBandPktPeriod(char period)
{
  setMemoryConfigParameter(0x2F, period);     // 0: disabled, 1-15: hours
}


void dP_Sigfox::disableOutOfBandPkt()
{

}*/

//sleep mode
//read RSSI
//read power
//read temp
//read config memory
//set tx power
//set network mode (volatile) ??
//read quality indicator??
