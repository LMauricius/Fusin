#ifndef _FUSIN_GAMEPAD_DEVICE_H
#define _FUSIN_GAMEPAD_DEVICE_H

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
	class GamepadDeviceListener;

	class GamepadDevice : public Device
	{
	public:
		GamepadDevice(String name = FUSIN_STR("Gamepad"), size_t buttonNum = 0, size_t axisNum = 0,
			bool hasDPad = true, bool supportsVibration = true);
		~GamepadDevice();

		inline bool hasDPad() { return mHasDPad; }
		inline bool supportsVibration() { return mSupportsVibration; }

		String getStateString();

		ButtonPadComponent buttons;
		JoystickComponent axes;
		DPadComponent dPad;
		VibrationComponent vibration;

	protected:
		bool mHasDPad, mSupportsVibration;
	};

}

#endif