#ifndef _FUSIN_VIBRATION_COMPONENT_H
#define _FUSIN_VIBRATION_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	/*
	Component for devices with haptic feedback.
	*/
	class VibrationComponent : public DeviceComponent
	{
	public:
		VibrationComponent(DeviceType signalType);
		~VibrationComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		String getStateString();

		IOSignal leftForce;
		IOSignal leftAmplitude;
		IOSignal leftDuration;// How many milliseconds should the device vibrate. Set to 0 for indefinite vibration
		IOSignal rightForce;
		IOSignal rightAmplitude;
		IOSignal rightDuration;// How many milliseconds should the device vibrate. Set to 0 for indefinite vibration

		void setLeftVibration(float force = 0, size_t msDuration = 0);
		void setRightVibration(float force = 0, size_t msDuration = 0);
		void setVibration(float force = 0, size_t msDuration = 0);
		void setLeftHDVibration(float force = 0, float amplitude = 1, size_t msDuration = 0);
		void setRightHDVibration(float force = 0, float amplitude = 1, size_t msDuration = 0);
		void setHDVibration(float force = 0, float amplitude = 1, size_t msDuration = 0);

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mVibrations;
		size_t mLeftDuration, mRightDuration;// translated to integral type to increase precision
	};

}

#endif