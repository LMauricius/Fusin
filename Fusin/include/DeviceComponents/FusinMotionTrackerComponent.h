#pragma once

#ifndef _FUSIN_MOTION_TRACKER_DEVICE_H
#define _FUSIN_MOTION_TRACKER_DEVICE_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class MotionTrackerComponentListener;

	/*
	Describes the way MotionTrackerComponent's axes will be calculated.
	*/
	enum MotionTrackerMode {
		MTM_FULL_MANUAL,// No axes will be changed automatically. Allows full control over the MotionTrackerComponent
		MTM_GYRO_ACCEL// Only gyro and acceleration axes can be manually controlled. Other axes will be calculated depending on these two.
	};

	class MotionTrackerComponent : public DeviceComponent
	{
	public:
		MotionTrackerComponent(DeviceType signalType);
		~MotionTrackerComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		/*
		Returns the string with all values
		*/
		String getStateString();

		/*
		Sets the way the motion axes will be calculated.
		*/
		void _setMode(MotionTrackerMode mode);

		IOSignal gyroYaw, gyroPitch, gyroRoll;
		IOSignal gyroNegativeYaw, gyroNegativePitch, gyroNegativeRoll;
		IOSignal gyroPositiveYaw, gyroPositivePitch, gyroPositiveRoll;

		IOSignal accelerationX, accelerationY, accelerationZ;
		IOSignal accelerationNegativeX, accelerationNegativeY, accelerationNegativeZ;
		IOSignal accelerationPositiveX, accelerationPositiveY, accelerationPositiveZ;

		IOSignal orientationYaw, orientationPitch, orientationRoll;
		IOSignal orientationNegativeYaw, orientationNegativePitch, orientationNegativeRoll;
		IOSignal orientationPositiveYaw, orientationPositivePitch, orientationPositiveRoll;

		IOSignal velocityX, velocityY, velocityZ;
		IOSignal velocityNegativeX, velocityNegativeY, velocityNegativeZ;
		IOSignal velocityPositiveX, velocityPositiveY, velocityPositiveZ;

		IOSignal worldPosX, worldPosY, worldPosZ;
		IOSignal worldPosNegativeX, worldPosNegativeY, worldPosNegativeZ;
		IOSignal worldPosPositiveX, worldPosPositiveY, worldPosPositiveZ;

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mGyro, mAcceleration, mOrientation, mVelocity, mWorldPosition;
		MotionTrackerMode mTrackingMode;
	};

}

#endif
