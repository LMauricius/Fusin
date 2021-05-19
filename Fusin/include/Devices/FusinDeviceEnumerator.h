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

	class _FUSIN_EXPORT DeviceEnumerator
	{
	public:
		DeviceEnumerator();
		virtual ~DeviceEnumerator();

		/*
		If registerComponents is enabled, the DeviceComponents of the specified device
		will be registered automatically.
		Returns index of the registered device
		*/
		virtual Index registerDevice(Device* dev, bool registerComponents = true);
		virtual void unregisterDevice(Device* dev);
		virtual void registerComponent(DeviceComponent* comp);
		virtual void unregisterComponent(DeviceComponent* comp);

		/*
		Returns the device that has the device type t with the specified deviceIndex.
		If no device has the type or there is no device with the specified index, returns nullptr.
		Index 0 is usually reserved for the global device, so keep that in mind if registering manually.
		*/
		Device* getDevice(DeviceType t, Index index = 0) const;
		/*
		Returns the current max index of devices of the specified type.
		This can include unused indices. So if there are devices with indices 1 and 3
		for a specific type, but 0 and 2 are skipped, this returns 3.
		*/
		size_t maxDeviceIndex(DeviceType t) const;

		// Analoguous to getDevice()
		DeviceComponent* getDeviceComponent(DeviceType t, Index index = 0) const;
		// Analoguous to maxDeviceIndex()
		size_t maxDeviceComponentIndex(DeviceType t) const;

		/*
		Updates all registered Devices
		*/
		virtual void update(DurationMS msElapsed = 0);
		void addListener(DeviceEnumeratorListener* listener);
		void removeListener(DeviceEnumeratorListener* listener);

	protected:
		std::map<DeviceType, SlotArray<Device*> > mDeviceStructure;
		std::map<DeviceType, SlotArray<DeviceComponent*> > mComponentStructure;
		std::list<DeviceEnumeratorListener*> mDeviceEnumeratorListeners;
	};

	/*
	Inherit this class by objects that should listen to the DeviceEnumerator events
	and register them to the DeviceEnumerator.
	*/
	class _FUSIN_EXPORT DeviceEnumeratorListener
	{
	public:
		virtual ~DeviceEnumeratorListener() = 0;

		// called after a device has been registered
		virtual void deviceRegistered(DeviceEnumerator* de, Device* d) = 0;
		// called after a device has been unregistered
		virtual void deviceUnregistered(DeviceEnumerator* de, Device* d) = 0;

		// called before the devices are updated
		virtual void preUpdate(DeviceEnumerator* de) = 0;
		// called after the devices were updated
		virtual void postUpdate(DeviceEnumerator* de) = 0;
	};

}

#endif