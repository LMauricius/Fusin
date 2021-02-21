#ifndef _FUSIN_RAW_INPUT_DS3_HANDLER_H
#define _FUSIN_RAW_INPUT_DS3_HANDLER_H

#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	#include <Windows.h>
	extern "C"
	{
	#include "hidsdi.h"
	}

	namespace Fusin
	{
		class DSDevice;

		class _FUSIN_EXPORT RawInputDS3Handler : public RawInputReportHandler
		{
		public:
			RawInputDS3Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
			~RawInputDS3Handler();

			bool initialize();
			void handleInputReport(PBYTE pReport);
			void handleOutputReport(PBYTE pReport);
		};

	}

#endif // FUSIN_BUILD_RAW_INPUT

#endif