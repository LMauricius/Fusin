#ifndef _FUSIN_RAW_INPUT_DEVICE_HANDLER_H
#define _FUSIN_RAW_INPUT_DEVICE_HANDLER_H

#include "FusinPrerequisites.h"
#include "Devices/FusinDevice.h"
#define NOMINMAX
#include <windows.h>
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{
	class RawInputSystem;

	class RawInputDeviceHandler
	{
		//friend RawInputSystem;

	public:
		RawInputDeviceHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo);
		virtual ~RawInputDeviceHandler() = 0;

		// Returns whether successful
		virtual bool initialize();

		virtual void handleRawInput(PRAWINPUT pRawInput);
		virtual void update();

		inline Device* fusinDevice() { return mFusinDevice; }
		inline HANDLE riDevice() { return mRIDeviceHandle; }
		inline HANDLE hidDevice() { return mHidDeviceHandle; }

	protected:
		HANDLE mRIDeviceHandle;
		HANDLE mHidDeviceHandle;
		PRID_DEVICE_INFO mpDeviceInfo;
		PHIDP_PREPARSED_DATA mpPreparsedData;
		HIDP_CAPS mCaps;
		USHORT mInputReportLength, mOutputReportLength;

		Device *mFusinDevice;
		String mProductName;
	};

}

#endif