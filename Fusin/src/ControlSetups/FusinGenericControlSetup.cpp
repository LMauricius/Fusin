#include "FusinGenericControlSetup.h"
#include "FusinInputGesture.h"
#include "FusinKeyboardDevice.h"
#include "FusinMouseDevice.h"
#include "FusinGamepadDevice.h"
#include "FusinXBoxDevice.h"
#include "FusinDSDevice.h"
#include "FusinNintendoDevice.h"
#include "FusinInputManager.h"
#include "FusinXBox.h"
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

#define FOR_DEVICES(EXP) if (mEnabledInputTypes & IT_KEYBOARD) mInputManager->getKeyboardDevice(mDeviceIndices[IT_KEYBOARD])->EXP;\
						 if (mEnabledInputTypes & IT_MOUSE) mInputManager->getMouseDevice(mDeviceIndices[IT_MOUSE])->EXP;\
						 if (mEnabledInputTypes & IT_GAMEPAD) mInputManager->getGamepadDevice(mDeviceIndices[IT_GAMEPAD])->EXP;\
						 if (mEnabledInputTypes & IT_XBOX) mInputManager->getXBoxDevice(mDeviceIndices[IT_XBOX])->EXP;\
						 if (mEnabledInputTypes & IT_DS) mInputManager->getDSDevice(mDeviceIndices[IT_DS])->EXP;\
						 if (mEnabledInputTypes & IT_NINTENDO) mInputManager->getNintendoDevice(mDeviceIndices[IT_NINTENDO])->EXP;

#define FOR_GAMEPAD_DEVICES(EXP) if (mEnabledInputTypes & IT_GAMEPAD) mInputManager->getGamepadDevice(mDeviceIndices[IT_GAMEPAD])->EXP;\
						 if (mEnabledInputTypes & IT_XBOX) mInputManager->getXBoxDevice(mDeviceIndices[IT_XBOX])->EXP;\
						 if (mEnabledInputTypes & IT_DS) mInputManager->getDSDevice(mDeviceIndices[IT_DS])->EXP;\
						 if (mEnabledInputTypes & IT_NINTENDO) mInputManager->getNintendoDevice(mDeviceIndices[IT_NINTENDO])->EXP;

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
			any.trackGesture(it);
			it->setDeadZone(0.4);
		}
		for (auto it : { BUTTON_ACTIONS })
		{
			any.trackGesture(it);
			anyButton.trackGesture(it);
		}

		mDeviceIndices[IT_KEYBOARD] = 0;
		mDeviceIndices[IT_MOUSE] = 0;
		mDeviceIndices[IT_GAMEPAD] = 0;
		mDeviceIndices[IT_XBOX] = 0;
		mDeviceIndices[IT_DS] = 0;
	}

	void GenericControlSetup::setInputManager(InputManager* im)
	{
		Gesture *actions[]{ ALL_MEMBER_ACTIONS };

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

	void GenericControlSetup::setDefaultXBoxCodes(int slot)
	{
		lStick.xAxis.assignInputCode(XBOX_LEFT_X_AXIS, slot);
		lStick.yAxis.assignInputCode(XBOX_LEFT_Y_AXIS, slot);
		rStick.xAxis.assignInputCode(XBOX_RIGHT_X_AXIS, slot);
		rStick.yAxis.assignInputCode(XBOX_RIGHT_Y_AXIS, slot);
		dpadUp.assignInputCode(XBOX_DPAD_UP, slot);
		dpadDown.assignInputCode(XBOX_DPAD_DOWN, slot);
		dpadLeft.assignInputCode(XBOX_DPAD_LEFT, slot);
		dpadRight.assignInputCode(XBOX_DPAD_RIGHT, slot);
		face1.assignInputCode(XBOX_A, slot);
		face2.assignInputCode(XBOX_B, slot);
		face3.assignInputCode(XBOX_X, slot);
		face4.assignInputCode(XBOX_Y, slot);
		start.assignInputCode(XBOX_MENU, slot);
		select.assignInputCode(XBOX_VIEW, slot);
		shoulderL1.assignInputCode(XBOX_LB, slot);
		shoulderR1.assignInputCode(XBOX_RB, slot);
		shoulderL2.assignInputCode(XBOX_LT, slot);
		shoulderR2.assignInputCode(XBOX_RT, slot);
		lStickPress.assignInputCode(XBOX_LSTICK, slot);
		rStickPress.assignInputCode(XBOX_RSTICK, slot);
		ok.assignInputCode(XBOX_A, slot);
		cancel.assignInputCode(XBOX_B, slot);
		up.assignInputCode(XBOX_DPAD_UP, slot);
		down.assignInputCode(XBOX_DPAD_DOWN, slot);
		left.assignInputCode(XBOX_DPAD_LEFT, slot);
		right.assignInputCode(XBOX_DPAD_RIGHT, slot);
		up.assignInputCode(XBOX_LEFT_STICK_UP, slot);
		down.assignInputCode(XBOX_LEFT_STICK_DOWN, slot);
		left.assignInputCode(XBOX_LEFT_STICK_LEFT, slot);
		right.assignInputCode(XBOX_LEFT_STICK_RIGHT, slot);
	}

	void GenericControlSetup::setDefaultDSCodes(int slot)
	{
		lStick.xAxis.assignInputCode(DS_LEFT_X_AXIS, slot);
		lStick.yAxis.assignInputCode(DS_LEFT_Y_AXIS, slot);
		rStick.xAxis.assignInputCode(DS_RIGHT_X_AXIS, slot);
		rStick.yAxis.assignInputCode(DS_RIGHT_Y_AXIS, slot);
		dpadUp.assignInputCode(DS_DPAD_UP, slot);
		dpadDown.assignInputCode(DS_DPAD_DOWN, slot);
		dpadLeft.assignInputCode(DS_DPAD_LEFT, slot);
		dpadRight.assignInputCode(DS_DPAD_RIGHT, slot);
		face1.assignInputCode(DS_CROSS, slot);
		face2.assignInputCode(DS_CIRCLE, slot);
		face3.assignInputCode(DS_SQUARE, slot);
		face4.assignInputCode(DS_TRIANGLE, slot);
		start.assignInputCode(DS_OPTIONS, slot);
		select.assignInputCode(DS_TOUCHPAD_BUTTON, slot);
		shoulderL1.assignInputCode(DS_L1, slot);
		shoulderR1.assignInputCode(DS_R1, slot);
		shoulderL2.assignInputCode(DS_L2, slot);
		shoulderR2.assignInputCode(DS_R2, slot);
		lStickPress.assignInputCode(DS_L3, slot);
		rStickPress.assignInputCode(DS_R3, slot);
		ok.assignInputCode(DS_CROSS, slot);
		cancel.assignInputCode(DS_CIRCLE, slot);
		up.assignInputCode(DS_DPAD_UP, slot);
		down.assignInputCode(DS_DPAD_DOWN, slot);
		left.assignInputCode(DS_DPAD_LEFT, slot);
		right.assignInputCode(DS_DPAD_RIGHT, slot);
		up.assignInputCode(DS_LEFT_STICK_UP, slot);
		down.assignInputCode(DS_LEFT_STICK_DOWN, slot);
		left.assignInputCode(DS_LEFT_STICK_LEFT, slot);
		right.assignInputCode(DS_LEFT_STICK_RIGHT, slot);
	}

	void GenericControlSetup::assign(int position, const GenericControlSetup& source, unsigned int first, unsigned int last)
	{
		for (auto it : { ALL_BASIC_ACTIONS })
		{
			it->assign(position, *(InputGesture*)((char*)it - (char*)this + (char*)(&source)), first, last);
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
		if (mEnabledInputTypes & IT_DS) mInputManager->getDSDevice(mDeviceIndices[IT_DS])->setColor(c);
		//if (mEnabledInputTypes & IT_DS) mInputManager->getDSDevice(mDeviceIndices[IT_DS]);
	}

}