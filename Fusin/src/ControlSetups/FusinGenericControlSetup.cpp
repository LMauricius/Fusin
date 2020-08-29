#include "FusinGenericControlSetup.h"
#include "FusinInputGesture.h"
#include "FusinKeyboardDevice.h"
#include "FusinMouseDevice.h"
#include "FusinGamepadDevice.h"
#include "FusinXInputDevice.h"
#include "FusinDSDevice.h"
#include "FusinNintendoDevice.h"
#include "FusinInputManager.h"
#include "FusinXInput.h"
#include "FusinDS.h"

#define STICK_ACTIONS &lStick, &rStick
#define BUTTON_ACTIONS &dpadUp, &dpadDown, &dpadLeft, &dpadRight, &face1, &face2, &face3, &face4, &start, &select, &select,\
					  &shoulderL1, &shoulderR1, &shoulderL2, &shoulderR2, &lStickPress, &rStickPress,\
					  &ok, &cancel, &up, &down, &left, &right
#define ALL_MEMBER_ACTIONS STICK_ACTIONS, BUTTON_ACTIONS
#define ALL_BASIC_ACTIONS &lStick.xAxis, &lStick.yAxis, &lStick.directionAxis,\
						  &lStick.leftDirection, &lStick.rightDirection, &lStick.upDirection, &lStick.downDirection,\
						  &rStick.xAxis, &rStick.yAxis, &rStick.directionAxis,\
						  &rStick.leftDirection, &rStick.rightDirection, &rStick.upDirection, &rStick.downDirection,BUTTON_ACTIONS

#define FOR_DEVICES(EXP) if (mEnabledInputTypes & IO_KEYBOARD) mInputManager->getKeyboardDevice(mDeviceIndices[IO_KEYBOARD])->EXP;\
						 if (mEnabledInputTypes & IO_MOUSE) mInputManager->getMouseDevice(mDeviceIndices[IO_MOUSE])->EXP;\
						 if (mEnabledInputTypes & IO_GAMEPAD) mInputManager->getGamepadDevice(mDeviceIndices[IO_GAMEPAD])->EXP;\
						 if (mEnabledInputTypes & IO_XInput) mInputManager->getXInputDevice(mDeviceIndices[IO_XInput])->EXP;\
						 if (mEnabledInputTypes & IO_DS) mInputManager->getDSDevice(mDeviceIndices[IO_DS])->EXP;\
						 if (mEnabledInputTypes & IO_NINTENDO) mInputManager->getNintendoDevice(mDeviceIndices[IO_NINTENDO])->EXP;

#define FOR_GAMEPAD_DEVICES(EXP) if (mEnabledInputTypes & IO_GAMEPAD) mInputManager->getGamepadDevice(mDeviceIndices[IO_GAMEPAD])->EXP;\
						 if (mEnabledInputTypes & IO_XInput) mInputManager->getXInputDevice(mDeviceIndices[IO_XInput])->EXP;\
						 if (mEnabledInputTypes & IO_DS) mInputManager->getDSDevice(mDeviceIndices[IO_DS])->EXP;\
						 if (mEnabledInputTypes & IO_NINTENDO) mInputManager->getNintendoDevice(mDeviceIndices[IO_NINTENDO])->EXP;

namespace Fusin
{

	GenericControlSetup::GenericControlSetup(InputManager* im)
		: mInputManager(im)
		, lStick(im)
		, rStick(im)
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
		, lStickPress(im)
		, rStickPress(im)
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
			it->setDeadZone(0.4);
		}
		for (auto it : { BUTTON_ACTIONS })
		{
			any.trackCommand(it);
			anyButton.trackCommand(it);
		}

		mDeviceIndices[IO_KEYBOARD] = 0;
		mDeviceIndices[IO_MOUSE] = 0;
		mDeviceIndices[IO_GAMEPAD] = 0;
		mDeviceIndices[IO_XInput] = 0;
		mDeviceIndices[IO_DS] = 0;
	}

	void GenericControlSetup::setInputManager(InputManager* im)
	{
		Command *actions[]{ ALL_MEMBER_ACTIONS };

		for (auto it : actions)
		{
			it->setInputManager(im);
		}

		mInputManager = im;
	}

	void GenericControlSetup::setDeviceIndex(unsigned int ind, IOType t)
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
			if ((*it).first & t) (*it).second = ind;
		}
	}

	unsigned int GenericControlSetup::getDeviceIndex(IOType t)
	{
		for (auto it = mDeviceIndices.begin(); it != mDeviceIndices.end(); it++)
		{
			if ((*it).first & t) return (*it).second;
		}
	}

	void GenericControlSetup::setEnabledInputTypes(IOType t)
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

	IOType GenericControlSetup::getEnabledInputTypes()
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
		lStickPress.assignIOCode(XINPUT_LSTICK, slot);
		rStickPress.assignIOCode(XINPUT_RSTICK, slot);
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
		FOR_GAMEPAD_DEVICES(setLeftVibration(strength, msDuration))
	}

	void GenericControlSetup::setRightVibration(float strength, unsigned int msDuration)
	{
		FOR_GAMEPAD_DEVICES(setRightVibration(strength, msDuration))
	}

	void GenericControlSetup::setVibration(float strength, unsigned int msDuration)
	{
		FOR_GAMEPAD_DEVICES(setVibration(strength, msDuration))
	}

	void GenericControlSetup::setColor(ColorRGB c)
	{
		if (mEnabledInputTypes & IO_DS) mInputManager->getDSDevice(mDeviceIndices[IO_DS])->setColor(c);
		//if (mEnabledInputTypes & IO_DS) mInputManager->getDSDevice(mDeviceIndices[IO_DS]);
	}

}