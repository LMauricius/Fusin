#ifndef _FUSIN_XINPUT_DEVICE_H
#define _FUSIN_XINPUT_DEVICE_H

#include "FusinDevice.h"
#include "FusinButtonPadComponent.h"
#include "FusinJoystickComponent.h"
#include "FusinDPadComponent.h"
#include "FusinVibrationComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class XInputDeviceListener;

	class XInputDevice : public Device
	{
	public:
		XInputDevice(String name = FUSIN_STR("XInput Controller"));
		~XInputDevice();

		String getStateString();

		IOSignal buttonA, buttonB, buttonX, buttonY;
		IOSignal buttonLB, buttonRB;
		IOSignal buttonView, buttonMenu, buttonLeftStick, buttonRightStick, buttonGuide;

		IOSignal axisLeftX, axisLeftY, axisRightX, axisRightY;
		IOSignal axisPositiveLeftX, axisNegativeLeftX, axisPositiveLeftY, axisNegativeLeftY;
		IOSignal axisPositiveRightX, axisNegativeRightX, axisPositiveRightY, axisNegativeRightY;
		IOSignal axisLT, axisRT;

		ButtonPadComponent buttons;
		JoystickComponent axes;
		DPadComponent dPad;
		VibrationComponent vibration;
	};

}

#endif