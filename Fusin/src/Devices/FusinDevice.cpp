#include "Devices/FusinDevice.h"

namespace Fusin
{

	Device::Device(String name, DeviceType devType, const std::list<DeviceComponent*>& components, bool hasBattery)
		: mName(name)
		, mDeviceType(devType)
		, mFlags(IOF_NONE)
		, battery(devType)
	{
		if (hasBattery)
			registerComponent(&battery);

		for (auto comp : components)
			registerComponent(comp);
	}

	Device::~Device()
	{
	}


	// Values
	float Device::getValue(const IOCode& ic)
	{
		IOSignal* signal = getIOSignal(ic);
		if (signal)
			return signal->value();
		else
			return 0;
	}

	IOSignal * Device::getIOSignal(const IOCode & ic)
	{
		if ((ic.deviceType == DT_NONE || ic.deviceType >= DT_FIRST_DEVICE) && ic.deviceType != mDeviceType)
			return nullptr;

		// check by component type
		if (ic.deviceType >= DT_FIRST_COMPONENT && ic.deviceType <= DT_LAST_COMPONENT)
		{
			auto it = mDeviceTypeComponentMap.find(ic.deviceType);
			if (it != mDeviceTypeComponentMap.end())
				return it->second->getIOSignal(ic);
		}
		else if (ic.deviceType == DT_ANY || ic.deviceType == mDeviceType)
		{
			auto it = mIOTypeComponentMap.find(ic.ioType);
			if (it != mIOTypeComponentMap.end())
				return it->second->getIOSignal(ic);
		}

		return nullptr;
	}

	IOSignal* Device::getFirstIOSignal(const IOFlags filter)
	{
		if (!(filter & mFlags & IOF_ANY_DEVICE) || !(filter & mFlags & IOF_ANY_INPUT))
			return nullptr;

		for (auto typeCompPair : mDeviceTypeComponentMap)
		{
			if (auto signal = typeCompPair.second->getFirstIOSignal(filter))
				return signal;
		}

		return nullptr;
	}

	IOSignal* Device::getStrongestIOSignal(const IOFlags filter)
	{
		if (!(filter & mFlags & IOF_ANY_DEVICE) || !(filter & mFlags & IOF_ANY_INPUT))
			return nullptr;

		float max = 0.0f;
		IOSignal* result = nullptr;

		if (!(filter & mFlags & IOF_ANY_DEVICE) || !(filter & mFlags & IOF_ANY_INPUT))
			return nullptr;

		for (auto typeCompPair : mDeviceTypeComponentMap)
		{
			if (auto signal = typeCompPair.second->getStrongestIOSignal(filter))
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

	std::vector<DeviceComponent*> Device::getDeviceComponents() const
	{
		std::vector<DeviceComponent*> comps;

		for (auto& typeCompPair : mDeviceTypeComponentMap)
		{
			comps.push_back(typeCompPair.second);
		}

		return comps;
	}

	void Device::clear()
	{
		for (auto& typeCompPair : mDeviceTypeComponentMap)
			typeCompPair.second->clear();
	}

	String Device::getStateString()
	{
		return (battery.energy.value() >= 0? battery.getStateString() : String());
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