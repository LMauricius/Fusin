#ifndef _FUSIN_LED_COMPONENT_H
#define _FUSIN_LED_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	using LEDFlags = unsigned int;
	const LEDFlags LED_NONE = 0;
	const LEDFlags LED_1 = 1 << 0;
	const LEDFlags LED_2 = 1 << 1;
	const LEDFlags LED_3 = 1 << 2;
	const LEDFlags LED_4 = 1 << 3;
	const LEDFlags LED_CAPS = LED_1;
	const LEDFlags LED_NUM = LED_2;
	const LEDFlags LED_SCROLL = LED_3;
	const LEDFlags LED_AUTO = -1;

	class LEDComponent : public DeviceComponent
	{
	public:
		LEDComponent(DeviceType signalType, size_t ledNum);
		~LEDComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		size_t LEDNumber() const { return mLEDs.size(); }

		/*
		Returns the string with all axis values and indices of buttons that are currently held
		*/
		String getStateString();

		IOSignal& operator[](size_t ind);

		void _setLEDCount(size_t ledNum);
		void _update(size_t msElapsed = 0);

	protected:
		DeviceType mSignalDeviceType;
		std::vector<IOSignal*> mLEDs;
	};

}

#endif