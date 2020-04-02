#ifndef _FUSIN_ANY_JOYSTICK_H
#define _FUSIN_ANY_JOYSTICK_H

#include "FusinIOCode.h"
#include "FusinPrerequisites.h"
#include <vector>

namespace Fusin
{
	/*
	Returns the DT_COMPONENT_JOYSTICK, IO_BUTTON IOCode for the specified button
	*/
	IOCode AnyJoystickButton(Index b);

	/*
	Returns the DT_COMPONENT_JOYSTICK, IO_AXIS IOCode for the specified gamepad axis.
	*/
	IOCode AnyJoystickAxis(Index a);

	/*
	Returns the DT_COMPONENT_JOYSTICK, IO_AXIS IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode AnyJoystickPositiveAxis(Index a);

	/*
	Returns the DT_COMPONENT_JOYSTICK, IO_AXIS IOCode for the specified gamepad axis' negative direction.
	*/
	IOCode AnyJoystickNegativeAxis(Index a);

	const IOCode ANY_JOYSTICK_X_AXIS = GamepadAxis(0);
	const IOCode ANY_JOYSTICK_Y_AXIS = GamepadAxis(1);
	const IOCode ANY_JOYSTICK_Z_AXIS = GamepadAxis(2);
	const IOCode ANY_JOYSTICK_ROTATION_Z_AXIS = GamepadAxis(3);

	const IOCode ANY_JOYSTICK_X_POSITIVE = ANY_JOYSTICK_X_AXIS.positiveVersion();
	const IOCode ANY_JOYSTICK_X_NEGATIVE = ANY_JOYSTICK_X_AXIS.negativeVersion();
	const IOCode ANY_JOYSTICK_Y_POSITIVE = ANY_JOYSTICK_Y_AXIS.positiveVersion();
	const IOCode ANY_JOYSTICK_Y_NEGATIVE = ANY_JOYSTICK_Y_AXIS.negativeVersion();
	const IOCode ANY_JOYSTICK_Z_POSITIVE = ANY_JOYSTICK_Z_AXIS.positiveVersion();
	const IOCode ANY_JOYSTICK_Z_NEGATIVE = ANY_JOYSTICK_Z_AXIS.negativeVersion();
	const IOCode ANY_JOYSTICK_ROTATION_Z_POSITIVE = ANY_JOYSTICK_ROTATION_Z_AXIS.positiveVersion();
	const IOCode ANY_JOYSTICK_ROTATION_Z_NEGATIVE = ANY_JOYSTICK_ROTATION_Z_AXIS.negativeVersion();

	const std::vector<String> ANY_JOYSTICK_AXIS_NAMES{
		FUSIN_STR("X_Axis"), FUSIN_STR("Y_Axis"), FUSIN_STR("Z_Axis"), FUSIN_STR("Rot-Z_Axis")
	};

}

#endif