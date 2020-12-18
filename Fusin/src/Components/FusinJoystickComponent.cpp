#include "Components/FusinJoystickComponent.h"
#include "Utilities/FusinInputSignalManipulation.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{
	JoystickComponent::JoystickComponent(DeviceType signalType, size_t axisNum):
		DeviceComponent(
			{ {IO_AXIS, &mAxes} },
			{}),
		mSignalDeviceType(signalType),
		mReservedJoystickAxisCount(0)
	{
		_setAxisCount(axisNum);
	}

	JoystickComponent::~JoystickComponent()
	{
		for (int i = mReservedJoystickAxisCount; i < mAxes.size(); i++)
		{
			delete mAxes[i];
			delete mPositiveAxes[i];
			delete mNegativeAxes[i];
		}
	}

	DeviceType JoystickComponent::deviceType() const
	{
		return DT_COMPONENT_JOYSTICK;
	}

	IOFlags JoystickComponent::flags() const
	{
		return IOF_JOYSTICK | IOF_AXIS;
	}

	void JoystickComponent::_update(size_t msElapsed)
	{
		// read from covered devices
		coverInputSignalVectorDynamic(mCoveredComponents, &JoystickComponent::mAxes, mAxes, mNegativeAxes, mPositiveAxes,
			mSignalDeviceType, IO_AXIS);

		// update
		updateInputSignalVector(mAxes);
	}

	IOSignal& JoystickComponent::operator[](Index ind)
	{
		if (ind >= mAxes.size()) throw std::out_of_range((std::stringstream() <<
			"Axis index out of range. Trying to acces axis " << ind <<
			" but this device only has " << mAxes.size() << " axes.").str());
		return *mAxes[ind];
	}

	// Input

	String JoystickComponent::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		for (auto ax : mAxes)
		{
			switch (ax->ioCode().index)
			{
			case 0:
				ss << "X";
				break;
			case 1:
				ss << "Y";
				break;
			case 2:
				ss << "Z";
				break;
			case 3:
				ss << "R-Z";
				break;
			default:
				ss << "axis" << ax->ioCode().index;
				break;
			}
			ss << "(" << ax->value() << ") ";
		}

		return ss.str();
	}

	void JoystickComponent::_setAxisCount(size_t axisNum)
	{
		if (axisNum > mAxes.size())
		{
			mAxes.reserve(axisNum);
			mPositiveAxes.reserve(axisNum);
			mNegativeAxes.reserve(axisNum);
			for (int i = mAxes.size(); i < axisNum; i++) {
				mPositiveAxes.push_back(new IOSignal(IOCode(mSignalDeviceType, IO_AXIS, i).positiveVersion()));
				mNegativeAxes.push_back(new IOSignal(IOCode(mSignalDeviceType, IO_AXIS, i).negativeVersion()));
				mAxes.push_back(new IOSignal(IOCode(mSignalDeviceType, IO_AXIS, i),
					mPositiveAxes.back(), mNegativeAxes.back()));
			}
		}
		else if (axisNum > mReservedJoystickAxisCount)
		{
			for (int i = axisNum; i < mAxes.size(); i++) {
				delete mAxes[i];
				delete mPositiveAxes[i];
				delete mNegativeAxes[i];
			}
			mAxes.erase(mAxes.begin() + axisNum, mAxes.end());
			mPositiveAxes.erase(mPositiveAxes.begin() + axisNum, mPositiveAxes.end());
			mNegativeAxes.erase(mNegativeAxes.begin() + axisNum, mNegativeAxes.end());
		}
		else
		{
			throw std::length_error("Trying to set the axis count lower than the reserved axis num, which would delete non-owned IOSignals.");
		}
	}

	void JoystickComponent::_registerExternalAxes(const std::vector<IOSignal*>& axes)
	{
		mAxes.insert(mAxes.begin() + mReservedJoystickAxisCount, axes.begin(), axes.end());
		mNegativeAxes.insert(mNegativeAxes.begin() + mReservedJoystickAxisCount, axes.size(), nullptr);
		mPositiveAxes.insert(mPositiveAxes.begin() + mReservedJoystickAxisCount, axes.size(), nullptr);
		for (int i = 0; i < axes.size(); i++)
		{
			mNegativeAxes[mReservedJoystickAxisCount + i] = mAxes[mReservedJoystickAxisCount + i]->negativeSignal();
			mPositiveAxes[mReservedJoystickAxisCount + i] = mAxes[mReservedJoystickAxisCount + i]->positiveSignal();
		}
		mReservedJoystickAxisCount += axes.size();
	}
}