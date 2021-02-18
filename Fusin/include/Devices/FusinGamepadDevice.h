#ifndef _FUSIN_GAMEPAD_DEVICE_H
#define _FUSIN_GAMEPAD_DEVICE_H

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
	class _FUSIN_EXPORT GamepadDevice : public Device
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