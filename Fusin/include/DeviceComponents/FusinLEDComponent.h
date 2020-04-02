#ifndef _FUSIN_LED_COMPONENT_H
#define _FUSIN_LED_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
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