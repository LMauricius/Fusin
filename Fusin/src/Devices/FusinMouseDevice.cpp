#include "Devices/FusinMouseDevice.h"
#include "Commands/FusinCommand.h"
#include "IOCodes/FusinMouse.h"
#include <algorithm>

namespace Fusin
{
	MouseDevice::MouseDevice(String name, bool hasBattery):
		Device(name, DT_MOUSE, hasBattery, { &movement, &buttons, &wheel }),
		movement(DT_MOUSE),
		buttons(DT_MOUSE, 0),
		wheel(DT_MOUSE),
		buttonLeft(MOUSE_BUTTON_LEFT), 
		buttonRight(MOUSE_BUTTON_RIGHT),
		buttonMiddle(MOUSE_BUTTON_MIDDLE),
		button4(MOUSE_BUTTON_4),
		button5(MOUSE_BUTTON_5)
	{
		buttons._registerExternalButtons({ &buttonLeft, &buttonRight, &buttonMiddle, &button4, &button5 });
	}

	MouseDevice::~MouseDevice()
	{
	}

	String MouseDevice::getStateString()
	{
		StringStream ss;

		ss <<  movement.getStateString() << " " << wheel.getStateString() << " ";

		ss << "buttons: ";

		bool somePressed = false;
		if (buttonLeft.value())
		{
			ss << "L";
			somePressed = true;
		}
		if (buttonRight.value())
		{
			ss << "R";
			somePressed = true;
		}
		if (buttonMiddle.value())
		{
			ss << "M";
			somePressed = true;
		}
		if (button4.value())
		{
			ss << "4";
			somePressed = true;
		}
		if (button5.value())
		{
			ss << "5";
			somePressed = true;
		}
		if (!somePressed)
			ss << "none";

		return ss.str();
	}

}