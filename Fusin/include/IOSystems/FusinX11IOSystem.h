#ifndef _FUSIN_X11_SYSTEM_H
#define _FUSIN_X11_SYSTEM_H

#include "IOSystems/FusinIOSystem.h"
#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_X11

	#include <map>
	#include <vector>
	#include <list>

	#include <X11/Xlib.h>

	namespace Fusin
	{
		class InputManager;
		class Device;
		class KeyboardDevice;
		class MouseDevice;

		class _FUSIN_EXPORT X11IOSystem : public IOSystem
		{
		public:
			X11IOSystem();
			~X11IOSystem();

			void initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window);
			void updateDeviceList();
			void update();
			void handleMessage(XEvent* msg);

		protected:
			Window mWindow;
			Display *mDisplay;
			bool mCreatedWindow;// If the message window has been created
			bool mUsingXInput;// X11 Xinput extension
			int mXIExtOpcode;// Opcode for loaded XInput extension

			int mLastMouseX, mLastMouseY;

			std::map<int, KeyboardDevice*> mKeyboardDevicesPerID;
			std::map<int, MouseDevice*> mMouseDevicesPerID;
		};

	}

#endif//FUSIN_BUILD_XINPUT

#endif