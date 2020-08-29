#ifndef _FUSIN_JOYSTICK_COMPONENT_H
#define _FUSIN_JOYSTICK_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class JoystickComponent : public DeviceComponent
	{
	public:
		JoystickComponent(DeviceType signalType, size_t axisNum);
		~JoystickComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		size_t axisNumber() const { return mAxes.size(); }

		/*
		Returns the string with all axis values and indices of buttons that are currently held
		*/
		String getStateString();

		IOSignal& operator[](Index ind);

		void _setAxisCount(size_t axisNum);
		/*
		Inserts external IOSignals into the component's signals.
		The external IOSignals' ownership won't be taken.
		The IOSignals are inserted after the previously inserted external IOSignals and
		before the IOSignals owned by this JoystickComponent.
		Keep in mind that the IOSignals' IOCodes won't be changed, so you need to keep
		track of their indices yourself.
		*/
		void _registerExternalAxes(const std::vector<IOSignal*>& axes);
		void _update(size_t msElapsed = 0);

	protected:
		DeviceType mSignalDeviceType;
		std::vector<IOSignal*> mAxes, mPositiveAxes, mNegativeAxes;
		size_t mReservedJoystickAxisCount;
	};

}

#endif