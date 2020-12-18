#pragma once
#ifndef _FUSIN_BATTERY_H
#define _FUSIN_BATTERY_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{
	const IOCode ANY_BATTERY_ENERGY = IOCode(DT_COMPONENT_BATTERY, IO_BATTERY, 0);
	const IOCode ANY_BATTERY_CHARGING = IOCode(DT_COMPONENT_DPAD, IO_BATTERY, 1);

	const std::vector<String> BATTERY_NAMES{
		FUSIN_STR("Energy"), FUSIN_STR("Charging")
	};

}

#endif