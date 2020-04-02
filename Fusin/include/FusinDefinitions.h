#ifndef _FUSIN_DEFINITIONS_H
#define _FUSIN_DEFINITIONS_H

#include <string>
#include <sstream>
#include <limits>

namespace Fusin
{

	typedef wchar_t Char;
	typedef std::wstring String;
	typedef std::wstringstream StringStream;

	const double PI = 4 * atan(1);

	const float MAX_FLOAT = (std::numeric_limits<float>::max)();
	const float MIN_FLOAT = (std::numeric_limits<float>::min)();

	const int MAX_INT = (std::numeric_limits<int>::max)();
	const int MIN_INT = (std::numeric_limits<int>::min)();

	enum ConnectionMode { BT, USB };
	enum BatteryLevel { BL_EMPTY = 0, BL_CRITICAL = 5, BL_LOW = 25, BL_MEDIUM = 50, BL_FULL = 100 };

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

#define FUSE_SIGNED_RANGE_BYTE(VAL) ((float)(VAL) / 255.0f * 2.0f - 1.0f)
#define FUSE_UNSIGNED_RANGE_BYTE(VAL) ((float)(VAL) / 255.0f)
#define FUSE_SIGNED_RANGE_2BYTE(VAL) ((float)(VAL) / 65535.0f * 2.0f - 1.0f)
#define FUSE_UNSIGNED_RANGE_2BYTE(VAL) ((float)(VAL) / 65535.0f)
#define FUSE_SIGNED_RANGE_12BIT(VAL) ((float)(VAL) / 4095.0f * 2.0f - 1.0f)
#define FUSE_UNSIGNED_RANGE_12BIT(VAL) ((float)(VAL) / 4095.0f)


#ifdef _WIN32

	#define _FUSIN_INPUT_SYSTEM1 RawInputSystem
	#define _FUSIN_INPUT_SYSTEM1_HEADER "FuseRawInputSystem.h"
	/*#define _FUSIN_INPUT_SYSTEM2 HIDInputSystem
	#define _FUSIN_INPUT_SYSTEM2_HEADER "FuseHIDInputSystem.h"*/
	#define _FUSIN_INPUT_SYSTEM3 XInputSystem
	#define _FUSIN_INPUT_SYSTEM3_HEADER "FuseXInputSystem.h"

	struct tagMSG;
	typedef tagMSG MSG;
	#define _FUSIN_MESSAGE_TYPE MSG
	#define _FUSIN_WINDOW_PTR_TYPE HWND

	#define RI_MOUSE_HWHEEL 0x0800

#endif

#endif