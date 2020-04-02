#include "FusinDevice.h"

namespace Fusin
{

	Device::Device(String name, DeviceType devType, const std::list<DeviceComponent*>& components)
		: mName(name)
		, mDeviceType(devType)
		, mFlags(IOF_NONE)
	{
		for (auto comp : components)
			registerComponent(comp);
	}

	Device::~Device()
	{
	}


	// Values
	float Device::getValue(const IOCode& ic)
	{
		IOSignal* signal = getInputSignal(ic);
		if (signal)
			return signal->value();
		else
			return 0;
	}

	IOSignal * Device::getInputSignal(const IOCode & ic)
	{
		if ((ic.deviceType == DT_NONE || ic.deviceType >= DT_FIRST_DEVICE) && ic.deviceType != mDeviceType)
			return nullptr;

		// check by component type
		if (ic.deviceType >= DT_FIRST_COMPONENT && ic.deviceType <= DT_LAST_COMPONENT)
		{
			auto it = mDeviceTypeComponentMap.find(ic.deviceType);
			if (it != mDeviceTypeComponentMap.end())
				return it->second->getInputSignal(ic);
		}
		else if (ic.deviceType == DT_ANY || ic.deviceType == mDeviceType)
		{
			auto it = mIOTypeComponentMap.find(ic.ioType);
			if (it != mIOTypeComponentMap.end())
				return it->second->getInputSignal(ic);
		}

		return nullptr;
	}

	IOSignal* Device::getFirstInputSignal(const IOFlags filter)
	{
		if (!(filter & mFlags & IOF_ANY_DEVICE) || !(filter & mFlags & IOF_ANY_INPUT))
			return nullptr;

		for (auto typeCompPair : mDeviceTypeComponentMap)
		{
			if (auto signal = typeCompPair.second->getFirstInputSignal(filter))
				return signal;
		}

		return nullptr;
	}

	IOSignal* Device::getStrongestInputSignal(const IOFlags filter)
	{
		if (!(filter & mFlags & IOF_ANY_DEVICE) || !(filter & mFlags & IOF_ANY_INPUT))
			return nullptr;

		float max = 0.0f;
		IOSignal* result = nullptr;

		if (!(filter & mFlags & IOF_ANY_DEVICE) || !(filter & mFlags & IOF_ANY_INPUT))
			return nullptr;

		for (auto typeCompPair : mDeviceTypeComponentMap)
		{
			if (auto signal = typeCompPair.second->getStrongestInputSignal(filter))
			{
				if (signal->distance() > max)
				{
					max = signal->distance();
					result = signal;
				}
			}
		}

		return result;
	}

	void Device::clear()
	{
		for (auto& typeCompPair : mDeviceTypeComponentMap)
			typeCompPair.second->clear();
	}

	String Device::getStateString()
	{
		return String();
	}


	// Internal
	void Device::_setName(String name)
	{
		mName = name;
	}

	void Device::_setConnectionMode(ConnectionMode mode)
	{
		mConnectionMode = mode;
	}

	void Device::_update(size_t msElapsed)
	{
		for (auto& typeCompPair : mDeviceTypeComponentMap)
			typeCompPair.second->_update(msElapsed);
	}

	void Device::registerComponent(DeviceComponent* component)
	{
		mFlags |= component->flags();

		// add based on the device type
		mDeviceTypeComponentMap[component->deviceType()] = component;

		// add based on the IOTypes
		for (auto t : ALL_INPUT_TYPES)
			if (FLAG_PER_IO[t] & component->flags())
				mIOTypeComponentMap[t] = component;
		
		for (auto t : ALL_OUTPUT_TYPES)
			if (FLAG_PER_IO[t] & component->flags())
				mIOTypeComponentMap[t] = component;
	}
}