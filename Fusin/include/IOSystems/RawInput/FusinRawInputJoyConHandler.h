#ifndef _FUSIN_RAW_INPUT_JOYCON_HANDLER_H
#define _FUSIN_RAW_INPUT_JOYCON_HANDLER_H

#include "FusinRawInputReportHandler.h"

namespace Fusin
{
	class NintendoDevice;

	class RawInputJoyConHandler : public RawInputReportHandler
	{
		friend RawInputSystem;

	public:
		RawInputJoyConHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, bool rightSide);
		~RawInputJoyConHandler();

		bool initialize();
		void handleInputReport(PBYTE pReport);
		void handleOutputReport(PBYTE pReport);
	};

}

#endif