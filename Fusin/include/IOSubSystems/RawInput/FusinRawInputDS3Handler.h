#ifndef _FUSIN_RAW_INPUT_DS3_HANDLER_H
#define _FUSIN_RAW_INPUT_DS3_HANDLER_H

#include "FusinRawInputReportHandler.h"
#include <Windows.h>
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{
	class DSDevice;

	class RawInputDS3Handler : public RawInputReportHandler
	{
		friend RawInputSystem;

	public:
		RawInputDS3Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
		~RawInputDS3Handler();

		void handleInputReport(PBYTE pReport);
		void handleOutputReport(PBYTE pReport);
	};

}

#endif