#include "FusinRGBComponent.h"
#include "FusinInputSignalManipulation.h"
#include "FusinAnyRGB.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{
	RGBComponent::RGBComponent(DeviceType signalType) :
		DeviceComponent(
			{ {IO_RGB, &mColorSignals} },
			{}),
		red(ANY_RGB_RED.withDeviceType(signalType)),
		green(ANY_RGB_GREEN.withDeviceType(signalType)),
		blue(ANY_RGB_BLUE.withDeviceType(signalType)),
		mColorSignals{ &red, &green, &blue }
	{
	}

	RGBComponent::~RGBComponent()
	{
	}

	DeviceType RGBComponent::deviceType() const
	{
		return DT_COMPONENT_RGB;
	}

	IOFlags RGBComponent::flags() const
	{
		return IOF_RGB;
	}

	void RGBComponent::setColor(const ColorRGB& c)
	{
		red.setValue(c.r);
		green.setValue(c.g);
		blue.setValue(c.b);
	}

	ColorRGB RGBComponent::getColor()
	{
		return ColorRGB(red.value(), green.value(), blue.value());
	}

	void RGBComponent::_update(size_t msElapsed)
	{
		// cover devices
		coverInputSignalVector(mCoveredComponents, &RGBComponent::mColorSignals, mColorSignals);

		// update
		updateInputSignalVector(mColorSignals);
	}


	String RGBComponent::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		ss << "RGB(" << red.value() << "," << green.value() << blue.value() << ")";

		return ss.str();
	}
}