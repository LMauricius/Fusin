#include "Commands/FusinOutputCommand.h"
#include "Components/FusinIOSignal.h"
#include "Devices/FusinDevice.h"
#include <algorithm>

namespace Fusin
{

	OutputCommand::OutputCommand(DeviceEnumerator* devEnum) :
		IOCommand(devEnum)
	{
		mUpdateAfterDeviceUpdate = false;// The update should occur before the Devices' update
	}

	OutputCommand::~OutputCommand()
	{
	}

	void OutputCommand::setValue(float value)
	{
		mValue = value;
	}

	void OutputCommand::syncValueSignal(IOSignal* sig, Index devInd)
	{
		IOCode ioCode = sig->ioCode();

		if (std::abs(mValue) <= getDeadZone(ioCode.deviceType, ioCode.ioType))
			sig->setValue(0.0);
		else
		{
			float maxval = getMaxValue(ioCode.deviceType, ioCode.ioType);
			sig->setValue(std::max(-maxval, std::min(maxval, mValue)) * getFactor(ioCode.deviceType, ioCode.ioType));
		}
	}

}