#ifndef _FUSIN_DS_CONTROLLER_H
#define _FUSIN_DS_CONTROLLER_H

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
	class _FUSIN_EXPORT DSDevice : public Device
	{
	public:
		DSDevice(String name = FUSIN_STR("DS Controller"), bool hasBattery = false);
		~DSDevice();

		String getStateString();

		IOSignal buttonCross, buttonCircle, buttonSquare, buttonTriangle;
		IOSignal buttonL1, buttonR1;
		IOSignal buttonTouchPad, buttonOptions, buttonL3, buttonR3, buttonPS, buttonCreate;

		IOSignal axisLeftStickX, axisLeftStickY, axisRightStickX, axisRightStickY;
		IOSignal axisLeftStickRight, axisLeftStickLeft, axisLeftStickDown, axisLeftStickUp;
		IOSignal axisRightStickRight, axisRightStickLeft, axisRightStickDown, axisRightStickUp;
		IOSignal axisL2, axisR2;

		ButtonPadComponent buttons;
		JoystickComponent axes;
		DPadComponent dPad;
		MotionTrackerComponent motion;
		TouchComponent touchPad;
		VibrationComponent vibration;
		RGBComponent rgb;
		LEDComponent leds;
	};

}

#endif