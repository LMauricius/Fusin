#include "FusinShakeGesture.h"
#include "FusinInputManager.h"
#include <algorithm>

#define FOR_SUBACTIONS(EXP) axis1.EXP; axis2.EXP; axis3.EXP;

namespace Fusin
{

	ShakeGesture::ShakeGesture(InputManager* im)
	{
		mInputManager = im;
		if (mInputManager) mInputManager->addGesture(this);
		FOR_SUBACTIONS(setInputManager(im))

			mDeviceIndices[IT_KEYBOARD] = 0;
		mDeviceIndices[IT_MOUSE] = 0;
		mDeviceIndices[IT_GAMEPAD] = 0;
		mDeviceIndices[IT_XBOX] = 0;
		mDeviceIndices[IT_DS] = 0;
		for (IOType i : ALL_CODE_TYPES)
		{
			mDeadZones[i] = 0.0f;
			mMaxValues[i] = MAX_FLOAT;
			mFactors[i] = 1.0f;
		}
		mEnabledInputTypes = IT_ANY;
	}

	ShakeGesture::~ShakeGesture()
	{
		if (mInputManager) mInputManager->removeGesture(this);
	}

	void ShakeGesture::setInputManager(InputManager* im)
	{
		if (mInputManager) mInputManager->removeGesture(this);
		mInputManager = im;
		if (mInputManager) mInputManager->addGesture(this);
		FOR_SUBACTIONS(setInputManager(im))
	}

	void ShakeGesture::_beginUpdate()
	{
		mPrevValue = mValue;
		mValue = 0;
		for (auto& it : mValuesByType)
		{
			it.second = 0.0f;
		}
	}

	void ShakeGesture::_endUpdate()
	{
		for (auto& it : mValuesByType)
		{
			float a1 = axis1.value(it.first & IT_ANY_CODE);
			float a2 = axis2.value(it.first & IT_ANY_CODE);
			float a3 = axis3.value(it.first & IT_ANY_CODE);
			float& lastA1 = mLastAxis1ByType[it.first];
			float& lastA2 = mLastAxis2ByType[it.first];
			float& lastA3 = mLastAxis3ByType[it.first];
			float dz = mDeadZones[it.first];
			float d = sqrt(a1*a1 + a2*a2 + a3*a3);

			if (d >= dz)
			{
				if (lastA1 != 0.0f || lastA2 != 0.0f || lastA3 != 0.0f)
				{
					float s = (a1*lastA1 + a2*lastA2 + a3*lastA3) / d / dz;

					if (s <= -dz)
					{
						it.second = std::min(-s, mMaxValues[it.first]) * mFactors[it.first];
						if (it.second > mValue)
						{
							mValue = it.second;
						}
					}
				}

				lastA1 = a1;
				lastA2 = a2;
				lastA3 = a3;
			}
		}

		mPressed = (mValue >= mThreshold && mPrevValue < mThreshold);
		mReleased = (mValue < mThreshold && mPrevValue >= mThreshold);
	}


	void ShakeGesture::setDeviceIndex(unsigned int ind, IOType t)
	{
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) (*it).second = ind;
		}
		FOR_SUBACTIONS(setDeviceIndex(ind, t))
	}

	unsigned int ShakeGesture::getDeviceIndex(IOType t)
	{
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void ShakeGesture::setEnabledInputTypes(IOType t)
	{
		mEnabledInputTypes = t;
		FOR_SUBACTIONS(setEnabledInputTypes(t))
	}

	IOType ShakeGesture::getEnabledInputTypes()
	{
		return mEnabledInputTypes;
	}


	void ShakeGesture::setDeadZone(float dz, IOType t)
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) (*it).second = dz;
		}
	}

	float ShakeGesture::getDeadZone(IOType t)
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void ShakeGesture::setMaxValue(float val, IOType t)
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) (*it).second = val;
		}
	}

	float ShakeGesture::getMaxValue(IOType t)
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void ShakeGesture::setFactor(float f, IOType t)
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) (*it).second = f;
		}
	}

	float ShakeGesture::getFactor(IOType t)
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

}