#include "IOSystems/RawInput/FusinRawInputGamepadHandler.h"

#ifdef FUSIN_BUILD_RAW_INPUT

#include "Devices/FusinGamepadDevice.h"
#include "IOCodes/FusinGamepad.h"
#include "Utilities/FusinLog.h"
#include "Utilities/FusinByteDebug.h"
#include <algorithm>
#include <iostream>

namespace Fusin
{

	RawInputGamepadHandler::RawInputGamepadHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: RawInputReportHandler(riDeviceHandle, riDeviceInfo, true)
	{
	}

	bool RawInputGamepadHandler::initialize()
	{
		if (!RawInputDeviceHandler::initialize()) return false;

		mpButtonCaps = nullptr;
		mpValueCaps = nullptr;

		// Button caps
		mButtonCapsLength = mCaps.NumberInputButtonCaps;
		mpButtonCaps = (PHIDP_BUTTON_CAPS)new char[sizeof(HIDP_BUTTON_CAPS) * mButtonCapsLength];
		if (HidP_GetButtonCaps(HidP_Input, mpButtonCaps, &mButtonCapsLength, mpPreparsedData) == HIDP_STATUS_SUCCESS)
		{
			mButtonNum = mpButtonCaps->Range.UsageMax - mpButtonCaps->Range.UsageMin + 1;
		}
		else
		{
			return false;
		}

		// Value caps
		bool hasDPad = false;
		mValueCapsLength = mCaps.NumberInputValueCaps;
		mpValueCaps = (PHIDP_VALUE_CAPS)new char[sizeof(HIDP_VALUE_CAPS) * mValueCapsLength];
		if (HidP_GetValueCaps(HidP_Input, mpValueCaps, &mValueCapsLength, mpPreparsedData) == HIDP_STATUS_SUCCESS)
		{
			mAxisNum = mValueCapsLength;

			std::sort(mpValueCaps, mpValueCaps + mValueCapsLength,
				[](const HIDP_VALUE_CAPS& a, const HIDP_VALUE_CAPS& b)
			{
				return a.Range.UsageMin < b.Range.UsageMin;
			});
			int c = 0;
			for (int i = 0; i < mValueCapsLength; i++)
			{
				// Make sure that the X, Y, Z, Rot-Z and Hat switch axes come first, with their reserved axis indices
				switch (mpValueCaps[i].Range.UsageMin)
				{
				case 0x30:	// X-axis
					mUsageAxisMap[mpValueCaps[i].Range.UsageMin] = 0;
					break;
				case 0x31:	// Y-axis
					mUsageAxisMap[mpValueCaps[i].Range.UsageMin] = 1;
					break;
				case 0x32: // Z-axis
					mUsageAxisMap[mpValueCaps[i].Range.UsageMin] = 2;
					break;
				case 0x35: // Rotate-Z
					mUsageAxisMap[mpValueCaps[i].Range.UsageMin] = 3;
					break;
				case 0x39:	// Hat Switch
					mUsageAxisMap[mpValueCaps[i].Range.UsageMin] = 4;
					hasDPad = true;
					break;
				default:  // other axes
					mUsageAxisMap[mpValueCaps[i].Range.UsageMin] = 5 + c;
					c++;
					break;
				}
			}
			mAxisNum = 5 + c;
		}
		else
		{
			return false;
		}

		// Check vibration
		bool hasVibration = false;

		// Now that we found all the info create the Device object
		mFusinDevice = new GamepadDevice(mProductName, mButtonNum, mAxisNum, hasDPad, hasVibration);

		Log::singleton() << "Gamepad Device found: " << mProductName <<
			"\n    Button num: " << mButtonNum << 
			"\n    Axis num: " << mAxisNum <<
			"\n    Has DPad: " << hasDPad <<
			"\n    Has Vibration: " << hasVibration << "\n";

		return true;
	}

	RawInputGamepadHandler::~RawInputGamepadHandler()
	{
		if (mpButtonCaps) delete[] mpButtonCaps;
		if (mpValueCaps) delete[] mpValueCaps;
	}

	void RawInputGamepadHandler::handleInputReport(PBYTE pReport)
	{
		GamepadDevice& gpDevice = *static_cast<GamepadDevice*>(mFusinDevice);
		const unsigned int maxButtons = 128;
		USAGE usage[maxButtons];
		ULONG usageLength, value;

		// Debug data
		//std::wcout << byteString(pReport, (mInputReportLength < 100 ? mInputReportLength : 100)) << "\n";
		/*BYTE a, b;
		String str;
		for (int i = 0; i < (mInputReportLength < 100 ? mInputReportLength : 100); i++)
		{
			a = *((BYTE*)pReport + i) / 16;
			b = *((BYTE*)pReport + i) % 16;
			if (a > 9) str += (Char)a - 10 + 65;
			else str += (Char)a + 48;
			if (b > 9) str += (Char)b - 10 + 65;
			else str += (Char)b + 48;
		}
		std::wcout << str << "\n";*/

		gpDevice.clear();

		// Get the state of discrete-valued-controls
		for (int i = 0; i < mValueCapsLength; i++)
		{
			if (HidP_GetUsageValue(HidP_Input, mpValueCaps[i].UsagePage, 0, mpValueCaps[i].Range.UsageMin, &value, mpPreparsedData,
				(PCHAR)pReport, mInputReportLength) == HIDP_STATUS_SUCCESS)
			{
				// D-Pad
				if (mpValueCaps[i].Range.UsageMin == 0x39)
				{
					if (value > 7)
					{
						gpDevice.dPad.angle.setValue(0.0f);
					}
					else
					{
						gpDevice.dPad.angle.setValue((value == 0) ? 360.0f : (45.0f * value));
					}
				}
				// Other axes
				else
				{
					float tVal = (float)value / 255.0f * 2.0f - 1.0f;// -1 - 1
					gpDevice.axes[mUsageAxisMap[mpValueCaps[i].Range.UsageMin]].setValue(tVal);
				}
			}
		}

		// Get the pressed buttons
		usageLength = mButtonNum;
		if (HidP_GetUsages(HidP_Input, mpButtonCaps->UsagePage, 0, usage, &usageLength, mpPreparsedData,
			(PCHAR)pReport, mInputReportLength) == HIDP_STATUS_SUCCESS)
		{
			for (size_t i = 0; i < usageLength; i++)
			{
				gpDevice.buttons[usage[i] - mpButtonCaps->Range.UsageMin].press();
			}
		}
	}

	void RawInputGamepadHandler::handleOutputReport(PBYTE pReport)
	{
		/*PBYTE outputReport = new BYTE[mOutputReportLength];
		memset(outputReport, 0, mOutputReportLength);

		if (gpDevice.hasVibrationFeedback())
		{
		HidP_SetUsageValue(HidP_Output, );
		}

		HidD_SetOutputReport(mHidDevice, outputReport, mOutputReportLength);
		delete[] outputReport;*/
	}

}

#endif // FUSIN_BUILD_RAW_INPUT