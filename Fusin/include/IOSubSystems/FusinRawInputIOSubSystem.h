#ifndef _FUSIN_RAW_INPUT_SYSTEM_H
#define _FUSIN_RAW_INPUT_SYSTEM_H

#include "FusinIOSubSystem.h"
#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	#include <map>
	#include <vector>
	#include <list>
	#include <windows.h>

	namespace Fusin
	{
		class RawInputDeviceHandler;
		class Device;
		class KeyboardDevice;
		class MouseDevice;
		class GamepadDevice;
		class DSDevice;

		class RawInputIOSubSystem : public IOSubSystem
		{
		public:
			RawInputIOSubSystem();
			~RawInputIOSubSystem();

			void initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window);

			void updateDeviceList();
			void update();
			void handleMessage(const MSG* msg);

		protected:
			HWND mWindow;
			bool mCreatedHiddenWindow;// If the hidden message window has been created
			RAWINPUTDEVICE *mRIDevices;

			std::map<HANDLE, RawInputDeviceHandler*> mHandlerPerHandle;
			std::map<HANDLE, int> mMessageCounterPerHandle;

			bool mReceiveInputOutsideFocus, mSupportXInput, mSupportDS, mSupportNintendo;
			int mMaxGamepadMessages;
		};

	}

	// Windows headers lack this definition for horizontal scroll, but it *supposedly* works
	#define RI_MOUSE_HWHEEL 0x0800

#endif // FUSIN_BUILD_RAW_INPUT

#endif