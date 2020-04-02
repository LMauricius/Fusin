#ifndef _FUSIN_RAW_INPUT_DEVICE_HANDLER_H
#define _FUSIN_RAW_INPUT_DEVICE_HANDLER_H

#include "FusinPrerequisites.h"
#include "FusinDevice.h"
#define NOMINMAX
#include <Windows.h>
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
		RawInputDeviceHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, Device* device);
		virtual ~RawInputDeviceHandler() = 0;
		virtual void handleRawInput(PRAWINPUT pRawInput);
		virtual void update();

		inline bool isSuccessful() { return mSuccess; }
		inline Device* device() { return mDevice; }
		inline HANDLE riDevice() { return mRIDeviceHandle; }
		inline HANDLE hidDevice() { return mHidDeviceHandle; }

	protected:
		bool mSuccess;
		HANDLE mRIDeviceHandle;
		HANDLE mHidDeviceHandle;
		PRID_DEVICE_INFO mpDeviceInfo;
		PHIDP_PREPARSED_DATA mpPreparsedData;
		HIDP_CAPS mCaps;
		USHORT mInputReportLength, mOutputReportLength;

		Device *mDevice;
		String mProductName;
	};

}

#endif