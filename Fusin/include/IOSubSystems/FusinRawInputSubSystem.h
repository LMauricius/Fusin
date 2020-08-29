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

	class RawInputSystem : public InputSystem
	{
	public:
		RawInputSystem(DeviceEnumerator* de, const std::map<String, String>& config, void* window);
		~RawInputSystem();

		void updateDeviceList();
		void update();
		void handleMessage(const MSG* msg);

	protected:
		HWND mWindow;
		bool mCreatedHiddenWindow;
		RAWINPUTDEVICE *mRIDevices;

		std::map<HANDLE, RawInputDeviceHandler*> mHandleMap;
		std::map<HANDLE, int> mHandleMessageCounter;

		bool mReceiveInputOutsideFocus, mSupportXInput, mSupportDS, mSupportNintendo;
		int mMaxGamepadMessages;
	};

}

#endif // FUSIN_BUILD_RAW_INPUT

#endif