#ifndef _FUSIN_DPAD_H
#define _FUSIN_DPAD_H

#include "FusinIOCode.h"
#include <vector>

namespace Fusin
{
	const IOCode ANY_DPAD_ANGLE = IOCode(DT_COMPONENT_DPAD, IO_ANGLE, 0);
	const IOCode ANY_DPAD_UP    = IOCode(DT_COMPONENT_DPAD, IO_DIRECTION, 0);
	const IOCode ANY_DPAD_DOWN  = IOCode(DT_COMPONENT_DPAD, IO_DIRECTION, 1);
	const IOCode ANY_DPAD_LEFT  = IOCode(DT_COMPONENT_DPAD, IO_DIRECTION, 2);
	const IOCode ANY_DPAD_RIGHT = IOCode(DT_COMPONENT_DPAD, IO_DIRECTION, 3);

	const std::vector<String> DPAD_ANGLE_NAMES{
		FUSIN_STR("D-Pad_Angle")
	};
	const std::vector<String> DPAD_DIRECTION_NAMES{
		FUSIN_STR("D-Pad_Up"), FUSIN_STR("D-Pad_Down"), FUSIN_STR("D-Pad_Left"), FUSIN_STR("D-Pad_Right")
	};

}

#endif