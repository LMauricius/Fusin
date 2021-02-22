#ifndef _FUSIN_WIN_KC_CONVERT_H
#define _FUSIN_WIN_KC_CONVERT_H

#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	namespace Fusin
	{
        Char virtualKeycodeToVirtualKey(int vk);
	}

#endif // FUSIN_BUILD_RAW_INPUT

#endif