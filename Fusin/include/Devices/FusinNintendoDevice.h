#ifndef _FUSIN_NINTENDO_CONTROLLER_H
#define _FUSIN_NINTENDO_CONTROLLER_H

#include "FusinGamepadDevice.h"
#include "FusinMotionTrackerDevice.h"
#include "FusinColor.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class NintendoDeviceListener;

	enum NintendoDeviceType { NDT_JOYCON, NDT_JOYCON_PAIR, NDT_PRO_CONTROLLER };
	enum JoyConSide { JC_LEFT, JC_RIGHT, JC_PAIR };

	class NintendoDevice : public GamepadDevice
	{
		friend InputManager;

	public:
		NintendoDevice(NintendoDeviceType ninType, String name = FUSIN_STR("Nintendo Controller"), size_t index = 0);
		~NintendoDevice();

		// Nintendo controller info
		inline NintendoDeviceType nintendoDeviceType() { return mNintendoDeviceType; }
		bool charging() { return mCharging; }

		/*
		Returns the string with all axis values and indices of buttons that are currently held
		*/
		String getStateString();

		struct ButtonSignals {
			IOSignal a, b, x, y;
			IOSignal l, zl, r, zr;
			IOSignal minus, plus, leftStick, rightStick, home, capture, sl, sr;

			ButtonSignals();
		} buttons;
		struct AxisSignals {
			IOSignal leftX, leftY, rightX, rightY;
			IOSignal positiveLeftX, negativeLeftX, positiveLeftY, negativeLeftY;
			IOSignal positiveRightX, negativeRightX, positiveRightY, negativeRightY;

			AxisSignals();
		} axes;
		MotionTrackerDevice motion;


		JoyConSide joyConSide();
		/*
		Returns the NintendoDevice that serves as a pair of both this and the other joycon, nullptr if it's not paired.
		(The paired device works just like a normal gamepad)
		*/
		NintendoDevice* joyConPair();
		/*
		Returns the other joycon in a pair, nullptr if it's not paired
		*/
		NintendoDevice* joyConOther();
		/*
		Pairs the joy con to the other, using pair as the device which combines the two, unless the joy con has already been paired.
		Returns whether successful.
		*/
		bool pairJoyCon(NintendoDevice* other, NintendoDevice* pair);
		bool unpairJoyCon();

		void addListener(NintendoDeviceListener* listener);
		void removeListener(NintendoDeviceListener* listener);

		void _update(size_t msElapsed = 0);
		void _setCharging(bool charging);
		void _setJoyConSide(JoyConSide side);
		void _setJoyConChargingGrip(bool connected);

	protected:
		std::vector<IOSignal*> mButtonPtrs, mAxisPtrs;
		NintendoDeviceType mNintendoDeviceType;
		bool mCharging;
		std::list<NintendoDeviceListener*> mNintendoDeviceListeners;
		JoyConSide mJoyConSide;
		bool mChargingGrip;
		NintendoDevice *mJoyConPair, *mJoyConOther;
	};

	class NintendoDeviceListener
	{
	public:
		virtual ~NintendoDeviceListener() = 0;

		/*
		Called when the device has been plugged out or turned off by other means.
		Also called when the InputManager is being shut down.
		*/
		virtual void nintendoDeviceDeleted(NintendoDevice* device);

		virtual void valueChanged(NintendoDevice* device, const IOCode& ic, float oldVal, float newVal);
		virtual void valueUpdated(NintendoDevice* device, const IOCode& ic, float val);

		virtual void buttonStateChanged(NintendoDevice* device, size_t button, bool oldState, bool newState);
		virtual void axisValueChanged(NintendoDevice* device, size_t axis, float oldVal, float newVal);
		virtual void dPadButtonStateChanged(NintendoDevice* device, size_t ind, bool oldState, bool newState);
		virtual void signedAxisValueChanged(NintendoDevice* device, size_t ind, float oldVal, float newVal);
		virtual void leftVibrationChanged(NintendoDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration);
		virtual void rightVibrationChanged(NintendoDevice* device, float oldStrength, size_t oldMsDuration, float newStrength, size_t newMsDuration);

		virtual void buttonStateUpdated(NintendoDevice* device, size_t button, bool state);
		virtual void axisValueUpdated(NintendoDevice* device, size_t axis, float val);
		virtual void dPadButtonStateUpdated(NintendoDevice* device, size_t ind, bool state);
		virtual void signedAxisValueUpdated(NintendoDevice* device, size_t ind, float val);
		virtual void leftVibrationUpdated(NintendoDevice* device, float strength, size_t msDuration);
		virtual void rightVibrationUpdated(NintendoDevice* device, float strength, size_t msDuration);
		virtual void preUpdate(NintendoDevice* device);
		virtual void postUpdate(NintendoDevice* device);
	};

}

#endif