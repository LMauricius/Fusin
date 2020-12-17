#ifndef _FUSIN_RAW_INPUT_DS4_HANDLER_H
#define _FUSIN_RAW_INPUT_DS4_HANDLER_H

#include "FusinRawInputReportHandler.h"
#include <Windows.h>
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{
	class DSDevice;

	class RawInputDS4Handler : public RawInputReportHandler
	{
	public:
		RawInputDS4Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
		~RawInputDS4Handler();

		bool initialize();
		void handleInputReport(PBYTE pReport);
		void handleOutputReport(PBYTE pReport);
	};

}

#endif