#ifndef _FUSIN_RAW_INPUT_SYSTEM_H
#define _FUSIN_RAW_INPUT_SYSTEM_H

#include "FusinInputSystem.h"
#include "FusinPrerequisites.h"
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

		bool mReceiveInputOutsideFocus, mSupportXBox, mSupportDS, mSupportNintendo;
		int mMaxGamepadMessages;
	};

}

#endif