#include "Devices/FusinDeviceEnumerator.h"
#include "Devices/FusinDevice.h"
#include "Components/FusinDeviceComponent.h"
#include "Commands/FusinCommand.h"

#define FOR_LISTENERS(EXP) for (auto& lis : mDeviceEnumeratorListeners) {lis->EXP;}

namespace Fusin
{

	DeviceEnumerator::DeviceEnumerator()
	{
	}


	DeviceEnumerator::~DeviceEnumerator()
	{
		for (auto& typeSlotPair : mDeviceStructure)
		{
			SlotArray<Device*> &slots = typeSlotPair.second;
			for (int i = 0; i < slots.size(); i++)
			{
				if (!slots.isSlotFree(i)) delete slots[i];
			}
		}
	}

	Index DeviceEnumerator::registerDevice(Device * dev, bool registerComponents)
	{
		SlotArray<Device*> &slots = mDeviceStructure[dev->type()];
		Index ind = slots.getFreeSlot();
		slots[ind] = dev;

		for (DeviceComponent* comp : dev->getDeviceComponents())
		{
			registerComponent(comp);
		}

		FOR_LISTENERS(deviceRegistered(this, dev));
		return ind;
	}

	void DeviceEnumerator::unregisterDevice(Device * dev)
	{
		SlotArray<Device*> &slots = mDeviceStructure[dev->type()];
		slots.freeSlot(slots.find(dev));

		for (DeviceComponent* comp : dev->getDeviceComponents())
		{
			unregisterComponent(comp);
		}

		FOR_LISTENERS(deviceUnregistered(this, dev));
	}

	void DeviceEnumerator::registerComponent(DeviceComponent* comp)
	{
		SlotArray<DeviceComponent*>& slots = mComponentStructure[comp->deviceType()];
		Index ind = slots.getFreeSlot();
		slots[ind] = comp;
	}

	void DeviceEnumerator::unregisterComponent(DeviceComponent* comp)
	{
		SlotArray<DeviceComponent*>& slots = mComponentStructure[comp->deviceType()];
		slots.freeSlot(slots.find(comp));
	}

	Device* DeviceEnumerator::getDevice(DeviceType t, Index index) const
	{
		auto it = mDeviceStructure.find(t);
		if (it != mDeviceStructure.end())
		{
			const SlotArray<Device*> &slots = it->second;
			if (!slots.isSlotFree(index))
			{
				return slots[index];
			}
		}
		return nullptr;
	}

	size_t DeviceEnumerator::maxDeviceIndex(DeviceType t) const
	{
		auto it = mDeviceStructure.find(t);
		if (it != mDeviceStructure.end())
		{
			return it->second.size();
		}
		return 0;
	}

	DeviceComponent* DeviceEnumerator::getDeviceComponent(DeviceType t, Index index) const
	{
		return nullptr;
	}

	size_t DeviceEnumerator::maxDeviceComponentIndex(DeviceType t) const
	{
		return size_t();
	}

	void DeviceEnumerator::update(DurationMS msElapsed)
	{
		FOR_LISTENERS(preUpdate(this));

		// TO DO: Needs optimized implementation
		for (DeviceType type : ALL_DEVICE_TYPES)
		{
			for (Index i = 0; i < maxDeviceIndex(type); i++)
			{
				if (Device* device = getDevice(type, i))
				{
					device->_update(msElapsed);
				}
			}
		}

		FOR_LISTENERS(postUpdate(this));
	}

	void DeviceEnumerator::addListener(DeviceEnumeratorListener* listener)
	{
		mDeviceEnumeratorListeners.push_back(listener);
	}

	void DeviceEnumerator::removeListener(DeviceEnumeratorListener* listener)
	{
		mDeviceEnumeratorListeners.remove(listener);
	}

	DeviceEnumeratorListener::~DeviceEnumeratorListener() {}
	void DeviceEnumeratorListener::deviceRegistered(DeviceEnumerator * de, Device * d) {}
	void DeviceEnumeratorListener::deviceUnregistered(DeviceEnumerator * de, Device * d) {}

	void DeviceEnumeratorListener::preUpdate(DeviceEnumerator* de) {}
	void DeviceEnumeratorListener::postUpdate(DeviceEnumerator* de) {}

}