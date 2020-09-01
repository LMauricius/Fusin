#include "IOSystems/RawInput/FusinRawInputDS4Handler.h"
#include "FusinDSDevice.h"
#include "FusinDS.h"
#include "Utilities/FusinLog.h"
#include "FusinByteDebug.h"
#include <algorithm>

namespace Fusin
{

	RawInputDS4Handler::RawInputDS4Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: RawInputReportHandler(riDeviceHandle, riDeviceInfo, true, new DSDevice())
	{
		if (!mSuccess) return;

		mDevice->_setConnectionMode(mInputReportLength == 64 ? CM_USB : CM_BT);
		mDevice->_setName(mProductName);

		Log::singleton() << "DS4 Device found: " << mProductName << "\n";
	}

	RawInputDS4Handler::~RawInputDS4Handler()
	{

	}

	void RawInputDS4Handler::handleInputReport(PBYTE pReport)
	{
		DSDevice& dsD = *static_cast<DSDevice*>(mDevice);
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
			dsD.axes.leftX.setValue(FUSIN_SIGNED_RANGE_BYTE(inputReport[0]));
			dsD.axes.leftY.setValue(FUSIN_SIGNED_RANGE_BYTE(inputReport[1]));
			dsD.axes.rightX.setValue(FUSIN_SIGNED_RANGE_BYTE(inputReport[2]));
			dsD.axes.rightY.setValue(FUSIN_SIGNED_RANGE_BYTE(inputReport[3]));
			dsD.axes.l2.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[7]));
			dsD.axes.r2.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[8]));

			dsD.buttons.triangle.setValue(FUSIN_RANGE_BIT(inputReport[4], 7));
			dsD.buttons.circle.setValue(FUSIN_RANGE_BIT(inputReport[4], 6));
			dsD.buttons.cross.setValue(FUSIN_RANGE_BIT(inputReport[4], 5));
			dsD.buttons.square.setValue(FUSIN_RANGE_BIT(inputReport[4], 4));
			if (inputReport[4] & 0xf > 7)
			{
				dsD.dPad.angle.setValue(0);
			}
			else
			{
				dsD.dPad.angle.setValue((inputReport[4] & 0xF == 0) ? 360 : (inputReport[4] & 0xF * 45));
			}

			dsD.buttons.r3.setValue(FUSIN_RANGE_BIT(inputReport[5], 7));
			dsD.buttons.l3.setValue(FUSIN_RANGE_BIT(inputReport[5], 6));
			dsD.buttons.options.setValue(FUSIN_RANGE_BIT(inputReport[5], 5));
			dsD.buttons.share.setValue(FUSIN_RANGE_BIT(inputReport[5], 4));
			dsD.buttons.r2.setValue(FUSIN_RANGE_BIT(inputReport[5], 3));
			dsD.buttons.l2.setValue(FUSIN_RANGE_BIT(inputReport[5], 2));
			dsD.buttons.r1.setValue(FUSIN_RANGE_BIT(inputReport[5], 1));
			dsD.buttons.l1.setValue(FUSIN_RANGE_BIT(inputReport[5], 0));

			dsD.buttons.ps.setValue(FUSIN_RANGE_BIT(inputReport[6], 0));
			dsD.buttons.touchPad.setValue(FUSIN_RANGE_BIT(inputReport[6], 1));
		}

		if (accelReport)
		{
			dsD.motion.acceleration.xAxis.setValue(-(float)accelReport[0] / 8192);
			dsD.motion.acceleration.yAxis.setValue(-(float)accelReport[1] / 8192);
			dsD.motion.acceleration.zAxis.setValue(-(float)accelReport[2] / 8192);
		}
		if (gyroReport)
		{
			dsD.motion.gyro.pitchAxis.setValue((float)gyroReport[0] / 32768);
			dsD.motion.gyro.yawAxis.setValue((float)gyroReport[1] / 32768);
			dsD.motion.gyro.rollAxis.setValue((float)gyroReport[2] / 32768);
		}
		if (extraReport)
		{
			/*dsD.mPhone = extraReport[0] & (1 << 6));
			dsD.mMic = extraReport[0] & (1 << 5));
			//dsD.mUsb = extraReport[0] & (1 << 5));*/
			dsD._setCharging(FUSIN_RANGE_BIT(extraReport[0], 4));
			dsD._setBattery((extraReport[0] & 0x0f) * 10);
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
				TouchDevice::TouchSignals* touch = dsD.touchPad.getTouch(1);
				touch->state.setValue(!FUSIN_RANGE_BIT(touchReport[touchOffset], 7));
				touch->positionX.setValue(((UINT)(touchReport[touchOffset + 2] & 0x0f) << 8) + touchReport[touchOffset + 1]);//2, 3, 4
				touch->positionY.setValue((((UINT)touchReport[touchOffset + 3]) << 4) + (touchReport[touchOffset + 2] >> 4));

				touch = dsD.touchPad.getTouch(2);
				touch->state.setValue(!FUSIN_RANGE_BIT(touchReport[touchOffset + 4], 7));
				touch->positionX.setValue(((UINT)(touchReport[touchOffset + 6] & 0x0f) << 8) + touchReport[touchOffset + 5]);//6, 7, 8
				touch->positionY.setValue((((UINT)touchReport[touchOffset + 7]) << 4) + (touchReport[touchOffset + 6] >> 4));
			}
		}
	}

	void RawInputDS4Handler::handleOutputReport(PBYTE pReport)
	{
		DSDevice& dsD = *static_cast<DSDevice*>(mDevice);

		if (dsD.connectionMode() == CM_BT)
		{
			pReport[0] = 0x11;
			pReport[1] = 0x80;
			pReport[3] = 0xff;//enable rumble
			pReport[6] = dsD.getRightVibration() * 255; //fast motor
			pReport[7] = dsD.getLeftVibration() * 255; //slow motor
			pReport[8] = dsD.getColor().r * 255; //red
			pReport[9] = dsD.getColor().g * 255; //green
			pReport[10] = dsD.getColor().b * 255; //blue
			pReport[11] = 0; //flash on duration
			pReport[12] = 0; //flash off duration
		}
		else
		{
			pReport[0] = 0x05;
			pReport[1] = 0xf3;
			pReport[4] = dsD.getRightVibration() * 255; //fast motor
			pReport[5] = dsD.getLeftVibration() * 255; //slow  motor
			pReport[6] = dsD.getColor().r * 255; //red
			pReport[7] = dsD.getColor().g * 255; //green
			pReport[8] = dsD.getColor().b * 255; //blue
			pReport[9] = 0; //flash on duration
			pReport[10] = 0; //flash off duration
		}
	}

}