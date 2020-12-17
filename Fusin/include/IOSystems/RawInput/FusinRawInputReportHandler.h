#ifndef _FUSIN_RAW_INPUT_REPORT_HANDLER_H
#define _FUSIN_RAW_INPUT_REPORT_HANDLER_H

#include "FusinRawInputDeviceHandler.h"
#include <Windows.h>
extern "C"
{
#include "hidsdi.h"
}

namespace Fusin
{
	class RawInputReportHandler : public RawInputDeviceHandler
	{
	public:
		RawInputReportHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, bool readEachRawInput, Device* fusinDevice = nullptr);
		~RawInputReportHandler() = 0;
		
		bool initialize();
		void update();
		void handleRawInput(PRAWINPUT pRawInput);

		virtual void handleInputReport(PBYTE pReport) = 0;
		virtual void handleOutputReport(PBYTE pReport) = 0;

	protected:
		/*
		If true, handleInputReport() will be called for each raw ioType sent.
		If false, it will be called once per _update() call.
		*/
		bool mReadEachRawInput;
		long long int mUpdateCounter;
	};

}

#endif