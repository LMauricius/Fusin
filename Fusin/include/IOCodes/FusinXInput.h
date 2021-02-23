#ifndef _FUSIN_XINPUT_H
#define _FUSIN_XINPUT_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{
	/*
	Returns the DT_XINPUT, IO_BUTTON IOCode for the specified button
	*/
	IOCode _FUSIN_EXPORT XInputButton(Index b);

	/*
	Returns the DT_XINPUT, IO_AXIS IOCode for the specified gamepad axis.
	*/
	IOCode _FUSIN_EXPORT XInputAxis(Index a);

	/*
	Returns the DT_XINPUT, IO_AXIS IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode _FUSIN_EXPORT XInputPositiveAxis(Index a);

	/*
	Returns the DT_XINPUT, IO_AXIS IOCode for the specified gamepad axis' negative direction.
	*/
	IOCode _FUSIN_EXPORT XInputNegativeAxis(Index a);

	const IOCode XINPUT_A = XInputButton(0);
	const IOCode XINPUT_B = XInputButton(1);
	const IOCode XINPUT_X = XInputButton(2);
	const IOCode XINPUT_Y = XInputButton(3);
	const IOCode XINPUT_LB = XInputButton(4);
	const IOCode XINPUT_RB = XInputButton(5);
	const IOCode XINPUT_VIEW = XInputButton(6);
	const IOCode XINPUT_MENU = XInputButton(7);
	const IOCode XINPUT_LEFT_STICK_PRESS = XInputButton(8);
	const IOCode XINPUT_RIGHT_STICK_PRESS = XInputButton(9);
	const IOCode XINPUT_GUIDE = XInputButton(10);

	const IOCode XINPUT_LEFT_X_AXIS = XInputAxis(0);
	const IOCode XINPUT_LEFT_Y_AXIS = XInputAxis(1);
	const IOCode XINPUT_RIGHT_X_AXIS = XInputAxis(4);
	const IOCode XINPUT_RIGHT_Y_AXIS = XInputAxis(5);
	const IOCode XINPUT_LT = XInputAxis(2);
	const IOCode XINPUT_RT = XInputAxis(3);

	const IOCode XINPUT_DPAD_ANGLE = IOCode(DT_XINPUT, IO_ANGLE, 0);
	const IOCode XINPUT_DPAD_UP    = IOCode(DT_XINPUT, IO_DIRECTION, 0);
	const IOCode XINPUT_DPAD_DOWN  = IOCode(DT_XINPUT, IO_DIRECTION, 1);
	const IOCode XINPUT_DPAD_LEFT  = IOCode(DT_XINPUT, IO_DIRECTION, 2);
	const IOCode XINPUT_DPAD_RIGHT = IOCode(DT_XINPUT, IO_DIRECTION, 3);

	const IOCode XINPUT_LEFT_STICK_RIGHT = XInputPositiveAxis(0);
	const IOCode XINPUT_LEFT_STICK_LEFT = XInputNegativeAxis(0);
	const IOCode XINPUT_LEFT_STICK_DOWN = XInputPositiveAxis(1);
	const IOCode XINPUT_LEFT_STICK_UP = XInputNegativeAxis(1);
	const IOCode XINPUT_RIGHT_STICK_RIGHT = XInputPositiveAxis(4);
	const IOCode XINPUT_RIGHT_STICK_LEFT = XInputNegativeAxis(4);
	const IOCode XINPUT_RIGHT_STICK_DOWN = XInputPositiveAxis(5);
	const IOCode XINPUT_RIGHT_STICK_UP = XInputNegativeAxis(5);
	
	const IOCode XINPUT_VIBRATION_LEFT_FORCE      = IOCode(DT_XINPUT, IO_VIBRATION, 0);
	const IOCode XINPUT_VIBRATION_LEFT_DURATION   = IOCode(DT_XINPUT, IO_VIBRATION, 2);
	const IOCode XINPUT_VIBRATION_RIGHT_FORCE     = IOCode(DT_XINPUT, IO_VIBRATION, 3);
	const IOCode XINPUT_VIBRATION_RIGHT_DURATION  = IOCode(DT_XINPUT, IO_VIBRATION, 5);

	const std::vector<String> XINPUT_BUTTON_NAMES{
		FUSIN_STR("A"), FUSIN_STR("B"), FUSIN_STR("X"), FUSIN_STR("Y"),
		FUSIN_STR("LB"), FUSIN_STR("RB"), FUSIN_STR("Back"), FUSIN_STR("Start"), FUSIN_STR("Left_Stick"), FUSIN_STR("Right_Stick"), FUSIN_STR("Guide"),
	};
	const std::vector<String> XINPUT_AXIS_NAMES{
		FUSIN_STR("Left_Stick_X"), FUSIN_STR("Left_Stick_Y"), FUSIN_STR("LT"), FUSIN_STR("RT"),
		FUSIN_STR("Right_Stick_X"), FUSIN_STR("Right_Stick_Y")
	};

	const unsigned int XINPUT_BUTTON_NUMBER = 11;
	const unsigned int XINPUT_AXIS_NUMBER = 7;

}

#endif