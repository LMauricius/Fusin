#pragma once
#ifndef _FUSIN_BATTERY_COMPONENT_H
#define _FUSIN_BATTERY_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	using BatteryLevel = float;
	const BatteryLevel BAT_UNKNOWN = -1;
	const BatteryLevel BAT_EMPTY = 0.0f;
	const BatteryLevel BAT_CRITICAL = 5.0f;
	const BatteryLevel BAT_LOW = 25.0f;
	const BatteryLevel BAT_MEDIUM = 50.0f;
	const BatteryLevel BAT_FULL = 100.0f;

	class BatteryComponent : public DeviceComponent
	{
	public:
		BatteryComponent(DeviceType signalType);
		~BatteryComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		String getStateString();

		IOSignal energy, charging;

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mBatterySignals;
	};

}

#endif