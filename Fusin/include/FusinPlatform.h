#ifndef _FUSIN_PLATFORM_H
#define _FUSIN_PLATFORM_H

#include "FusinConfig.h"

#ifdef FUSIN_DLL_BUILD
#	define _FUSIN_EXPORT __declspec( dllexport )
#else
#	ifdef FUSIN_DLL
#		define _FUSIN_EXPORT __declspec( dllimport )
#	else
#		define _FUSIN_EXPORT
#	endif
#endif

#endif