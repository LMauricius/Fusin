#include "FusinNintendo.h"

namespace Fusin
{

	IOCode NintendoButton(Index b)
	{
		return IOCode(DT_NINTENDO, IT_BUTTON, b);
	}

	IOCode NintendoAxis(Index a)
	{
		return IOCode(DT_NINTENDO, IT_AXIS, a);
	}

	IOCode NintendoDPadButton(Index a)
	{
		return IOCode(DT_NINTENDO, IT_DIRECTION, a);
	}

	IOCode NintendoPositiveAxis(Index a)
	{
		return IOCode(DT_NINTENDO, IT_AXIS, a).positiveVersion();
	}

	IOCode NintendoNegativeAxis(Index a)
	{
		return IOCode(DT_NINTENDO, IT_AXIS, a).negativeVersion();
	}

}