#include "FusinInputManager.h"
#include "FusinInputSystem.h"
#include "Commands/FusinCommand.h"
#include "FusinDevice.h"
#include "FusinKeyboardDevice.h"
#include "FusinMouseDevice.h"
#include "FusinGamepadDevice.h"
#include "FusinXInputDevice.h"
#include "FusinDSDevice.h"
#include "FusinXInputDevice.h"
#include "FusinNintendoDevice.h"
#include <ctime>

#ifdef _FUSIN_INPUT_SYSTEM1
	#include _FUSIN_INPUT_SYSTEM1_HEADER
#endif
#ifdef _FUSIN_INPUT_SYSTEM2
	#include _FUSIN_INPUT_SYSTEM2_HEADER
#endif
#ifdef _FUSIN_INPUT_SYSTEM3
	#include _FUSIN_INPUT_SYSTEM3_HEADER
#endif
#ifdef _FUSIN_INPUT_SYSTEM4
	#include _FUSIN_INPUT_SYSTEM4_HEADER
#endif
#ifdef _FUSIN_INPUT_SYSTEM5
	#include _FUSIN_INPUT_SYSTEM5_HEADER
#endif
#ifdef _FUSIN_INPUT_SYSTEM6
	#include _FUSIN_INPUT_SYSTEM6_HEADER
#endif

#define FOR_LISTENERS(EXP) for (auto& it : mListeners) {it->EXP;}
#define PREINIT if (mInitialized) return;

namespace Fusin
{

	InputManager::InputManager()
		: mEnabledTypes(IO_ANY_DEVICE)
		, mWindowHandle(0)
		, mInitialized(false)
		, mLastTime(0)
	{

	}

	InputManager::~InputManager()
	{
		std::list<Command*> copiedList = mGestureList;
		for (auto g : copiedList)
		{
			g->setInputManager(nullptr);
		}
		for (auto it = mInputSystems.begin(); it != mInputSystems.end(); it++)
		{
			delete (*it);
		}
	}

	void InputManager::initialize(const std::map<String, String>& config)
	{
		// Input Systems
		InputSystem *is;
		
#ifdef _FUSIN_INPUT_SYSTEM1
		is = new _FUSIN_INPUT_SYSTEM1(this, config, mWindowHandle);
		mInputSystems.push_back(is);
#endif
#ifdef _FUSIN_INPUT_SYSTEM2
		is = new _FUSIN_INPUT_SYSTEM2(this);
		mInputSystems.push_back(is);
#endif
#ifdef _FUSIN_INPUT_SYSTEM3
		is = new _FUSIN_INPUT_SYSTEM3(this);
		mInputSystems.push_back(is);
#endif
#ifdef _FUSIN_INPUT_SYSTEM4
		is = new _FUSIN_INPUT_SYSTEM4(this);
		mInputSystems.push_back(is);
#endif
#ifdef _FUSIN_INPUT_SYSTEM5
		is = new _FUSIN_INPUT_SYSTEM5(this);
		mInputSystems.push_back(is);
#endif
#ifdef _FUSIN_INPUT_SYSTEM6
		is = new _FUSIN_INPUT_SYSTEM5(this);
		mInputSystems.push_back(is);
#endif

		// Global Devices
		registerDevice(new KeyboardDevice(FUSIN_STR("Global Keyboard"), 0));
		registerDevice(new MouseDevice(FUSIN_STR("Global Mouse"), 0));
		registerDevice(new GamepadDevice(0, 0, false, false, FUSIN_STR("Global Gamepad"), 0));
		registerDevice(new XInputDevice(FUSIN_STR("Global XInput Cotroller"), 0));
		registerDevice(new DSDevice(FUSIN_STR("Global DS Cotroller"), 0));
		registerDevice(new NintendoDevice(NDT_PRO_CONTROLLER, FUSIN_STR("Global Nintendo Controller"), 0));

		mInitialized = true;
	}

	void InputManager::setInputWindow(void *handle)
	{
		PREINIT;
		mWindowHandle = handle;
	}

	void* InputManager::getInputWindow()
	{
		return mWindowHandle;
	}

	void InputManager::enableDevices(IOType t)
	{
		mEnabledTypes = t;
	}

	IOType InputManager::getEnabledDevices()
	{
		return mEnabledTypes;
	}


	void InputManager::update(TimeMS msElapsed)
	{
		FOR_LISTENERS(preUpdate(this));

		clock_t curTime = clock() * 1000 / CLOCKS_PER_SEC;
		if (msElapsed == 0 && mLastTime != 0)
		{
			msElapsed = curTime - mLastTime;
		}
		mLastTime = curTime;

		// pre-_update actions
		for (auto& it : mGestureList)
		{
			it->_beginUpdate();
		}

		// Update ioType systems
		for (auto& it : mInputSystems)
		{
			if (it->getTypes() & mEnabledTypes)
			{
				it->updateDeviceList();
				it->update();
			}
		}

		// Update devices
		for (IOType type : ALL_DEVICE_TYPES)
		{
			for (Index i = 0; i < maxDeviceIndex(type); i++)
			{
				if (Device* device = getDevice(type, i))
				{
					device->_update(msElapsed);
				}
			}
		}

		// post-_update actions
		for (auto& it : mGestureList)
		{
			it->_endUpdate();
		}

		FOR_LISTENERS(postUpdate(this))
	}

#ifdef FUSIN_MESSAGE_TYPE
	void InputManager::handleMessage(const FUSIN_MESSAGE_TYPE* msg)
	{
		for (auto& it : mInputSystems)
		{
			if (it->getTypes() & mEnabledTypes)
				it->handleMessage(msg);
		}
	}
#endif


	KeyboardDevice* InputManager::getKeyboardDevice(Index slot)
	{
		return static_cast<KeyboardDevice*>(getDevice(IO_KEYBOARD, slot));
	}

	unsigned int InputManager::maxKeyboardDeviceIndex()
	{
		return maxDeviceIndex(IO_KEYBOARD);
	}

	MouseDevice* InputManager::getMouseDevice(Index slot)
	{
		return static_cast<MouseDevice*>(getDevice(IO_MOUSE, slot));
	}

	unsigned int InputManager::maxMouseDeviceIndex()
	{
		return maxDeviceIndex(IO_MOUSE);
	}

	GamepadDevice* InputManager::getGamepadDevice(Index slot)
	{
		return static_cast<GamepadDevice*>(getDevice(IO_GAMEPAD, slot));
	}

	unsigned int InputManager::maxGamepadDeviceIndex()
	{
		return maxDeviceIndex(IO_GAMEPAD);
	}

	XInputDevice* InputManager::getXInputDevice(Index slot)
	{
		return static_cast<XInputDevice*>(getDevice(IO_XInput, slot));
	}

	unsigned int InputManager::maxXInputDeviceIndex()
	{
		return maxDeviceIndex(IO_XInput);
	}

	DSDevice* InputManager::getDSDevice(Index slot)
	{
		return static_cast<DSDevice*>(getDevice(IO_DS, slot));
	}

	unsigned int InputManager::maxDSDeviceIndex()
	{
		return maxDeviceIndex(IO_DS);
	}

	NintendoDevice* InputManager::getNintendoDevice(Index slot)
	{
		return static_cast<NintendoDevice*>(getDevice(IO_NINTENDO, slot));
	}

	unsigned int InputManager::maxNintendoDeviceIndex()
	{
		return maxDeviceIndex(IO_NINTENDO);
	}

	IOSignal * InputManager::getIOSignal(const IOCode & ic, Index deviceSlot) const
	{
		Device* dev = getDevice(ic.type & IO_ANY_DEVICE, deviceSlot);
		if (dev != nullptr)
		{
			return dev->getIOSignal(ic);
		}
		return nullptr;
	}


	float InputManager::getValue(const IOCode & ic, Index deviceSlot) const
	{
		IOSignal* s = getIOSignal(ic, deviceSlot);
		if (s != nullptr)
		{
			return s->value();
		}
		return 0.0;
	}

	void InputManager::registerDevice(Device * dev)
	{
		Device* global = getDevice(dev->type(), 0);
		if (global) global->_coverDevice(dev);
		DeviceEnumerator::registerDevice(dev);

		FOR_LISTENERS(deviceRegistered(this, dev));

		for (auto g : mGestureList)
		{
			g->_plug(dev);
		}
	}

	void InputManager::unregisterDevice(Device * dev)
	{
		DeviceEnumerator::unregisterDevice(dev);
		Device* global = getDevice(dev->type(), 0);
		if (global) global->_uncoverDevice(dev);

		FOR_LISTENERS(deviceUnregistered(this, dev));

		for (auto g : mGestureList)
		{
			g->_unplug(dev);
		}
	}

	void InputManager::addGesture(Command * g)
	{
		mGestureList.push_back(g);
	}

	void InputManager::removeGesture(Command * g)
	{
		mGestureList.remove(g);
	}

	void InputManager::addListener(InputManagerListener* listener)
	{
		mListeners.push_back(listener);
	}

	void InputManager::removeListener(InputManagerListener* listener)
	{
		mListeners.remove(listener);
	}

	/*XInputDevice* InputManager::getXInputDevice(Index slot)
	{
	if (mXInputDevices.isSlotFree(slot))
	{
	return nullptr;
	}

	return static_cast<XInputDevice*>(mXInputDevices[slot]);
	}*/

	/*
	Adding/removig actions to/from the correct list
	*/
	/*void InputManager::_associateGestureWithDevice(Command* a, const IOCode& c, unsigned int deviceIndex)
	{
		Device* device = getDevice(c.type, deviceIndex);
		if (device)
		{
			device->_associateGesture(a, c);
		}
	}

	void InputManager::_unassociateGestureWithDevice(Command* a, const IOCode& c, unsigned int deviceIndex)
	{
		Device* device = getDevice(c.type, deviceIndex);
		if (device)
		{
			device->_unassociateGesture(a, c);
		}
	}*/


	InputManagerListener::~InputManagerListener() {}
	void InputManagerListener::deviceRegistered(InputManager* im, Device* d) {}
	void InputManagerListener::deviceUnregistered(InputManager* im, Device* d) {}
	void InputManagerListener::preUpdate(InputManager* im) {}
	void InputManagerListener::postUpdate(InputManager* im) {}

}