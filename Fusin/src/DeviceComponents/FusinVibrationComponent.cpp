#include "FusinVibrationComponent.h"
#include "FusinInputSignalManipulation.h"
#include "FusinAnyVibration.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{
	VibrationComponent::VibrationComponent(DeviceType signalType):
		DeviceComponent(
			{ {IO_VIBRATION, &mVibrations} },
			{}),
		leftForce(ANY_VIBRATION_LEFT_FORCE.withDeviceType(signalType)),
		leftAmplitude(ANY_VIBRATION_LEFT_AMPLITUDE.withDeviceType(signalType)),
		leftDuration(ANY_VIBRATION_LEFT_DURATION.withDeviceType(signalType)),
		rightForce(ANY_VIBRATION_RIGHT_FORCE.withDeviceType(signalType)),
		rightAmplitude(ANY_VIBRATION_RIGHT_AMPLITUDE.withDeviceType(signalType)),
		rightDuration(ANY_VIBRATION_RIGHT_DURATION.withDeviceType(signalType)),
		mVibrations{ &leftForce, &leftAmplitude, &leftDuration, &rightForce, &rightAmplitude, &rightDuration }
	{
	}

	VibrationComponent::~VibrationComponent()
	{
	}

	DeviceType VibrationComponent::deviceType() const
	{
		return DT_COMPONENT_VIBRATION;
	}

	IOFlags VibrationComponent::flags() const
	{
		return IOF_VIBRATION;
	}

	void VibrationComponent::setLeftVibration(float force, size_t msDuration)
	{
		leftForce.setValue(force);
		leftAmplitude.setValue(1.0f);
		leftDuration.setValue(msDuration);
	}

	void VibrationComponent::setRightVibration(float force, size_t msDuration)
	{
		rightForce.setValue(force);
		leftAmplitude.setValue(1.0f);
		rightDuration.setValue(msDuration);
	}

	void VibrationComponent::setVibration(float force, size_t msDuration)
	{
		setLeftVibration(force, msDuration);
		setRightVibration(force, msDuration);
	}

	void VibrationComponent::setLeftHDVibration(float force, float amplitude, size_t msDuration)
	{
		leftForce.setValue(force);
		leftAmplitude.setValue(amplitude);
		leftDuration.setValue(msDuration);
	}

	void VibrationComponent::setRightHDVibration(float force, float amplitude, size_t msDuration)
	{
		rightForce.setValue(force);
		rightAmplitude.setValue(amplitude);
		rightDuration.setValue(msDuration);
	}

	void VibrationComponent::setHDVibration(float force, float amplitude, size_t msDuration)
	{
		setLeftHDVibration(force, amplitude, msDuration);
		setRightHDVibration(force, amplitude, msDuration);
	}

	void VibrationComponent::_update(size_t msElapsed)
	{
		/*
		cover devices
		*/
		// left
		if (leftForce.changed() || leftAmplitude.changed() || leftDuration.changed())
		{
			for (auto comp : mCoveredComponents)
			{
				static_cast<VibrationComponent*>(comp)->setLeftHDVibration(
					leftForce.nextValue(), leftAmplitude.nextValue(), leftDuration.nextValue()
				);
			}
		}
		else if (mCoveredComponents.size())
		{
			float strength = 0.0f;

			for (auto comp : mCoveredComponents)
			{
				VibrationComponent* vComp = static_cast<VibrationComponent*>(comp);

				float s = std::sqrt(
					vComp->leftForce.nextValue()*vComp->leftForce.nextValue() +
					vComp->leftAmplitude.nextValue()*vComp->leftAmplitude.nextValue()
				);

				if (s > strength)
				{
					strength = s;
					leftForce.setValue(leftForce.nextValue());
					leftAmplitude.setValue(leftAmplitude.nextValue());
				}
			}
		}
		// right
		if (rightForce.changed() || rightAmplitude.changed() || rightDuration.changed())
		{
			for (auto comp : mCoveredComponents)
			{
				static_cast<VibrationComponent*>(comp)->setLeftHDVibration(
					rightForce.nextValue(), rightAmplitude.nextValue(), rightDuration.nextValue()
				);
			}
		}
		else if (mCoveredComponents.size())
		{
			float strength = 0.0f;

			for (auto comp : mCoveredComponents)
			{
				VibrationComponent* vComp = static_cast<VibrationComponent*>(comp);

				float s = std::sqrt(
					vComp->rightForce.nextValue() * vComp->rightForce.nextValue() +
					vComp->rightAmplitude.nextValue() * vComp->rightAmplitude.nextValue()
				);

				if (s > strength)
				{
					strength = s;
					rightForce.setValue(rightForce.nextValue());
					rightAmplitude.setValue(rightAmplitude.nextValue());
				}
			}
		}

		/*
		calc vibration (timers)
		*/
		if (leftDuration.changed())
			mLeftDuration = leftDuration.nextValue();
		else
		{
			if (mLeftDuration > 0)
			{
				mLeftDuration -= msElapsed;
				if (mLeftDuration <= 0)
				{
					mLeftDuration = 0;
					leftForce.setValue(0.0f);
					leftAmplitude.setValue(0.0f);
				}
			}
			leftDuration.setValue(mLeftDuration);
		}
		if (rightDuration.changed())
			mRightDuration = rightDuration.nextValue();
		else
		{
			if (mRightDuration > 0)
			{
				mRightDuration -= msElapsed;
				if (mLeftDuration <= 0)
				{
					mRightDuration = 0;
					rightForce.setValue(0.0f);
					rightAmplitude.setValue(0.0f);
				}
			}
			rightDuration.setValue(mRightDuration);
		}

		/*
		update
		*/
		updateInputSignalVector(mVibrations);
	}


	String VibrationComponent::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		ss << "Left(F" << leftForce.value() << ",A" << leftAmplitude.value();
		if (leftDuration.value())
			ss << " for " << leftDuration.value() / 1000.0f << "s";
		ss << ")";

		ss << "Right(F" << rightForce.value() << ",A" << rightAmplitude.value();
		if (rightDuration.value())
			ss << " for " << rightDuration.value() / 1000.0f << "s";
		ss << ")";

		return ss.str();
	}
}