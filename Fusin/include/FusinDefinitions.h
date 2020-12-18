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