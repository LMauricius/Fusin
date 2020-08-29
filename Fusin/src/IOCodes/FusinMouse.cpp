#include "IOCodes/FusinMouse.h"

namespace Fusin
{

	IOCode MouseButton(Index b)
	{
		return IOCode(DT_MOUSE, IO_BUTTON, b);
	}

}