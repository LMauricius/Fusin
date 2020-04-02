#include "FusinInputSystem.h"

namespace Fusin
{

	InputSystem::InputSystem(DeviceEnumerator * enumerator)
		: mDeviceEnumerator(enumerator)
	{
	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::updateDeviceList()
	{
	}

	void InputSystem::update()
	{

	}

#ifdef FUSIN_MESSAGE_TYPE
	void InputSystem::handleMessage(const FUSIN_MESSAGE_TYPE* msg)
	{

	}
#endif


}