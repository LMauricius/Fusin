#ifndef _FUSIN_DEVICE_ENUMERATOR_H
#define _FUSIN_DEVICE_ENUMERATOR_H

#include "IOCodes/FusinIOCode.h"
#include "Utilities/FusinSlotArray.h"
#include <map>
#include <list>

namespace Fusin
{
	class Device;
	class DeviceComponent;
	class Command;
	class DeviceEnumeratorListener;

	class DeviceEnumerator
	{
	public:
		DeviceEnumerator();
		~DeviceEnumerator();

		virtual void registerDevice(Device* dev, bool registerComponents = true);
		virtual void unregisterDevice(Device* dev);
		virtual void registerComponent(DeviceComponent* comp);
		virtual void unregisterComponent(DeviceComponent* comp);

		/*
		Returns the device that has the device type with the specified deviceIndex.
		If no device has the type or there is no device with the specified index, returns nullptr.
		*/
		Device* getDevice(DeviceType t, Index index = 0) const;
		size_t maxDeviceIndex(DeviceType t) const;
		DeviceComponent* getDeviceComponent(DeviceType t, Index index = 0) const;
		size_t maxDeviceComponentIndex(DeviceType t) const;

		/*
		Updates all registered Devices
		*/
		virtual void update(TimeMS msElapsed = 0);
		void addListener(DeviceEnumeratorListener* listener);
		void removeListener(DeviceEnumeratorListener* listener);

	protected:
		std::map<DeviceType, SlotArray<Device*> > mDeviceStructure;
		std::map<DeviceType, SlotArray<DeviceComponent*> > mComponentStructure;
		std::list<DeviceEnumeratorListener*> mDeviceEnumeratorListeners;
	};

	class DeviceEnumeratorListener
	{
	public:
		virtual ~DeviceEnumeratorListener() = 0;

		virtual void deviceRegistered(DeviceEnumerator* de, Device* d);
		virtual void deviceUnregistered(DeviceEnumerator* de, Device* d);

		virtual void preUpdate(DeviceEnumerator* de);
		virtual void postUpdate(DeviceEnumerator* de);
	};

}

#endif