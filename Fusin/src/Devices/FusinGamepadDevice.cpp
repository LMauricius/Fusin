#include "Devices/FusinGamepadDevice.h"
#include "IOCodes/FusinGamepad.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{

	GamepadDevice::GamepadDevice(String name, size_t buttonNum, size_t axisNum, bool hasDPad, bool supportsVibration):
		Device(name, DT_GAMEPAD),
		buttons(DT_GAMEPAD, buttonNum),
		axes(DT_GAMEPAD, axisNum),
		dPad(DT_GAMEPAD),
		vibration(DT_GAMEPAD),
		mHasDPad(hasDPad),
		mSupportsVibration(supportsVibration)
	{
		registerComponents({ &buttons, &axes});

		if (mHasDPad)
			registerComponent(&dPad);

		if (mSupportsVibration)
			registerComponent(&vibration);
	}

	GamepadDevice::~GamepadDevice()
	{
	}

	String GamepadDevice::getStateString()
	{
		StringStream ss;

		ss << axes.getStateString() << " ";
		
		if (mHasDPad)
			ss << dPad.getStateString() << " ";
		if (mSupportsVibration && (vibration.leftForce.value() || vibration.rightForce.value()))
			ss << "Vibration[" << vibration.getStateString() << "] ";
		
		ss << buttons.getStateString();

		return ss.str();
	}
}