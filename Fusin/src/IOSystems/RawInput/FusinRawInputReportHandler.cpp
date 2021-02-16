#include "IOSystems/RawInput/FusinRawInputReportHandler.h"
#include "Utilities/FusinLog.h"
#include "Utilities/FusinByteDebug.h"
#include <algorithm>
#include <iostream>

namespace Fusin
{

	RawInputReportHandler::RawInputReportHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, bool readEachRawInput, Device* fusinDevice)
		: RawInputDeviceHandler(riDeviceHandle, riDeviceInfo)
		, mReadEachRawInput(readEachRawInput)
		, mUpdateCounter(0)
	{

	}

	RawInputReportHandler::~RawInputReportHandler()
	{

	}

	void RawInputReportHandler::handleRawInput(PRAWINPUT pRawInput)
	{
		if (mReadEachRawInput)
		{
			handleInputReport(pRawInput->data.hid.bRawData);
		}
	}

	void RawInputReportHandler::update()
	{
		/*
		Input report
		*/
		if (!mReadEachRawInput)
		{
			if (mInputReportLength)
			{
				PBYTE pInputReport = new BYTE[mInputReportLength];

				DWORD bytesRead;
				if (!ReadFile(mHidDeviceHandle, pInputReport, mInputReportLength, &bytesRead, 0))
				{
					delete[] pInputReport;
					return;
				}
				/*if (!HidD_GetInputReport(mHidDeviceHandle, pInputReport, mInputReportLength))
				{
				delete[] pInputReport;
				return;
				}*/

				handleInputReport(pInputReport);
				delete[] pInputReport;
			}
		}

		/*
		Output report
		*/
		if (mOutputReportLength)
		{
			PBYTE pOutputReport = new BYTE[mOutputReportLength];
			memset(pOutputReport, 0, mOutputReportLength);

			handleOutputReport(pOutputReport);

			DWORD bytesWritten;
			WriteFile(mHidDeviceHandle, pOutputReport, mOutputReportLength, &bytesWritten, false);
			//if (!HidD_SetOutputReport(mHidDevice, pOutputReport, mOutputReportLength))
			/*if (!bytesWritten)
			{
				std::cout << "g";
			}*/
			delete[] pOutputReport;
		}
		mUpdateCounter++;
	}

	void RawInputReportHandler::handleInputReport(PBYTE pReport) {}
	void RawInputReportHandler::handleOutputReport(PBYTE pReport) {}
}