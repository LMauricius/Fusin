#include "IOCodes/FusinAnyLED.h"

namespace Fusin
{

	IOCode AnyLED(Index l)
	{
		return IOCode(DT_COMPONENT_LEDS, IO_LEDS, l);
	}

}