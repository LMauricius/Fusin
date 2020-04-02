#ifndef _FUSIN_VIBRATION_H
#define _FUSIN_VIBRATION_H

#include "FusinIOCode.h"
#include <vector>

namespace Fusin
{
	const IOCode ANY_VIBRATION_LEFT_FORCE      = IOCode(DT_COMPONENT_VIBRATION, IO_VIBRATION, 0);
	const IOCode ANY_VIBRATION_LEFT_AMPLITUDE  = IOCode(DT_COMPONENT_VIBRATION, IO_VIBRATION, 1);
	const IOCode ANY_VIBRATION_LEFT_DURATION   = IOCode(DT_COMPONENT_VIBRATION, IO_VIBRATION, 2);
	const IOCode ANY_VIBRATION_RIGHT_FORCE     = IOCode(DT_COMPONENT_VIBRATION, IO_VIBRATION, 3);
	const IOCode ANY_VIBRATION_RIGHT_AMPLITUDE = IOCode(DT_COMPONENT_VIBRATION, IO_VIBRATION, 4);
	const IOCode ANY_VIBRATION_RIGHT_DURATION  = IOCode(DT_COMPONENT_VIBRATION, IO_VIBRATION, 5);

	const std::vector<String> VIBRATION_NAMES{
		FUSIN_STR("Vibration_Left_Force"), FUSIN_STR("Vibration_Left_Amplitude"), FUSIN_STR("Vibration_Left_Duration"),
		FUSIN_STR("Vibration_Right_Force"), FUSIN_STR("Vibration_Right_Amplitude"), FUSIN_STR("Vibration_Right_Duration")
	};
}

#endif