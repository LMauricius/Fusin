#ifndef _FUSIN_RAW_INPUT_DS4_HANDLER_H
#define _FUSIN_RAW_INPUT_DS4_HANDLER_H

#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	#include "FusinRawInputReportHandler.h"

	#include <Windows.h>
	extern "C"
	{
	#include "hidsdi.h"
	}

	namespace Fusin
	{
		class DSDevice;

		class _FUSIN_EXPORT RawInputDS4Handler : public RawInputReportHandler
		{
		public:
			RawInputDS4Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
			~RawInputDS4Handler();

			bool initialize();
			void handleInputReport(PBYTE pReport);
			void handleOutputReport(PBYTE pReport);
		};

	}

#endif // FUSIN_BUILD_RAW_INPUT

#endif