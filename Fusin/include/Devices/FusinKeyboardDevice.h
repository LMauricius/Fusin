#ifndef _FUSIN_KEYBOARD_DEVICE_H
#define _FUSIN_KEYBOARD_DEVICE_H

#include "FusinDevice.h"
#include "Components/FusinTypingComponent.h"
#include "Components/FusinLEDComponent.h"
#include <list>
#include <vector>
#include <map>

namespace Fusin
{
	class KeyboardDevice : public Device
	{
	public:
		KeyboardDevice(String name = FUSIN_STR("Keyboard"), size_t keyNum = 0, size_t funcKeyNum = 0, size_t ledNum = 0);
		~KeyboardDevice();

		String getStateString();

		TypingComponent keys;
		LEDComponent leds;

	protected:
	};

}

#endif