#ifndef _FUSIN_WIN_KC_CONVERT_H
#define _FUSIN_WIN_KC_CONVERT_H

#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	#include <windows.h>
	
	namespace Fusin
	{
        Char virtualKeycodeToVirtualKey(int vk, int scancode, USHORT flags);
	}

#endif // FUSIN_BUILD_RAW_INPUT

#endif