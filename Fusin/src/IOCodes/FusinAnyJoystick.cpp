#include "IOCodes/FusinAnyJoystick.h"

namespace Fusin
{

	IOCode AnyJoystickButton(Index b)
	{
		return IOCode(DT_COMPONENT_JOYSTICK, IO_BUTTON, b);
	}

	IOCode AnyJoystickAxis(Index a)
	{
		return IOCode(DT_COMPONENT_JOYSTICK, IO_AXIS, a);
	}

	IOCode AnyJoystickPositiveAxis(Index a)
	{
		return IOCode(DT_COMPONENT_JOYSTICK, IO_AXIS, a).positiveVersion();
	}

	IOCode AnyJoystickNegativeAxis(Index a)
	{
		return IOCode(DT_COMPONENT_JOYSTICK, IO_AXIS, a).negativeVersion();
	}

}