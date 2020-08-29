#include "IOCodes/FusinAnyTouch.h"

namespace Fusin
{
	IOCode AnyTouchActivation(Index touchInd)
	{
		return IOCode(DT_COMPONENT_TOUCHPAD, IO_PRESSURE, touchInd);
	}


	IOCode AnyTouchPositionX(Index touchInd)
	{
		return IOCode(DT_COMPONENT_TOUCHPAD, IO_POSITION, touchInd * 2);
	}

	IOCode AnyTouchPositionY(Index touchInd)
	{
		return IOCode(DT_COMPONENT_TOUCHPAD, IO_POSITION, touchInd * 2 + 1);
	}


	IOCode AnyTouchMoveX(Index touchInd)
	{
		return IOCode(DT_COMPONENT_TOUCHPAD, IO_MOVEMENT, touchInd * 2);
	}

	IOCode AnyTouchMoveY(Index touchInd)
	{
		return IOCode(DT_COMPONENT_TOUCHPAD, IO_MOVEMENT, touchInd * 2 + 1);
	}


	IOCode AnyTouchMoveLeft(Index touchInd)
	{
		return AnyTouchMoveX(touchInd).negativeVersion();
	}

	IOCode AnyTouchMoveRight(Index touchInd)
	{
		return AnyTouchMoveX(touchInd).positiveVersion();
	}

	IOCode AnyTouchMoveUp(Index touchInd)
	{
		return AnyTouchMoveY(touchInd).negativeVersion();
	}

	IOCode AnyTouchMoveDown(Index touchInd)
	{
		return AnyTouchMoveY(touchInd).positiveVersion();
	}
}