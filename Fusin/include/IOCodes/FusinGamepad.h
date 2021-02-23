#ifndef _FUSIN_GAMEPAD_H
#define _FUSIN_GAMEPAD_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{
	/*
	Returns the DT_GAMEPAD, IO_BUTTON IOCode for the specified button
	*/
	IOCode _FUSIN_EXPORT GamepadButton(Index b);

	/*
	Returns the DT_GAMEPAD, IO_AXIS IOCode for the specified gamepad axis.
	*/
	IOCode _FUSIN_EXPORT GamepadAxis(Index a);

	/*
	Returns the DT_GAMEPAD, IO_AXIS IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode _FUSIN_EXPORT GamepadPositiveAxis(Index a);

	/*
	Returns the DT_GAMEPAD, IO_AXIS IOCode for the specified gamepad axis' negative direction.
	*/
	IOCode _FUSIN_EXPORT GamepadNegativeAxis(Index a);

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
	
	const IOCode GAMEPAD_VIBRATION_LEFT_FORCE      = IOCode(DT_GAMEPAD, IO_VIBRATION, 0);
	const IOCode GAMEPAD_VIBRATION_LEFT_DURATION   = IOCode(DT_GAMEPAD, IO_VIBRATION, 2);
	const IOCode GAMEPAD_VIBRATION_RIGHT_FORCE     = IOCode(DT_GAMEPAD, IO_VIBRATION, 3);
	const IOCode GAMEPAD_VIBRATION_RIGHT_DURATION  = IOCode(DT_GAMEPAD, IO_VIBRATION, 5);

}

#endif