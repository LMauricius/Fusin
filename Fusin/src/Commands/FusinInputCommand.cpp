#include "FusinInputGesture.h"
#include "FusinInputManager.h"
#include "FusinInputSignal.h"
#include "FusinDevice.h"
#include <algorithm>

namespace Fusin
{

	InputGesture::InputGesture(InputManager* im)
	{
		mInputManager = im;
		if (mInputManager) mInputManager->addGesture(this);

		for (IOType i : ALL_DEVICE_TYPES)
		{
			mDeviceIndices[i] = 0;
		}
		for (IOType i : ALL_CODE_TYPES)
		{
			mDeadZones[i] = 0.0f;
			mMaxValues[i] = MAX_FLOAT;
			mFactors[i] = 1.0f;
		}
		mEnabledInputTypes = IT_ANY;
	}

	InputGesture::~InputGesture()
	{
		mInputCodes.clear();
		mTrackedGestures.clear();
		if (mInputManager) mInputManager->removeGesture(this);
	}

	void InputGesture::setInputManager(InputManager* im)
	{
		if (mInputManager)
		{
			for (int i = 0; i < mInputCodes.size(); i++)
			{
				clearProccess(mInputCodes[i]);
			}
			mInputManager->removeGesture(this);
		}
		mInputManager = im;
		if (mInputManager)
		{
			for (int i = 0; i < mInputCodes.size(); i++)
			{
				assignProccess(mInputCodes[i]);
			}
			if (mInputManager) mInputManager->addGesture(this);
		}
	}

	void InputGesture::_plug(Device * d)
	{
		if (d->type() & mEnabledInputTypes)
		{
			for (int i = 0; i < mInputCodes.size(); i++)
			{
				if (mInputCodes[i].type & d->type()) assignProccess(mInputCodes[i]);
			}
		}
	}

	void InputGesture::_unplug(Device * d)
	{
		if (d->type() & mEnabledInputTypes)
		{
			for (int i = 0; i < mInputCodes.size(); i++)
			{
				if (mInputCodes[i].type & d->type()) clearProccess(mInputCodes[i]);
			}
		}
	}

	void InputGesture::_beginUpdate()
	{
		/*
		Reset mValue so the Manager can change it as needed
		*/
		mPrevValue = mValue;
		mValue = 0;
		for (auto& it : mValuesByType)
		{
			it.second = 0.0f;
		}
	}

	void InputGesture::_updateValue(float value, IOType t)
	{
		for (auto& it : mValuesByType)
		{
			if (it.first & t)
			{
				if (abs(value) >= mDeadZones[it.first] && abs(value) > abs(it.second))
				{
					it.second = ((value > 0) - (value < 0)) * std::min(abs(value), mMaxValues[it.first]) * mFactors[it.first];
				}
				if (abs(it.second) > abs(mValue))
				{
					mValue = it.second;
				}
			}
		}
	}

	void InputGesture::_endUpdate()
	{
		/*
		Check the values of tracked Gestures, set pressed and released values
		*/
		for (auto it = mTrackedGestures.begin(); it != mTrackedGestures.end(); it++)
		{
			_updateValue((*it)->value());
		}
		mPressed = (std::abs(mValue) >= mThreshold && std::abs(mPrevValue) < mThreshold);
		mReleased = (std::abs(mValue) < mThreshold && std::abs(mPrevValue) >= mThreshold);
	}


	void InputGesture::assign(int position, const InputGesture& source, Index first, Index last)
	{
		for (Index i = first; i < last; i++)
		{
			Index ind;
			if (position < 0)
			{
				ind = mInputCodes.getFreeSlot();
			}
			else
			{
				ind = position + i - first;
				if (!mInputCodes.isSlotFree(ind)) clearProccess(mInputCodes[ind]);
			}
			mInputCodes[ind] = source.mInputCodes[i];
			assignProccess(mInputCodes[ind]);
		}
	}

	void InputGesture::assignInputCode(const IOCode& ic, Index slot)
	{
		if (slot == -1)
		{
			slot = mInputCodes.getFreeSlot();
			mInputCodes[slot] = ic;
			assignProccess(mInputCodes[slot]);
		}
		else
		{
			if (!mInputCodes.isSlotFree(slot)) clearProccess(mInputCodes[slot]);
			mInputCodes[slot] = ic;
			assignProccess(mInputCodes[slot]);
		}
	}

	IOCode InputGesture::getInputCode(Index slot) const
	{
		return mInputCodes[slot];
	}

	Index InputGesture::findInputCode(const IOCode& ic) const
	{
		for (int i = 0; i < mInputCodes.size(); i++)
		{
			if (mInputCodes[i] == ic)
			{
				return i;
			}
		}
		return -1;
	}

	void InputGesture::clearInputCode(const IOCode& ic)
	{
		for (int i = mInputCodes.size()-1; i >= 0; i--)
		{
			if (mInputCodes[i] == ic)
			{
				clearProccess(mInputCodes[i]);
				mInputCodes[i] = IOCode::NULLCODE;
				mInputCodes.freeSlot(i);
			}
		}
	}

	void InputGesture::clearSlot(Index slot)
	{
		clearProccess(mInputCodes[slot]);
		mInputCodes[slot] = IOCode::NULLCODE;
		mInputCodes.freeSlot(slot);
	}

	void InputGesture::clearAll()
	{
		for (int i = 0; i < mInputCodes.size(); i++)
		{
			clearProccess(mInputCodes[i]);
		}
		mInputCodes.clear();
	}

	size_t InputGesture::slotCount() const
	{
		return mInputCodes.size();
	}

	Device * InputGesture::findDevice(const IOCode & ic)
	{
		if (mInputManager != nullptr && (ic.type & mEnabledInputTypes))
		{
			Device* d = mInputManager->getDevice(ic.type & IT_ANY_DEVICE,
				mDeviceIndices[ic.type & IT_ANY_DEVICE]);
			IOSignal* signal = mInputManager->getInputSignal(ic, mDeviceIndices[ic.type & IT_ANY_DEVICE]);
			if (d)
			{
				return d;
			}
		}
		return nullptr;
	}

	IOSignal * InputGesture::findSignal(const IOCode & ic)
	{
		if (mInputManager != nullptr && (ic.type & mEnabledInputTypes))
		{
			IOSignal* signal = mInputManager->getInputSignal(ic, mDeviceIndices[ic.type & IT_ANY_DEVICE]);
			if (signal) return signal;
		}
		return nullptr;
	}

	void InputGesture::assignProccess(const IOCode& ic)
	{
		if (ic.type & mEnabledInputTypes)
		{
			IOSignal* signal = findSignal(ic);
			if (signal)
			{
				signal->connect(this);
			}
		}
	}

	void InputGesture::clearProccess(const IOCode& ic)
	{
		if (ic.type & mEnabledInputTypes)
		{
			IOSignal* signal = findSignal(ic);
			if (signal)
			{
				signal->disconnect(this);
			}
		}
	}


	void InputGesture::trackGesture(Gesture* a)
	{
		mTrackedGestures.push_back(a);
	}

	void InputGesture::untrackGesture(Gesture* a)
	{
		mTrackedGestures.remove(a);
	}

	void InputGesture::untrackAll()
	{
		mTrackedGestures.clear();
	}

	const std::list<Gesture*>& InputGesture::_accessTrackedGestures() const
	{ 
		return mTrackedGestures; 
	}

	bool InputGesture::isGestureTracked(Gesture* a) const
	{
		return std::find(mTrackedGestures.begin(), mTrackedGestures.end(), a) != mTrackedGestures.end();
	}


	void InputGesture::setDeviceIndex(Index ind, IOType t)
	{
		for (int i = 0; i < mInputCodes.size(); i++)
		{
			if (mInputCodes[i].type & t)
			{
				clearProccess(mInputCodes[i]);
			}
		}
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) (*it).second = ind;
		}
		for (int i = 0; i < mInputCodes.size(); i++)
		{
			if (mInputCodes[i].type & t)
			{
				assignProccess(mInputCodes[i]);
			}
		}
	}

	Index InputGesture::getDeviceIndex(IOType t) const
	{
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void InputGesture::setEnabledInputTypes(IOType t)
	{
		for (int i = 0; i < mInputCodes.size(); i++)
		{
			if ((mInputCodes[i].type & t) && !(mInputCodes[i].type & mEnabledInputTypes))
				assignProccess(mInputCodes[i]);
			if (!(mInputCodes[i].type & t) && (mInputCodes[i].type & mEnabledInputTypes))
				clearProccess(mInputCodes[i]);
		}
		mEnabledInputTypes = t;
	}

	IOType InputGesture::getEnabledInputTypes() const
	{
		return mEnabledInputTypes;
	}


	void InputGesture::setDeadZone(float dz, IOType t)
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) (*it).second = dz;
		}
	}

	float InputGesture::getDeadZone(IOType t) const
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void InputGesture::setMaxValue(float val, IOType t)
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) (*it).second = val;
		}
	}

	float InputGesture::getMaxValue(IOType t) const
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void InputGesture::setFactor(float f, IOType t)
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) (*it).second = f;
		}
	}

	float InputGesture::getFactor(IOType t) const
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

}