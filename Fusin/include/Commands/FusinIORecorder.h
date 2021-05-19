#ifndef _FUSIN_ANY_LISTENER_H
#define _FUSIN_ANY_LISTENER_H

#include "Commands/FusinCommand.h"
#include "Devices/FusinDeviceEnumerator.h"
#include "Devices/FusinDevice.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class _FUSIN_EXPORT IORecorder : public DeviceEnumeratorListener
	{
	public:
		IORecorder(DeviceEnumerator* de = nullptr);
		~IORecorder();

		void setDeviceEnumerator(DeviceEnumerator* de);

		IOSignal* getStrongestIOSignal() const;
		Index getStrongestDeviceIndex() const;

		/*
		Sets the Index of the Device whose signals will be connected, for the specified device type.
		Use Index 0 is for global devices.
		*/
		void setDeviceIndex(Index ind, DeviceType t = DT_ANY);
		/*
		Sets the Index of the Device whose signals will be recorded, for the specified device type.
		Use Index 0 is for global devices.
		EXAMPLE: setDeviceIndex(1, IO_KEYBOARD | IO_MOUSE) <= Sets the index for keyboard and mouse device inputs to 1
		*/
		void setDeviceIndex(Index ind, IOFlags filter);
		Index getDeviceIndex(DeviceType t = DT_ANY) const;
		/*
		Sets the filter that specifies which IOSignals will be recorded.
		*/
		void setEnabledInputTypes(IOFlags filter);
		IOFlags getEnabledInputTypes() const;

	protected:
		DeviceEnumerator *mDeviceEnumerator;
		IOFlags mEnabledInputFlags;
		// Choosen indices to track (not specified indices default to DT_ANY's index)
		std::map < DeviceType, Index > mDeviceIndices;
		bool mInvalidated;

		// Actual (device,index) pairs we track, per DeviceType
		std::map < DeviceType, std::set<std::pair<Device*, Index>> > mDeviceIndexPairSets;
		IOSignal* mStrongestIOSignal;
		Index mStrongestDeviceIndex;

		void trackProccess();
		void untrackProccess();
		void replug();
		void replugDeviceType(DeviceType t);

		void deviceRegistered(DeviceEnumerator* de, Device* d);
		void deviceUnregistered(DeviceEnumerator* de, Device* d);
		void preUpdate(DeviceEnumerator* de);
		void postUpdate(DeviceEnumerator* de);
	};
}

#endif