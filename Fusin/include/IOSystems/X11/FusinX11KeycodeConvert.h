#ifndef _FUSIN_X11_DEVICE_HANDLER_H
#define _FUSIN_X11_DEVICE_HANDLER_H

#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_X11

	namespace Fusin
	{
        Char keysymToVirtualKey(int keysym);
	}

#endif // FUSIN_BUILD_X11

#endif