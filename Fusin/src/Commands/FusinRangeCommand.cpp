#include "Commands/FusinRangeCommand.h"

#include <algorithm>

namespace Fusin
{

	RangeCommand::RangeCommand(DeviceEnumerator* devEnum, float min, float max)
		: InputCommand(devEnum)
		, mMin(MIN_FLOAT)
		, mMax(MAX_FLOAT)
	{

	}

	RangeCommand::~RangeCommand()
	{

	}

	void RangeCommand::setValue(float value, IOCode ioCode, Index deviceIndex)
	{
		InputCommand::setValue(std::min(std::max(value, mMin), mMax), ioCode, deviceIndex);
	}

	void RangeCommand::updateValue(float value, IOCode ioCode, Index deviceIndex)
	{
		InputCommand::updateValue(std::min(std::max(value, mMin), mMax), ioCode, deviceIndex);
	}

	void RangeCommand::setRange(float min, float max)
	{
		mMin = min;
		mMax = max;
	}

}