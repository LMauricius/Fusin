#include "IOCodes/FusinNintendo.h"

namespace Fusin
{

	IOCode NintendoButton(Index b)
	{
		return IOCode(DT_NINTENDO, IO_BUTTON, b);
	}

	IOCode NintendoAxis(Index a)
	{
		return IOCode(DT_NINTENDO, IO_AXIS, a);
	}

	IOCode NintendoDPadButton(Index a)
	{
		return IOCode(DT_NINTENDO, IO_DIRECTION, a);
	}

	IOCode NintendoPositiveAxis(Index a)
	{
		return IOCode(DT_NINTENDO, IO_AXIS, a).positiveVersion();
	}

	IOCode NintendoNegativeAxis(Index a)
	{
		return IOCode(DT_NINTENDO, IO_AXIS, a).negativeVersion();
	}

}