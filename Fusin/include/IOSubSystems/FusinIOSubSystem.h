#ifndef _FUSIN_INPUT_SYSTEM_H
#define _FUSIN_INPUT_SYSTEM_H

#include "IOCodes/FusinIOCode.h"

namespace Fusin
{
	class DeviceEnumerator;

	class IOSubSystem
	{
	public:
		IOSubSystem();
		virtual ~IOSubSystem();

		virtual void initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window);

		inline IOType getTypes() { return mTypes; }

		virtual void updateDeviceList() = 0;
		virtual void update() = 0;

#ifdef _WIN32
		virtual void handleMessage(const MSG* msg) = 0;
#endif

	protected:
		DeviceEnumerator* mDeviceEnumerator;
		IOType mTypes;
	};

}

#endif