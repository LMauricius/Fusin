#include "Components/FusinDPadComponent.h"
#include "IOCodes/FusinAnyDPad.h"
#include "Utilities/FusinInputSignalManipulation.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{
	DPadComponent::DPadComponent(DeviceType signalType):
		DeviceComponent(
			{ {IO_DIRECTION, &mDPadDirections}, {IO_ANGLE, &mDPadAngles} },
			{}),
		angle(ANY_DPAD_ANGLE.withDeviceType(signalType)),
		up(ANY_DPAD_UP.withDeviceType(signalType)),
		down(ANY_DPAD_DOWN.withDeviceType(signalType)),
		left(ANY_DPAD_LEFT.withDeviceType(signalType)),
		right(ANY_DPAD_RIGHT.withDeviceType(signalType)),
		mDPadDirections{ &up, &down, &left, &right },
		mDPadAngles{ &angle }
	{
	}

	DPadComponent::~DPadComponent()
	{
	}

	DeviceType DPadComponent::deviceType() const
	{
		return DT_COMPONENT_DPAD;
	}

	IOFlags DPadComponent::flags() const
	{
		return IOF_DPAD | IOF_DIRECTION | IOF_ANGLE;
	}

	void DPadComponent::_update(size_t msElapsed)
	{
		/*
		Read from covered components
		*/

		// If it is, we need to calculate directions based on the angle.
		// If not, the angle has already been calculated based on directions
		bool isAngleDominant;

		// we will need this. Made into lambda cuz it seemed simpler
		auto calcAngleFromDir = [](DPadComponent* comp)
		{
			double x = comp->right.value() - comp->left.value();
			double y = comp->down.value() - comp->up.value();
			if (x == 0 && y == 0) {
				return 0.0;
			}
			else {
				double a = std::atan2(-y, x) * RADTODEG_FACTOR;
				return (a == 0 ? 360.0 : a);
			}
		};

		// If state has changed, change angles of all covered devices
		if (angle.changed() || up.changed() || down.changed() || left.changed() || right.changed())
		{
			// if angle has changed we will calculate directions later
			if (angle.changed())
			{
				isAngleDominant = true;
			}
			else// if not, we calculate angle now and use that to change covered devices
			{
				isAngleDominant = false;
				angle.setValue(calcAngleFromDir(this));
			}

			// change covered
			for (auto dev : mCoveredComponents)
				static_cast<DPadComponent*>(dev)->angle.setValue(angle.value());
		}
		else// Set the angle to the max angle of covered devices
		{
			float maxA = 0.0f;

			for (auto dev : mCoveredComponents)
			{
				auto covDPad = static_cast<DPadComponent*>(dev);
				if (covDPad->angle.changed() || 
					covDPad->up.changed() || covDPad->down.changed() || 
					covDPad->left.changed() || covDPad->right.changed())
				{
					float a;

					// Angle has dominance over directions; Calc using directions only if they have changed and the angle has not;
					if (covDPad->angle.changed())
					{
						a = covDPad->angle.value();
					}
					else
					{
						a = calcAngleFromDir(covDPad);
					}

					if (a > maxA)
					{
						maxA = a;
						angle.setValue(maxA);
						isAngleDominant = true;
					}
				}
			}
		}

		// If the angle has changed and directions haven't, update the directions' values
		if (isAngleDominant)
		{
			double x = std::sin(angle.value() * DEGTORAD_FACTOR);
			double y = -std::cos(angle.value() * DEGTORAD_FACTOR);
			left.setValue(x < 0.0);
			right.setValue(x > 0.0);
			up.setValue(y < 0.0);
			down.setValue(y > 0.0);
		}

		/*
		Now that all values have been properly set, update the signals
		*/
		angle.update();
		left.update();
		right.update();
		up.update();
		down.update();
	}

	String DPadComponent::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		ss << "DPad(" << angle.value();
		if (up.value()) ss << "^";
		if (down.value()) ss << "v";
		if (left.value()) ss << "<";
		if (right.value()) ss << ">";
		ss << ") ";

		return ss.str();
	}
}