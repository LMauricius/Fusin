#pragma once
#ifndef _FUSIN_TOUCH_H
#define _FUSIN_TOUCH_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{

	/*
	Returns the DT_TOUCHPAD, IO_ACTIVATION IOCode for the specified touch's activation state
	*/
	IOCode _FUSIN_EXPORT AnyTouchActivation(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_POSITION IOCode for the specified touch's x position
	*/
	IOCode _FUSIN_EXPORT AnyTouchPositionX(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_POSITION IOCode for the specified touch's y position
	*/
	IOCode _FUSIN_EXPORT AnyTouchPositionY(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_MOVEMENT IOCode for the specified touch's x movement
	*/
	IOCode _FUSIN_EXPORT AnyTouchMoveX(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_MOVEMENT IOCode for the specified touch's y movement
	*/
	IOCode _FUSIN_EXPORT AnyTouchMoveY(Index touchInd = 0);

	IOCode _FUSIN_EXPORT AnyTouchMoveLeft(Index touchInd = 0);
	IOCode _FUSIN_EXPORT AnyTouchMoveRight(Index touchInd = 0);
	IOCode _FUSIN_EXPORT AnyTouchMoveUp(Index touchInd = 0);
	IOCode _FUSIN_EXPORT AnyTouchMoveDown(Index touchInd = 0);

	const std::vector<String> TOUCH_PRESSURE_NAMES{
		FUSIN_STR("Pressure")
	};
	const std::vector<String> TOUCH_POSITION_NAMES{
		FUSIN_STR("Position_X"), FUSIN_STR("Position_Y")
	};
	const std::vector<String> TOUCH_MOVEMENT_NAMES{
		FUSIN_STR("Move_X"), FUSIN_STR("Move_Y")
	};

}

#endif
