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

	/*
	Component for devices with LED lights.
	Each LED has its own index, with indices starting at 0 and
	continuing until the value of LEDCount()-1.
	*/
	class _FUSIN_EXPORT LEDComponent : public DeviceComponent
	{
	public:
		LEDComponent(DeviceType signalType, size_t ledNum);
		~LEDComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		size_t LEDCount() const { return mLEDs.size(); }

		/*
		Returns the string with all axis values and indices of buttons that are currently held
		*/
		String getStateString();

		IOSignal& operator[](size_t ind);

		void setLEDFlags(LEDFlags leds);
		LEDFlags ledFlags();
		LEDFlags nextLedFlags();

		/*
		If enabled, the LEDs will be set to their default values depending on the device
		*/
		/*void setUseDefaults(bool enable);
		bool useDefaults();*/

		void _setLEDCount(size_t ledNum);
		void _update(size_t msElapsed = 0);

	protected:
		DeviceType mSignalDeviceType;
		//bool mUseDefaults;
		std::vector<IOSignal*> mLEDs;
	};

}

#endif