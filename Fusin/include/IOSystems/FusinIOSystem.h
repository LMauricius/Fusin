#ifndef _FUSIN_INPUT_SYSTEM_H
#define _FUSIN_INPUT_SYSTEM_H

#include "IOCodes/FusinIOCode.h"

#ifdef _WIN32
	#include <Windows.h>
#endif

namespace Fusin
{
	class DeviceEnumerator;

	class IOSystem
	{
	public:
		IOSystem(IOFlags flags);
		virtual ~IOSystem();

		virtual void initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window);

		inline IOFlags getFlags() { return mFlags; }

		virtual void updateDeviceList() = 0;
		virtual void update() = 0;

#ifdef _WIN32
		virtual void handleMessage(const MSG* msg) = 0;
#endif

	protected:
		DeviceEnumerator* mDeviceEnumerator;
		IOFlags mFlags;
	};

}

#endif