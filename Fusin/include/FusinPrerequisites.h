#pragma once
#ifndef _FUSIN_PREREQUISITES_H
#define _FUSIN_PREREQUISITES_H

#include "FusinPlatform.h"
#include <string>
#include <sstream>
#include <limits>
#include <math.h>

namespace Fusin
{
#if FUSIN_USE_WCHAR == 1
	typedef wchar_t Char;
	typedef std::wstring String;
	typedef std::wstringstream StringStream;

	// Use FUSIN_STR(STR) on String literals to always get compatible string type, no matter theChar type
	#define FUSIN_STR(STR) L##STR
#else
	typedef char Char;
	typedef std::string String;
	typedef std::stringstream StringStream;

	// Use FUSIN_STR(STR) on String literals to always get compatible string type, no matter theChar type
#   define FUSIN_STRING(STR) STR
#endif

	typedef unsigned int Index;
	typedef unsigned long TimeMS;

	const double PI = 4 * atan(1);
	const double RADTODEG_FACTOR = 180.0 / PI;
	const double DEGTORAD_FACTOR = PI / 180.0;

	const float MAX_FLOAT = (std::numeric_limits<float>::max)();
	const float MIN_FLOAT = (std::numeric_limits<float>::min)();

	const int MAX_INT = (std::numeric_limits<int>::max)();
	const int MIN_INT = (std::numeric_limits<int>::min)();
}


/*#define FUSIN_SIGNED_RANGE_BYTE(VAL) ((float)(VAL) / 255.0f * 2.0f - 1.0f)
#define FUSIN_UNSIGNED_RANGE_BYTE(VAL) ((float)(VAL) / 255.0f)
#define FUSIN_SIGNED_RANGE_2BYTE(VAL) ((float)(VAL) / 65535.0f * 2.0f - 1.0f)
#define FUSIN_UNSIGNED_RANGE_2BYTE(VAL) ((float)(VAL) / 65535.0f)
#define FUSIN_SIGNED_RANGE_12BIT(VAL) ((float)(VAL) / 4095.0f * 2.0f - 1.0f)
#define FUSIN_UNSIGNED_RANGE_12BIT(VAL) ((float)(VAL) / 4095.0f)*/
/*#define FUSIN_RANGE_BIT(VAL, BIT) (bool)(VAL & (1 << BIT))
//#define FUSIN_MASK1(START, END) ((~0ULL >> (sizeof(unsigned long long)-END))<<START)
#define FUSIN_MASK1(START, END) (((1ULL << (END-START))-1)<<START)
#define FUSIN_MASK0(START, END) (~FUSIN_MASK1(START, END))
#define SET_FLAG(VAR, FLAGS, SET) VAR = (VAR & ~(FLAGS)) | (FLAGS) * (SET)*/


#ifdef _WIN32

	/*#define _FUSIN_INPUT_SYSTEM1 RawInputSystem
	#define _FUSIN_INPUT_SYSTEM1_HEADER "FusinRawInputSystem.h"*/
	/*#define _FUSIN_INPUT_SYSTEM3 XInputSystem
	#define _FUSIN_INPUT_SYSTEM3_HEADER "FusinXInputSystem.h"*/
/*
	struct tagMSG;
	typedef tagMSG MSG;
	#define FUSIN_MESSAGE_TYPE MSG
	#define FUSIN_WINDOW_PTR_TYPE HWND*/

#endif

#endif