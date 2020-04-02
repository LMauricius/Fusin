#pragma once
#ifndef _FUSIN_DEVICE_COMPONENT_H
#define _FUSIN_DEVICE_COMPONENT_H

#include "FusinPrerequisites.h"
#include "FusinIOSignal.h"

#include <list>
#include <set>
#include <map>
#include <functional>

namespace Fusin
{

	class DeviceComponent
	{
	public:
		DeviceComponent(
			const std::map<IOType, std::vector<IOSignal*>*>& orderedSignalsMap,
			const std::map<IOType, std::map<Index, IOSignal*>*>& mappedSignalsMap
		);
		~DeviceComponent();

		/*
		Returns the io types supported by this device.
		*/
		virtual DeviceType deviceType() const = 0;

		/*
		Returns the io types supported by this device.
		*/
		virtual IOFlags flags() const = 0;

		/*
		Returns the current value of the specified IOCode.
		Returns 0.0 if the device doesn't support the specified IOCode.
		*/
		float getValue(const IOCode& ic) const;

		/*
		Returns the device's IOSignal corresponding to the specified IOCode.
		Returns nullptr if no IOSignal corresponds to  the specified IOCode.
		*/
		IOSignal* getInputSignal(const IOCode& ic) const;
		/*
		Returns the first IOSignal corresponding to the filter.
		Returns nullptr if no IOSignal corresponds to  the filter.
		*/
		IOSignal* getFirstInputSignal(const IOFlags filter) const;
		/*
		Returns the IOSignal with the largest absolute value (distance) whose IOCode corresponds to the filter.
		If multiple InputSignals have the largest value, the first one is returned.
		Returns nullptr if no codes correspond to the filter.
		*/
		IOSignal* getStrongestInputSignal(const IOFlags filter) const;

		/*
		Resets all ioType values of the device to 0.
		The exact behavior is device dependant, as inputs of some devices (keyboard, mouse...) won't be recognized until the user
		releases the key/button and presses it again, while inputs of the devices that get updated on each InputManager::_update()
		will be reset on _update.
		*/
		virtual void clear();

		/*
		Returns the string with all axis values and indices of buttons that are currently being held. Useful for debugging.
		*/
		virtual String getStateString();

		virtual void _update(size_t msElapsed = 0);
		/*
		Cover system is used to connect the global (0th index) device components to actual device components.
		A global DeviceComponent automatically sets each of its values to the most influential value
		of the DeviceComponents it covers.
		Setting a value of a global DeviceComponent changes the values of covered DeviceComponents.
		The covered DeviceComponents are stored in a set, so they can't be added or removed multiple times.

		The covered component must be of the same type as the cover component, otherwise std::invalid_argument is thrown
		*/
		void _coverDeviceComponent(DeviceComponent* component);
		void _uncoverDeviceComponent(DeviceComponent* component);

	protected:
		std::map<IOType, std::vector<IOSignal*>*> mOrderedSignalsByType;
		std::map<IOType, std::map<Index, IOSignal*>*> mMappedSignalsByType;

		std::set<DeviceComponent*> mCoveredComponents;
	};

}

#endif