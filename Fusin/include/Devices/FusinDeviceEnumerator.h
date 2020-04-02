#ifndef _FUSIN_DEVICE_ENUMERATOR_H
#define _FUSIN_DEVICE_ENUMERATOR_H

#include "FusinInputCode.h"
#include "FusinSlotArray.h"
#include <map>
#include <list>

namespace Fusin
{
	class Device;
	class Gesture;

	class DeviceEnumerator
	{
	public:
		DeviceEnumerator();
		~DeviceEnumerator();

		virtual void registerDevice(Device* dev);
		virtual void unregisterDevice(Device* dev);

		/*
		Returns the device that supports ioType of the specified type with the specified deviceIndex.
		If no device supports the ioType type or there is no device with the specified index, returns nullptr.
		*/
		Device* getDevice(IOType t, Index index = 0) const;
		size_t maxDeviceIndex(IOType t) const;

	protected:
		std::map<IOType, SlotArray<Device*> > mDeviceStructure;
	};

	class DeviceEnumeratorListener
	{
	public:
		virtual ~DeviceEnumeratorListener() = 0;

		virtual void deviceRegistered(DeviceEnumerator* de, Device* d);
		virtual void deviceUnregistered(DeviceEnumerator* de, Device* d);
	};

}

#endif