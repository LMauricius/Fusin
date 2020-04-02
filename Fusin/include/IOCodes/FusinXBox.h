#ifndef _FUSIN_XBOX_H
#define _FUSIN_XBOX_H

#include "FusinInputCode.h"
#include "FusinPrerequisites.h"
#include <vector>

namespace Fusin
{
	const IOType IT_XBOX_BUTTON = IT_XBOX | IT_BUTTON;
	const IOType IT_XBOX_AXIS = IT_XBOX | IT_AXIS;
	const IOType IT_XBOX_DPAD_ANGLE = IT_XBOX | IT_ANGLE;
	const IOType IT_XBOX_DPAD_BUTTON = IT_XBOX | IT_DIRECTION;
	const IOType IT_XBOX_SIGNED_AXIS = IT_XBOX_AXIS | IT_SIGNED_VERSION;

	/*
	Returns the IT_XBOX_BUTTON IOCode for the specified button
	*/
	IOCode XBoxButton(Index b);

	/*
	Returns the IT_XBOX_AXIS IOCode for the specified gamepad axis.
	*/
	IOCode XBoxAxis(Index a);

	/*
	Returns the IT_XBOX_DPAD_BUTTON IOCode for the specified value.
	*/
	IOCode XBoxDPadButton(Index a);

	/*
	Returns the IT_XBOX_SIGNED_AXIS IOCode for the specified value.
	*/
	IOCode XBoxSignedAxis(Index a);

	/*
	Returns the IT_XBOX_SPECIAL IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode XBoxPositiveAxis(Index a);

	/*
	Returns the IT_XBOX_SPECIAL IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode XBoxNegativeAxis(Index a);

	const IOCode XBOX_A = XBoxButton(0);
	const IOCode XBOX_B = XBoxButton(1);
	const IOCode XBOX_X = XBoxButton(2);
	const IOCode XBOX_Y = XBoxButton(3);
	const IOCode XBOX_LB = XBoxButton(4);
	const IOCode XBOX_RB = XBoxButton(5);
	const IOCode XBOX_VIEW = XBoxButton(6);
	const IOCode XBOX_MENU = XBoxButton(7);
	const IOCode XBOX_LSTICK = XBoxButton(8);
	const IOCode XBOX_RSTICK = XBoxButton(9);
	const IOCode XBOX_GUIDE = XBoxButton(10);

	const IOCode XBOX_LEFT_X_AXIS = XBoxAxis(0);
	const IOCode XBOX_LEFT_Y_AXIS = XBoxAxis(1);
	const IOCode XBOX_RIGHT_X_AXIS = XBoxAxis(4);
	const IOCode XBOX_RIGHT_Y_AXIS = XBoxAxis(5);
	const IOCode XBOX_LT = XBoxAxis(2);
	const IOCode XBOX_RT = XBoxAxis(3);

	const IOCode XBOX_DPAD_UP = XBoxDPadButton(0);
	const IOCode XBOX_DPAD_DOWN = XBoxDPadButton(1);
	const IOCode XBOX_DPAD_LEFT = XBoxDPadButton(2);
	const IOCode XBOX_DPAD_RIGHT = XBoxDPadButton(3);

	const IOCode XBOX_LEFT_STICK_RIGHT = XBoxPositiveAxis(0);
	const IOCode XBOX_LEFT_STICK_LEFT = XBoxNegativeAxis(0);
	const IOCode XBOX_LEFT_STICK_DOWN = XBoxPositiveAxis(1);
	const IOCode XBOX_LEFT_STICK_UP = XBoxNegativeAxis(1);
	const IOCode XBOX_RIGHT_STICK_RIGHT = XBoxPositiveAxis(2);
	const IOCode XBOX_RIGHT_STICK_LEFT = XBoxNegativeAxis(2);
	const IOCode XBOX_RIGHT_STICK_DOWN = XBoxPositiveAxis(3);
	const IOCode XBOX_RIGHT_STICK_UP = XBoxNegativeAxis(3);

	const std::vector<String> XBOX_BUTTON_NAMES{
		FUSIN_STR("A"), FUSIN_STR("B"), FUSIN_STR("X"), FUSIN_STR("Y"),
		FUSIN_STR("LB"), FUSIN_STR("RB"), FUSIN_STR("Back"), FUSIN_STR("Start"), FUSIN_STR("Left Stick"), FUSIN_STR("Right Stick"), FUSIN_STR("Guide"),
	};
	const std::vector<String> XBOX_AXIS_NAMES{
		FUSIN_STR("Left Stick X"), FUSIN_STR("Left Stick Y"), FUSIN_STR("LT"), FUSIN_STR("RT"),
		FUSIN_STR("Right Stick X"), FUSIN_STR("Right Stick Y")
	};

	const unsigned int XBOX_BUTTON_NUMBER = 11;
	const unsigned int XBOX_AXIS_NUMBER = 7;
	const unsigned long XBOX_VENDOR_ID = 0x045E;
	const  std::vector<unsigned long> XBOX_PRODUCT_IDS{ 0x028E };
	const unsigned int XBOX_PRODUCT_ID_COUNT = 1;

}

#endif