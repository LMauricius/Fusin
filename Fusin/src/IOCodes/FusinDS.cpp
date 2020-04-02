#include "FusinDS.h"

namespace Fusin
{

	IOCode DSButton(Index b)
	{
		return IOCode(DT_DUALSHOCK, IT_BUTTON, b);
	}

	IOCode DSAxis(Index a)
	{
		return IOCode(DT_DUALSHOCK, IT_AXIS, a);
	}

	IOCode DSDPadButton(Index a)
	{
		return IOCode(DT_DUALSHOCK, IT_DIRECTION, a);
	}

	IOCode DSPositiveAxis(Index a)
	{
		return IOCode(DT_DUALSHOCK, IT_AXIS, a).positiveVersion();
	}

	IOCode DSNegativeAxis(Index a)
	{
		return IOCode(DT_DUALSHOCK, IT_AXIS, a).negativeVersion();
	}

}