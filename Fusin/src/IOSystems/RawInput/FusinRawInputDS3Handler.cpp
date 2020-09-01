#include "IOSystems/RawInput/FusinRawInputDS3Handler.h"
#include "FusinDSDevice.h"
#include "FusinDS.h"
#include "Utilities/FusinLog.h"
#include "FusinByteDebug.h"
#include <algorithm>

namespace Fusin
{

	RawInputDS3Handler::RawInputDS3Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: RawInputReportHandler(riDeviceHandle, riDeviceInfo, true, new DSDevice())
	{
		if (!mSuccess) return;

		mDevice->_setConnectionMode(mInputReportLength == 64 ? CM_USB : CM_BT);
		mDevice->_setName(mProductName);

		Log::singleton() << "DS3 Device found: " << mProductName << "\n";
	}

	RawInputDS3Handler::~RawInputDS3Handler()
	{

	}

	void RawInputDS3Handler::handleInputReport(PBYTE pReport)
	{
		/*
		Big thanks to sicksaxis library
		*/
		DSDevice& dsD = *static_cast<DSDevice*>(mDevice);
		PBYTE inputReport = nullptr;
		PBYTE extraReport = nullptr;
		PSHORT accelReport = nullptr;
		PSHORT gyroReport = nullptr;

		inputReport = pReport + 2;
		accelReport = reinterpret_cast<PSHORT>(pReport + 40);
		//gyroReport = reinterpret_cast<PSHORT>(pReport + 46);
		extraReport = pReport + 30;

		if (inputReport)
		{
			if ((inputReport[0] >> 4) == 0)
			{
				dsD.dPad.angle.setValue(0);
			}
			else
			{
				float h = (float)(bool)(inputReport[0] & (1 << 5)) - (float)(bool)(inputReport[0] & (1 << 7));
				float v = (float)(bool)(inputReport[0] & (1 << 4)) - (float)(bool)(inputReport[0] & (1 << 6));
				float a = std::atan2(v, -h)*4.0f*PI;
				dsD.dPad.angle.setValue((a == 0.0f) ? 360.0f :
					(a * 45.0f));
			}
			dsD.buttons.options.setValue(FUSIN_RANGE_BIT(inputReport[0], 3));
			dsD.buttons.r3.setValue(FUSIN_RANGE_BIT(inputReport[0], 2));
			dsD.buttons.l3.setValue(FUSIN_RANGE_BIT(inputReport[0], 1));
			dsD.buttons.touchPad.setValue(FUSIN_RANGE_BIT(inputReport[0], 0));

			dsD.buttons.square.setValue(FUSIN_RANGE_BIT(inputReport[1], 7));
			dsD.buttons.cross.setValue(FUSIN_RANGE_BIT(inputReport[1], 6));
			dsD.buttons.circle.setValue(FUSIN_RANGE_BIT(inputReport[1], 5));
			dsD.buttons.triangle.setValue(FUSIN_RANGE_BIT(inputReport[1], 4));

			dsD.buttons.ps.setValue(FUSIN_RANGE_BIT(inputReport[2], 0));

			dsD.axes.leftX.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[3]));
			dsD.axes.leftY.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[4]));
			dsD.axes.rightX.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[5]));
			dsD.axes.rightY.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[6]));

			dsD.axes.l2.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[15]));
			dsD.axes.r2.setValue(FUSIN_UNSIGNED_RANGE_BYTE(inputReport[16]));
		}

		if (gyroReport)
		{
			dsD.motion.gyro.pitchAxis.setValue((float)gyroReport[0] / 32768);
			dsD.motion.gyro.yawAxis.setValue((float)gyroReport[1] / 32768);
			dsD.motion.gyro.rollAxis.setValue((float)gyroReport[2] / 32768);
		}
		if (accelReport)
		{
			dsD.motion.acceleration.xAxis.setValue(-(float)accelReport[0] / 8192);
			dsD.motion.acceleration.zAxis.setValue(-(float)accelReport[1] / 8192);
			dsD.motion.acceleration.yAxis.setValue(-(float)accelReport[2] / 8192);
		}
		/*if (extraReport)
		{*/
			/*dsD.mPhone = extraReport[0] & (1 << 6));
			dsD.mMic = extraReport[0] & (1 << 5));
			//dsD.mUsb = extraReport[0] & (1 << 5));*/
			/*dsD._setCharging(extraReport[0] & (1 << 4));
			dsD._setBattery((extraReport[0] & 0x0f) * 10);
		}*/
	}

	void RawInputDS3Handler::handleOutputReport(PBYTE pReport)
	{
		/*
		Big thanks to sicksaxis library
		*/
		DSDevice& dsD = *static_cast<DSDevice*>(mDevice);

		pReport[0] = 0x52;
		pReport[1] = 0xff * (bool)dsD.getRightVibration();
		pReport[2] = dsD.getRightVibration() * 255; //fast motor
		pReport[3] = 0xff * (bool)dsD.getLeftVibration();
		pReport[4] = dsD.getLeftVibration() * 255; //slow  motor
		pReport[5] = 0xff; //Gyro
		pReport[6] = 0x80; //Gyro
		pReport[7] = 0x00;
		pReport[8] = 0x00;
		pReport[9] = dsD.getLEDs() << 1;// the second bit is used for the first led, etc
		/* LED_4 */
		pReport[10] = 0xff;
		pReport[11] = 0x27;
		pReport[12] = 0x10;
		pReport[13] = 0x00;
		pReport[14] = 0x32,
		/* LED_3 */
		pReport[15] = 0xff;
		pReport[16] = 0x27;
		pReport[17] = 0x10;
		pReport[18] = 0x00;
		pReport[19] = 0x32;
		/* LED_2 */
		pReport[20] = 0xff;
		pReport[21] = 0x27;
		pReport[22] = 0x10;
		pReport[23] = 0x00;
		pReport[24] = 0x32;
		/* LED_1 */
		pReport[25] = 0xff;
		pReport[26] = 0x27;
		pReport[27] = 0x10;
		pReport[28] = 0x00;
		pReport[29] = 0x32;
	}

}