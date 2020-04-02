#ifndef _FUSIN_INPUT_SYSTEM_H
#define _FUSIN_INPUT_SYSTEM_H

#include "FusinInputCode.h"

namespace Fusin
{
	class DeviceEnumerator;

	class InputSystem
	{
	public:
		InputSystem(DeviceEnumerator* enumerator);
		virtual ~InputSystem();

		inline IOType getTypes() { return mTypes; }

		virtual void updateDeviceList() = 0;
		virtual void update() = 0;

#ifdef FUSIN_MESSAGE_TYPE
		virtual void handleMessage(const FUSIN_MESSAGE_TYPE* msg) = 0;
#endif

	protected:

		DeviceEnumerator* mDeviceEnumerator;
		IOType mTypes;
	};

}

#endif