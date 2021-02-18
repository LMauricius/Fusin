#ifndef _FUSIN_XINPUT_DEVICE_H
#define _FUSIN_XINPUT_DEVICE_H

#include "FusinDevice.h"
#include "Components/FusinButtonPadComponent.h"
#include "Components/FusinJoystickComponent.h"
#include "Components/FusinDPadComponent.h"
#include "Components/FusinVibrationComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class _FUSIN_EXPORT XInputDevice : public Device
	{
	public:
		XInputDevice(String name = FUSIN_STR("XInput Controller"), bool hasBattery = false);
		~XInputDevice();

		String getStateString();

		IOSignal buttonA, buttonB, buttonX, buttonY;
		IOSignal buttonLB, buttonRB;
		IOSignal buttonView, buttonMenu, buttonLeftStick, buttonRightStick, buttonGuide;

		IOSignal axisLeftStickX, axisLeftStickY, axisRightStickX, axisRightStickY;
		IOSignal axisLeftStickRight, axisLeftStickLeft, axisLeftStickDown, axisLeftStickUp;
		IOSignal axisRightStickRight, axisRightStickLeft, axisRightStickDown, axisRightStickUp;
		IOSignal axisLT, axisRT;

		ButtonPadComponent buttons;
		JoystickComponent axes;
		DPadComponent dPad;
		VibrationComponent vibration;
	};

}

#endif