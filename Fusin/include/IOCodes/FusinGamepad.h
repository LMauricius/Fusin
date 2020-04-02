#ifndef _FUSIN_GAMEPAD_H
#define _FUSIN_GAMEPAD_H

#include "FusinIOCode.h"
#include "FusinPrerequisites.h"
#include <vector>

namespace Fusin
{
	/*
	Returns the DT_GAMEPAD, IT_BUTTON IOCode for the specified button
	*/
	IOCode GamepadButton(Index b);

	/*
	Returns the DT_GAMEPAD, IT_AXIS IOCode for the specified gamepad axis.
	*/
	IOCode GamepadAxis(Index a);

	/*
	Returns the DT_GAMEPAD, IT_AXIS IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode GamepadPositiveAxis(Index a);

	/*
	Returns the DT_GAMEPAD, IT_AXIS IOCode for the specified gamepad axis' negative direction.
	*/
	IOCode GamepadNegativeAxis(Index a);

	const IOCode GAMEPAD_X_AXIS = GamepadAxis(0);
	const IOCode GAMEPAD_Y_AXIS = GamepadAxis(1);
	const IOCode GAMEPAD_Z_AXIS = GamepadAxis(2);
	const IOCode GAMEPAD_ROTATION_Z_AXIS = GamepadAxis(3);

	const IOCode GAMEPAD_DPAD_ANGLE = IOCode(DT_GAMEPAD, IO_ANGLE, 0); 
	const IOCode GAMEPAD_DPAD_UP = IOCode(DT_GAMEPAD, IO_DIRECTION, 0);
	const IOCode GAMEPAD_DPAD_DOWN = IOCode(DT_GAMEPAD, IO_DIRECTION, 1);
	const IOCode GAMEPAD_DPAD_LEFT = IOCode(DT_GAMEPAD, IO_DIRECTION, 2);
	const IOCode GAMEPAD_DPAD_RIGHT = IOCode(DT_GAMEPAD, IO_DIRECTION, 3);

	const IOCode GAMEPAD_X_POSITIVE = GAMEPAD_X_AXIS.positiveVersion();
	const IOCode GAMEPAD_X_NEGATIVE = GAMEPAD_X_AXIS.negativeVersion();
	const IOCode GAMEPAD_Y_POSITIVE = GAMEPAD_Y_AXIS.positiveVersion();
	const IOCode GAMEPAD_Y_NEGATIVE = GAMEPAD_Y_AXIS.negativeVersion();
	const IOCode GAMEPAD_Z_POSITIVE = GAMEPAD_Z_AXIS.positiveVersion();
	const IOCode GAMEPAD_Z_NEGATIVE = GAMEPAD_Z_AXIS.negativeVersion();
	const IOCode GAMEPAD_ROTATION_Z_POSITIVE = GAMEPAD_ROTATION_Z_AXIS.positiveVersion();
	const IOCode GAMEPAD_ROTATION_Z_NEGATIVE = GAMEPAD_ROTATION_Z_AXIS.negativeVersion();

}

#endif