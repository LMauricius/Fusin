#include "RawInput/FusinRawInputMouseHandler.h"
#include "FusinMouseDevice.h"
#include "FusinMouse.h"
#include "FusinLog.h"
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{

	RawInputMouseHandler::RawInputMouseHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: RawInputDeviceHandler(riDeviceHandle, riDeviceInfo, new MouseDevice())
	{
		if (!mSuccess) return;

		mDevice->_setName(mProductName);
		((MouseDevice*)mDevice)->_setProperties(mpDeviceInfo->mouse.dwNumberOfButtons, mpDeviceInfo->mouse.fHasHorizontalWheel);

		Log::singleton() << "Mouse Device found:" << mProductName <<
			"\n    button number: " << mpDeviceInfo->mouse.dwNumberOfButtons <<
			"\n    has horizontal wheel: " << mpDeviceInfo->mouse.fHasHorizontalWheel << "\n";
	}

	RawInputMouseHandler::~RawInputMouseHandler()
	{

	}

	void RawInputMouseHandler::handleRawInput(PRAWINPUT pRawInput)
	{
		MouseDevice& md = *static_cast<MouseDevice*>(mDevice);
		RAWMOUSE& mouse = pRawInput->data.mouse;

		// movement
		if (mouse.usFlags == MOUSE_MOVE_RELATIVE)
		{
			md.simulateMouseMove(mouse.lLastX, mouse.lLastY);
		}
		/*if (mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
		{
			
		}*/
		/*if (mouse.usFlags & MOUSE_VIRTUAL_DESKTOP)
		{
			md.movement.desktopXAxis.setValue(md.movement.desktopXAxis.nextValue() + mouse.lLastX);
			md.movement.desktopYAxis.setValue(md.movement.desktopYAxis.nextValue() + mouse.lLastY);
		}*/

		// buttons
		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
		{
			md.getButton(MOUSE_BUTTON_LEFT.data)->press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
		{
			md.getButton(MOUSE_BUTTON_LEFT.data)->release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
		{
			md.getButton(MOUSE_BUTTON_RIGHT.data)->press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
		{
			md.getButton(MOUSE_BUTTON_RIGHT.data)->release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
		{
			md.getButton(MOUSE_BUTTON_MIDDLE.data)->press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
		{
			md.getButton(MOUSE_BUTTON_MIDDLE.data)->release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
		{
			md.getButton(MOUSE_BUTTON_4.data)->press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
		{
			md.getButton(MOUSE_BUTTON_4.data)->release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
		{
			md.getButton(MOUSE_BUTTON_5.data)->press();
		}
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
		{
			md.getButton(MOUSE_BUTTON_5.data)->release();
		}
		if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			float delta = ((float)(short)mouse.usButtonData) / WHEEL_DELTA;
			md.simulateWheelMove(delta);
		}
		if (mouse.usButtonFlags & RI_MOUSE_HWHEEL)
		{
			float delta = ((float)(short)mouse.usButtonData) / WHEEL_DELTA;
			md.simulateHorizontalWheelMove(delta);
		}
	}

}