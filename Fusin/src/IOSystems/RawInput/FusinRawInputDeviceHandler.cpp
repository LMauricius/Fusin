#include "IOSystems/RawInput/FusinRawInputDeviceHandler.h"
#include "Utilities/FusinLog.h"

namespace Fusin
{

	RawInputDeviceHandler::RawInputDeviceHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: mRIDeviceHandle(riDeviceHandle)
		, mpDeviceInfo(riDeviceInfo)
	{
	}

	RawInputDeviceHandler::~RawInputDeviceHandler()
	{
		if (mHidDeviceHandle != INVALID_HANDLE_VALUE) CloseHandle(mHidDeviceHandle);
		if (mpPreparsedData) HidD_FreePreparsedData(mpPreparsedData);
	}
	
	bool RawInputDeviceHandler::initialize()
	{
		// get the fusinDevice registry name
		wchar_t *pRegName;
		UINT regCharCount;

		if (GetRawInputDeviceInfoW(mRIDeviceHandle, RIDI_DEVICENAME, NULL, &regCharCount) == -1) return false;
		pRegName = new wchar_t[regCharCount];
		if (pRegName)
		{
			if (GetRawInputDeviceInfoW(mRIDeviceHandle, RIDI_DEVICENAME, pRegName, &regCharCount) == -1)
			{
				delete[] pRegName;
				return false;
			}
		}

		// Create hidDevice handle for ioType/output
		mHidDeviceHandle = CreateFileW(pRegName, MAXIMUM_ALLOWED, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		delete[] pRegName;
		if (mHidDeviceHandle == INVALID_HANDLE_VALUE) return false;

		// Get data
		mpPreparsedData = nullptr;
		if (HidD_GetPreparsedData(mHidDeviceHandle, &mpPreparsedData) == -1) return false;
		if (HidP_GetCaps(mpPreparsedData, &mCaps) == -1) return false;
		mInputReportLength = mCaps.InputReportByteLength;
		mOutputReportLength = mCaps.OutputReportByteLength;

		// Get name
		UINT charCount = 128;
		wchar_t *pName = new wchar_t[charCount];
		if (!pName) return false;
		if (HidD_GetProductString(mHidDeviceHandle, pName, charCount))
		{
			mProductName = pName;
		}
		delete[] pName;
		
		return true;
	}

	void RawInputDeviceHandler::handleRawInput(PRAWINPUT pRawInput)
	{

	}

	void RawInputDeviceHandler::update()
	{

	}

	void RawInputDeviceHandler::setDeviceIndex(Index ind)
	{
		mDeviceIndex = ind;
	}


}