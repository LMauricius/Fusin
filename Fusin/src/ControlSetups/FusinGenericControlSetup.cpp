#include "ControlSetups/FusinGenericControlSetup.h"

#include "Commands/FusinInputCommand.h"
#include "Devices/FusinKeyboardDevice.h"
#include "Devices/FusinMouseDevice.h"
#include "Devices/FusinGamepadDevice.h"
#include "Devices/FusinXInputDevice.h"
#include "Devices/FusinDSDevice.h"
#include "Devices/FusinNintendoDevice.h"
#include "FusinInputManager.h"
#include "IOCodes/FusinXInput.h"
#include "IOCodes/FusinDS.h"

#define STICK_ACTIONS &lStick, &rStick
#define BUTTON_ACTIONS &dpadUp, &dpadDown, &dpadLeft, &dpadRight, &face1, &face2, &face3, &face4, &start, &select, &select,\
					  &shoulderL1, &shoulderR1, &shoulderL2, &shoulderR2, &lStickPress, &rStickPress,\
					  &ok, &cancel, &up, &down, &left, &right
#define ALL_MEMBER_ACTIONS STICK_ACTIONS, BUTTON_ACTIONS
#define ALL_BASIC_ACTIONS &lStick.xAxis, &lStick.yAxis, &lStick.angleAxis,\
						  &lStick.leftDirection, &lStick.rightDirection, &lStick.upDirection, &lStick.downDirection,\
						  &rStick.xAxis, &rStick.yAxis, &rStick.angleAxis,\
						  &rStick.leftDirection, &rStick.rightDirection, &rStick.upDirection, &rStick.downDirection,BUTTON_ACTIONS

#define FOR_DEVICES(EXP) if (mDeviceIndices[DT_KEYBOARD] > 0) mInputManager->getKeyboardDevice(mDeviceIndices[DT_KEYBOARD])->EXP;\
						 if (mDeviceIndices[DT_MOUSE] > 0) mInputManager->getMouseDevice(mDeviceIndices[DT_MOUSE])->EXP;\
						 if (mDeviceIndices[DT_GAMEPAD] > 0) mInputManager->getGamepadDevice(mDeviceIndices[DT_GAMEPAD])->EXP;\
						 if (mDeviceIndices[DT_XINPUT] > 0) mInputManager->getXInputDevice(mDeviceIndices[DT_XINPUT])->EXP;\
						 if (mDeviceIndices[DT_DUALSHOCK] > 0) mInputManager->getDSDevice(mDeviceIndices[DT_DUALSHOCK])->EXP;\
						 if (mDeviceIndices[DT_NINTENDO] > 0) mInputManager->getNintendoDevice(mDeviceIndices[DT_NINTENDO])->EXP;

#define FOR_GAMEPAD_DEVICES(EXP) if (mDeviceIndices[DT_GAMEPAD] > 0) mInputManager->getGamepadDevice(mDeviceIndices[DT_GAMEPAD])->EXP;\
						 if (mDeviceIndices[DT_XINPUT] > 0) mInputManager->getXInputDevice(mDeviceIndices[DT_XINPUT])->EXP;\
						 if (mDeviceIndices[DT_DUALSHOCK] > 0) mInputManager->getDSDevice(mDeviceIndices[DT_DUALSHOCK])->EXP;\
						 if (mDeviceIndices[DT_NINTENDO] > 0) mInputManager->getNintendoDevice(mDeviceIndices[DT_NINTENDO])->EXP;

namespace Fusin
{

	GenericControlSetup::GenericControlSetup(InputManager* im)
		: mInputManager(im)
		, lStickPress(im)
		, rStickPress(im)
		, dpadUp(im)
		, dpadDown(im)
		, dpadLeft(im)
		, dpadRight(im)
		, face1(im)
		, face2(im)
		, face3(im)
		, face4(im)
		, start(im)
		, select(im)
		, shoulderL1(im)
		, shoulderR1(im)
		, shoulderL2(im)
		, shoulderR2(im)
		, ok(im)
		, cancel(im)
		, up(im)
		, down(im)
		, left(im)
		, right(im)
	{
		for (auto it : { STICK_ACTIONS })
		{
			any.trackCommand(it);
			it->setDeadZone(0.4f);
		}
		for (auto it : { BUTTON_ACTIONS })
		{
			any.trackCommand(it);
			anyButton.trackCommand(it);
		}

		mDeviceIndices[DT_KEYBOARD] = 0;
		mDeviceIndices[DT_MOUSE] = 0;
		mDeviceIndices[DT_GAMEPAD] = 0;
		mDeviceIndices[DT_XINPUT] = 0;
		mDeviceIndices[DT_DUALSHOCK] = 0;
	}

	void GenericControlSetup::setInputManager(InputManager* im)
	{
		Command *actions[]{ ALL_MEMBER_ACTIONS };

		for (auto it : actions)
		{
			it->setDeviceEnumerator(im);
		}

		mInputManager = im;
	}

	void GenericControlSetup::setDeviceIndex(int ind, DeviceType t)
	{
		for (auto it : { STICK_ACTIONS })
		{
			it->setDeviceIndex(ind, t);
		}
		for (auto it : { BUTTON_ACTIONS })
		{
			it->setDeviceIndex(ind, t);
		}

		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first == t) (*it).second = ind;
		}
	}

	int GenericControlSetup::getDeviceIndex(DeviceType t)
	{
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
		return -1;
	}

	void GenericControlSetup::setEnabledInputTypes(IOFlags t)
	{
		for (auto it : { STICK_ACTIONS })
		{
			it->setEnabledInputTypes(t);
		}
		for (auto it : { BUTTON_ACTIONS })
		{
			it->setEnabledInputTypes(t);
		}

		mEnabledInputTypes = t;
	}

	IOFlags GenericControlSetup::getEnabledInputTypes()
	{
		return mEnabledInputTypes;
	}

	void GenericControlSetup::setDefaultXInputCodes(int slot)
	{
		lStick.xAxis.assignIOCode(XINPUT_LEFT_X_AXIS, slot);
		lStick.yAxis.assignIOCode(XINPUT_LEFT_Y_AXIS, slot);
		rStick.xAxis.assignIOCode(XINPUT_RIGHT_X_AXIS, slot);
		rStick.yAxis.assignIOCode(XINPUT_RIGHT_Y_AXIS, slot);
		dpadUp.assignIOCode(XINPUT_DPAD_UP, slot);
		dpadDown.assignIOCode(XINPUT_DPAD_DOWN, slot);
		dpadLeft.assignIOCode(XINPUT_DPAD_LEFT, slot);
		dpadRight.assignIOCode(XINPUT_DPAD_RIGHT, slot);
		face1.assignIOCode(XINPUT_A, slot);
		face2.assignIOCode(XINPUT_B, slot);
		face3.assignIOCode(XINPUT_X, slot);
		face4.assignIOCode(XINPUT_Y, slot);
		start.assignIOCode(XINPUT_MENU, slot);
		select.assignIOCode(XINPUT_VIEW, slot);
		shoulderL1.assignIOCode(XINPUT_LB, slot);
		shoulderR1.assignIOCode(XINPUT_RB, slot);
		shoulderL2.assignIOCode(XINPUT_LT, slot);
		shoulderR2.assignIOCode(XINPUT_RT, slot);
		lStickPress.assignIOCode(XINPUT_LEFT_STICK_PRESS, slot);
		rStickPress.assignIOCode(XINPUT_RIGHT_STICK_PRESS, slot);
		ok.assignIOCode(XINPUT_A, slot);
		cancel.assignIOCode(XINPUT_B, slot);
		up.assignIOCode(XINPUT_DPAD_UP, slot);
		down.assignIOCode(XINPUT_DPAD_DOWN, slot);
		left.assignIOCode(XINPUT_DPAD_LEFT, slot);
		right.assignIOCode(XINPUT_DPAD_RIGHT, slot);
		up.assignIOCode(XINPUT_LEFT_STICK_UP, slot);
		down.assignIOCode(XINPUT_LEFT_STICK_DOWN, slot);
		left.assignIOCode(XINPUT_LEFT_STICK_LEFT, slot);
		right.assignIOCode(XINPUT_LEFT_STICK_RIGHT, slot);
	}

	void GenericControlSetup::setDefaultDSCodes(int slot)
	{
		lStick.xAxis.assignIOCode(DS_LEFT_X_AXIS, slot);
		lStick.yAxis.assignIOCode(DS_LEFT_Y_AXIS, slot);
		rStick.xAxis.assignIOCode(DS_RIGHT_X_AXIS, slot);
		rStick.yAxis.assignIOCode(DS_RIGHT_Y_AXIS, slot);
		dpadUp.assignIOCode(DS_DPAD_UP, slot);
		dpadDown.assignIOCode(DS_DPAD_DOWN, slot);
		dpadLeft.assignIOCode(DS_DPAD_LEFT, slot);
		dpadRight.assignIOCode(DS_DPAD_RIGHT, slot);
		face1.assignIOCode(DS_CROSS, slot);
		face2.assignIOCode(DS_CIRCLE, slot);
		face3.assignIOCode(DS_SQUARE, slot);
		face4.assignIOCode(DS_TRIANGLE, slot);
		start.assignIOCode(DS_OPTIONS, slot);
		select.assignIOCode(DS_TOUCHPAD_BUTTON, slot);
		shoulderL1.assignIOCode(DS_L1, slot);
		shoulderR1.assignIOCode(DS_R1, slot);
		shoulderL2.assignIOCode(DS_L2, slot);
		shoulderR2.assignIOCode(DS_R2, slot);
		lStickPress.assignIOCode(DS_L3, slot);
		rStickPress.assignIOCode(DS_R3, slot);
		ok.assignIOCode(DS_CROSS, slot);
		cancel.assignIOCode(DS_CIRCLE, slot);
		up.assignIOCode(DS_DPAD_UP, slot);
		down.assignIOCode(DS_DPAD_DOWN, slot);
		left.assignIOCode(DS_DPAD_LEFT, slot);
		right.assignIOCode(DS_DPAD_RIGHT, slot);
		up.assignIOCode(DS_LEFT_STICK_UP, slot);
		down.assignIOCode(DS_LEFT_STICK_DOWN, slot);
		left.assignIOCode(DS_LEFT_STICK_LEFT, slot);
		right.assignIOCode(DS_LEFT_STICK_RIGHT, slot);
	}

	void GenericControlSetup::assign(int position, const GenericControlSetup& source, unsigned int first, unsigned int last)
	{
		for (auto it : { ALL_BASIC_ACTIONS })
		{
			it->assign(position, *(InputCommand*)((char*)it - (char*)this + (char*)(&source)), first, last);
		}
	}

	void GenericControlSetup::setLeftVibration(float strength, unsigned int msDuration)
	{
		FOR_GAMEPAD_DEVICES(vibration.setLeftVibration(strength, msDuration))
	}

	void GenericControlSetup::setRightVibration(float strength, unsigned int msDuration)
	{
		FOR_GAMEPAD_DEVICES(vibration.setRightVibration(strength, msDuration))
	}

	void GenericControlSetup::setVibration(float strength, unsigned int msDuration)
	{
		FOR_GAMEPAD_DEVICES(vibration.setVibration(strength, msDuration))
	}

	void GenericControlSetup::setColor(ColorRGB c)
	{
		mInputManager->getDSDevice(mDeviceIndices[DT_DUALSHOCK])->rgb.setColor(c);
		//if (mEnabledInputTypes & IO_DS) mInputManager->getDSDevice(mDeviceIndices[IO_DS]);
	}

}