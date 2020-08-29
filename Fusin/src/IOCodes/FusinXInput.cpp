#include "IOCodes/FusinXInput.h"

namespace Fusin
{

	IOCode XInputButton(Index b)
	{
		return IOCode(DT_XINPUT, IO_BUTTON, b);
	}

	IOCode XInputAxis(Index a)
	{
		return IOCode(DT_XINPUT, IO_AXIS, a);
	}

	IOCode XInputDPadButton(Index a)
	{
		return IOCode(DT_XINPUT, IO_DIRECTION, a);
	}

	IOCode XInputPositiveAxis(Index a)
	{
		return XInputAxis(a).positiveVersion();
	}

	IOCode XInputNegativeAxis(Index a)
	{
		return XInputAxis(a).negativeVersion();
	}

}