#ifndef _FUSIN_XINPUT_SYSTEM_H
#define _FUSIN_XINPUT_SYSTEM_H

#include "FusinInputSystem.h"
#include "FusinPrerequisites.h"
#include <map>
#include <vector>
#include <list>

namespace Fusin
{
	class InputManager;
	class Device;
	class XBoxDevice;

	class XInputSystem : public InputSystem
	{
		friend InputManager;

	public:
		void update();

	protected:
		XInputSystem(DeviceEnumerator* de, const std::map<String, String>& config, void* window);
		~XInputSystem();

		InputManager* mInputManager;

		std::vector<XBoxDevice*> mDevices;

	};

}

#endif