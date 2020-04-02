#include "FusinNintendoDevice.h"
#include "FusinGesture.h"
#include "FusinNintendo.h"
#include "FusinInputManager.h"
#include "FusinInputSignalManipulation.h"
#include <iomanip>
#include <algorithm>

#define FOR_LISTENERS(EXP) for (auto& it : mDeviceListeners) {it->EXP;} \
						   for (auto& it : mGamepadDeviceListeners) {it->EXP;} \
						   for (auto& it : mNintendoDeviceListeners) {it->EXP;}
#define FOR_GAMEPAD_LISTENERS(EXP) for (auto& it : mGamepadDeviceListeners) {it->EXP;} for (auto& it : mNintendoDeviceListeners) {it->EXP;}
#define FOR_NINTENDO_LISTENERS(EXP) for (auto& it : mNintendoDeviceListeners) {it->EXP;}

namespace Fusin
{
	NintendoDevice::ButtonSignals::ButtonSignals()
		: a(NINTENDO_A)
		, b(NINTENDO_B)
		, x(NINTENDO_X)
		, y(NINTENDO_Y)
		, l(NINTENDO_L)
		, zl(NINTENDO_ZL)
		, r(NINTENDO_R)
		, zr(NINTENDO_ZR)
		, minus(NINTENDO_MINUS)
		, plus(NINTENDO_PLUS)
		, leftStick(NINTENDO_LSTICK)
		, rightStick(NINTENDO_RSTICK)
		, home(NINTENDO_HOME)
		, capture(NINTENDO_CAPTURE)
		, sl(NINTENDO_SL)
		, sr(NINTENDO_SR)
	{
	}
	NintendoDevice::AxisSignals::AxisSignals()
		: leftX(NINTENDO_LEFT_X_AXIS, &positiveLeftX, &negativeLeftX)
		, leftY(NINTENDO_LEFT_Y_AXIS, &positiveLeftY, &negativeLeftY)
		, rightX(NINTENDO_RIGHT_X_AXIS, &positiveRightX, &negativeRightX)
		, rightY(NINTENDO_RIGHT_Y_AXIS, &positiveRightY, &negativeRightY)
		, positiveLeftX(NINTENDO_LEFT_STICK_RIGHT)
		, negativeLeftX(NINTENDO_LEFT_STICK_LEFT)
		, positiveLeftY(NINTENDO_LEFT_STICK_DOWN)
		, negativeLeftY(NINTENDO_LEFT_STICK_UP)
		, positiveRightX(NINTENDO_RIGHT_STICK_RIGHT)
		, negativeRightX(NINTENDO_RIGHT_STICK_LEFT)
		, positiveRightY(NINTENDO_RIGHT_STICK_DOWN)
		, negativeRightY(NINTENDO_RIGHT_STICK_UP)
	{
	}

	NintendoDevice::NintendoDevice(NintendoDeviceType ninType, String name, size_t index)
		: GamepadDevice(NINTENDO_BUTTON_NUMBER, NINTENDO_AXIS_NUMBER, true, true, name, index)
		, mNintendoDeviceType(ninType)
	{
		mDeviceType = IT_NINTENDO;

		/*
		We use the cover system to connect the more readable NintendoDevice button signals ("buttons" structure)
		to the numbered GamepadDevice signals ("mButtons"). Same goes for the axes.
		*/
		buttons.a.coverInputSignal(&mButtons[NINTENDO_A.data]);
		buttons.b.coverInputSignal(&mButtons[NINTENDO_B.data]);
		buttons.x.coverInputSignal(&mButtons[NINTENDO_X.data]);
		buttons.y.coverInputSignal(&mButtons[NINTENDO_Y.data]);
		buttons.l.coverInputSignal(&mButtons[NINTENDO_L.data]);
		buttons.zl.coverInputSignal(&mButtons[NINTENDO_ZL.data]);
		buttons.r.coverInputSignal(&mButtons[NINTENDO_R.data]);
		buttons.zr.coverInputSignal(&mButtons[NINTENDO_ZR.data]);
		buttons.minus.coverInputSignal(&mButtons[NINTENDO_MINUS.data]);
		buttons.plus.coverInputSignal(&mButtons[NINTENDO_PLUS.data]);
		buttons.leftStick.coverInputSignal(&mButtons[NINTENDO_LSTICK.data]);
		buttons.rightStick.coverInputSignal(&mButtons[NINTENDO_RSTICK.data]);
		buttons.home.coverInputSignal(&mButtons[NINTENDO_HOME.data]);
		buttons.capture.coverInputSignal(&mButtons[NINTENDO_CAPTURE.data]);
		buttons.sl.coverInputSignal(&mButtons[NINTENDO_SL.data]);
		buttons.sr.coverInputSignal(&mButtons[NINTENDO_SR.data]);

		axes.leftX.coverInputSignal(&mAxes[NINTENDO_LEFT_X_AXIS.data]);
		axes.leftY.coverInputSignal(&mAxes[NINTENDO_LEFT_Y_AXIS.data]);
		axes.rightX.coverInputSignal(&mAxes[NINTENDO_RIGHT_X_AXIS.data]);
		axes.rightY.coverInputSignal(&mAxes[NINTENDO_RIGHT_Y_AXIS.data]);
		axes.positiveLeftX.coverInputSignal(&mAxes[NINTENDO_LEFT_STICK_RIGHT.data]);
		axes.negativeLeftX.coverInputSignal(&mAxes[NINTENDO_LEFT_STICK_LEFT.data]);
		axes.positiveLeftY.coverInputSignal(&mAxes[NINTENDO_LEFT_STICK_DOWN.data]);
		axes.negativeLeftY.coverInputSignal(&mAxes[NINTENDO_LEFT_STICK_UP.data]);
		axes.positiveRightX.coverInputSignal(&mAxes[NINTENDO_RIGHT_STICK_RIGHT.data]);
		axes.negativeRightX.coverInputSignal(&mAxes[NINTENDO_RIGHT_STICK_LEFT.data]);
		axes.positiveRightY.coverInputSignal(&mAxes[NINTENDO_RIGHT_STICK_DOWN.data]);
		axes.negativeRightY.coverInputSignal(&mAxes[NINTENDO_RIGHT_STICK_UP.data]);
	}

	NintendoDevice::~NintendoDevice()
	{
		FOR_NINTENDO_LISTENERS(nintendoDeviceDeleted(this))
	}

	void NintendoDevice::_update(size_t msElapsed)
	{
		mUpdating = true;
		FOR_LISTENERS(preUpdate(this));

		if (mVibrationLeft > 0.0f && mVibrationLeftDuration > 0)
		{
			mVibrationLeftDuration -= msElapsed;
			if (mVibrationLeftDuration <= 0)
			{
				FOR_GAMEPAD_LISTENERS(leftVibrationChanged(this, mVibrationLeft, mVibrationLeftDuration + msElapsed, 0.0f, 0));
				mVibrationLeft = 0.0f;
				mVibrationLeftDuration = 0;
			}
			FOR_GAMEPAD_LISTENERS(leftVibrationUpdated(this, mVibrationLeft, mVibrationLeftDuration));
		}
		if (mVibrationRight > 0.0f && mVibrationRightDuration > 0)
		{
			mVibrationRightDuration -= msElapsed;
			if (mVibrationRightDuration <= 0)
			{
				FOR_GAMEPAD_LISTENERS(rightVibrationChanged(this, mVibrationRight, mVibrationRightDuration + msElapsed, 0.0f, 0));
				mVibrationRight = 0.0f;
				mVibrationRightDuration = 0;
			}
			FOR_GAMEPAD_LISTENERS(rightVibrationUpdated(this, mVibrationRight, mVibrationRightDuration));
		}

		// read from covered devices
		for (auto dev : mCoveredDevices)
		{
			// for all buttons
			coverInputSignalVector(static_cast<NintendoDevice*>(dev)->mButtons, mButtons, IT_NINTENDO_BUTTON, &mInputSignalProxy);
			// for all axes
			coverInputSignalVector(static_cast<NintendoDevice*>(dev)->mAxes, mAxes, mNegativeAxes, mPositiveAxes,
				IT_NINTENDO_AXIS, &mInputSignalProxy);
			// dpad axis
			if (std::abs(static_cast<NintendoDevice*>(dev)->dPad.angle.nextValue()) >= std::abs(dPad.angle.nextValue()))
				dPad.angle.setValue(static_cast<NintendoDevice*>(dev)->dPad.angle.nextValue());
		}

		// _update signals
		updateInputSignalVector(mButtons);
		updateInputSignalVector(mAxes);
		dPad.angle.update();
		dPad.up.update();
		dPad.down.update();
		dPad.left.update();
		dPad.right.update();

		FOR_LISTENERS(postUpdate(this));
		mUpdating = false;
	}


	String NintendoDevice::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		ss << (mCharging ? L"Charging: " : L"Battery: ") << mBattery << " ";

		ss << "LX(" << axes.leftX.value() << ") ";
		ss << "LY(" << axes.leftY.value() << ") ";
		ss << "RX(" << axes.rightX.value() << ") ";
		ss << "RY(" << axes.rightY.value() << ") ";
		ss << "dPad(" << dPad.angle.value() << ") ";
		ss << motion.getStateString();

		ss << " Buttons: ";
		if (buttons.a.value())
			ss << "A ";
		if (buttons.b.value())
			ss << "B ";
		if (buttons.x.value())
			ss << "X ";
		if (buttons.y.value())
			ss << "Y ";
		if (buttons.l.value())
			ss << "L ";
		if (buttons.r.value())
			ss << "R ";
		if (buttons.zl.value())
			ss << "ZL ";
		if (buttons.zr.value())
			ss << "ZR ";
		if (buttons.leftStick.value())
			ss << "LStick ";
		if (buttons.rightStick.value())
			ss << "RStick ";
		if (buttons.minus.value())
			ss << "- ";
		if (buttons.plus.value())
			ss << "+ ";
		if (buttons.home.value())
			ss << "Home ";
		if (buttons.capture.value())
			ss << "Capture ";
		if (dPad.up.value())
			ss << "^ ";
		if (dPad.down.value())
			ss << "v ";
		if (dPad.left.value())
			ss << "< ";
		if (dPad.right.value())
			ss << "> ";

		return ss.str();
	}


	void NintendoDevice::addListener(NintendoDeviceListener* listener)
	{
		mNintendoDeviceListeners.push_back(listener);
	}

	void NintendoDevice::removeListener(NintendoDeviceListener* listener)
	{
		mNintendoDeviceListeners.remove(listener);
	}

	void NintendoDevice::_setCharging(bool charging)
	{
		mCharging = charging;
	}

	void NintendoDevice::_setJoyConSide(JoyConSide side)
	{
		mJoyConSide = side;
	}

	void NintendoDevice::_setJoyConChargingGrip(bool connected)
	{
		mChargingGrip = connected;
	}

	JoyConSide NintendoDevice::joyConSide()
	{
		return mJoyConSide;
	}

	NintendoDevice* NintendoDevice::joyConPair()
	{
		return mJoyConPair;
	}

	NintendoDevice* NintendoDevice::joyConOther()
	{
		return mJoyConOther;
	}

	bool NintendoDevice::pairJoyCon(NintendoDevice* other, NintendoDevice* pair)
	{
		if (!mJoyConPair && !other->mJoyConPair)
		{
			if (mJoyConSide != other->mJoyConSide)
			{
				mJoyConPair = pair;
				other->mJoyConPair = pair;
				mJoyConOther = other;
				other->mJoyConOther = this;

				pair->mCoveredDevices.push_back(this);
				pair->mCoveredDevices.push_back(other);
				return true;
			}
		}
		return false;
	}

	bool NintendoDevice::unpairJoyCon()
	{
		if (mJoyConPair)
		{
			mJoyConPair = nullptr;
			mJoyConOther->mJoyConPair = nullptr;
			mJoyConOther->mJoyConOther = nullptr;
			mJoyConOther = nullptr;
			return true;
		}
		return false;
	}

	NintendoDeviceListener::~NintendoDeviceListener() {}
	void NintendoDeviceListener::nintendoDeviceDeleted(NintendoDevice* device) {}

	void NintendoDeviceListener::valueChanged(NintendoDevice* device, const IOCode& ic, float oldVal, float newVal) {}
	void NintendoDeviceListener::valueUpdated(NintendoDevice* device, const IOCode& ic, float val) {}

	void NintendoDeviceListener::buttonStateChanged(NintendoDevice* device, size_t button, bool oldState, bool newState) {}
	void NintendoDeviceListener::axisValueChanged(NintendoDevice* device, size_t axis, float oldVal, float newVal) {}
	void NintendoDeviceListener::dPadButtonStateChanged(NintendoDevice* device, size_t button, bool oldState, bool newState) {}
	void NintendoDeviceListener::signedAxisValueChanged(NintendoDevice* device, size_t ind, float oldVal, float newVal) {}
	void NintendoDeviceListener::leftVibrationChanged(NintendoDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration) {}
	void NintendoDeviceListener::rightVibrationChanged(NintendoDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration) {}

	void NintendoDeviceListener::buttonStateUpdated(NintendoDevice* device, size_t button, bool state) {}
	void NintendoDeviceListener::axisValueUpdated(NintendoDevice* device, size_t axis, float val) {}
	void NintendoDeviceListener::dPadButtonStateUpdated(NintendoDevice* device, size_t button, bool state) {}
	void NintendoDeviceListener::signedAxisValueUpdated(NintendoDevice* device, size_t ind, float val) {}
	void NintendoDeviceListener::leftVibrationUpdated(NintendoDevice* device, float strength, size_t msDuration) {}
	void NintendoDeviceListener::rightVibrationUpdated(NintendoDevice* device, float strength, size_t msDuration) {}

	void NintendoDeviceListener::preUpdate(NintendoDevice* device) {}
	void NintendoDeviceListener::postUpdate(NintendoDevice* device) {}

}