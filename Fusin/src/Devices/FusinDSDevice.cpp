#include "Devices/FusinDSDevice.h"
#include "IOCodes/FusinDS.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{

	DSDevice::DSDevice(String name, bool hasBattery) :
		Device(name, DT_DUALSHOCK, hasBattery),
		buttons(DT_DUALSHOCK, 0),
		axes(DT_DUALSHOCK, 0),
		dPad(DT_DUALSHOCK),
		motion(DT_DUALSHOCK),
		touchPad(DT_DUALSHOCK, 2),
		vibration(DT_DUALSHOCK),
		rgb(DT_DUALSHOCK),
		leds(DT_DUALSHOCK, 4),

		buttonCross(DS_CROSS),
		buttonCircle(DS_CIRCLE),
		buttonSquare(DS_SQUARE),
		buttonTriangle(DS_TRIANGLE),
		buttonL1(DS_L1),
		buttonR1(DS_R1),
		buttonTouchPad(DS_TOUCHPAD_BUTTON),
		buttonOptions(DS_OPTIONS),
		buttonL3(DS_L3),
		buttonR3(DS_R3),
		buttonPS(DS_PS),
		buttonCreate(DS_CREATE),

		axisLeftStickX(DS_LEFT_X_AXIS, &axisLeftStickRight, &axisLeftStickLeft),
		axisLeftStickY(DS_LEFT_Y_AXIS, &axisLeftStickDown, &axisLeftStickUp),
		axisRightStickX(DS_RIGHT_X_AXIS, &axisRightStickRight, &axisRightStickLeft),
		axisRightStickY(DS_RIGHT_Y_AXIS, &axisRightStickDown, &axisRightStickUp),

		axisLeftStickRight(DS_LEFT_STICK_RIGHT),
		axisLeftStickLeft(DS_LEFT_STICK_LEFT),
		axisLeftStickDown(DS_LEFT_STICK_DOWN),
		axisLeftStickUp(DS_LEFT_STICK_UP),
		axisRightStickRight(DS_RIGHT_STICK_RIGHT),
		axisRightStickLeft(DS_RIGHT_STICK_LEFT),
		axisRightStickDown(DS_RIGHT_STICK_DOWN),
		axisRightStickUp(DS_RIGHT_STICK_UP),

		axisL2(DS_L2),
		axisR2(DS_R2)
	{
		registerComponents({ &buttons, &axes, &dPad, &vibration });

		buttons._registerExternalButtons({
				&buttonCross, &buttonCircle, &buttonSquare, &buttonTriangle,
				&buttonL1, &buttonR1,
				&buttonTouchPad, &buttonOptions, &buttonL3, &buttonR3, &buttonPS, &buttonCreate
			});
		axes._registerExternalAxes({
				&axisLeftStickX, &axisLeftStickY, &axisRightStickX, &axisRightStickY,
				&axisL2, &axisR2
			});
	}

	DSDevice::~DSDevice()
	{
	}

	String DSDevice::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		// axes
		ss << "L(" << axisLeftStickX.value() << "," << axisLeftStickY.value() <<
			") R(" << axisRightStickX.value() << "," << axisLeftStickY.value() <<
			") L2(" << axisL2.value() <<
			") R2(" << axisR2.value() << ") ";

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
		if (buttonCross.value())
			ss << "X ";
		if (buttonSquare.value())
#if FUSIN_USE_WCHAR==1
			ss << "\u2B1C ";// large white square
#else
			ss << "\xFE ";// black square
#endif// I really wanted to see squares and triangles in console :D
		if (buttonCircle.value())
#if FUSIN_USE_WCHAR==1
			ss << "\u25EF ";// white circle
#else
			ss << "O ";
#endif
		if (buttonTriangle.value())
#if FUSIN_USE_WCHAR==1
			ss << "\u25B3 ";// white triangle up
#else
			ss << "A ";// similar to triangle
#endif
		if (buttonL1.value())
			ss << "L1 ";
		if (buttonR1.value())
			ss << "R1 ";
		if (buttonL3.value())
			ss << "L3 ";
		if (buttonR3.value())
			ss << "R3 ";
		if (buttonCreate.value())
			ss << "SHR ";
		if (buttonOptions.value())
			ss << "OPT ";
		if (buttonPS.value())
			ss << "PS ";
		if (buttonTouchPad.value())
			ss << "TP Press ";

		return ss.str();
		ss << buttons.getStateString();

		return ss.str();
	}
}