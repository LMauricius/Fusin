#include "Commands/FusinInputCommand.h"
#include "Components/FusinIOSignal.h"
#include "Devices/FusinDevice.h"
#include <algorithm>

namespace Fusin
{

	InputCommand::InputCommand(DeviceEnumerator* devEnum):
		IOCommand(devEnum)
	{
	}

	InputCommand::~InputCommand()
	{
	}

	void InputCommand::setValue(float value, IOCode ioCode, Index deviceIndex)
	{
		if (abs(value) <= getDeadZone(ioCode.deviceType, ioCode.ioType))
			mValue = 0.0;
		else
		{
			float dz = getMaxValue(ioCode.deviceType, ioCode.ioType);
			mValue = std::max(-dz, std::min(dz, value)) * getFactor(ioCode.deviceType, ioCode.ioType);
		}
		mStrongestIOCode = ioCode;
		mStrongestDeviceIndex = deviceIndex;
	}

	void InputCommand::updateValue(float value, IOCode ioCode, Index deviceIndex)
	{
		if (abs(value) > abs(mValue) && abs(value) > getDeadZone(ioCode.deviceType, ioCode.ioType))
		{
			float dz = getMaxValue(ioCode.deviceType, ioCode.ioType);
			mValue = std::max(-dz, std::min(dz, value)) * getFactor(ioCode.deviceType, ioCode.ioType);
			mStrongestIOCode = ioCode;
			mStrongestDeviceIndex = deviceIndex;
		}
	}

	void InputCommand::syncValueSignal(IOSignal* sig, Index devInd)
	{
		updateValue(sig->value(), sig->ioCode(), devInd);
	}
	
	void InputCommand::preUpdate(DeviceEnumerator* de)
	{
		mValue = 0.0f;
		mStrongestIOCode = IOCode::NULLCODE;
		mStrongestDeviceIndex = -1;
	}

}