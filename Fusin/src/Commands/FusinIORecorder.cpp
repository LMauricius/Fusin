#include "Commands/FusinIORecorder.h"
#include <algorithm>

namespace Fusin
{

	IORecorder::IORecorder(DeviceEnumerator* de):
		mInvalidated(false)
	{
		mDeviceIndices[DT_ANY] = 0;

		mDeviceEnumerator = de;
		trackProccess();

		mEnabledInputFlags = IOF_ANY;
	}

	IORecorder::~IORecorder()
	{
		untrackProccess();
	}

	void IORecorder::setDeviceEnumerator(DeviceEnumerator* de)
	{
		untrackProccess();
		mDeviceEnumerator = de;
		trackProccess();
	}

	IOSignal* IORecorder::getStrongestIOSignal() const
	{
		return mStrongestIOSignal;
	}

	Index IORecorder::getStrongestDeviceIndex() const
	{
		return mStrongestDeviceIndex;
	}


	void IORecorder::setDeviceIndex(Index ind, DeviceType t)
	{
		if (t == DT_ANY)
		{
			mDeviceIndices.clear();
			mDeviceIndices[DT_ANY] = ind;
		}
		else
		{
			mDeviceIndices[t] = ind;
		}
		mInvalidated = true;
	}

	void IORecorder::setDeviceIndex(Index ind, IOFlags filter)
	{
		for (DeviceType devt : ALL_COMPONENT_TYPES)
		{
			if (BASE_FLAGS_PER_DEVICE[devt] & filter & IOF_ANY_DEVICE)
				setDeviceIndex(ind, devt);
		}

		for (DeviceType devt : ALL_DEVICE_TYPES)
		{
			if (BASE_FLAGS_PER_DEVICE[devt] & filter & IOF_ANY_DEVICE)
				setDeviceIndex(ind, devt);
		}
	}

	Index IORecorder::getDeviceIndex(DeviceType t) const
	{
		auto it = mDeviceIndices.find(t);
		if (it == mDeviceIndices.end())
			return mDeviceIndices.at(DT_ANY);
		else
			return it->second;
	}

	void IORecorder::setEnabledInputTypes(IOFlags filter)
	{
		mEnabledInputFlags = filter;
		mInvalidated = true;
	}

	IOFlags IORecorder::getEnabledInputTypes() const
	{
		return mEnabledInputFlags;
	}


	void IORecorder::trackProccess()
	{
		if (mDeviceEnumerator != nullptr)
		{
			mDeviceEnumerator->addListener(this);
		}
		replug();
	}

	void IORecorder::untrackProccess()
	{
		if (mDeviceEnumerator != nullptr)
		{
			mDeviceEnumerator->removeListener(this);
		}
		replug();
	}

	void IORecorder::replug()
	{
		mDeviceIndexPairSets.clear();
		if (mDeviceEnumerator != nullptr)
		{
			for (DeviceType t : ALL_DEVICE_TYPES)
			{
				replugDeviceType(t);
			}
		}
		mInvalidated = false;
	}
	
	void IORecorder::replugDeviceType(DeviceType t)
	{
		// if device enabled
		if (mEnabledInputFlags & BASE_FLAGS_PER_DEVICE[t] & IOF_ANY_DEVICE)
		{
			Index ind = getDeviceIndex(t);
			if (ind == 0)
			{
				// add all devices
				for (size_t i = 1; i <= mDeviceEnumerator->maxDeviceIndex(t); i++)
				{
					Device* d = mDeviceEnumerator->getDevice(t, i);
					if (d)
					{
						mDeviceIndexPairSets[t].insert({d, i});
					}
				}
			}
			else
			{
				// add device with index ind
				Device* d = mDeviceEnumerator->getDevice(t, ind);
				if (d)
				{
					mDeviceIndexPairSets[t].insert({d, ind});
				}
			}
		}
	}

	void IORecorder::deviceRegistered(DeviceEnumerator* de, Device* d)
	{
		replugDeviceType(d->type());
	}

	void IORecorder::deviceUnregistered(DeviceEnumerator* de, Device* d)
	{
		replugDeviceType(d->type());
	}

	void IORecorder::preUpdate(DeviceEnumerator* de)
	{
		mStrongestIOSignal = nullptr;
		mStrongestDeviceIndex = -1;
	}

	void IORecorder::postUpdate(DeviceEnumerator* de)
	{
		float maxDis = 0.0f;
		
		if (mInvalidated)
		{
			replug();
		}

		for (auto& typeSetPair : mDeviceIndexPairSets)
		{
			for (auto& devIndexPair : typeSetPair.second)
			{
				IOSignal *s = devIndexPair.first->getStrongestIOSignal(mEnabledInputFlags);
				if (s && s->distance() > maxDis)
				{
					maxDis = s->distance();
					mStrongestIOSignal = s;
					mStrongestDeviceIndex = devIndexPair.second;
				}
			}
		}
	}

}