#ifndef _FUSIN_MOUSE_DEVICE_H
#define _FUSIN_MOUSE_DEVICE_H

#include "FusinDevice.h"
#include "FusinCursorComponent.h"
#include "FusinButtonPadComponent.h"
#include "FusinWheelComponent.h"
#include <vector>
#include <list>
#include <map>

namespace Fusin
{
	class MouseDevice : public Device
	{
	public:
		MouseDevice(String name = FUSIN_STR("Mouse"));
		~MouseDevice();

		String getStateString();

		CursorComponent movement;
		ButtonPadComponent buttons;
		WheelComponent wheel;

		IOSignal buttonLeft, buttonRight, buttonMiddle, button4, button5;

	protected:
	};

}

#endif