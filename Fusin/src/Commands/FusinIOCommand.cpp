#include "Commands/FusinIOCommand.h"
#include "Components/FusinIOSignal.h"
#include "Devices/FusinDevice.h"
#include <algorithm>
#include <limits>

namespace Fusin
{

	IOCommand::IOCommand(DeviceEnumerator* devEnum) :
		Command(devEnum)
	{
		mDeadZones[DT_ANY][IO_ANY] = 0.0f;
		mMaxValues[DT_ANY][IO_ANY] = std::numeric_limits<float>::max();
		mFactors[DT_ANY][IO_ANY] = 1.0f;
	}

	IOCommand::~IOCommand()
	{
		mIOCodes.clear();
		mTrackedCommands.clear();
	}

	void IOCommand::assign(Index position, const IOCommand& source, Index first, Index last)
	{
		for (Index i = first; i < last; i++)
		{
			Index ind;
			if (position < 0)
				ind = mIOCodes.getFreeSlot();
			else
				ind = position + i - first;
			mIOCodes[ind] = source.mIOCodes[i];
			replugSlot(ind);
		}
	}

	void IOCommand::assignIOCode(const IOCode& ic, Index slot)
	{
		if (slot == -1)
		{
			slot = mIOCodes.getFreeSlot();
			mIOCodes[slot] = ic;
			replugSlot(slot);
		}
		else
		{
			mIOCodes[slot] = ic;
			replugSlot(slot);
		}
	}

	void IOCommand::assign(Index position, const IOCommand& source, Index first, Index last)
	{
	}

	IOCode IOCommand::getIOCode(Index slot) const
	{
		return mIOCodes[slot];
	}

	Index IOCommand::findIOCode(const IOCode& ic) const
	{
		for (int i = 0; i < mIOCodes.size(); i++)
		{
			if (mIOCodes[i] == ic)
			{
				return i;
			}
		}
		return -1;
	}

	void IOCommand::clearIOCode(const IOCode& ic)
	{
		for (int i = mIOCodes.size() - 1; i >= 0; i--)
		{
			if (mIOCodes[i] == ic)
			{
				clearSlot(i);
			}
		}
	}

	void IOCommand::clearIOCode(IOFlags filter)
	{
		for (int i = mIOCodes.size() - 1; i >= 0; i--)
		{
			if (mIOCodes[i].fitsFilter(filter))
			{
				clearSlot(i);
			}
		}
	}

	void IOCommand::clearSlot(Index ind)
	{
		mIOCodes[ind] = IOCode::NULLCODE;
		mIOCodes.freeSlot(ind);
		mSignalIndexPairs.freeSlot(ind);
	}

	void IOCommand::clearAll()
	{
		mIOCodes.clear();
		mSignalIndexPairs.clear();
	}

	size_t IOCommand::slotCount() const
	{
		return mIOCodes.size();
	}

	void IOCommand::trackCommand(Command* a)
	{
		mTrackedCommands.push_back(a);
	}

	void IOCommand::untrackCommand(Command* a)
	{
		mTrackedCommands.remove(a);
	}

	void IOCommand::untrackAll()
	{
		mTrackedCommands.clear();
	}

	bool IOCommand::isCommandTracked(Command* a) const
	{
		return std::find(mTrackedCommands.begin(), mTrackedCommands.end(), a) != mTrackedCommands.end();
	}

	const std::list<Command*>& IOCommand::_accessTrackedCommands() const
	{
		return mTrackedCommands;
	}


	void IOCommand::setDeviceIndex(Index ind, DeviceType t)
	{
		if (t == DT_ANY)
		{
			mDeviceIndices.clear();
			mDeviceIndices[DT_ANY] = ind;
			replug();
		}
		else
		{
			mDeviceIndices[t] = ind;

			for (int i = 0; i < mIOCodes.size(); i++)
			{
				if (mIOCodes.isSlotFree(i) && mIOCodes[i].deviceType == t)
					replugSlot(i);
			}
		}
	}

	void IOCommand::setDeviceIndex(Index ind, IOFlags filter)
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

	Index IOCommand::getDeviceIndex(DeviceType t) const
	{
		auto it = mDeviceIndices.find(t);
		if (it == mDeviceIndices.end())
			return mDeviceIndices.at(DT_ANY);
		else
			return it->second;
	}

	void IOCommand::setEnabledInputTypes(IOFlags filter)
	{
		mEnabledInputFlags = filter;
	}

	IOFlags IOCommand::getEnabledInputTypes() const
	{
		return mEnabledInputFlags;
	}

	void IOCommand::setDeadZone(float dz, DeviceType deviceType, IOType ioType)
	{
		ValueModifierUtilities::setModifier(mDeadZones, dz, deviceType, ioType);
	}

	void IOCommand::setDeadZone(float dz, IOFlags filter)
	{
		ValueModifierUtilities::setModifier(mDeadZones, dz, filter);
	}

	float IOCommand::getDeadZone(DeviceType deviceType, IOType ioType) const
	{
		return ValueModifierUtilities::getModifier(mDeadZones, deviceType, ioType);
	}

	void IOCommand::setMaxValue(float val, DeviceType deviceType, IOType ioType)
	{
		ValueModifierUtilities::setModifier(mMaxValues, val, deviceType, ioType);
	}

	void IOCommand::setMaxValue(float val, IOFlags filter)
	{
		ValueModifierUtilities::setModifier(mMaxValues, val, filter);
	}

	float IOCommand::getMaxValue(DeviceType deviceType, IOType ioType) const
	{
		return ValueModifierUtilities::getModifier(mMaxValues, deviceType, ioType);
	}

	void IOCommand::setFactor(float f, DeviceType deviceType, IOType ioType)
	{
		ValueModifierUtilities::setModifier(mFactors, f, deviceType, ioType);
	}

	void IOCommand::setFactor(float f, IOFlags filter)
	{
		ValueModifierUtilities::setModifier(mFactors, f, filter);
	}

	float IOCommand::getFactor(DeviceType deviceType, IOType ioType) const
	{
		return ValueModifierUtilities::getModifier(mFactors, deviceType, ioType);
	}

	void IOCommand::update()
	{
		Command::update();

		// handle tracked gestures
		for (auto g : mTrackedCommands)
		{
			if (g->distance() > distance())
			{
				mValue = g->value();
				mStrongestIOCode = g->strongestIOCode();
				mStrongestDeviceIndex = g->strongestDeviceIndex();
			}
		}

		// handle slots
		for (Index i = 0; i < mSignalIndexPairs.size(); i++)
		{
			if (!mSignalIndexPairs.isSlotFree(i) && mIOCodes[i].fitsFilter(mEnabledInputFlags))
				syncValueSignal(mSignalIndexPairs[i].first, mSignalIndexPairs[i].second);
		}
	}

	void IOCommand::replug()
	{
		mSignalIndexPairs.clear();
		for (Index i = 0; i < mIOCodes.size(); i++)
			replugSlot(i);
	}

	void IOCommand::replugSlot(Index ind)
	{
		IOSignal* sig = nullptr;
		Index ind;

		if (!mIOCodes.isSlotFree(ind))
		{
			IOCode code = mIOCodes[ind];
			ind = getDeviceIndex(code.deviceType);

			if (code.deviceType >= DT_FIRST_COMPONENT && code.deviceType <= DT_LAST_COMPONENT)
			{
				DeviceComponent* comp = mDeviceEnumerator->getDeviceComponent(code.deviceType, ind);

				if (comp)
					sig = comp->getIOSignal(code);
			}
			else
			{
				Device* dev = mDeviceEnumerator->getDevice(code.deviceType, ind);

				if (dev)
					sig = dev->getIOSignal(code);
			}
		}

		if (sig)
			mSignalIndexPairs[ind] = { sig, ind };
		else
			mSignalIndexPairs.freeSlot(ind);
	}

	void ValueModifierUtilities::setModifier(std::map<DeviceType, std::map<IOType, float>>& modMap,
		float val, DeviceType deviceType, IOType ioType)
	{
		if (ioType == IO_ANY)
		{
			modMap[deviceType].clear();
			modMap[deviceType][IO_ANY] = val;
		}
		else
		{
			modMap[deviceType][ioType] = val;
		}
	}

	void ValueModifierUtilities::setModifier(std::map<DeviceType, std::map<IOType, float>>& modMap,
		float val, IOFlags filter)
	{
		for (IOType iot : ALL_INPUT_TYPES)
		{
			if (FLAG_PER_IO[iot] & filter & (IOF_ANY_INPUT | IOF_ANY_OUTPUT))
			{
				for (DeviceType devt : ALL_COMPONENT_TYPES)
				{
					if (BASE_FLAGS_PER_DEVICE[devt] & filter & IOF_ANY_DEVICE)
						setModifier(modMap, val, devt, iot);
				}

				for (DeviceType devt : ALL_DEVICE_TYPES)
				{
					if (BASE_FLAGS_PER_DEVICE[devt] & filter & IOF_ANY_DEVICE)
						setModifier(modMap, val, devt, iot);
				}
			}
		}
	}

	float ValueModifierUtilities::getModifier(const std::map<DeviceType, std::map<IOType, float>>& modMap,
		DeviceType deviceType, IOType ioType)
	{
		const float* modPtr;
		const std::map<IOType, float>* ioTMapPtr;

		auto it = modMap.find(deviceType);
		if (it == modMap.end())
			ioTMapPtr = &modMap.at(DT_ANY);
		else
			ioTMapPtr = &it->second;

		auto it2 = ioTMapPtr->find(ioType);
		if (it2 == ioTMapPtr->end())
			modPtr = &(*ioTMapPtr).at(IO_ANY);
		else
			modPtr = &it2->second;

		return *modPtr;
	}

	void IOCommand::syncValueSignal(IOSignal* sig, Index devInd)
	{
	}

}