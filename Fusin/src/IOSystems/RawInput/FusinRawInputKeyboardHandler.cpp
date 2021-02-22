#include "IOSystems/RawInput/FusinRawInputKeyboardHandler.h"

#ifdef FUSIN_BUILD_RAW_INPUT

#include "IOSystems/RawInput/FusinRawInputKeyboardHandler.h"

#include "Devices/FusinKeyboardDevice.h"
#include "Utilities/FusinLog.h"
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{

	RawInputKeyboardHandler::RawInputKeyboardHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo):
		RawInputDeviceHandler(riDeviceHandle, riDeviceInfo)
	{
	}

	RawInputKeyboardHandler::~RawInputKeyboardHandler()
	{

	}
	
	bool RawInputKeyboardHandler::initialize()
	{
		if (!RawInputDeviceHandler::initialize()) return false;

		// Create the device
		mFusinDevice = new KeyboardDevice(
			mProductName,
			mpDeviceInfo->keyboard.dwNumberOfKeysTotal,
			mpDeviceInfo->keyboard.dwNumberOfFunctionKeys,
			mpDeviceInfo->keyboard.dwNumberOfIndicators
		);

		Log::singleton() << "Keyboard Device found: " << mProductName <<
			"\n    key number: " << mpDeviceInfo->keyboard.dwNumberOfKeysTotal <<
			"\n    function key number: " << mpDeviceInfo->keyboard.dwNumberOfFunctionKeys <<
			"\n    LED number: " << mpDeviceInfo->keyboard.dwNumberOfIndicators << "\n";

		return true;
	}

	void RawInputKeyboardHandler::update()
	{

	}

	void RawInputKeyboardHandler::handleRawInput(PRAWINPUT pRawInput)
	{
		KeyboardDevice* kbD = static_cast<KeyboardDevice*>(mFusinDevice);
		RAWKEYBOARD& kb = pRawInput->data.keyboard;

		std::vector<BYTE> keyBuf(256, 0);
		int sc = MapVirtualKey(kb.VKey, MAPVK_VK_TO_VSC);
		
		Char keyc = kb.VKey;
		wchar_t unicodeC;
		int retVal = ToUnicode(
				kb.VKey, sc, 
				keyBuf.data(),
				&unicodeC, 1, 0
			);
		if (retVal > 0)
		{
			keyc = unicodeC;
		}

		if (kb.Flags & RI_KEY_BREAK)
		{
			kbD->keys[keyc].release();
		}
		else 
		{
			kbD->keys[keyc].press();
			kbD->keys.getTypedKey(keyc).press();
			Log::singleton() << keyc << "\n";
		}
	}

}

#endif // FUSIN_BUILD_RAW_INPUT