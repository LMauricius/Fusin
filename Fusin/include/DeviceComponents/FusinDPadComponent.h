#ifndef _FUSIN_DPAD_COMPONENT_H
#define _FUSIN_DPAD_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class DPadComponent : public DeviceComponent
	{
		friend InputManager;

	public:
		DPadComponent(DeviceType signalType);
		~DPadComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		String getStateString();

		IOSignal angle;
		IOSignal up, down, left, right;

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mDPadDirections;
		std::vector<IOSignal*> mDPadAngles;
	};

}

#endif