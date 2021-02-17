#include "IOSystems/RawInput/FusinRawInputJoyConHandler.h"
#include "Devices/FusinNintendoDevice.h"
#include "IOCodes/FusinNintendo.h"
#include "Utilities/FusinLog.h"
#include "Utilities/FusinByteDebug.h"
#include "Utilities/FusinBitCast.h"
#include <algorithm>
#include <math.h>
#include <iostream>

namespace Fusin
{

	RawInputJoyConHandler::RawInputJoyConHandler(HANDLE riDeviceHandle, PRID_DEVICE_INFO riDeviceInfo, bool rightSide)
		: RawInputReportHandler(riDeviceHandle, riDeviceInfo, true)
		, mRightSide(rightSide)
	{
	}

	bool RawInputJoyConHandler::initialize()
	{
		if (!RawInputReportHandler::initialize()) return false;

		mFusinDevice = new NintendoDevice(mProductName, true);

		//jcDevice->_setConnectionMode(mInputReportLength == 64 ? ConnectionMode::USB : ConnectionMode::BT);
		//((NintendoDevice*)mFusinDevice)->_setJoyConSide(mRightSide ? JC_RIGHT : JC_LEFT);

		//PBYTE pReport = new BYTE[mOutputReportLength];

		Log::singleton() << "JoyCon Device found: " << mProductName << "\n" <<
			"    " << (mRightSide? "Right-Handed" : "Left-Handed") << "\n";

		return true;
	}

	RawInputJoyConHandler::~RawInputJoyConHandler()
	{

	}

	void RawInputJoyConHandler::handleInputReport(PBYTE pReport)
	{
		NintendoDevice& jcD = *static_cast<NintendoDevice*>(mFusinDevice);
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
			jcD.buttonY.setValue(inputReport[0] & (1 << 0));
			jcD.buttonX.setValue(inputReport[0] & (1 << 1));
			jcD.buttonB.setValue(inputReport[0] & (1 << 2));
			jcD.buttonA.setValue(inputReport[0] & (1 << 3));
			jcD.buttonSR.setValue((inputReport[0] & (1 << 4)) || (inputReport[2] & (1 << 4)));// Uses byte 0 if right, 2 if left
			jcD.buttonSL.setValue((inputReport[0] & (1 << 5)) || (inputReport[2] & (1 << 5)));
			/*jcD.buttonrightSR.setValue(NINTENDO_RIGHT_SR, inputReport[0] & (1 << 4));// Uses byte 0 if right, 2 if left
			jcD.buttonrightSL.setValue(NINTENDO_RIGHT_SL, inputReport[0] & (1 << 5));
			jcD.buttonleftSR.setValue(NINTENDO_LEFT_SR, (inputReport[2] & (1 << 4));// Uses byte 0 if right, 2 if left
			jcD.buttonleftSL.setValue(NINTENDO_LEFT_SL, (inputReport[2] & (1 << 5));*/
			jcD.buttonR.setValue(inputReport[0] & (1 << 6));
			jcD.buttonZR.setValue(inputReport[0] & (1 << 7));

			jcD.buttonMinus.setValue(inputReport[1] & (1 << 0));
			jcD.buttonPlus.setValue(inputReport[1] & (1 << 1));
			jcD.buttonRightStick.setValue(inputReport[1] & (1 << 2));
			jcD.buttonLeftStick.setValue(inputReport[1] & (1 << 3));
			jcD.buttonHome.setValue(inputReport[1] & (1 << 4));// Uses byte 0 if right, 2 if left
			jcD.buttonCapture.setValue(inputReport[1] & (1 << 5));
			//jcD.charg._setJoyConChargingGrip(inputReport[1] & (1 << 7));

			/*jcD.setButtonState(NINTENDO_DPAD_DOWN, (inputReport[2] & (1 << 0)));
			jcD.setButtonState(NINTENDO_DPAD_UP, (inputReport[2] & (1 << 1)));
			jcD.setButtonState(NINTENDO_DPAD_RIGHT, (inputReport[2] & (1 << 2)));
			jcD.setButtonState(NINTENDO_DPAD_LEFT, (inputReport[2] & (1 << 3)));*/
			jcD.buttonL.setValue(inputReport[2] & (1 << 6));
			jcD.buttonZL.setValue(inputReport[2] & (1 << 7));
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

			jcD.axisLeftStickX.setValue(fromBitRange12(inputReport[3] | ((inputReport[4] & 0xF) << 8))*2.0f);
			//jcD.axisleftX.setValue((inputReport[3] | ((inputReport[4] & 0xF) << 8)));
			unsigned int lxint;
			std::wcout << byteStringCondensed(&lxint, 4) << " " << bitString(&lxint, 4) << "\n";

			jcD.axisLeftStickY.setValue(fromBitRange12((inputReport[4] >> 4) | (inputReport[5] << 4))*2.0f);
			jcD.axisRightStickX.setValue(fromBitRange12(inputReport[6] | ((inputReport[7] & 0xF) << 8))*2.0f);
			jcD.axisRightStickY.setValue(fromBitRange12((inputReport[7] >> 4) | (inputReport[8] << 4))*2.0f);
		}

		if (accelReport)
		{
			float x = (float)accelReport[0] * 0.000244f;
			float y = (float)accelReport[1] * 0.000244f;
			float z = (float)accelReport[2] * 0.000244f;
			jcD.motion.accelerationX.setValue(x);
			jcD.motion.accelerationY.setValue(y);
			jcD.motion.accelerationZ.setValue(z);
			//std::cout << x << " " << y << " " << z << "\n";
		}
		if (gyroReport)
		{
			float x = (float)gyroReport[0] * 0.06103f;
			float y = (float)gyroReport[1] * 0.06103f;
			float z = (float)gyroReport[2] * 0.06103f;
			//vertical
			if (false)//(jcD.joyConPair())
			{
				//right	
				if (mRightSide)
				{
					jcD.motion.gyroYaw.setValue(z);
					jcD.motion.gyroPitch.setValue(x);
					jcD.motion.gyroRoll.setValue(y);
				}
				//left
				else
				{
					jcD.motion.gyroYaw.setValue(-z);
					jcD.motion.gyroPitch.setValue(-x);
					jcD.motion.gyroRoll.setValue(-y);
				}
			}
			else//horizontal
			{
				//right
				if (mRightSide)
				{
					jcD.motion.gyroYaw.setValue(z);
					jcD.motion.gyroPitch.setValue(y);
					jcD.motion.gyroRoll.setValue(z);
				}
				//left
				else
				{
					jcD.motion.gyroYaw.setValue(-z);
					jcD.motion.gyroPitch.setValue(-y);
					jcD.motion.gyroRoll.setValue(-x);
				}
			}
		}
		if (extraReport)
		{
			/*jcD.mPhone = extraReport[0] & (1 << 6));
			jcD.mMic = extraReport[0] & (1 << 5));
			//jcD.mUsb = extraReport[0] & (1 << 5));*/
			jcD.battery.charging.setValue((float)(extraReport[0] & (1 << 4)));
			switch ((extraReport[0] >> 5) << 1)
			{
			case 0:
				jcD.battery.energy.setValue(BAT_EMPTY);
				break;
			case 2:
				jcD.battery.energy.setValue(BAT_CRITICAL);
				break;
			case 4:
				jcD.battery.energy.setValue(BAT_LOW);
				break;
			case 6:
				jcD.battery.energy.setValue(BAT_MEDIUM);
				break;
			case 8:
				jcD.battery.energy.setValue(BAT_FULL);
				break;
			}
		}
	}

	void RawInputJoyConHandler::handleOutputReport(PBYTE pReport)
	{
		NintendoDevice& jcD = *static_cast<NintendoDevice*>(mFusinDevice);
		
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