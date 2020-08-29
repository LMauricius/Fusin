#include "Components/FusinBatteryComponent.h"
#include "IOCodes/FusinAnyBattery.h"
#include "Utilities/FusinInputSignalManipulation.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{
	BatteryComponent::BatteryComponent(DeviceType signalType) :
		DeviceComponent(
			{ {IO_BATTERY, &mBatterySignals}},
			{}),
		energy(ANY_BATTERY_ENERGY.withDeviceType(signalType)),
		charging(ANY_BATTERY_CHARGING.withDeviceType(signalType))
	{
		energy.setValue(-1);
		energy.update();
	}

	BatteryComponent::~BatteryComponent()
	{
	}

	DeviceType BatteryComponent::deviceType() const
	{
		return DT_COMPONENT_BATTERY;
	}

	IOFlags BatteryComponent::flags() const
	{
		return IOF_BATTERY;
	}

	void BatteryComponent::_update(size_t msElapsed)
	{
		// read from covered devices
		coverInputSignalVector(mCoveredComponents, &BatteryComponent::mBatterySignals, mBatterySignals);

		// update
		updateInputSignalVector(mBatterySignals);
	}

	String BatteryComponent::getStateString()
	{
		StringStream ss;

		ss << "Bat" << (int)energy.value();

		if (charging.value()) ss << "+";
		else ss << '%';

		return ss.str();
	}
}