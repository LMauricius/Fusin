#include "IOSystems/RawInput/FusinRawInputDS4Handler.h"

#ifdef FUSIN_BUILD_RAW_INPUT

#include "Devices/FusinDSDevice.h"
#include "IOCodes/FusinDS.h"
#include "Utilities/FusinLog.h"
#include "Utilities/FusinByteDebug.h"
#include "Utilities/FusinBitCast.h"
#include <algorithm>

namespace Fusin
{

	RawInputDS4Handler::RawInputDS4Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: RawInputReportHandler(riDeviceHandle, riDeviceInfo, true)
	{
	}

	RawInputDS4Handler::~RawInputDS4Handler()
	{

	}

	bool RawInputDS4Handler::initialize()
	{
		if (!RawInputDeviceHandler::initialize()) return false;

		mFusinDevice = new DSDevice(mProductName, true);
		mFusinDevice->_setConnectionMode(mInputReportLength == 64 ? CM_USB : CM_BT);
		mFusinDevice->_setName(mProductName);

		Log::singleton() << "DS4 Device found: " << mProductName << "\n";

		return true;
	}

	void RawInputDS4Handler::handleInputReport(PBYTE pReport)
	{
		DSDevice& dsD = *static_cast<DSDevice*>(mFusinDevice);
		PBYTE inputReport = nullptr;
		PBYTE extraReport = nullptr;
		PSHORT accelReport = nullptr;
		PSHORT gyroReport = nullptr;
		PBYTE touchReport = nullptr;

		if (dsD.connectionMode() == CM_USB)
		{
			inputReport = pReport + 1;
			accelReport = reinterpret_cast<PSHORT>(pReport + 13);
			gyroReport = reinterpret_cast<PSHORT>(pReport + 19);
			extraReport = pReport + 30;
			touchReport = pReport + 33;
		}
		else if (pReport[0] == 0x11)
		{
			inputReport = pReport + 3;
			accelReport = reinterpret_cast<PSHORT>(pReport + 15);
			gyroReport = reinterpret_cast<PSHORT>(pReport + 21);
			extraReport = pReport + 32;
			touchReport = pReport + 35;
		}
		else if (pReport[0] == 0x01)
		{
			inputReport = pReport + 1;
		}

		if (inputReport)
		{
			dsD.axisLeftStickX.setValue(fromBitRange8(inputReport[0]));
			dsD.axisLeftStickX.setValue(fromBitRange8(inputReport[1]));
			dsD.axisRightStickX.setValue(fromBitRange8(inputReport[2]));
			dsD.axisRightStickX.setValue(fromBitRange8(inputReport[3]));
			dsD.axisL2.setValue(fromBitRange8U(inputReport[7]));
			dsD.axisRightStickX.setValue(fromBitRange8U(inputReport[8]));

			dsD.buttonTriangle.setValue(getFlag(inputReport[4], 7));
			dsD.buttonCircle.setValue(getFlag(inputReport[4], 6));
			dsD.buttonCross.setValue(getFlag(inputReport[4], 5));
			dsD.buttonSquare.setValue(getFlag(inputReport[4], 4));
			if ((inputReport[4] & 0xf) > 7)
			{
				dsD.dPad.angle.setValue(0);
			}
			else
			{
				dsD.dPad.angle.setValue((float)(((inputReport[4] & 0xF) == 0) ? 360 : ((inputReport[4] & 0xF) * 45)));
			}

			dsD.buttonR3.setValue(getFlag(inputReport[5], 7));
			dsD.buttonL3.setValue(getFlag(inputReport[5], 6));
			dsD.buttonOptions.setValue(getFlag(inputReport[5], 5));
			dsD.buttonCreate.setValue(getFlag(inputReport[5], 4));
			dsD.buttonR3.setValue(getFlag(inputReport[5], 3));
			dsD.buttonL3.setValue(getFlag(inputReport[5], 2));
			dsD.buttonR1.setValue(getFlag(inputReport[5], 1));
			dsD.buttonL3.setValue(getFlag(inputReport[5], 0));

			dsD.buttonPS.setValue(getFlag(inputReport[6], 0));
			dsD.buttonTouchPad.setValue(getFlag(inputReport[6], 1));
		}

		if (accelReport)
		{
			dsD.motion.accelerationX.setValue(-(float)accelReport[0] / 8192);
			dsD.motion.accelerationY.setValue(-(float)accelReport[1] / 8192);
			dsD.motion.accelerationZ.setValue(-(float)accelReport[2] / 8192);
		}
		if (gyroReport)
		{
			dsD.motion.gyroPitch.setValue((float)gyroReport[0] / 32768);
			dsD.motion.gyroYaw.setValue((float)gyroReport[1] / 32768);
			dsD.motion.gyroRoll.setValue((float)gyroReport[2] / 32768);
		}
		if (extraReport)
		{
			/*dsD.mPhone = extraReport[0] & (1 << 6));
			dsD.mMic = extraReport[0] & (1 << 5));
			//dsD.mUsb = extraReport[0] & (1 << 5));*/
			dsD.battery.charging.setValue(getFlag(extraReport[0], 4));
			dsD.battery.energy.setValue((float)(extraReport[0] & 0x0f) * 10);
			/*dsD._setCharging(getFlag(extraReport[0], 4));
			dsD._setBattery((extraReport[0] & 0x0f) * 10);*/
		}

		if (touchReport)
		{
			for (int touches = 0, touchOffset = 2; touches < touchReport[0]; touches++, touchOffset += 9)
			{
				/*
				touchReport[touchOffset+1 ... touchOffset+3]
				| A | B | C  | D  | E  | F |
				00000000 0000|0000 00000000
				^x       ^y   ^x   ^y
				x = DAB
				y = EFC
				*/
				int x1 = ((UINT)(touchReport[touchOffset + 2] & 0x0f) << 8) + touchReport[touchOffset + 1];//2, 3, 4
				int y1 = (((UINT)touchReport[touchOffset + 3]) << 4) + (touchReport[touchOffset + 2] >> 4);
				int x2 = ((UINT)(touchReport[touchOffset + 6] & 0x0f) << 8) + touchReport[touchOffset + 5];//6, 7, 8
				int y2 = (((UINT)touchReport[touchOffset + 7]) << 4) + (touchReport[touchOffset + 6] >> 4);

				TouchComponent::TouchSignals* touch = &dsD.touchPad[0];
				touch->pressure.setValue(!getFlag(touchReport[touchOffset], 7));
				touch->positionX.setValue((float)x1);
				touch->positionY.setValue((float)y1);

				touch = &dsD.touchPad[1];
				touch->pressure.setValue(!getFlag(touchReport[touchOffset + 4], 7));
				touch->positionX.setValue((float)x2);
				touch->positionY.setValue((float)y2);

				/*
				touch->positionX.setValue((float)(((UINT)(touchReport[touchOffset + 2] & 0x0f) << 8) + touchReport[touchOffset + 1]));//2, 3, 4
				touch->positionY.setValue((float)((((UINT)touchReport[touchOffset + 3]) << 4) + (touchReport[touchOffset + 2] >> 4)));

				touch = &dsD.touchPad[1];
				touch->pressure.setValue(!getFlag(touchReport[touchOffset + 4], 7));
				touch->positionX.setValue((float)(((UINT)(touchReport[touchOffset + 6] & 0x0f) << 8) + touchReport[touchOffset + 5]));//6, 7, 8
				touch->positionY.setValue((float)((((UINT)touchReport[touchOffset + 7]) << 4) + (touchReport[touchOffset + 6] >> 4)));
				*/
			}
		}
	}

	void RawInputDS4Handler::handleOutputReport(PBYTE pReport)
	{
		DSDevice& dsD = *static_cast<DSDevice*>(mFusinDevice);

		if (dsD.connectionMode() == CM_BT)
		{
			pReport[0] = 0x11;
			pReport[1] = 0x80;
			pReport[3] = 0xff;//enable rumble
			pReport[6] = (BYTE)(dsD.vibration.rightForce.value() * 255); //fast motor
			pReport[7] = (BYTE)(dsD.vibration.leftForce.value() * 255); //slow motor
			pReport[8] = (BYTE)(dsD.rgb.red.value() * 255); //red
			pReport[9] = (BYTE)(dsD.rgb.green.value() * 255); //green
			pReport[10] = (BYTE)(dsD.rgb.blue.value() * 255); //blue
			pReport[11] = 0; //flash on duration
			pReport[12] = 0; //flash off duration
		}
		else
		{
			pReport[0] = 0x05;
			pReport[1] = 0xf3;
			pReport[4] = (BYTE)(dsD.vibration.rightForce.value() * 255); //fast motor
			pReport[5] = (BYTE)(dsD.vibration.leftForce.value() * 255); //slow motor
			pReport[6] = (BYTE)(dsD.rgb.red.value() * 255); //red
			pReport[7] = (BYTE)(dsD.rgb.green.value() * 255); //green
			pReport[8] = (BYTE)(dsD.rgb.blue.value() * 255); //blue
			pReport[9] = 0; //flash on duration
			pReport[10] = 0; //flash off duration
		}
	}

}

#endif // FUSIN_BUILD_RAW_INPUT