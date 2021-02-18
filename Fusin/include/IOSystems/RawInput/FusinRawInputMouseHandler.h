#ifndef _FUSIN_RAW_INPUT_MOUSE_DEVICE_HANDLER_H
#define _FUSIN_RAW_INPUT_MOUSE_DEVICE_HANDLER_H

#include "FusinRawInputDeviceHandler.h"

namespace Fusin
{
	class _FUSIN_EXPORT RawInputMouseHandler : public RawInputDeviceHandler
	{
	public:
		RawInputMouseHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
		~RawInputMouseHandler();
		
		bool initialize();
		void handleRawInput(PRAWINPUT pRawInput);
	};

}

#endif