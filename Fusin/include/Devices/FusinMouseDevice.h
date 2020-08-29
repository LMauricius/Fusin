#ifndef _FUSIN_MOUSE_DEVICE_H
#define _FUSIN_MOUSE_DEVICE_H

#include "FusinDevice.h"
#include "Components/FusinCursorComponent.h"
#include "Components/FusinButtonPadComponent.h"
#include "Components/FusinWheelComponent.h"
#include <vector>
#include <list>
#include <map>

namespace Fusin
{
	class MouseDevice : public Device
	{
	public:
		MouseDevice(String name = FUSIN_STR("Mouse"), bool hasBattery);
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