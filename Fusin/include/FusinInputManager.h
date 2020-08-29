#ifndef _FUSIN_INPUT_MANAGER_H
#define _FUSIN_INPUT_MANAGER_H

#include "IOCodes/FusinIOCode.h"
#include "Devices/FusinDeviceEnumerator.h"

#include <list>
#include <map>

namespace Fusin
{
	class Command;
	class InputCommand;
	class IOSignal;
	class Device;
	class KeyboardDevice;
	class MouseDevice;
	class GamepadDevice;
	class DSDevice;
	class XInputDevice;
	class NintendoDevice;
	class InputSystem;
	typedef std::list<Command*> GesturePtrList;
	class InputManagerListener;

	class InputManager : public DeviceEnumerator
	{
	public:
		InputManager();
		~InputManager();

		/*
		If enabled, XInput devices will be handled by RawInput System and listed as GamepadDevices.
		This won't disable XInput Systems, so you probably don't want to assign IO_XInput IOCodes
		along with IO_GAMEPAD ones to Gestures if this is enabled, otherwise the Gestures will get updated for both device types.
		Call this before initializing the InputManager.
		Default is false;
		*/
		/*void treatXInputControllersAsGamepads(bool enable);
		bool treatingXInputControllersAsGamepads();*/
		/*
		If enabled, DualShock4 devices will be handled by RawInput and listed as GamepadDevices.
		Call this before initializing the InputManager.
		Default is true.
		*/
		/*void treatDSControllersAsGamepads(bool enable);
		bool treatingDSControllersAsGamepads();*/
		/*
		If not specified, the InputSystem will create its own hidden window and ioType will always
		be received even if the window is not in focus.
		If you use this, you will need to call handleMessage() in your own window message loop to _update RawInput.
		Call this before initializing the InputManager.
		*/
		void setInputWindow(void *handle);
		void* getInputWindow();
		/*
		Enable this to receive ioType even when the window isn't in focus. This won't 
		have effect if you don't specify your own window handle with setInputWindow().
		Call this before initializing the InputManager.
		Default is false.
		*/
		/*void receiveInputOutsideFocus(bool enable);
		bool receivingInputOutsideFocus();*/
		/*
		The amount of window messages that will be accepted per device between two _update() calls.
		Set this to 0 if you want all messages to be accepted, but keep in mind that might cause a severe lag
		if the device sends too many messages in a short time.
		Call this before initializing the InputManager.
		Default is 10.
		*/
		//void setMaxMessagesPerDevice(int n);
		/*
		Specifies which Devices will be updated during an _update() call.
		You can still manually read ioType from disabled devices, but Gestures which 
		are dependant on these devices won't have their values updated.
		Default is IO_ANY_DEVICE.
		*/
		void enableDevices(IOFlags t = IOF_ANY_DEVICE);
		IOType getEnabledDevices();

		/*
		Initializes all Input Systems with previously set properties.
		*/
		void initialize(const std::map<String, String>& config = std::map<String, String>());

		/*
		Updates the ioType values of all Devices and Gestures.
		*/
		void update(TimeMS msElapsed = 0);

#ifdef FUSIN_MESSAGE_TYPE
		/*
		Call this in the game window's message loop.
		*/
		void handleMessage(const FUSIN_MESSAGE_TYPE* msg);
#endif

		/*
		Returns the device that supports ioType of the specified type with the specified deviceIndex.
		If no device supports the ioType type or there is no device with the specified index, returns nullptr.
		Use index 0 to get the global device.
		Internally, this is the same as calling enumerator.getDevice()
		*/
		//Device* getDevice(IOType t, Index index = 0) const;
		/*
		Returns the current max index of devices that support ioType of the specified type.
		This can include unused indices. So if there are devices with indices 1 and 3
		for a specific type, but 0 and 2 are skipped, this returns 3.
		Internally, this is the same as calling enumerator.maxDeviceIndex()
		*/
		//unsigned int maxDeviceIndex(IOType t);

		/*
		Returns nullptr if there is no KeyboardDevice with the specified index.
		Use index 0 to get the global KeyboardDevice.
		*/
		KeyboardDevice* getKeyboardDevice(Index index = 0);
		unsigned int maxKeyboardDeviceIndex();
		/*
		Returns nullptr if there is no MouseDevice with the specified index.
		Use index 0 to get the global MouseDevice.
		*/
		MouseDevice* getMouseDevice(Index index = 0);
		unsigned int maxMouseDeviceIndex();
		/*
		Returns nullptr if there is no GamepadDevice with the specified index.
		Use index 0 to get the global GamepadDevice.
		*/
		GamepadDevice* getGamepadDevice(Index index = 0);
		unsigned int maxGamepadDeviceIndex();
		/*
		Returns nullptr if there is no XInputDevice with the specified index.
		Use index 0 to get the global XInputDevice.
		*/
		XInputDevice* getXInputDevice(Index index = 0);
		unsigned int maxXInputDeviceIndex();
		/*
		Returns nullptr if there is no DSDevice with the specified index.
		Use index 0 to get the global DSDevice.
		*/
		DSDevice* getDSDevice(Index index = 0);
		unsigned int maxDSDeviceIndex();
		/*
		Returns nullptr if there is no NintendoDevice with the specified index.
		Use index 0 to get the global NintendoDevice.
		*/
		NintendoDevice* getNintendoDevice(Index index = 0);
		unsigned int maxNintendoDeviceIndex();

		/*
		Returns the IOSignal with the specified IOCode.
		Returns nullptr if no device supports the specified IOCode.
		*/
		IOSignal* getIOSignal(const IOCode& ic, Index deviceSlot) const;

		/*
		Returns the current value of the specified IOCode.
		The value will be 0 (not pressed) or 1 (pressed) for Button inputs
		and in a device dependant range for Axis inputs.
		Returns 0 if no device supports the specified IOCode.
		*/
		float getValue(const IOCode& ic, Index deviceSlot) const;

		void registerDevice(Device* dev);
		void unregisterDevice(Device* dev);
		void addGesture(Command* g);
		void removeGesture(Command* g);
		void addListener(InputManagerListener* listener);
		void removeListener(InputManagerListener* listener);

	protected:
		//bool mTreatXInputAsGamepads, mTreatDSAsGamepads, mReceiveInputOutsideFocus;
		void *mWindowHandle;
		IOType mEnabledTypes;
		bool mInitialized;
		TimeMS mLastTime;

		std::list<Command*> mGestureList;
		std::list<InputSystem*> mInputSystems;
		std::list<InputManagerListener*> mListeners;
	};

	class InputManagerListener
	{
	public:
		virtual ~InputManagerListener() = 0;

		virtual void deviceRegistered(InputManager* im, Device* d);
		virtual void deviceUnregistered(InputManager* im, Device* d);
		virtual void preUpdate(InputManager* im);
		virtual void postUpdate(InputManager* im);
	};

}

#endif