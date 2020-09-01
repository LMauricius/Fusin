#include "IOSystems/FusinIOSystem.h"

namespace Fusin
{

	IOSystem::IOSystem(IOType type)
		: mTypes(type)
	{
	}

	IOSystem::~IOSystem()
	{

	}

	void IOSystem::updateDeviceList()
	{
	}

	void IOSystem::initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
	{
		mDeviceEnumerator = de;
	}

	void IOSystem::update()
	{
	}

#ifdef _WIN32
	void IOSystem::handleMessage(const MSG* msg)
	{
	}
#endif


}