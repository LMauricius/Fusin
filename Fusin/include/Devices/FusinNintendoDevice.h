#ifndef _FUSIN_NINTENDO_CONTROLLER_H
#define _FUSIN_NINTENDO_CONTROLLER_H

#include "FusinDevice.h"
#include "Components/FusinButtonPadComponent.h"
#include "Components/FusinJoystickComponent.h"
#include "Components/FusinDPadComponent.h"
#include "Components/FusinMotionTrackerComponent.h"
#include "Components/FusinTouchComponent.h"
#include "Components/FusinVibrationComponent.h"
#include "Components/FusinRGBComponent.h"
#include "Components/FusinLEDComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class NintendoDevice : public Device
	{
	public:
		NintendoDevice(String name = FUSIN_STR("Nintendo Controller"), bool hasBattery = false);
		~NintendoDevice();

		String getStateString();

		IOSignal buttonB, buttonA, buttonY, buttonX;
		IOSignal buttonL, buttonR, buttonZL, buttonZR, buttonSL, buttonSR;
		IOSignal buttonMinus, buttonPlus, buttonLeftStick, buttonRightStick, buttonHome, buttonCapture;

		IOSignal axisLeftStickX, axisLeftStickY, axisRightStickX, axisRightStickY;
		IOSignal axisLeftStickRight, axisLeftStickLeft, axisLeftStickDown, axisLeftStickUp;
		IOSignal axisRightStickRight, axisRightStickLeft, axisRightStickDown, axisRightStickUp;

		ButtonPadComponent buttons;
		JoystickComponent axes;
		DPadComponent dPad;
		MotionTrackerComponent motion;
		TouchComponent touchPad;
		VibrationComponent vibration;
	};
}

#endif