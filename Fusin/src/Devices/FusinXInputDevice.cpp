#include "Devices/FusinXInputDevice.h"
#include "IOCodes/FusinXInput.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{

	XInputDevice::XInputDevice(String name, bool hasBattery):
		Device(name, DT_XINPUT, hasBattery),
		buttons(DT_XINPUT, 0),
		axes(DT_XINPUT, 0),
		dPad(DT_XINPUT),
		vibration(DT_XINPUT),
		
		buttonA(XINPUT_A),
		buttonB(XINPUT_B),
		buttonX(XINPUT_X),
		buttonY(XINPUT_Y),
		buttonLB(XINPUT_LB),
		buttonRB(XINPUT_RB),
		buttonView(XINPUT_VIEW),
		buttonMenu(XINPUT_MENU),
		buttonLeftStick(XINPUT_LEFT_STICK_PRESS),
		buttonRightStick(XINPUT_RIGHT_STICK_PRESS),
		buttonGuide(XINPUT_GUIDE),

		axisLeftStickX(XINPUT_LEFT_X_AXIS, &axisLeftStickRight, &axisLeftStickLeft),
		axisLeftStickY(XINPUT_LEFT_Y_AXIS, &axisLeftStickDown, &axisLeftStickUp),
		axisRightStickX(XINPUT_RIGHT_X_AXIS, &axisRightStickRight, &axisRightStickLeft),
		axisRightStickY(XINPUT_RIGHT_Y_AXIS, &axisRightStickDown, &axisRightStickUp),

		axisLeftStickRight(XINPUT_LEFT_STICK_RIGHT),
		axisLeftStickLeft(XINPUT_LEFT_STICK_LEFT),
		axisLeftStickDown(XINPUT_LEFT_STICK_DOWN),
		axisLeftStickUp(XINPUT_LEFT_STICK_UP),
		axisRightStickRight(XINPUT_RIGHT_STICK_RIGHT),
		axisRightStickLeft(XINPUT_RIGHT_STICK_LEFT),
		axisRightStickDown(XINPUT_RIGHT_STICK_DOWN),
		axisRightStickUp(XINPUT_RIGHT_STICK_UP),

		axisLT(XINPUT_LT),
		axisRT(XINPUT_RT)
	{
		registerComponents({ &buttons, &axes, &dPad, &vibration });

		buttons._registerExternalButtons({
				&buttonA, &buttonB, &buttonX, &buttonY, &buttonLB, &buttonRB,
				&buttonView, &buttonMenu, &buttonLeftStick, &buttonRightStick, &buttonGuide 
			});
		axes._registerExternalAxes({
				&axisLeftStickX, &axisLeftStickY, &axisRightStickX, &axisRightStickY,
				&axisLT, &axisRT
			});
	}									  
	XInputDevice::~XInputDevice()
	{
	}

	String XInputDevice::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		// axes
		ss << "L(" << axisLeftStickX.value() << "," << axisLeftStickY.value() <<
			") R(" << axisRightStickX.value() << "," << axisLeftStickY.value() <<
			") LT(" << axisLT.value() <<
			") RT(" << axisRT.value() << ") ";

		// default component strings
		ss << dPad.getStateString() << " ";

		// vibration
		if (vibration.leftForce.value() || vibration.rightForce.value())
			ss << "Vibration[" << vibration.getStateString() << "] ";


		// buttons
		ss << " Buttons: ";
		if (buttonA.value())
			ss << "A ";
		if (buttonB.value())
			ss << "B ";
		if (buttonX.value())
			ss << "X ";
		if (buttonY.value())
			ss << "Y ";
		if (buttonLB.value())
			ss << "LB ";
		if (buttonRB.value())
			ss << "RB ";
		if (buttonLeftStick.value())
			ss << "LStick ";
		if (buttonRightStick.value())
			ss << "RStick ";
		if (buttonView.value())
			ss << "View ";
		if (buttonMenu.value())
			ss << "Menu ";
		if (buttonGuide.value())
			ss << "Guide ";

		return ss.str();
	}
}