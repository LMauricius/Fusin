#include "IOSubSystems/FusinIOSubSystem.h"

namespace Fusin
{

	IOSubSystem::IOSubSystem(IOType type)
		: mTypes(type)
	{
	}

	IOSubSystem::~IOSubSystem()
	{

	}

	void IOSubSystem::updateDeviceList()
	{
	}

	void IOSubSystem::initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
	{
		mDeviceEnumerator = de;
	}

	void IOSubSystem::update()
	{
	}

#ifdef _WIN32
	void IOSubSystem::handleMessage(const MSG* msg)
	{
	}
#endif


}