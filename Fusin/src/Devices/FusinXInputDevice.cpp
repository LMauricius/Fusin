#include "FusinXInputDevice.h"
#include "FusinXInput.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{

	XInputDevice::XInputDevice(String name):
		Device(name, DT_XINPUT, { &buttons, &axes, &dPad, &vibration }),
		buttons(DT_XINPUT, 0),
		axes(DT_XINPUT, 0),
		dPad(DT_XINPUT),
		vibration(DT_XINPUT),
		buttonA(),
		buttonB(),
		buttonX(),
		buttonY(),
		buttonLB(),
		buttonRB(),
		buttonView(),
		buttonMenu(),
		buttonLeftStick(),
		buttonRightStick(),
		buttonGuide(),

	axisLeftX(), 
		axisLeftY(), 
		axisRightX(), 
		axisRightY(),
	axisPositiveLeftX(), 
		axisNegativeLeftX(), 
		axisPositiveLeftY(), 
		axisNegativeLeftY(),
	axisPositiveRightX(), 
		axisNegativeRightX(), 
		axisPositiveRightY(), 
		axisNegativeRightY(),
	axisLT(), 
		axisRT()
	{
	}

	XInputDevice::~XInputDevice()
	{
	}

	String XInputDevice::getStateString()
	{
		StringStream ss;

		ss << axes.getStateString() << " " << dPad.getStateString() << " ";

		if (vibration.leftForce.value() || vibration.rightForce.value())
			ss << "Vibration[" << vibration.getStateString() << "] ";

		ss << buttons.getStateString();

		return ss.str();
	}
}