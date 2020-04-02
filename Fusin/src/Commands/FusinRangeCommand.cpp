#include "FusinRangeGesture.h"

namespace Fusin
{

	RangeGesture::RangeGesture(InputManager* im)
		: InputGesture(im)
		, mMin(MIN_FLOAT)
		, mMax(MAX_FLOAT)
	{

	}

	RangeGesture::~RangeGesture()
	{

	}

	void RangeGesture::_updateValue(float val, IOType t)
	{
		if (val >= mMin && val <= mMax)
		{
			Gesture::_updateValue(val, t);
		}
	}

	void RangeGesture::setRange(float min, float max)
	{
		mMin = min;
		mMax = max;
	}

	float RangeGesture::getRangeMin() const
	{
		return mMin;
	}

	float RangeGesture::getRangeMax() const
	{
		return mMax;
	}


	PositiveRangeGesture::PositiveRangeGesture(InputManager* im)
		: InputGesture(im)
	{

	}

	PositiveRangeGesture::~PositiveRangeGesture()
	{

	}

	void PositiveRangeGesture::_updateValue(float val, IOType t)
	{
		if (val > 0)
		{
			Gesture::_updateValue(val, t);
		}
	}


	NegativeRangeGesture::NegativeRangeGesture(InputManager* im)
		: InputGesture(im)
	{

	}

	NegativeRangeGesture::~NegativeRangeGesture()
	{

	}

	void NegativeRangeGesture::_updateValue(float val, IOType t)
	{
		if (val < 0)
		{
			Gesture::_updateValue(val, t);
		}
	}

}