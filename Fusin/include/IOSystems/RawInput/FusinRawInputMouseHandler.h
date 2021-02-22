#ifndef _FUSIN_RAW_INPUT_MOUSE_DEVICE_HANDLER_H
#define _FUSIN_RAW_INPUT_MOUSE_DEVICE_HANDLER_H

#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_RAW_INPUT

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

#endif // FUSIN_BUILD_RAW_INPUT

#endif