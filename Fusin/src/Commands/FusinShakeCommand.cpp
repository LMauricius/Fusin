#include "Commands/FusinShakeCommand.h"
#include <algorithm>

#define FOR_SUBACTIONS(EXP) for (auto& axis : mAxes) { axis.EXP; }

namespace Fusin
{

	ShakeCommand::ShakeCommand(DeviceEnumerator* devEnum) :
		Command(devEnum),
		mShakeThreshold(0.5f)
	{
	}

	ShakeCommand::~ShakeCommand()
	{

	}

	void ShakeCommand::setDeviceEnumerator(DeviceEnumerator* devEnum)
	{
		Command::setDeviceEnumerator(devEnum);
		FOR_SUBACTIONS(setDeviceEnumerator(devEnum));
	}

	void ShakeCommand::setShakeDetectionThreshold(float threshold)
	{
		mShakeThreshold = threshold;
	}

	void ShakeCommand::setDeviceIndex(Index ind, DeviceType t) {
		FOR_SUBACTIONS(setDeviceIndex(ind, t));
	}

	void ShakeCommand::setDeviceIndex(Index ind, IOFlags filter) {
		FOR_SUBACTIONS(setDeviceIndex(ind, filter));
	}

	void ShakeCommand::setEnabledInputTypes(IOFlags filter) {
		FOR_SUBACTIONS(setEnabledInputTypes(filter));
	}

	void ShakeCommand::setDeadZone(float dz, DeviceType deviceType, IOType ioType) {
		FOR_SUBACTIONS(setDeadZone(dz, deviceType, ioType));
	}

	void ShakeCommand::setDeadZone(float dz, IOFlags filter) {
		FOR_SUBACTIONS(setDeadZone(dz, filter));
	}

	void ShakeCommand::setMaxValue(float val, DeviceType deviceType, IOType ioType) {
		FOR_SUBACTIONS(setMaxValue(val, deviceType, ioType));
	}

	void ShakeCommand::setMaxValue(float val, IOFlags filter) {
		FOR_SUBACTIONS(setDeadZone(val, filter));
	}

	void ShakeCommand::setFactor(float f, DeviceType deviceType, IOType ioType)
	{
		FOR_SUBACTIONS(setFactor(f, deviceType, ioType));
	}

	void ShakeCommand::setFactor(float f, IOFlags filter)
	{
		FOR_SUBACTIONS(setDeadZone(f, filter));
	}

	InputCommand& ShakeCommand::getAxis(Index ind)
	{
		if (mAxes.size() <= ind)
		{
			mAxes.reserve(ind + 1);
			mPrevStretchValues.reserve(ind + 1);
			for (int i = mAxes.size(); i <= ind; i++)
			{
				mAxes.emplace_back(mDeviceEnumerator);
				mPrevStretchValues.emplace_back(0);
			}
		}
		return mAxes[ind];
	}

	void ShakeCommand::update()
	{
		/*
		(distance = length of the axis' values vector,
		vector = vector of axis' values,
		threshold = shake detection threshold)

		Value calculating algorithm:
			Each time the distance gets greater than the threshold,
			if the angle between the current vector and the vector that
			was recorded the last time it surpassed the threshold (prev vector) is
			greater or equal to 90 degrees, the value is calculated as negative of 
			the two vectors' dot product.
		*/
		
		mPrevValue = mValue;
		mValue = 0;

		// calc distances
		float d = 0, dot = 0;
		for (int i = 0; i < mAxes.size(); i++)
		{
			d += mAxes[i].value() * mAxes[i].value();
		}
		d = std::sqrt(d);

		// detect shaking
		if (d >= mShakeThreshold)
		{
			for (int i = 0; i < mAxes.size(); i++)
			{
				dot += mAxes[i].value() * mPrevStretchValues[i];
				mPrevStretchValues[i] = mAxes[i].value();
			}

			if (dot < 0) mValue = -dot;
		}

		// Detect passing the value threshold
		mPressed = (mValue >= mThreshold && mPrevValue < mThreshold);
		mReleased = (mValue < mThreshold&& mPrevValue >= mThreshold);
	}

}