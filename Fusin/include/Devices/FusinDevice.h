#ifndef _FUSIN_DEVICE_H
#define _FUSIN_DEVICE_H

#include "Components/FusinIOSignal.h"
#include "Components/FusinDeviceComponent.h"
#include "Components/FusinBatteryComponent.h"
#include "Components/FusinDeviceEnums.h"

#include <list>
#include <set>
#include <map>
#include <functional>

namespace Fusin
{
	class InputManager;
	class Command;
	typedef std::list<Command*> GesturePtrList;
	class DeviceListener;

	class Device
	{
		friend InputManager;

	public:
		/*
		@param name The name of this device
		@param devType The type of this device, DT_NONE is default
		@param components The components that will be registered at construction using the protected function registerComponent().
		*/
		Device(String name = FUSIN_STR(""), DeviceType devType = DT_NONE,
			const std::list<DeviceComponent*>& components = {}, bool hasBattery = false);
		~Device();

		// Device info
		/*
		Returns the DeviceType of this device.
		*/
		inline DeviceType type() const { return mDeviceType; }
		/*
		Returns the io flags of this device and its components.
		*/
		inline IOFlags flags() const { return mFlags; }
		/*
		Returns the name of this device
		*/
		inline String name() const { return mName; }
		/*
		Returns the way this device is connected to the computer
		*/
		inline ConnectionMode connectionMode() const { return mConnectionMode; }

		/*
		Returns the current value of the specified IOCode.
		Returns 0.0 if the device doesn't support the specified IOCode. 
		*/
		float getValue(const IOCode& ic);

		/*
		Returns the device's IOSignal corresponding to the specified IOCode.
		Returns nullptr if no IOSignal corresponds to  the specified IOCode.
		*/
		IOSignal* getIOSignal(const IOCode& ic);
		/*
		Returns the first IOSignal corresponding to the filter.
		Returns nullptr if no IOSignal corresponds to  the filter.
		*/
		IOSignal* getFirstIOSignal(const IOFlags filter);
		/*
		Returns the IOSignal with the largest absolute value (distance) whose IOCode corresponds to the filter.
		If multiple InputSignals have the largest value, the first one is returned.
		Returns nullptr if no codes correspond to the filter.
		*/
		IOSignal* getStrongestIOSignal(const IOFlags filter);

		/*
		Returns the list of all registered device components of this device.
		*/
		std::vector<DeviceComponent*> getDeviceComponents() const;

		/*
		Resets all io values of the device to 0.
		The exact behavior is device dependant, as inputs of some devices (keyboard, mouse...) won't be recognized until the user
		releases the key/button and presses it again, while inputs of the devices that get updated on each InputManager::update()
		will be reset on update.
		*/
		void clear();

		/*
		Returns the string with all axis values and indices of buttons that are currently being held. Useful for debugging.
		*/
		virtual String getStateString();

		BatteryComponent battery;

		void _setName(String name);
		void _setConnectionMode(ConnectionMode mode);
		void _update(size_t msElapsed = 0);

	protected:
		DeviceType mDeviceType;
		IOFlags mFlags;
		String mName;
		ConnectionMode mConnectionMode;

		std::vector<DeviceComponent*> mIOTypeComponentMap;
		std::map<DeviceType, DeviceComponent*> mDeviceTypeComponentMap;
		std::map<IOType, DeviceComponent*> mIOTypeComponentMap;

		/*
		Assigns the component to the appropriate keys in mDeviceTypeComponentMap and mIOTypeComponentMap maps and
		adds the component's io flags to this device's flags
		*/
		void registerComponent(DeviceComponent* component);
	};

}

#endif