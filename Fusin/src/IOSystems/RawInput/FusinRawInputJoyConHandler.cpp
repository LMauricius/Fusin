#include "IOSystems/RawInput/FusinRawInputJoyConHandler.h"
#include "FusinNintendoDevice.h"
#include "FusinNintendo.h"
#include "Utilities/FusinLog.h"
#include "FusinByteDebug.h"
#include <algorithm>
#include <math.h>
#include <iostream>

namespace Fusin
{

	RawInputJoyConHandler::RawInputJoyConHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, bool rightSide)
		: RawInputReportHandler(riDeviceHandle, riDeviceInfo, true, new NintendoDevice(NDT_JOYCON))
	{
		if (!mSuccess) return;

		//jcDevice->_setConnectionMode(mInputReportLength == 64 ? ConnectionMode::USB : ConnectionMode::BT);
		mDevice->_setName(mProductName);
		((NintendoDevice*)mDevice)->_setJoyConSide(rightSide ? JC_RIGHT : JC_LEFT);

		PBYTE pReport = new BYTE[mOutputReportLength];

		Log::singleton() << "JoyCon Device found: " << mProductName << "\n" <<
			"    " << (rightSide? "Right-Handed" : "Left-Handed") << "\n";
	}

	RawInputJoyConHandler::~RawInputJoyConHandler()
	{

	}

	void RawInputJoyConHandler::handleInputReport(PBYTE pReport)
	{
		NintendoDevice& jcD = *static_cast<NintendoDevice*>(mDevice);
		PBYTE inputReport = nullptr;
		PBYTE extraReport = nullptr;
		PSHORT accelReport = nullptr;
		PSHORT gyroReport = nullptr;

		if (//pReport[0] == 0x21 //||
			pReport[0] == 0x30 //||
			//pReport[0] == 0x31// ||
			//pReport[0] == 0x32 ||
			/*pReport[0] == 0x33*/)
		{
			BYTE id = pReport[0];
			inputReport = pReport + 3;
			extraReport = pReport + 2;
			accelReport = reinterpret_cast<PSHORT>(pReport + 13);
			gyroReport = reinterpret_cast<PSHORT>(pReport + 19);

			//std::wcout << byteStringCondensed(pReport, 80) << "\n";
		}
		if (pReport[0] == 0x30)
		{
			accelReport = reinterpret_cast<PSHORT>(pReport + 13);
			gyroReport = reinterpret_cast<PSHORT>(pReport + 19);
			//std::wcout << byteStringCondensed(pReport, 80) << "\n";
		}

		if (inputReport)
		{
			if (inputReport[0] & (1 << 0))
			{
				std::cout << "Y";
			}
			if (inputReport[0] & (1 << 1))
			{
				std::cout << "X";
			}
			if (inputReport[0] & (1 << 2))
			{
				std::cout << "B";
			}
			if (inputReport[0] & (1 << 3))
			{
				std::cout << "A";
			}
			jcD.buttons.y.setValue(inputReport[0] & (1 << 0));
			jcD.buttons.x.setValue(inputReport[0] & (1 << 1));
			jcD.buttons.b.setValue(inputReport[0] & (1 << 2));
			jcD.buttons.a.setValue(inputReport[0] & (1 << 3));
			jcD.buttons.sr.setValue((inputReport[0] & (1 << 4)) || (inputReport[2] & (1 << 4)));// Uses byte 0 if right, 2 if left
			jcD.buttons.sl.setValue((inputReport[0] & (1 << 5)) || (inputReport[2] & (1 << 5)));
			/*jcD.buttons.rightSR.setValue(NINTENDO_RIGHT_SR, inputReport[0] & (1 << 4));// Uses byte 0 if right, 2 if left
			jcD.buttons.rightSL.setValue(NINTENDO_RIGHT_SL, inputReport[0] & (1 << 5));
			jcD.buttons.leftSR.setValue(NINTENDO_LEFT_SR, (inputReport[2] & (1 << 4));// Uses byte 0 if right, 2 if left
			jcD.buttons.leftSL.setValue(NINTENDO_LEFT_SL, (inputReport[2] & (1 << 5));*/
			jcD.buttons.r.setValue(inputReport[0] & (1 << 6));
			jcD.buttons.zr.setValue(inputReport[0] & (1 << 7));

			jcD.buttons.minus.setValue(inputReport[1] & (1 << 0));
			jcD.buttons.plus.setValue(inputReport[1] & (1 << 1));
			jcD.buttons.rightStick.setValue(inputReport[1] & (1 << 2));
			jcD.buttons.leftStick.setValue(inputReport[1] & (1 << 3));
			jcD.buttons.home.setValue(inputReport[1] & (1 << 4));// Uses byte 0 if right, 2 if left
			jcD.buttons.capture.setValue(inputReport[1] & (1 << 5));
			jcD._setJoyConChargingGrip(inputReport[1] & (1 << 7));

			/*jcD.setButtonState(NINTENDO_DPAD_DOWN, (inputReport[2] & (1 << 0)));
			jcD.setButtonState(NINTENDO_DPAD_UP, (inputReport[2] & (1 << 1)));
			jcD.setButtonState(NINTENDO_DPAD_RIGHT, (inputReport[2] & (1 << 2)));
			jcD.setButtonState(NINTENDO_DPAD_LEFT, (inputReport[2] & (1 << 3)));*/
			jcD.buttons.l.setValue(inputReport[2] & (1 << 6));
			jcD.buttons.zl.setValue(inputReport[2] & (1 << 7));
			if ((float)(bool)(inputReport[2] & (1 << 0)) - (float)(bool)(inputReport[2] & (1 << 1)) == 0 &&
				(float)(bool)(inputReport[2] & (1 << 2)) - (float)(bool)(inputReport[2] & (1 << 3)) == 0)
			{
				jcD.dPad.angle.setValue(0);
			}
			else
			{
				float h = (float)(bool)(inputReport[2] & (1 << 2)) - (float)(bool)(inputReport[2] & (1 << 3));
				float v = (float)(bool)(inputReport[2] & (1 << 0)) - (float)(bool)(inputReport[2] & (1 << 1));
				float a = std::atan2(h, -v)*180.0f/PI;
				if (a <= 0.0f) a += 360.0f;
				jcD.dPad.angle.setValue(a);
			}

			jcD.axes.leftX.setValue(FUSIN_SIGNED_RANGE_12BIT(inputReport[3] | ((inputReport[4] & 0xF) << 8))*2.0f);
			//jcD.axes.leftX.setValue((inputReport[3] | ((inputReport[4] & 0xF) << 8)));
			unsigned int lxint;
			std::wcout << byteStringCondensed(&lxint, 4) << " " << bitString(&lxint, 4) << "\n";

			jcD.axes.leftY.setValue(FUSIN_SIGNED_RANGE_12BIT((inputReport[4] >> 4) | (inputReport[5] << 4))*2.0f);
			jcD.axes.rightX.setValue(FUSIN_SIGNED_RANGE_12BIT(inputReport[6] | ((inputReport[7] & 0xF) << 8))*2.0f);
			jcD.axes.rightY.setValue(FUSIN_SIGNED_RANGE_12BIT((inputReport[7] >> 4) | (inputReport[8] << 4))*2.0f);
		}

		if (accelReport)
		{
			float x = (float)accelReport[0] * 0.000244f;
			float y = (float)accelReport[1] * 0.000244f;
			float z = (float)accelReport[2] * 0.000244f;
			jcD.motion.acceleration.xAxis.setValue(x);
			jcD.motion.acceleration.yAxis.setValue(y);
			jcD.motion.acceleration.zAxis.setValue(z);
			//std::cout << x << " " << y << " " << z << "\n";
		}
		if (gyroReport)
		{
			float x = (float)gyroReport[0] * 0.06103f;
			float y = (float)gyroReport[1] * 0.06103f;
			float z = (float)gyroReport[2] * 0.06103f;
			//vertical
			if (jcD.joyConPair())
			{
				//left
				if (jcD.joyConSide() == JC_LEFT)
				{
					jcD.motion.gyro.yawAxis.setValue(-z);
					jcD.motion.gyro.pitchAxis.setValue(-x);
					jcD.motion.gyro.rollAxis.setValue(-y);
				}
				//right	
				else
				{
					jcD.motion.gyro.yawAxis.setValue(z);
					jcD.motion.gyro.pitchAxis.setValue(x);
					jcD.motion.gyro.rollAxis.setValue(y);
				}
			}
			else//horizontal
			{
				//left
				if (jcD.joyConSide() == JC_LEFT)
				{
					jcD.motion.gyro.yawAxis.setValue(-z);
					jcD.motion.gyro.pitchAxis.setValue(-y);
					jcD.motion.gyro.rollAxis.setValue(-x);
				}
				//right
				else
				{
					jcD.motion.gyro.yawAxis.setValue(z);
					jcD.motion.gyro.pitchAxis.setValue(y);
					jcD.motion.gyro.rollAxis.setValue(z);
				}
			}
		}
		if (extraReport)
		{
			/*jcD.mPhone = extraReport[0] & (1 << 6));
			jcD.mMic = extraReport[0] & (1 << 5));
			//jcD.mUsb = extraReport[0] & (1 << 5));*/
			jcD._setCharging(extraReport[0] & (1 << 4));
			switch ((extraReport[0] >> 5) << 1)
			{
			case 0:
				jcD._setBattery(BL_EMPTY);
				break;
			case 2:
				jcD._setBattery(BL_CRITICAL);
				break;
			case 4:
				jcD._setBattery(BL_LOW);
				break;
			case 6:
				jcD._setBattery(BL_MEDIUM);
				break;
			case 8:
				jcD._setBattery(BL_FULL);
				break;
			}
		}
	}

	void RawInputJoyConHandler::handleOutputReport(PBYTE pReport)
	{
		NintendoDevice& jcD = *static_cast<NintendoDevice*>(mDevice);
		DWORD bytesWritten;
		pReport[0] = 0x01;

		/*
		We need to send two commands to the joy con:
		one to enable full input mode, with full analog and button support,
		and the other to enable gyro detection.
		Since we need two output reports for the two commands, and only one is normally sent per _update, 
		we count the reports sent and choose the command depending on the counter.
		*/
		if (mUpdateCounter % 2 == 0)
		{
			pReport[10] = 0x03;// Set input mode
			pReport[11] = 0x30;// Set to full mode
		}
		else
		{
			pReport[10] = 0x40;// Enable gyro
			pReport[11] = 0x01;// 
		}

		/*
		pReport[0] = 0x01;
		pReport[1] = 0x03;
		pReport[2] = 0x08;
		pReport[3] = 0x00;
		pReport[4] = 0x92;
		pReport[5] = 0x00;
		pReport[6] = 0x01;
		pReport[7] = 0x00;
		pReport[8] = 0x00;
		pReport[9] = 0x69;
		pReport[10] = 0x2d;
		pReport[11] = 0x1f;*/
	}

}