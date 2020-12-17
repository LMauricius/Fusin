#ifndef _FUSIN_MOUSE_H
#define _FUSIN_MOUSE_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{
	/*
	Returns the DT_MOUSE, IO_BUTTON IOCode for the specified mouse button
	*/
	IOCode MouseButton(Index b);

	const IOCode MOUSE_BUTTON_LEFT = MouseButton(0);
	const IOCode MOUSE_BUTTON_RIGHT = MouseButton(1);
	const IOCode MOUSE_BUTTON_MIDDLE = MouseButton(2);
	const IOCode MOUSE_BUTTON_4 = MouseButton(3);
	const IOCode MOUSE_BUTTON_5 = MouseButton(4);

	const IOCode MOUSE_WHEEL = IOCode(DT_MOUSE, IO_DIRECTION, 0);
	const IOCode MOUSE_HORIZONTAL_WHEEL = IOCode(DT_MOUSE, IO_DIRECTION, 1);
	const IOCode MOUSE_WHEEL_UP = MOUSE_WHEEL.positiveVersion();
	const IOCode MOUSE_WHEEL_DOWN = MOUSE_WHEEL.negativeVersion();
	const IOCode MOUSE_WHEEL_LEFT = MOUSE_HORIZONTAL_WHEEL.negativeVersion();
	const IOCode MOUSE_WHEEL_RIGHT = MOUSE_HORIZONTAL_WHEEL.positiveVersion();
	const IOCode MOUSE_POSITION_X = IOCode(DT_MOUSE, IO_POSITION, 0);
	const IOCode MOUSE_POSITION_Y = IOCode(DT_MOUSE, IO_POSITION, 1);
	const IOCode MOUSE_MOVE_X = IOCode(DT_MOUSE, IO_MOVEMENT, 0);
	const IOCode MOUSE_MOVE_Y = IOCode(DT_MOUSE, IO_MOVEMENT, 1);
	const IOCode MOUSE_MOVE_LEFT = MOUSE_MOVE_X.negativeVersion();
	const IOCode MOUSE_MOVE_RIGHT = MOUSE_MOVE_X.positiveVersion();
	const IOCode MOUSE_MOVE_UP = MOUSE_MOVE_Y.negativeVersion();
	const IOCode MOUSE_MOVE_DOWN = MOUSE_MOVE_Y.positiveVersion();

	const const std::vector<String> MOUSE_BUTTON_NAMES = {
		FUSIN_STR("Left"), FUSIN_STR("Right"), FUSIN_STR("Middle"), FUSIN_STR("4"), FUSIN_STR("5")
	};

}

#endif