#ifndef _FUSIN_RANGE_ACTION_H
#define _FUSIN_RANGE_ACTION_H

#include "FusinInputCommand.h"

namespace Fusin
{

	/*
	The more abstract Command used to detect values only if they are in a certain range

	The default range is [MIN_FLOAT, MAX_FLOAT], i.e. all float values
	*/
	class _FUSIN_EXPORT RangeCommand : public InputCommand
	{
	public:
		RangeCommand(DeviceEnumerator* devEnum = nullptr, float min = MIN_FLOAT, float max = MAX_FLOAT);
		~RangeCommand();

		void setValue(float value, IOCode ioCode = IOCode::ANYCODE, Index deviceIndex = 0);
		void updateValue(float value, IOCode ioCode = IOCode::ANYCODE, Index deviceIndex = 0);

		void setRange(float min, float max);
		inline float getRangeMin() const { return mMin; }
		inline float getRangeMax() const { return mMax; }

	protected:
		float mMin, mMax;
	};
}

#endif