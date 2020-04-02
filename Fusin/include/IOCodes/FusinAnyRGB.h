#ifndef _FUSIN_RGB_H
#define _FUSIN_RGB_H

#include "FusinIOCode.h"
#include <vector>

namespace Fusin
{
	const IOCode ANY_RGB_RED   = IOCode(DT_COMPONENT_RGB, IO_RGB, 0);
	const IOCode ANY_RGB_GREEN = IOCode(DT_COMPONENT_RGB, IO_RGB, 1);
	const IOCode ANY_RGB_BLUE  = IOCode(DT_COMPONENT_RGB, IO_RGB, 2);

	const std::vector<String> VIBRATION_NAMES{
		FUSIN_STR("RGB_Red"), FUSIN_STR("RGB_Green"), FUSIN_STR("RGB_Blue")
	};
}

#endif