#ifndef _FUSIN_RAW_INPUT_MOUSE_DEVICE_HANDLER_H
#define _FUSIN_RAW_INPUT_MOUSE_DEVICE_HANDLER_H

#include "FusinRawInputDeviceHandler.h"
#include <Windows.h>

namespace Fusin
{
	class MouseDevice;

	class RawInputMouseHandler : public RawInputDeviceHandler
	{
		friend RawInputSystem;

	public:
		RawInputMouseHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
		~RawInputMouseHandler();
		void handleRawInput(PRAWINPUT pRawInput);
	};

}

#endif