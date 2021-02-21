#include "IOSystems/RawInput/FusinRawInputMouseHandler.h"

#ifdef FUSIN_BUILD_RAW_INPUT

#include "Devices/FusinMouseDevice.h"
#include "IOCodes/FusinMouse.h"
#include "Utilities/FusinLog.h"
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{

	RawInputMouseHandler::RawInputMouseHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo): 
		RawInputDeviceHandler(riDeviceHandle, riDeviceInfo)
	{
	}

	RawInputMouseHandler::~RawInputMouseHandler()
	{

	}
	
	bool RawInputMouseHandler::initialize()
	{
		if (!RawInputDeviceHandler::initialize()) return false;

		mFusinDevice = new MouseDevice(
			mProductName,
			mpDeviceInfo->mouse.dwNumberOfButtons,
			mpDeviceInfo->mouse.fHasHorizontalWheel,
			false
		);
		
		Log::singleton() << "Mouse Device found:" << mProductName <<
			"\n    button number: " << mpDeviceInfo->mouse.dwNumberOfButtons <<
			"\n    has horizontal wheel: " << mpDeviceInfo->mouse.fHasHorizontalWheel << "\n";

		return true;
	}

	void RawInputMouseHandler::handleRawInput(PRAWINPUT pRawInput)
	{
		MouseDevice& md = *static_cast<MouseDevice*>(mFusinDevice);
		RAWMOUSE& mouse = pRawInput->data.mouse;

		// movement
		if (mouse.usFlags == MOUSE_MOVE_RELATIVE)
		{
			md.movement.simulateMovement((float)mouse.lLastX, (float)mouse.lLastY);
		}
		/*if (mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
		{
			
		}*/
		if (mouse.usFlags & MOUSE_VIRTUAL_DESKTOP)
		{
			md.movement.setPosition((float)mouse.lLastX, (float)mouse.lLastY);
		}

		// buttons
		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
		{
			md.buttonLeft.press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
		{
			md.buttonLeft.release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
		{
			md.buttonRight.press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
		{
			md.buttonRight.release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
		{
			md.buttonMiddle.press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
		{
			md.buttonMiddle.release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
		{
			md.button4.press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
		{
			md.button4.release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
		{
			md.button5.press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
		{
			md.button5.release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			float delta = ((float)(short)mouse.usButtonData) / WHEEL_DELTA;
			md.wheel.simulateRotation(0, delta);
		}
		if (mouse.usButtonFlags & RI_MOUSE_HWHEEL)
		{
			float delta = ((float)(short)mouse.usButtonData) / WHEEL_DELTA;
			md.wheel.simulateRotation(delta, 0);
		}
	}

}

#endif // FUSIN_BUILD_RAW_INPUT