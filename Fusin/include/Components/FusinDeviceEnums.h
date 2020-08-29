#pragma once
#ifndef _FUSIN_DEVICE_ENUMS_H
#define _FUSIN_DEVICE_ENUMS_H

namespace Fusin
{
	enum ConnectionMode { CM_UNKNOWN = -1, CM_USB, CM_BT, CM_LAN };

	typedef float BatteryLevel;
	const BatteryLevel BAT_UNKNOWN = -1;
	const BatteryLevel BAT_EMPTY = 0.0f;
	const BatteryLevel BAT_CRITICAL = 5.0f;
	const BatteryLevel BAT_LOW = 25.0f;
	const BatteryLevel BAT_MEDIUM = 50.0f;
	const BatteryLevel BAT_FULL = 100.0f;

	typedef unsigned int LEDFlags;
	const LEDFlags LED_NONE = 0;
	const LEDFlags LED_1 = 1 << 0;
	const LEDFlags LED_2 = 1 << 1;
	const LEDFlags LED_3 = 1 << 2;
	const LEDFlags LED_4 = 1 << 3;
	const LEDFlags LED_CAPS = LED_1;
	const LEDFlags LED_NUM = LED_2;
	const LEDFlags LED_SCROLL = LED_3;
	const LEDFlags LED_AUTO = -1;
}

#endif