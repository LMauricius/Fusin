#include "FusinMouse.h"

namespace Fusin
{

	IOCode MouseButton(Index b)
	{
		return IOCode(DT_MOUSE, IT_BUTTON, b);
	}

}