#ifndef _FUSIN_RAW_INPUT_KEYBOARD_HANDLER_H
#define _FUSIN_RAW_INPUT_KEYBOARD_HANDLER_H

#ifdef FUSIN_BUILD_RAW_INPUT

	#include "FusinRawInputDeviceHandler.h"
	#include <Windows.h>

	namespace Fusin
	{
		class KeyboardDevice;

		class _FUSIN_EXPORT RawInputKeyboardHandler : public RawInputDeviceHandler
		{
			friend RawInputSystem;

		public:
			RawInputKeyboardHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
			~RawInputKeyboardHandler();
			
			bool initialize();
			void update();
			void handleRawInput(PRAWINPUT pRawInput);
		};

	}

#endif // FUSIN_BUILD_RAW_INPUT

#endif