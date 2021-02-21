#ifndef _FUSIN_RAW_INPUT_JOYCON_HANDLER_H
#define _FUSIN_RAW_INPUT_JOYCON_HANDLER_H

#include "FusinPrerequisites.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	namespace Fusin
	{
		class NintendoDevice;

		class _FUSIN_EXPORT RawInputJoyConHandler : public RawInputReportHandler
		{
		public:
			RawInputJoyConHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, bool rightSide);
			~RawInputJoyConHandler();

			bool initialize();
			void handleInputReport(PBYTE pReport);
			void handleOutputReport(PBYTE pReport);

		protected:
			bool mRightSide;
		};

	}

#endif // FUSIN_BUILD_RAW_INPUT

#endif