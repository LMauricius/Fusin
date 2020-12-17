#ifndef _FUSIN_WHEEL_COMPONENT_H
#define _FUSIN_WHEEL_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <vector>
#include <list>
#include <map>

namespace Fusin
{
	class WheelComponent : virtual public DeviceComponent
	{
	public:
		WheelComponent(DeviceType signalType, bool hasHorizontalWheel);
		~WheelComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		void simulateRotation(float ver, float hor =0.0f);
		String getStateString();
		inline bool hasHorizontalWheel() {return mHasHorizontalWheel;}

		IOSignal scrollVertical, scrollHorizontal;
		IOSignal scrollLeft, scrollRight, scrollUp, scrollDown;

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mWheelRotation;
		bool mHasHorizontalWheel;
	};

}

#endif