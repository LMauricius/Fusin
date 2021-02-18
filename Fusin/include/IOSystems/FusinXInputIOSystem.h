#ifndef _FUSIN_XINPUT_SYSTEM_H
#define _FUSIN_XINPUT_SYSTEM_H

#include "IOSystems/FusinIOSystem.h"
#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_XINPUT

	#include <map>
	#include <vector>
	#include <list>

	namespace Fusin
	{
		class InputManager;
		class Device;
		class XInputDevice;

		class _FUSIN_EXPORT XInputIOSystem : public IOSystem
		{
		public:
			XInputIOSystem();
			~XInputIOSystem();

			void initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window);
			void updateDeviceList();
			void update();

		protected:
			std::vector<XInputDevice*> mDevices;
		};

	}

#endif//FUSIN_BUILD_XINPUT

#endif