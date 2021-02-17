#include "Devices/FusinKeyboardDevice.h"

namespace Fusin
{
	KeyboardDevice::KeyboardDevice(String name, size_t keyNum, size_t funcKeyNum, size_t ledNum):
		Device(name, DT_KEYBOARD),
		keys(DT_KEYBOARD, keyNum, funcKeyNum),
		leds(DT_KEYBOARD, ledNum)
	{
		registerComponents({&keys, &leds});
	}

	KeyboardDevice::~KeyboardDevice()
	{
	}

	String KeyboardDevice::getStateString()
	{
		return String(FUSIN_STR("Keys: ")) + keys.getKeyString() + String(FUSIN_STR("LEDs: ")) + leds.getStateString();
	}

}