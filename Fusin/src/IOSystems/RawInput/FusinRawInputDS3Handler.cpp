#ifdef FUSIN_BUILD_RAW_INPUT

#include "IOSystems/RawInput/FusinRawInputDS3Handler.h"
#include "Devices/FusinDSDevice.h"
#include "IOCodes/FusinDS.h"
#include "Utilities/FusinLog.h"
#include "Utilities/FusinByteDebug.h"
#include "Utilities/FusinBitCast.h"
#include <algorithm>

namespace Fusin
{

	RawInputDS3Handler::RawInputDS3Handler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo)
		: RawInputReportHandler(riDeviceHandle, riDeviceInfo, true)
	{
	}

	RawInputDS3Handler::~RawInputDS3Handler()
	{

	}
	
	bool RawInputDS3Handler::initialize()
	{
		if (!RawInputDeviceHandler::initialize()) return false;

		// Create the device
		mFusinDevice = new DSDevice(mProductName, true);
		mFusinDevice->_setConnectionMode(mInputReportLength == 64 ? CM_USB : CM_BT);

		Log::singleton() << "DS3 Device found: " << mProductName << "\n";

		return true;
	}

	void RawInputDS3Handler::handleInputReport(PBYTE pReport)
	{
		/*
		Big thanks to sicksaxis library
		*/
		DSDevice& dsD = *static_cast<DSDevice*>(mFusinDevice);
		PBYTE inputReport = nullptr;
		PBYTE extraReport = nullptr;
		PSHORT accelReport = nullptr;
		PSHORT gyroReport = nullptr;

		inputReport = pReport + 2;
		accelReport = reinterpret_cast<PSHORT>(pReport + 40);
		gyroReport = reinterpret_cast<PSHORT>(pReport + 46);
		extraReport = pReport + 30;

		if (inputReport)
		{
			if ((inputReport[0] >> 4) == 0)
			{
				dsD.dPad.angle.setValue(0);
			}
			else
			{
				float h = (float)getFlag(inputReport[0], 5) - (float)getFlag(inputReport[0], 7);
				float v = (float)getFlag(inputReport[0], 4) - (float)getFlag(inputReport[0], 6);
				float a = (float)(std::atan2(v, -h)*4.0f*PI);
				dsD.dPad.angle.setValue((a == 0.0f) ? 360.0f :
					(a * 45.0f));
			}
			dsD.buttonOptions.setValue(getFlag(inputReport[0], 3));
			dsD.buttonR3.setValue(getFlag(inputReport[0], 2));
			dsD.buttonL3.setValue(getFlag(inputReport[0], 1));
			dsD.buttonTouchPad.setValue(getFlag(inputReport[0], 0));

			dsD.buttonSquare.setValue(getFlag(inputReport[1], 7));
			dsD.buttonCross.setValue(getFlag(inputReport[1], 6));
			dsD.buttonCircle.setValue(getFlag(inputReport[1], 5));
			dsD.buttonTriangle.setValue(getFlag(inputReport[1], 4));

			dsD.buttonPS.setValue(getFlag(inputReport[2], 0));

			dsD.axisLeftStickX.setValue(fromBitRange8U(inputReport[3]));
			dsD.axisLeftStickY.setValue(fromBitRange8U(inputReport[4]));
			dsD.axisRightStickX.setValue(fromBitRange8U(inputReport[5]));
			dsD.axisRightStickY.setValue(fromBitRange8U(inputReport[6]));

			dsD.axisL2.setValue(fromBitRange8U(inputReport[15]));
			dsD.axisR2.setValue(fromBitRange8U(inputReport[16]));
		}

		if (gyroReport)
		{
			dsD.motion.gyroPitch.setValue((float)gyroReport[0] / 32768);
			dsD.motion.gyroYaw.setValue((float)gyroReport[1] / 32768);
			dsD.motion.gyroRoll.setValue((float)gyroReport[2] / 32768);
		}
		if (accelReport)
		{
			dsD.motion.accelerationX.setValue(-(float)accelReport[0] / 8192);
			dsD.motion.accelerationZ.setValue(-(float)accelReport[1] / 8192);
			dsD.motion.accelerationY.setValue(-(float)accelReport[2] / 8192);
		}
		if (extraReport)
		{
			/*dsD.mPhone = extraReport[0] & (1 << 6));
			dsD.mMic = extraReport[0] & (1 << 5));
			//dsD.mUsb = extraReport[0] & (1 << 5));*/
			/*dsD.battery.charging.setValue(extraReport[0] & (1 << 4));
			dsD.battery.energy.setValue((extraReport[0] & 0x0f) * 10);*/
		}
	}

	void RawInputDS3Handler::handleOutputReport(PBYTE pReport)
	{
		/*
		Big thanks to sicksaxis library
		*/
		DSDevice& dsD = *static_cast<DSDevice*>(mFusinDevice);

		// auto set leds
		if (dsD.leds.useDefaults())
		{
			int ledInd = mDeviceIndex-1;
			dsD.leds.setLEDFlags(1 << ledInd);
		}

		pReport[0] = 0x52;
		pReport[1] = 0xff * (bool)dsD.vibration.rightForce.value();
		pReport[2] = (BYTE)(dsD.vibration.rightForce.value() * 255); //fast motor
		pReport[3] = 0xff * (bool)dsD.vibration.leftForce.value();
		pReport[4] = (BYTE)(dsD.vibration.leftForce.value() * 255); //slow  motor
		pReport[5] = 0xff; //Gyro
		pReport[6] = 0x80; //Gyro
		pReport[7] = 0x00;
		pReport[8] = 0x00;
		pReport[9] = dsD.leds.ledFlags() << 1;// the second bit is used for the first led, etc
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

#endif // FUSIN_BUILD_RAW_INPUT