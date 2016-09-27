/*
 * dP_MicroSD.cpp is part of the duinoPRO firmware.
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

#include <dP_MicroSD.h>
#include <dP_Pin.h>

// Module Pins
#define CARD_DETECT_PIN 6
#define POWERDOWN_PIN 	5

dP_MicroSD::dP_MicroSD(int id) : dP_Module(id)
{
};

bool dP_MicroSD::begin()
{
    // pin(CARD_DETECT_PIN).mode(INPUT);
    // Pin *cs = dP_Module::spiSelectMap[id()];
	pin(POWERDOWN_PIN).mode(OUTPUT);
	pin(POWERDOWN_PIN).write(LOW);
	spiSelect().write(HIGH);
	spiSelect().mode(OUTPUT);
  return SD.begin(&spiSelect());
}

bool dP_MicroSD::cardDetect()
{
    return !pin(CARD_DETECT_PIN).read();
}

void dP_MicroSD::lowpower(bool lowpower)
{
    pin(POWERDOWN_PIN).write(lowpower);
}

void dP_MicroSD::timestampCallback(dateTimeFunction dateTime)
{
	SdFile::dateTimeCallback(dateTime);
}

void dP_MicroSD::timestampCancel(void)
{
	SdFile::dateTimeCallbackCancel();
}

void dP_MicroSD::printDirectory(File dir)
{
	printDirectory(dir, 0);
}

void dP_MicroSD::printDirectory(File dir, uint8_t numTabs)  // Modified from the 'printdirectory' function in the Arduino 'Listfiles' tutorial.
{
	if (numTabs == 0) {
		dir.rewindDirectory();
		Serial.println("Directory listing:");
	}
	while (true) {
		File entry = dir.openNextFile();
		if (! entry) {
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++) {
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if (entry.isDirectory()) {
			Serial.println("/");
			printDirectory(entry, numTabs + 1);
		} else {
			Serial.print("\t\t");
			Serial.print(entry.size(), DEC);
			Serial.println(" bytes");
		}
		entry.close();
  }
}
