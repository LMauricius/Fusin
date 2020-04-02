#include "RawInput/FusinRawInputKeyboardHandler.h"
#include "FusinKeyboardDevice.h"
#include "FusinLog.h"
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{

	RawInputKeyboardHandler::RawInputKeyboardHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: RawInputDeviceHandler(riDeviceHandle, riDeviceInfo, new KeyboardDevice())
	{
		if (!mSuccess) return;

		mDevice->_setName(mProductName);
		((KeyboardDevice*)mDevice)->_setProperties(mpDeviceInfo->keyboard.dwNumberOfKeysTotal,
			mpDeviceInfo->keyboard.dwNumberOfFunctionKeys, mpDeviceInfo->keyboard.dwNumberOfIndicators);

		Log::singleton() << "Keyboard Device found: " << mProductName <<
			"\n    key number: " << mpDeviceInfo->keyboard.dwNumberOfKeysTotal <<
			"\n    function key number: " << mpDeviceInfo->keyboard.dwNumberOfFunctionKeys <<
			"\n    LED number: " << mpDeviceInfo->keyboard.dwNumberOfIndicators << "\n";
	}

	RawInputKeyboardHandler::~RawInputKeyboardHandler()
	{

	}

	void RawInputKeyboardHandler::update()
	{

	}

	void RawInputKeyboardHandler::handleRawInput(PRAWINPUT pRawInput)
	{
		KeyboardDevice* kbD = static_cast<KeyboardDevice*>(mDevice);
		RAWKEYBOARD& kb = pRawInput->data.keyboard;

		Char keyc = kb.VKey;

		if (kb.Flags & RI_KEY_BREAK)
		{
			kbD->getKey(keyc)->release();
		}
		else 
		{
			kbD->getKey(keyc)->press();
		}
	}

}