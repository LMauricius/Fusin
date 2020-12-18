/*#include "IOCodes/FusinAnyListener.h"
#include <algorithm>

namespace Fusin
{

	AnyListener::AnyListener(InputManager* im)
	{
		mInputManager = im;
		if (mInputManager)
		{
			trackProccess();
			mInputManager->addGesture(this);
		}

		mTrackedDeviceIndices[IO_KEYBOARD] = 0;
		mTrackedDeviceIndices[IO_MOUSE] = 0;
		mTrackedDeviceIndices[IO_GAMEPAD] = 0;
		mTrackedDeviceIndices[IO_XInput] = 0;
		mTrackedDeviceIndices[IO_DS] = 0;
		for (IOType i : ALL_CODE_TYPES)
		{
			mDeadZones[i] = 0.0f;
			mMaxValues[i] = MAX_FLOAT;
			mFactors[i] = 1.0f;
		}
		mTrackedInputTypes = IO_ANY;
	}

	AnyListener::~AnyListener()
	{
		if (mInputManager)
		{
			untrackProccess();
			mInputManager->removeGesture(this);
		}
	}

	void AnyListener::setInputManager(InputManager* im)
	{
		if (mInputManager)
		{
			untrackProccess();
			mInputManager->removeGesture(this);
		}
		mInputManager = im;
		if (mInputManager)
		{
			trackProccess();
			mInputManager->addGesture(this);
		}
	}

	void AnyListener::_beginUpdate()
	{
		//Reset mValue before the _update
		mPrevValue = mValue;
		mValue = 0;
		mLastIOCode = IOCode::NULLCODE;
		mLastDeviceIndex = 0;
		for (auto& it : mValuesByType)
		{
			it.second = 0.0f;
		}
	}

	void AnyListener::_endUpdate()
	{
		//Set pressed and released values
		mPressed = (std::abs(mValue) >= mThreshold && std::abs(mPrevValue) < mThreshold);
		mReleased = (std::abs(mValue) < mThreshold && std::abs(mPrevValue) >= mThreshold);
	}

	IOCode AnyListener::getLastIOCode() const
	{
		return mLastIOCode;
	}

	unsigned int AnyListener::getLastDeviceIndex() const
	{
		return mLastDeviceIndex;
	}


	void AnyListener::setTrackedInputTypes(IOType t)
	{
		untrackProccess();
		mTrackedInputTypes = t;
		trackProccess();
	}

	IOType AnyListener::getTrackedInputTypes() const
	{
		return mTrackedInputTypes;
	}

	void AnyListener::setTrackedDeviceIndex(unsigned int ind, IOType t)
	{
		untrackProccess();
		for (auto it = mTrackedDeviceIndices.begin(); it != mTrackedDeviceIndices.end(); it++)
		{
			if ((*it).first & t) (*it).second = ind;
		}
		trackProccess();
	}

	unsigned int AnyListener::getTrackedDeviceIndex(IOType t) const
	{
		for (auto it = mTrackedDeviceIndices.begin(); it != mTrackedDeviceIndices.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}


	void AnyListener::setDeadZone(float dz, IOType t)
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) (*it).second = dz;
		}
	}

	float AnyListener::getDeadZone(IOType t) const
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void AnyListener::setMaxValue(float val, IOType t)
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) (*it).second = val;
		}
	}

	float AnyListener::getMaxValue(IOType t) const
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void AnyListener::setFactor(float f, IOType t)
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) (*it).second = f;
		}
	}

	float AnyListener::getFactor(IOType t) const
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}


	void AnyListener::trackProccess()
	{
		if (mInputManager != nullptr)
		{
			mInputManager->addListener(this);
			for (IOType t : ALL_DEVICE_TYPES)
			{
				if (mTrackedInputTypes & t)
				{
					size_t ind = mTrackedDeviceIndices[t];
					if (ind == 0)
					{
						for (size_t i = 1; i <= mInputManager->maxDeviceIndex(t); i++)
						{
							Device* d = mInputManager->getDevice(t, i);
							if (d)
							{
								d->addListener(this);
							}
						}
					}
					else
					{
						Device* d = mInputManager->getDevice(t, mTrackedDeviceIndices[t]);
						if (d)
						{
							d->addListener(this);
						}
					}
				}
			}
		}
	}

	void AnyListener::untrackProccess()
	{
		if (mInputManager != nullptr)
		{
			mInputManager->removeListener(this);
			for (IOType t : ALL_DEVICE_TYPES)
			{
				if (mTrackedInputTypes & t)
				{
					size_t ind = mTrackedDeviceIndices[t];
					if (ind == 0)
					{
						for (size_t i = 1; i <= mInputManager->maxDeviceIndex(t); i++)
						{
							Device* d = mInputManager->getDevice(t, i);
							if (d)
							{
								d->removeListener(this);
							}
						}
					}
					else
					{
						Device* d = mInputManager->getDevice(t, mTrackedDeviceIndices[t]);
						if (d)
						{
							d->removeListener(this);
						}
					}
				}
			}
		}
	}

	void AnyListener::deviceRegistered(InputManager* im, Device* d)
	{
		if (d->type() & mTrackedInputTypes && (d->index() == mTrackedDeviceIndices[d->type()] || mTrackedDeviceIndices[d->type()] == 0))
		{
			d->addListener(this);
		}
	}

	void AnyListener::valueUpdated(Device* device, const IOCode& ic, float val)
	{
		if ((ic.type & IO_ANY_CODE) & mTrackedInputTypes)
		{
			IOType t = ic.type;
			float calcV = ((val > 0) - (val < 0)) * std::min(abs(val), mMaxValues[t]) * mFactors[t];
			if (abs(val) >= mDeadZones[t] && abs(val) >= abs(mValuesByType[t]))
			{
				mValuesByType[t] = calcV;
			}
			if (abs(calcV) > abs(mValue))
			{
				mValue = calcV;
				mLastIOCode = ic;
				mLastDeviceIndex = device->index();
			}
		}
	}

}*/