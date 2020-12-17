#include "Devices/FusinMouseDevice.h"
#include "Commands/FusinCommand.h"
#include "IOCodes/FusinMouse.h"
#include <algorithm>

namespace Fusin
{
	MouseDevice::MouseDevice(String name, size_t buttonNum, bool hasHorizontalWheel, bool hasBattery):
		Device(name, DT_MOUSE, { &movement, &buttons, &wheel }, hasBattery),
		movement(DT_MOUSE),
		buttons(DT_MOUSE, (buttonNum>5)? buttonNum-5 : 0),
		wheel(DT_MOUSE, hasHorizontalWheel),
		buttonLeft(MOUSE_BUTTON_LEFT), 
		buttonRight(MOUSE_BUTTON_RIGHT),
		buttonMiddle(MOUSE_BUTTON_MIDDLE),
		button4(MOUSE_BUTTON_4),
		button5(MOUSE_BUTTON_5)
	{
		std::vector<IOSignal*> usableButtons;
		if (buttonNum>0) usableButtons.push_back(&buttonLeft);
		if (buttonNum>1) usableButtons.push_back(&buttonRight);
		if (buttonNum>2) usableButtons.push_back(&buttonMiddle);
		if (buttonNum>3) usableButtons.push_back(&button4);
		if (buttonNum>4) usableButtons.push_back(&button5);
		buttons._registerExternalButtons(usableButtons);
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