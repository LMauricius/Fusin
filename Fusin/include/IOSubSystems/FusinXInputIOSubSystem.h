#ifndef _FUSIN_XINPUT_SYSTEM_H
#define _FUSIN_XINPUT_SYSTEM_H

#include "IOSubSystems/FusinIOSubSystem.h"
#include "FusinPrerequisites.h"
#include <map>
#include <vector>
#include <list>

namespace Fusin
{
	class InputManager;
	class Device;
	class XInputDevice;

	class XInputSystem : public IOSubSystem
	{
	public:
		XInputSystem();
		~XInputSystem();

		void initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window);
		void updateDeviceList();
		void update();

	protected:
		std::vector<XInputDevice*> mDevices;
	};

}

#endif