#include "IOCodes/FusinDS.h"

namespace Fusin
{

	IOCode DSButton(Index b)
	{
		return IOCode(DT_DUALSHOCK, IO_BUTTON, b);
	}

	IOCode DSAxis(Index a)
	{
		return IOCode(DT_DUALSHOCK, IO_AXIS, a);
	}

	IOCode DSDPadButton(Index a)
	{
		return IOCode(DT_DUALSHOCK, IO_DIRECTION, a);
	}

	IOCode DSPositiveAxis(Index a)
	{
		return IOCode(DT_DUALSHOCK, IO_AXIS, a).positiveVersion();
	}

	IOCode DSNegativeAxis(Index a)
	{
		return IOCode(DT_DUALSHOCK, IO_AXIS, a).negativeVersion();
	}

}