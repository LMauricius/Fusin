#include "FusinPositionGesture.h"
#include "FusinInputManager.h"
#include <algorithm>

#define FOR_SUBACTIONS(EXP) xAxis.EXP; yAxis.EXP; directionAxis.EXP; leftDirection.EXP; rightDirection.EXP; upDirection.EXP; downDirection.EXP;
#define FOR_SUBACTIONS_EXCEPT_DIRAXIS(EXP) xAxis.EXP; yAxis.EXP; leftDirection.EXP; rightDirection.EXP; upDirection.EXP; downDirection.EXP;

namespace Fusin
{

	PositionGesture::PositionGesture(InputManager* im)
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

	PositionGesture::~PositionGesture()
	{
		if (mInputManager) mInputManager->removeGesture(this);
	}

	void PositionGesture::setInputManager(InputManager* im)
	{
		if (mInputManager) mInputManager->removeGesture(this);
		mInputManager = im;
		if (mInputManager) mInputManager->addGesture(this);
		FOR_SUBACTIONS(setInputManager(im))
	}

	float PositionGesture::x()
	{
		return mX;
	}

	float PositionGesture::y()
	{
		return mY;
	}

	float PositionGesture::direction()
	{
		return mDirection;
	}

	void PositionGesture::_beginUpdate()
	{
		mPrevValue = mValue;
		mPrevX = mX;
		mPrevY = mY;
		mPrevDirection = mDirection;
		mValue = 0;
		mX = 0;
		mY = 0;
		mDirection = 0.0f;
		for (auto& it : mValuesByType)
		{
			it.second = 0.0f;
		}
	}

	void PositionGesture::_endUpdate()
	{
		for (auto& it : mValuesByType)
		{
			float xv = 0, yv = 0, v;
			float d = directionAxis.value(it.first & IT_ANY_CODE);
			float xa = xAxis.value(it.first & IT_ANY_CODE);
			float ya = yAxis.value(it.first & IT_ANY_CODE);
			float xlr = rightDirection.value(it.first & IT_ANY_CODE) - leftDirection.value(it.first & IT_ANY_CODE);
			float yud = downDirection.value(it.first & IT_ANY_CODE) - upDirection.value(it.first & IT_ANY_CODE);

			if (abs(xa) > abs(xv)) xv = xa;
			if (abs(xlr) > abs(xv)) xv = xlr;
			if (abs(ya) > abs(yv)) yv = ya;
			if (abs(yud) > abs(yv)) yv = yud;

			v = sqrt(xv*xv + yv*yv);
			if (v >= mDeadZones[it.first])
			{
				it.second = std::min(v, mMaxValues[it.first]) * mFactors[it.first];
				if (it.second > mValue)
				{
					mValue = it.second;
					mDirection = atan2(yv, xv) * 180 / PI + 90;
					mX = xv / sqrt(xv*xv + yv*yv) * mValue;
					mY = yv / sqrt(xv*xv + yv*yv) * mValue;
				}
			}
			if (d != 0 && mDeadZones[it.first] < 1)
			{
				it.second = std::min(1.0f, mMaxValues[it.first]) * mFactors[it.first];
				if (it.second > mValue)
				{
					mValue = it.second;
					mDirection = d;
					mX = round(cos((d - 90) * PI / 180)*100000.0f) / 100000.0f;// rounded to avoid imprecisions by cos and sin
					mY = round(sin((d - 90) * PI / 180)*100000.0f) / 100000.0f;
				}
			}
		}
		if (mValue > 0)
		{
			if (mDirection > 360) mDirection -= 360;
			if (mDirection <= 0) mDirection += 360;

			/*mX = cos((mDirection - 90) / 180 * PI) * mValue;
			mY = sin((mDirection - 90) / 180 * PI) * mValue;*/
		}

		mPressed = (mValue >= mThreshold && mPrevValue < mThreshold);
		mReleased = (mValue < mThreshold && mPrevValue >= mThreshold);
	}


	void PositionGesture::setDeviceIndex(unsigned int ind, IOType t)
	{
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) (*it).second = ind;
		}
		FOR_SUBACTIONS(setDeviceIndex(ind, t))
	}

	unsigned int PositionGesture::getDeviceIndex(IOType t)
	{
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void PositionGesture::setEnabledInputTypes(IOType t)
	{
		mEnabledInputTypes = t;
		FOR_SUBACTIONS(setEnabledInputTypes(t))
	}

	IOType PositionGesture::getEnabledInputTypes()
	{
		return mEnabledInputTypes;
	}


	void PositionGesture::setDeadZone(float dz, IOType t)
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) (*it).second = dz;
		}
	}

	float PositionGesture::getDeadZone(IOType t)
	{
		for (auto it = mDeadZones.begin(); it != mDeadZones.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void PositionGesture::setMaxValue(float val, IOType t)
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) (*it).second = val;
		}
	}

	float PositionGesture::getMaxValue(IOType t)
	{
		for (auto it = mMaxValues.begin(); it != mMaxValues.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void PositionGesture::setFactor(float f, IOType t)
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) (*it).second = f;
		}
	}

	float PositionGesture::getFactor(IOType t)
	{
		for (auto it = mFactors.begin(); it != mFactors.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

}