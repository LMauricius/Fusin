#ifndef _FUSIN_RGB_H
#define _FUSIN_RGB_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{
	const IOCode ANY_RGB_RED   = IOCode(DT_COMPONENT_RGB, IO_RGB, 0);
	const IOCode ANY_RGB_GREEN = IOCode(DT_COMPONENT_RGB, IO_RGB, 1);
	const IOCode ANY_RGB_BLUE  = IOCode(DT_COMPONENT_RGB, IO_RGB, 2);

	const std::vector<String> COLOR_NAMES{
		FUSIN_STR("Red"), FUSIN_STR("Green"), FUSIN_STR("Blue")
	};
}

#endif