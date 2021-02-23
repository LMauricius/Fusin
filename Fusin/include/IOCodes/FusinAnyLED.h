#ifndef _FUSIN_RGB_H
#define _FUSIN_RGB_H

#include "IOCodes/FusinIOCode.h"
#include "Components/FusinLEDComponent.h"
#include <vector>

namespace Fusin
{
	/*
	Returns the DT_COMPONENT_LED, IO_LED IOCode for the specified LED index
	*/
	IOCode _FUSIN_EXPORT AnyLED(Index l);

	const IOCode ANY_LED_CAPS   = IOCode(DT_COMPONENT_LEDS, IO_LEDS, 0);
	const IOCode ANY_LED_NUM    = IOCode(DT_COMPONENT_LEDS, IO_LEDS, 1);
	const IOCode ANY_LED_SCROLL = IOCode(DT_COMPONENT_LEDS, IO_LEDS, 2);
}

#endif