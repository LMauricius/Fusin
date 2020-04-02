#include "FusinGesture.h"
#include "FusinInputManager.h"

namespace Fusin
{

	Gesture::Gesture()
	{
		for (IOType i : ALL_CODE_TYPES)
		{
			mValuesByType[i] = 0.0f;
		}
		mThreshold = 0.5f;
	}

	Gesture::~Gesture()
	{

	}

	void Gesture::setInputManager(InputManager* im)
	{
		mInputManager = im;
	}

	void Gesture::_beginUpdate()
	{

	}

	void Gesture::_updateValue(float value, DeviceType deviceType, IOType ioType)
	{
		for (auto& it : mValuesByType)
		{
			if (it.first & t)
			{
				if (abs(value) > abs(it.second))
				{
					it.second = value;
				}
			}
		}
		if (abs(value) > abs(mValue))
		{
			mValue = value;
		}
	}

	void Gesture::_plug(Device * d)
	{
	}

	void Gesture::_unplug(Device * d)
	{
	}

	void Gesture::_endUpdate()
	{

	}


	float Gesture::value()
	{
		return mValue;
	}

	float Gesture::value(IOType t)
	{
		for (auto& it : mValuesByType)
		{
			if (it.first & t) return it.second;
		}
		return 0.0f;
	}

	float Gesture::distance()
	{
		return std::abs(mValue);
	}

	bool Gesture::pressed()
	{
		return mPressed;
	}

	bool Gesture::released()
	{
		return mReleased;
	}

	bool Gesture::check()
	{
		return std::abs(mValue) > mThreshold;
	}

	float Gesture::prevValue()
	{
		return mPrevValue;
	}

	float Gesture::prevDistance()
	{
		return std::abs(mPrevValue);
	}

	void Gesture::setThreshold(float threshold)
	{
		mThreshold = threshold;
	}

}