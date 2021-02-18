#include "Devices/FusinNintendoDevice.h"
#include "IOCodes/FusinNintendo.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{

	NintendoDevice::NintendoDevice(String name, bool hasBattery) :
		Device(name, DT_NINTENDO, hasBattery),
		buttons(DT_NINTENDO, 0),
		axes(DT_NINTENDO, 0),
		dPad(DT_NINTENDO),
		motion(DT_NINTENDO),
		touchPad(DT_NINTENDO, 2),
		vibration(DT_NINTENDO),
		
		buttonB(NINTENDO_B),
		buttonA(NINTENDO_A),
		buttonY(NINTENDO_Y),
		buttonX(NINTENDO_X),
		buttonL(NINTENDO_L),
		buttonR(NINTENDO_R),
		buttonZL(NINTENDO_ZL),
		buttonZR(NINTENDO_ZR),
		buttonSL(NINTENDO_SL),
		buttonSR(NINTENDO_SR),
		buttonMinus(NINTENDO_MINUS),
		buttonPlus(NINTENDO_PLUS),
		buttonLeftStick(NINTENDO_LEFT_STICK_PRESS),
		buttonRightStick(NINTENDO_RIGHT_STICK_PRESS),
		buttonHome(NINTENDO_HOME),
		buttonCapture(NINTENDO_CAPTURE),

		axisLeftStickX(NINTENDO_LEFT_X_AXIS, &axisLeftStickRight, &axisLeftStickLeft),
		axisLeftStickY(NINTENDO_LEFT_Y_AXIS, &axisLeftStickDown, &axisLeftStickUp),
		axisRightStickX(NINTENDO_RIGHT_X_AXIS, &axisRightStickRight, &axisRightStickLeft),
		axisRightStickY(NINTENDO_RIGHT_Y_AXIS, &axisRightStickDown, &axisRightStickUp),

		axisLeftStickRight(NINTENDO_LEFT_STICK_RIGHT),
		axisLeftStickLeft(NINTENDO_LEFT_STICK_LEFT),
		axisLeftStickDown(NINTENDO_LEFT_STICK_DOWN),
		axisLeftStickUp(NINTENDO_LEFT_STICK_UP),
		axisRightStickRight(NINTENDO_RIGHT_STICK_RIGHT),
		axisRightStickLeft(NINTENDO_RIGHT_STICK_LEFT),
		axisRightStickDown(NINTENDO_RIGHT_STICK_DOWN),
		axisRightStickUp(NINTENDO_RIGHT_STICK_UP)
	{
		registerComponents({ &buttons, &axes, &dPad, &vibration });

		buttons._registerExternalButtons({
				&buttonB, &buttonA, &buttonY, &buttonX,
				&buttonL, &buttonR, &buttonZL, &buttonZR, &buttonSL, &buttonSR,
				&buttonMinus, &buttonPlus, &buttonLeftStick, &buttonRightStick, &buttonHome, &buttonCapture
			});
		axes._registerExternalAxes({
				&axisLeftStickX, &axisLeftStickY, &axisRightStickX, &axisRightStickY
			});
	}

	NintendoDevice::~NintendoDevice()
	{
	}

	String NintendoDevice::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		// axes
		ss << "L(" << axisLeftStickX.value() << "," << axisLeftStickY.value() <<
			") R(" << axisRightStickX.value() << "," << axisLeftStickY.value() << ") ";

		// default component strings
		ss << dPad.getStateString() << " " << motion.getStateString();

		// vibration
		if (vibration.leftForce.value() || vibration.rightForce.value())
			ss << "Vibration[" << vibration.getStateString() << "] ";
		ss << motion.getStateString();
		ss << " ";
		ss << touchPad.getStateString();

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
		if (buttonL.value())
			ss << "L ";
		if (buttonR.value())
			ss << "R ";
		if (buttonZL.value())
			ss << "ZL ";
		if (buttonZR.value())
			ss << "ZR ";
		if (buttonSL.value())
			ss << "SL ";
		if (buttonSR.value())
			ss << "SR ";
		if (buttonLeftStick.value())
			ss << "LStick ";
		if (buttonRightStick.value())
			ss << "RStick ";
		if (buttonMinus.value())
			ss << "- ";
		if (buttonPlus.value())
			ss << "+ ";
		if (buttonHome.value())
			ss << "Home ";
		if (buttonCapture.value())
			ss << "Capture ";

		return ss.str();
		ss << buttons.getStateString();

		return ss.str();
	}
}