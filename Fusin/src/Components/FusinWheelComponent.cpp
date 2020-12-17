#include "Components/FusinWheelComponent.h"
#include "IOCodes/FusinAnyWheel.h"
#include <algorithm>

namespace Fusin
{

	WheelComponent::WheelComponent(DeviceType signalType, bool hasHorizontalWheel) :
		DeviceComponent(
			{ {IO_DIRECTION, &mWheelRotation} },
			{}),
		scrollHorizontal(ANY_WHEEL_HORIZONTAL.withDeviceType(signalType), &scrollRight, &scrollLeft),
		scrollVertical(ANY_WHEEL_VERTICAL.withDeviceType(signalType), &scrollUp, &scrollDown),
		scrollLeft(ANY_WHEEL_LEFT.withDeviceType(signalType)),
		scrollRight(ANY_WHEEL_RIGHT.withDeviceType(signalType)),
		scrollUp(ANY_WHEEL_UP.withDeviceType(signalType)),
		scrollDown(ANY_WHEEL_DOWN.withDeviceType(signalType)),
		mWheelRotation{ &scrollHorizontal, &scrollVertical },
		mHasHorizontalWheel(hasHorizontalWheel)
	{
	}

	WheelComponent::~WheelComponent()
	{
	}

	DeviceType WheelComponent::deviceType() const
	{
		return DT_COMPONENT_WHEEL;
	}

	IOFlags WheelComponent::flags() const
	{
		return IOF_WHEEL | IOF_DIRECTION;
	}


	void WheelComponent::simulateRotation(float ver, float hor)
	{
		scrollHorizontal.setValue(scrollHorizontal.nextValue() + hor);
		scrollVertical.setValue(scrollVertical.nextValue() + ver);
	}

	String WheelComponent::getStateString()
	{
		StringStream ss;
		ss << "Rot(>" << scrollHorizontal.value() << ",^" << scrollVertical.value() << ")";
		return ss.str();
	}


	void WheelComponent::_update(size_t msElapsed)
	{
		DeviceComponent::_update(msElapsed);

		// _update signals
		scrollHorizontal.release();// The wheel's movement shouldn't have memory, i.e. it should stop if not specified otherwise
		scrollVertical.release();
	}

}