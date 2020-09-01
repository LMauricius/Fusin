#include "IOSystems/RawInput/FusinRawInputDeviceHandler.h"
#include "Utilities/FusinLog.h"

namespace Fusin
{

	RawInputDeviceHandler::RawInputDeviceHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, Device* device = nullptr)
		: mRIDeviceHandle(riDeviceHandle)
		, mpDeviceInfo(riDeviceInfo)
		, mDevice(device)
	{
		mSuccess = true;

		// get the device registry name
		wchar_t *pRegName;
		UINT regCharCount;

		if (GetRawInputDeviceInfoW(riDeviceHandle, RIDI_DEVICENAME, NULL, &regCharCount) == -1) { mSuccess = false; return; }
		pRegName = new wchar_t[regCharCount];
		if (pRegName)
		{
			if (GetRawInputDeviceInfoW(riDeviceHandle, RIDI_DEVICENAME, pRegName, &regCharCount) == -1)
			{
				delete[] pRegName;
				mSuccess = false;
				return;
			}
		}
		else return;

		// Create device handle for ioType/output
		mHidDeviceHandle = CreateFileW(pRegName, MAXIMUM_ALLOWED, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		delete[] pRegName;
		if (mHidDeviceHandle == INVALID_HANDLE_VALUE) { mSuccess = false; return; }

		// Get data
		mpPreparsedData = nullptr;
		if (HidD_GetPreparsedData(mHidDeviceHandle, &mpPreparsedData) == -1) { mSuccess = false; return; }
		if (HidP_GetCaps(mpPreparsedData, &mCaps) == -1) { mSuccess = false; return; }
		mInputReportLength = mCaps.InputReportByteLength;
		mOutputReportLength = mCaps.OutputReportByteLength;

		// Get name
		UINT charCount = 128;
		wchar_t *pName = new wchar_t[charCount];
		if (!pName) { mSuccess = false; return; }
		if (HidD_GetProductString(mHidDeviceHandle, pName, charCount))
		{
			mProductName = pName;
		}
		delete[] pName;
	}

	RawInputDeviceHandler::~RawInputDeviceHandler()
	{
		if (mHidDeviceHandle != INVALID_HANDLE_VALUE) CloseHandle(mHidDeviceHandle);
		if (mpPreparsedData) HidD_FreePreparsedData(mpPreparsedData);
	}

	void RawInputDeviceHandler::handleRawInput(PRAWINPUT pRawInput)
	{

	}

	void RawInputDeviceHandler::update()
	{

	}


}