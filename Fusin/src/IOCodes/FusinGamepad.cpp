#include "FusinGamepad.h"

namespace Fusin
{

	IOCode GamepadButton(Index b)
	{
		return IOCode(DT_GAMEPAD, IO_BUTTON, b);
	}

	IOCode GamepadAxis(Index a)
	{
		return IOCode(DT_GAMEPAD, IO_AXIS, a);
	}

	IOCode GamepadPositiveAxis(Index a)
	{
		return IOCode(DT_GAMEPAD, IO_AXIS, a).positiveVersion();
	}

	IOCode GamepadNegativeAxis(Index a)
	{
		return IOCode(DT_GAMEPAD, IO_AXIS, a).negativeVersion();
	}

}