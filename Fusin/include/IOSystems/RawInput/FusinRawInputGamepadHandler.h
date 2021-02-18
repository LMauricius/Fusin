#ifndef _FUSIN_RAW_INPUT_GAMEPAD_HANDLER_H
#define _FUSIN_RAW_INPUT_GAMEPAD_HANDLER_H

#include "FusinRawInputReportHandler.h"
#include <Windows.h>
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{
	class GamepadDevice;

	class _FUSIN_EXPORT RawInputGamepadHandler : public RawInputReportHandler
	{
	public:
		RawInputGamepadHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
		~RawInputGamepadHandler();

		bool initialize();
		void handleInputReport(PBYTE pReport);
		void handleOutputReport(PBYTE pReport);

	protected:
		PHIDP_BUTTON_CAPS mpButtonCaps;
		PHIDP_VALUE_CAPS mpValueCaps, mpOutValueCaps;
		USHORT mButtonCapsLength, mValueCapsLength, mOutValueCapsLength, mButtonNum, mAxisNum;
		std::map<USAGE, int> mUsageAxisMap;
	};

}

#endif