#ifndef _FUSIN_RANGE_ACTION_H
#define _FUSIN_RANGE_ACTION_H

#include "FusinInputGesture.h"

namespace Fusin
{
	class RangeGesture : public InputGesture
	{
	public:
		RangeGesture(InputManager* im = nullptr);
		~RangeGesture();

		void _updateValue(float value, IOType t = IT_ANY);

		void setRange(float min, float max);
		float getRangeMin() const;
		float getRangeMax() const;

	protected:
		float mMin, mMax;
	};

	class PositiveRangeGesture : public InputGesture
	{
	public:
		PositiveRangeGesture(InputManager* im = nullptr);
		~PositiveRangeGesture();

		void _updateValue(float value, IOType t = IT_ANY);
	};

	class NegativeRangeGesture : public InputGesture
	{
	public:
		NegativeRangeGesture(InputManager* im = nullptr);
		~NegativeRangeGesture();

		void _updateValue(float value, IOType t = IT_ANY);
	};
}

#endif