#include "FusinDeviceEnumerator.h"
#include "FusinDevice.h"
#include "FusinGesture.h"

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

	void DeviceEnumerator::registerDevice(Device * dev)
	{
		SlotArray<Device*> &slots = mDeviceStructure[dev->type()];
		Index ind = slots.getFreeSlot();
		slots[ind] = dev;
		dev->_setIndex(ind);
	}

	void DeviceEnumerator::unregisterDevice(Device * dev)
	{
		SlotArray<Device*> &slots = mDeviceStructure[dev->type()];
		slots.freeSlot(slots.find(dev));
	}

	Device* DeviceEnumerator::getDevice(IOType t, Index index) const
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

	size_t DeviceEnumerator::maxDeviceIndex(IOType t) const
	{
		auto it = mDeviceStructure.find(t);
		if (it != mDeviceStructure.end())
		{
			return it->second.size();
		}
		return 0;
	}

	DeviceEnumeratorListener::~DeviceEnumeratorListener() {}
	void DeviceEnumeratorListener::deviceRegistered(DeviceEnumerator * de, Device * d) {}
	void DeviceEnumeratorListener::deviceUnregistered(DeviceEnumerator * de, Device * d) {}

}