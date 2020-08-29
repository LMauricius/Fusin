#include "Components/FusinMotionTrackerComponent.h"
#include "IOCodes/FusinAnyMotionTracker.h"
#include "Utilities/FusinInputSignalManipulation.h"

#include <iomanip>

namespace Fusin
{

	MotionTrackerComponent::MotionTrackerComponent(DeviceType signalType):
		DeviceComponent(
			{
				{IO_GYRO, &mGyro}, {IO_ACCELERATION, &mAcceleration}, {IO_ORIENTATION, &mOrientation},
				{IO_VELOCITY, &mVelocity}, {IO_WORLD_POSITION, &mWorldPosition} 
			},
			{}),

		gyroYaw(ANY_MOTION_TRACKER_GYRO_YAW.withDeviceType(signalType), &gyroPositiveYaw, &gyroNegativeYaw),
		gyroPitch(ANY_MOTION_TRACKER_GYRO_PITCH.withDeviceType(signalType), &gyroPositivePitch, &gyroNegativePitch),
		gyroRoll(ANY_MOTION_TRACKER_GYRO_ROLL.withDeviceType(signalType), &gyroPositiveRoll, &gyroNegativeRoll),
		gyroNegativeYaw(ANY_MOTION_TRACKER_GYRO_YAW_NEGATIVE.withDeviceType(signalType)),
		gyroNegativePitch(ANY_MOTION_TRACKER_GYRO_PITCH_NEGATIVE.withDeviceType(signalType)),
		gyroNegativeRoll(ANY_MOTION_TRACKER_GYRO_ROLL_NEGATIVE.withDeviceType(signalType)),
		gyroPositiveYaw(ANY_MOTION_TRACKER_GYRO_YAW_POSITIVE.withDeviceType(signalType)),
		gyroPositivePitch(ANY_MOTION_TRACKER_GYRO_PITCH_POSITIVE.withDeviceType(signalType)),
		gyroPositiveRoll(ANY_MOTION_TRACKER_GYRO_ROLL_POSITIVE.withDeviceType(signalType)),

		accelerationX(ANY_MOTION_TRACKER_ACCELERATION_X.withDeviceType(signalType), &accelerationPositiveX, &accelerationNegativeX),
		accelerationY(ANY_MOTION_TRACKER_ACCELERATION_Y.withDeviceType(signalType), &accelerationPositiveY, &accelerationNegativeY),
		accelerationZ(ANY_MOTION_TRACKER_ACCELERATION_Z.withDeviceType(signalType), &accelerationPositiveZ, &accelerationNegativeZ),
		accelerationNegativeX(ANY_MOTION_TRACKER_ACCELERATION_X_NEGATIVE.withDeviceType(signalType)),
		accelerationNegativeY(ANY_MOTION_TRACKER_ACCELERATION_Y_NEGATIVE.withDeviceType(signalType)),
		accelerationNegativeZ(ANY_MOTION_TRACKER_ACCELERATION_Z_NEGATIVE.withDeviceType(signalType)),
		accelerationPositiveX(ANY_MOTION_TRACKER_ACCELERATION_X_POSITIVE.withDeviceType(signalType)),
		accelerationPositiveY(ANY_MOTION_TRACKER_ACCELERATION_Y_POSITIVE.withDeviceType(signalType)),
		accelerationPositiveZ(ANY_MOTION_TRACKER_ACCELERATION_Z_POSITIVE.withDeviceType(signalType)),

		orientationYaw(ANY_MOTION_TRACKER_GYRO_YAW.withDeviceType(signalType), &orientationPositiveYaw, &orientationNegativeYaw),
		orientationPitch(ANY_MOTION_TRACKER_GYRO_PITCH.withDeviceType(signalType), &orientationPositivePitch, &orientationNegativePitch),
		orientationRoll(ANY_MOTION_TRACKER_GYRO_ROLL.withDeviceType(signalType), &orientationPositiveRoll, &orientationNegativeRoll),
		orientationNegativeYaw(ANY_MOTION_TRACKER_GYRO_YAW_NEGATIVE.withDeviceType(signalType)),
		orientationNegativePitch(ANY_MOTION_TRACKER_GYRO_PITCH_NEGATIVE.withDeviceType(signalType)),
		orientationNegativeRoll(ANY_MOTION_TRACKER_GYRO_ROLL_NEGATIVE.withDeviceType(signalType)),
		orientationPositiveYaw(ANY_MOTION_TRACKER_GYRO_YAW_POSITIVE.withDeviceType(signalType)),
		orientationPositivePitch(ANY_MOTION_TRACKER_GYRO_PITCH_POSITIVE.withDeviceType(signalType)),
		orientationPositiveRoll(ANY_MOTION_TRACKER_GYRO_ROLL_POSITIVE.withDeviceType(signalType)),

		velocityX(ANY_MOTION_TRACKER_VELOCITY_X.withDeviceType(signalType), &velocityPositiveX, &velocityNegativeX),
		velocityY(ANY_MOTION_TRACKER_VELOCITY_Y.withDeviceType(signalType), &velocityPositiveY, &velocityNegativeY),
		velocityZ(ANY_MOTION_TRACKER_VELOCITY_Z.withDeviceType(signalType), &velocityPositiveZ, &velocityNegativeZ),
		velocityNegativeX(ANY_MOTION_TRACKER_VELOCITY_X_NEGATIVE.withDeviceType(signalType)),
		velocityNegativeY(ANY_MOTION_TRACKER_VELOCITY_Y_NEGATIVE.withDeviceType(signalType)),
		velocityNegativeZ(ANY_MOTION_TRACKER_VELOCITY_Z_NEGATIVE.withDeviceType(signalType)),
		velocityPositiveX(ANY_MOTION_TRACKER_VELOCITY_X_POSITIVE.withDeviceType(signalType)),
		velocityPositiveY(ANY_MOTION_TRACKER_VELOCITY_Y_POSITIVE.withDeviceType(signalType)),
		velocityPositiveZ(ANY_MOTION_TRACKER_VELOCITY_Z_POSITIVE.withDeviceType(signalType)),

		worldPosX(ANY_MOTION_TRACKER_POSITION_X.withDeviceType(signalType), &worldPosPositiveX, &worldPosNegativeX),
		worldPosY(ANY_MOTION_TRACKER_POSITION_Y.withDeviceType(signalType), &worldPosPositiveY, &worldPosNegativeY),
		worldPosZ(ANY_MOTION_TRACKER_POSITION_Z.withDeviceType(signalType), &worldPosPositiveZ, &worldPosNegativeZ),
		worldPosNegativeX(ANY_MOTION_TRACKER_POSITION_X_NEGATIVE.withDeviceType(signalType)),
		worldPosNegativeY(ANY_MOTION_TRACKER_POSITION_Y_NEGATIVE.withDeviceType(signalType)),
		worldPosNegativeZ(ANY_MOTION_TRACKER_POSITION_Z_NEGATIVE.withDeviceType(signalType)),
		worldPosPositiveX(ANY_MOTION_TRACKER_POSITION_X_POSITIVE.withDeviceType(signalType)),
		worldPosPositiveY(ANY_MOTION_TRACKER_POSITION_Y_POSITIVE.withDeviceType(signalType)),
		worldPosPositiveZ(ANY_MOTION_TRACKER_POSITION_Z_POSITIVE.withDeviceType(signalType)),

		mGyro{ &gyroYaw, &gyroPitch, &gyroRoll },
		mAcceleration{ &accelerationX, &accelerationZ, &accelerationZ },
		mOrientation{ &orientationYaw, &orientationPitch, &orientationRoll },
		mVelocity{ &velocityX, &velocityZ, &velocityZ },
		mWorldPosition{ &worldPosX, &worldPosZ, &worldPosZ }
	{
	}

	MotionTrackerComponent::~MotionTrackerComponent()
	{
	}

	DeviceType MotionTrackerComponent::deviceType() const
	{
		return DT_COMPONENT_MOTION_TRACKER;
	}

	IOFlags MotionTrackerComponent::flags() const
	{
		return IOF_MOTION_TRACKER | IOF_GYRO | IOF_ACCELERATION | IOF_ORIENTATION | IOF_VELOCITY | IOF_WORLD_POSITION;
	}

	String MotionTrackerComponent::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);
		ss << "GYaw(" << gyroYaw.value() << ") ";
		ss << "GPitch(" << gyroPitch.value() << ") ";
		ss << "GRoll(" << gyroRoll.value() << ") ";
		ss << "AX(" << accelerationX.value() << ") ";
		ss << "AY(" << accelerationY.value() << ") ";
		ss << "AZ(" << accelerationZ.value() << ") ";
		return ss.str();
	}

	void MotionTrackerComponent::_setMode(MotionTrackerMode mode)
	{
		mTrackingMode = mode;
	}

	void MotionTrackerComponent::_update(size_t msElapsed)
	{
		/*
		Cover devices
		*/
		coverInputSignalVector(mCoveredComponents, &MotionTrackerComponent::mGyro, mGyro);
		coverInputSignalVector(mCoveredComponents, &MotionTrackerComponent::mAcceleration, mAcceleration);
		coverInputSignalVector(mCoveredComponents, &MotionTrackerComponent::mOrientation, mOrientation);
		coverInputSignalVector(mCoveredComponents, &MotionTrackerComponent::mVelocity, mVelocity);
		coverInputSignalVector(mCoveredComponents, &MotionTrackerComponent::mWorldPosition, mWorldPosition);

		/*
		Calculate new values depending on the mode
		*/


		/*
		Update signals
		*/
		updateInputSignalVector(mGyro);
		updateInputSignalVector(mAcceleration);
		updateInputSignalVector(mOrientation);
		updateInputSignalVector(mVelocity);
		updateInputSignalVector(mWorldPosition);
	}

}