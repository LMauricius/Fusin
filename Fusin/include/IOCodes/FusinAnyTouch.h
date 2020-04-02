#pragma once
#ifndef _FUSIN_TOUCH_H
#define _FUSIN_TOUCH_H

#include "FusinIOCode.h"
#include "FusinPrerequisites.h"
#include <vector>

namespace Fusin
{

	/*
	Returns the DT_TOUCHPAD, IO_ACTIVATION IOCode for the specified touch's activation state
	*/
	IOCode AnyTouchActivation(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_POSITION IOCode for the specified touch's x position
	*/
	IOCode AnyTouchPositionX(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_POSITION IOCode for the specified touch's y position
	*/
	IOCode AnyTouchPositionY(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_MOVEMENT IOCode for the specified touch's x movement
	*/
	IOCode AnyTouchMoveX(Index touchInd = 0);

	/*
	Returns the DT_TOUCHPAD, IO_MOVEMENT IOCode for the specified touch's y movement
	*/
	IOCode AnyTouchMoveY(Index touchInd = 0);

	IOCode AnyTouchMoveLeft(Index touchInd = 0);
	IOCode AnyTouchMoveRight(Index touchInd = 0);
	IOCode AnyTouchMoveUp(Index touchInd = 0);
	IOCode AnyTouchMoveDown(Index touchInd = 0);

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
