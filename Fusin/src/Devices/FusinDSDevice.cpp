#include "FusinDSDevice.h"
#include "FusinGesture.h"
#include "FusinDS.h"
#include "FusinInputSignalManipulation.h"
#include <iomanip>
#include <algorithm>
#include <math.h>

#define FOR_LISTENERS(EXP) for (auto& it : mDeviceListeners) {it->EXP;} \
						   for (auto& it : mDSDeviceListeners) {it->EXP;} \
						   for (auto& it : mDSDeviceListeners) {it->EXP;}
#define FOR_GAMEPAD_LISTENERS(EXP) for (auto& it : mDSDeviceListeners) {it->EXP;} for (auto& it : mDSDeviceListeners) {it->EXP;}
#define FOR_DS_LISTENERS(EXP) for (auto& it : mDSDeviceListeners) {it->EXP;}

namespace Fusin
{
	DSDevice::ButtonSignals::ButtonSignals()
		: square(DS_SQUARE)
		, cross(DS_CROSS)
		, circle(DS_CIRCLE)
		, triangle(DS_TRIANGLE)
		, l1(DS_L1)
		, l2(DS_L2_BUTTON)
		, r1(DS_R1)
		, r2(DS_R2_BUTTON)
		, share(DS_SHARE)
		, options(DS_OPTIONS)
		, l3(DS_L3)
		, r3(DS_R3)
		, ps(DS_PS)
		, touchPad(DS_TOUCHPAD_BUTTON)
	{
	}
	DSDevice::AxisSignals::AxisSignals()
		: leftX(DS_LEFT_X_AXIS, &positiveLeftX, &negativeLeftX)
		, leftY(DS_LEFT_Y_AXIS, &positiveLeftY, &negativeLeftY)
		, rightX(DS_RIGHT_X_AXIS, &positiveRightX, &negativeRightX)
		, rightY(DS_RIGHT_Y_AXIS, &positiveRightY, &negativeRightY)
		, positiveLeftX(DS_LEFT_STICK_RIGHT)
		, negativeLeftX(DS_LEFT_STICK_LEFT)
		, positiveLeftY(DS_LEFT_STICK_DOWN)
		, negativeLeftY(DS_LEFT_STICK_UP)
		, positiveRightX(DS_RIGHT_STICK_RIGHT)
		, negativeRightX(DS_RIGHT_STICK_LEFT)
		, positiveRightY(DS_RIGHT_STICK_DOWN)
		, negativeRightY(DS_RIGHT_STICK_UP)
		, l2(DS_L2)
		, r2(DS_R2)
	{
	}

	DSDevice::DSDevice(String name, Index index)
		: GamepadDevice(DS_BUTTON_NUMBER, DS_AXIS_NUMBER, true,true, name, index)
	{
		mDeviceType = IT_DS;

		/*
		We use the cover system to connect the more readable DSDevice button signals ("buttons" structure)
		to the numbered GamepadDevice signals ("mButtons"). Same goes for the axes.
		*/
		buttons.square.coverInputSignal(&mButtons[DS_SQUARE.data]);
		buttons.cross.coverInputSignal(&mButtons[DS_CROSS.data]);
		buttons.circle.coverInputSignal(&mButtons[DS_CIRCLE.data]);
		buttons.triangle.coverInputSignal(&mButtons[DS_TRIANGLE.data]);
		buttons.l1.coverInputSignal(&mButtons[DS_L1.data]);
		buttons.l2.coverInputSignal(&mButtons[DS_L2_BUTTON.data]);
		buttons.r1.coverInputSignal(&mButtons[DS_R1.data]);
		buttons.r2.coverInputSignal(&mButtons[DS_R2_BUTTON.data]);
		buttons.share.coverInputSignal(&mButtons[DS_SHARE.data]);
		buttons.options.coverInputSignal(&mButtons[DS_OPTIONS.data]);
		buttons.l3.coverInputSignal(&mButtons[DS_L3.data]);
		buttons.r3.coverInputSignal(&mButtons[DS_R3.data]);
		buttons.ps.coverInputSignal(&mButtons[DS_PS.data]);
		buttons.touchPad.coverInputSignal(&mButtons[DS_TOUCHPAD_BUTTON.data]);

		axes.leftX.coverInputSignal(&mAxes[DS_LEFT_X_AXIS.data]);
		axes.leftY.coverInputSignal(&mAxes[DS_LEFT_Y_AXIS.data]);
		axes.rightX.coverInputSignal(&mAxes[DS_RIGHT_X_AXIS.data]);
		axes.rightY.coverInputSignal(&mAxes[DS_RIGHT_Y_AXIS.data]);
		axes.positiveLeftX.coverInputSignal(&mAxes[DS_LEFT_STICK_RIGHT.data]);
		axes.negativeLeftX.coverInputSignal(&mAxes[DS_LEFT_STICK_LEFT.data]);
		axes.positiveLeftY.coverInputSignal(&mAxes[DS_LEFT_STICK_DOWN.data]);
		axes.negativeLeftY.coverInputSignal(&mAxes[DS_LEFT_STICK_UP.data]);
		axes.positiveRightX.coverInputSignal(&mAxes[DS_RIGHT_STICK_RIGHT.data]);
		axes.negativeRightX.coverInputSignal(&mAxes[DS_RIGHT_STICK_LEFT.data]);
		axes.positiveRightY.coverInputSignal(&mAxes[DS_RIGHT_STICK_DOWN.data]);
		axes.negativeRightY.coverInputSignal(&mAxes[DS_RIGHT_STICK_UP.data]);
		axes.l2.coverInputSignal(&mAxes[DS_L2.data]);
		axes.r2.coverInputSignal(&mAxes[DS_R2.data]);
	}

	DSDevice::~DSDevice()
	{
		FOR_DS_LISTENERS(dsDeviceDeleted(this))
	}

	void DSDevice::_setCharging(bool charging)
	{
		mCharging = charging;
	}

	void DSDevice::_update(size_t msElapsed)
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
			coverInputSignalVector(static_cast<DSDevice*>(dev)->mButtons, mButtons, IT_DS_BUTTON, &mInputSignalProxy);
			// for all axes
			coverInputSignalVector(static_cast<DSDevice*>(dev)->mAxes, mAxes, mNegativeAxes, mPositiveAxes,
				IT_DS_AXIS, &mInputSignalProxy);
			// dpad axis
			if (std::abs(static_cast<DSDevice*>(dev)->dPad.angle.nextValue()) >= std::abs(dPad.angle.nextValue()))
				dPad.angle.setValue(static_cast<DSDevice*>(dev)->dPad.angle.nextValue());
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

	void DSDevice::_setConnectionMode(ConnectionMode m)
	{
		mConnectionMode = m;
	}

	IOSignal * DSDevice::getInputSignal(const IOCode & ic)
	{
		if ((ic.type & IT_ANY_CODE) & IT_DS_BUTTON) {
			if (ic.data < mButtons.size()) {
				return &mButtons[ic.data];
			}
		}
		else if ((ic.type & IT_ANY_CODE) & IT_DS_AXIS) {
			if (ic.type & IT_SIGNED_VERSION) {
				if (ic.data / 2 < mAxes.size()) {
					IOSignal *axis = &mAxes[ic.data / 2];
					if (ic.data % 2 == 0) {
						return axis->positiveSignal();
					}
					else {
						return axis->negativeSignal();
					}
				}
			}
			else {
				if (ic.data < mAxes.size()) {
					return &mAxes[ic.data];
				}
			}
		}
		else if ((ic.type & IT_ANY_CODE) & IT_DS_DPAD_ANGLE) {
			if (ic.data == 0) return &dPad.angle;
		}
		else if ((ic.type & IT_ANY_CODE) & IT_DS_DPAD_BUTTON) {
			if (ic.data == DS_DPAD_UP.data) return &dPad.up;
			else if (ic.data == DS_DPAD_DOWN.data) return &dPad.down;
			else if (ic.data == DS_DPAD_LEFT.data) return &dPad.left;
			else if (ic.data == DS_DPAD_RIGHT.data) return &dPad.right;
		}
		else {
			IOSignal * signal = nullptr;

			if ((signal = motion.getInputSignal(ic)) != nullptr)
				return signal;
			if ((signal = touchPad.getInputSignal(ic)) != nullptr)
				return signal;
		}
		return nullptr;
	}

	void DSDevice::clear()
	{
	}

	String DSDevice::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		ss << (mCharging ? "Charging: " : "Battery: ") << mBattery << " ";

		ss << "LX(" << axes.leftX.value() << ") ";
		ss << "LY(" << axes.leftY.value() << ") ";
		ss << "RX(" << axes.rightX.value() << ") ";
		ss << "RY(" << axes.rightY.value() << ") ";
		ss << "dPad(" << dPad.angle.value() << ") ";
		ss << "L2(" << axes.l2.value() << ") ";
		ss << "R2(" << axes.r2.value() << ") ";
		ss << motion.getStateString();
		ss << " ";
		ss << touchPad.getStateString();

		ss << " Buttons: ";
		if (buttons.cross.value())
			ss << "X ";
		if (buttons.square.value())
			ss << "SQR ";
		if (buttons.circle.value())
			ss << "CRC ";
		if (buttons.triangle.value())
			ss << "TRI ";
		if (buttons.l1.value())
			ss << "L1 ";
		if (buttons.r1.value())
			ss << "R1 ";
		if (buttons.l2.value())
			ss << "L2 ";
		if (buttons.r2.value())
			ss << "R2 ";
		if (buttons.l3.value())
			ss << "L3 ";
		if (buttons.r3.value())
			ss << "R3 ";
		if (buttons.share.value())
			ss << "SHR ";
		if (buttons.options.value())
			ss << "OPT ";
		if (buttons.ps.value())
			ss << "PS ";
		if (buttons.touchPad.value())
			ss << "TP Press ";
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


	// Vibration
	void DSDevice::setLeftVibration(float strength, size_t msDuration)
	{
		if (mVibrationLeft != strength || mVibrationLeftDuration != msDuration)
		{
			FOR_GAMEPAD_LISTENERS(leftVibrationChanged(this, mVibrationLeft, mVibrationLeftDuration, strength, msDuration))
			FOR_GAMEPAD_LISTENERS(leftVibrationUpdated(this, strength, msDuration))
			mVibrationLeft = strength;
			mVibrationLeftDuration = msDuration;
		}
		else if (strength && msDuration)
		{
			FOR_GAMEPAD_LISTENERS(leftVibrationUpdated(this, strength, msDuration))
		}
	}

	void DSDevice::setRightVibration(float strength, size_t msDuration)
	{
		if (mVibrationRight != strength || mVibrationRightDuration != msDuration)
		{
			FOR_GAMEPAD_LISTENERS(rightVibrationChanged(this, mVibrationRight, mVibrationRightDuration, strength, msDuration))
				FOR_GAMEPAD_LISTENERS(rightVibrationUpdated(this, strength, msDuration))
			mVibrationRight = strength;
			mVibrationRightDuration = msDuration;
		}
		else if (strength && msDuration)
		{
			FOR_GAMEPAD_LISTENERS(rightVibrationUpdated(this, strength, msDuration))
		}
	}


	// Color change
	void DSDevice::setColor(ColorRGB c)
	{
		if (c != mColor)
		{
			FOR_DS_LISTENERS(colorChanged(this, mColor, c))
			FOR_DS_LISTENERS(colorUpdated(this, c))
		}
		else if (c)
		{
			FOR_DS_LISTENERS(colorUpdated(this, c))
		}
		mColor = c;
	}

	ColorRGB DSDevice::getColor()
	{
		return mColor;
	}

	// LED change
	void DSDevice::setLEDs(LEDFlags leds)
	{
		if (leds != mLeds)
		{
			FOR_DS_LISTENERS(LEDChanged(this, mLeds, leds))
			FOR_DS_LISTENERS(LEDUpdated(this, leds))
		}
		else if (leds)
		{
			FOR_DS_LISTENERS(LEDUpdated(this, leds))
		}
		mLeds = leds;
	}

	LEDFlags DSDevice::getLEDs()
	{
		return mLeds;
	}


	void DSDevice::addListener(DSDeviceListener* listener)
	{
		mDSDeviceListeners.push_back(listener);
	}

	void DSDevice::removeListener(DSDeviceListener* listener)
	{
		mDSDeviceListeners.remove(listener);
	}


	DSDeviceListener::~DSDeviceListener() {}
	void DSDeviceListener::dsDeviceDeleted(DSDevice* device) {}

	void DSDeviceListener::valueChanged(DSDevice* device, const IOCode& ic, float oldVal, float newVal) {}
	void DSDeviceListener::valueUpdated(DSDevice* device, const IOCode& ic, float val) {}

	void DSDeviceListener::buttonStateChanged(DSDevice* device, size_t button, bool oldState, bool newState) {}
	void DSDeviceListener::axisValueChanged(DSDevice* device, size_t axis, float oldVal, float newVal) {}
	void DSDeviceListener::dPadButtonStateChanged(DSDevice* device, size_t button, bool oldState, bool newState) {}
	void DSDeviceListener::signedAxisValueChanged(DSDevice* device, size_t ind, float oldVal, float newVal) {}
	void DSDeviceListener::leftVibrationChanged(DSDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration) {}
	void DSDeviceListener::rightVibrationChanged(DSDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration) {}
	void DSDeviceListener::colorChanged(DSDevice* device, ColorRGB oldC, ColorRGB newC) {}
	void DSDeviceListener::LEDChanged(DSDevice* device, LEDFlags oldLEDs, LEDFlags newLEDs) {}

	void DSDeviceListener::buttonStateUpdated(DSDevice* device, size_t button, bool state) {}
	void DSDeviceListener::axisValueUpdated(DSDevice* device, size_t axis, float val) {}
	void DSDeviceListener::dPadButtonStateUpdated(DSDevice* device, size_t button, bool state) {}
	void DSDeviceListener::signedAxisValueUpdated(DSDevice* device, size_t ind, float val) {}
	void DSDeviceListener::leftVibrationUpdated(DSDevice* device, float strength, size_t msDuration) {}
	void DSDeviceListener::rightVibrationUpdated(DSDevice* device, float strength, size_t msDuration) {}
	void DSDeviceListener::colorUpdated(DSDevice* device, ColorRGB c) {}
	void DSDeviceListener::LEDUpdated(DSDevice* device, LEDFlags LEDs) {}

	void DSDeviceListener::preUpdate(DSDevice* device) {}
	void DSDeviceListener::postUpdate(DSDevice* device) {}
}