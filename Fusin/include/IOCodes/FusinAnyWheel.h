#ifndef _FUSIN_WHEEL_H
#define _FUSIN_WHEEL_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{
	const IOCode ANY_WHEEL_VERTICAL   = IOCode(DT_COMPONENT_WHEEL, IO_DIRECTION, 0);
	const IOCode ANY_WHEEL_HORIZONTAL = IOCode(DT_COMPONENT_WHEEL, IO_DIRECTION, 1);
	const IOCode ANY_WHEEL_DOWN  = ANY_WHEEL_VERTICAL.negativeVersion();
	const IOCode ANY_WHEEL_UP    = ANY_WHEEL_VERTICAL.positiveVersion();
	const IOCode ANY_WHEEL_LEFT  = ANY_WHEEL_HORIZONTAL.negativeVersion();
	const IOCode ANY_WHEEL_RIGHT = ANY_WHEEL_HORIZONTAL.positiveVersion();

	const const std::vector<String> WHEEL_ROTATION_NAMES = {
		FUSIN_STR("V_Scroll"), FUSIN_STR("H_Scroll")
	};

}

#endif