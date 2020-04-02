#ifndef _FUSIN_DS_CONTROLLER_H
#define _FUSIN_DS_CONTROLLER_H

#include "FusinGamepadDevice.h"
#include "FusinMotionTrackerDevice.h"
#include "FusinTouchDevice.h"
#include "FusinColor.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class DSDeviceListener;

	class DSDevice : public GamepadDevice
	{
	public:
		DSDevice(String name = FUSIN_STR("DS Controller"), Index index = 0);
		~DSDevice();

		// DS info

		ConnectionMode connectionMode() { return mConnectionMode; }
		bool charging() { return mCharging; }

		/*
		Returns the signal of the specified IOCode.
		Supports IOCode types IT_GAMEPAD_BUTTON, IT_GAMEPAD_AXIS, IT_GAMEPAD_DPAD_ANGLE, IT_GAMEPAD_DPAD_BUTTON, IT_GAMEPAD_SIGNED_AXIS.
		*/
		IOSignal* getInputSignal(const IOCode& ic);

		void clear();

		/*
		Returns the string with all axis values and indices of buttons that are currently held
		*/
		String getStateString();

		struct ButtonSignals {
			IOSignal square, cross, circle, triangle;
			IOSignal l1, l2, r1, r2;
			IOSignal share, options, l3, r3, ps, touchPad;

			ButtonSignals();
		} buttons;
		struct AxisSignals {
			IOSignal leftX, leftY, rightX, rightY;
			IOSignal positiveLeftX, negativeLeftX, positiveLeftY, negativeLeftY;
			IOSignal positiveRightX, negativeRightX, positiveRightY, negativeRightY;
			IOSignal l2, r2;

			AxisSignals();
		} axes;
		MotionTrackerDevice motion;
		TouchDevice touchPad;

		// Vibration
		void setLeftVibration(float strength = 0, size_t msDuration = 0);
		void setRightVibration(float strength = 0, size_t msDuration = 0);

		// Color change (DS4)
		void setColor(ColorRGB c);
		ColorRGB getColor();

		// LED change (DS3)
		void setLEDs(LEDFlags leds);
		LEDFlags getLEDs();

		void addListener(DSDeviceListener* listener);
		void removeListener(DSDeviceListener* listener);

		void _update(size_t msElapsed = 0);
		void _setConnectionMode(ConnectionMode m);
		void _setCharging(bool charging);

	protected:
		bool mCharging;
		ColorRGB mColor;
		LEDFlags mLeds;
		std::list<DSDeviceListener*> mDSDeviceListeners;
	};

	class DSDeviceListener : private GamepadDeviceListener
	{
		friend DSDevice;

	public:
		virtual ~DSDeviceListener() = 0;

		/*
		Called when the device has been plugged out or turned off by other means.
		Also called when the InputManager is being shut down.
		*/
		virtual void dsDeviceDeleted(DSDevice* device);

		virtual void valueChanged(DSDevice* device, const IOCode& ic, float oldVal, float newVal);
		virtual void valueUpdated(DSDevice* device, const IOCode& ic, float val);

		virtual void buttonStateChanged(DSDevice* device, size_t button, bool oldState, bool newState);
		virtual void axisValueChanged(DSDevice* device, size_t axis, float oldVal, float newVal);
		virtual void dPadButtonStateChanged(DSDevice* device, size_t ind, bool oldState, bool newState);
		virtual void signedAxisValueChanged(DSDevice* device, size_t ind, float oldVal, float newVal);
		virtual void leftVibrationChanged(DSDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration);
		virtual void rightVibrationChanged(DSDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration);
		virtual void colorChanged(DSDevice* device, ColorRGB oldC, ColorRGB newC);
		virtual void LEDChanged(DSDevice* device, LEDFlags oldLEDs, LEDFlags newLEDs);

		virtual void buttonStateUpdated(DSDevice* device, size_t button, bool state);
		virtual void axisValueUpdated(DSDevice* device, size_t axis, float val);
		virtual void dPadButtonStateUpdated(DSDevice* device, size_t ind, bool state);
		virtual void signedAxisValueUpdated(DSDevice* device, size_t ind, float val);
		virtual void leftVibrationUpdated(DSDevice* device, float strength, size_t msDuration);
		virtual void rightVibrationUpdated(DSDevice* device, float strength, size_t msDuration);
		virtual void colorUpdated(DSDevice* device, ColorRGB c);
		virtual void LEDUpdated(DSDevice* device, LEDFlags LEDs);
		virtual void preUpdate(DSDevice* device);
		virtual void postUpdate(DSDevice* device);
	};

}

#endif