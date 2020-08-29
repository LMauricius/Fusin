#include "FusinInputManager.h"

#include "IOSubSystems/FusinIOSubSystem.h"

#include "Devices/FusinDevice.h"
#include "Devices/FusinKeyboardDevice.h"
#include "Devices/FusinMouseDevice.h"
#include "Devices/FusinGamepadDevice.h"
#include "Devices/FusinXInputDevice.h"
#include "Devices/FusinDSDevice.h"
#include "Devices/FusinXInputDevice.h"
#include "Devices/FusinNintendoDevice.h"

// These won't always be built, only if needed macros are defined
#include "IOSubSystems/FusinRawInputSubSystem.h"

#include <stdexcept>
#include <ctime>

#define FOR_LISTENERS(EXP) for (auto& it : mInputManagerListeners) {it->EXP;}
#define PREINIT(FUNC_NAME) if (mInitialized) throw std::runtime_error("You need to call " FUNC_NAME " *before* initializing InputManager!");

namespace Fusin
{

	InputManager::InputManager()
		: mEnabledTypes(IOF_ANY_DEVICE)
		, mWindowHandle(0)
		, mInitialized(false)
		, mLastTime(0)
	{

	}

	InputManager::~InputManager()
	{
		for (auto it = mIOSubSystems.begin(); it != mIOSubSystems.end(); it++)
		{
			delete (*it);
		}
	}

	void InputManager::initialize(bool registerDefaultIOSubSystems, const std::map<String, String>& config)
	{
		// Default input Systems
		if (registerDefaultIOSubSystems)
		{
			#ifdef _WIN32
				mIOSubSystems.push_back(new RawInputIOSubSystem());
			#else

			#endif
		}

		// Init IO systems
		for (IOSubSystem* ioSys : mIOSubSystems) {
			ioSys->initialize(this, config, mWindowHandle);
		}

		// Global Devices
		registerDevice(new KeyboardDevice(FUSIN_STR("Global Keyboard")));
		registerDevice(new MouseDevice(FUSIN_STR("Global Mouse")));
		registerDevice(new GamepadDevice(FUSIN_STR("Global Gamepad")));
		registerDevice(new XInputDevice(FUSIN_STR("Global XInput Cotroller")));
		registerDevice(new DSDevice(FUSIN_STR("Global DS Cotroller")));
		registerDevice(new NintendoDevice(FUSIN_STR("Global Nintendo Controller")));

		mInitialized = true;
	}

	void InputManager::setInputWindow(void *handle)
	{
		PREINIT("setInputWindow()");
		mWindowHandle = handle;
	}

	void* InputManager::getInputWindow()
	{
		return mWindowHandle;
	}

	void InputManager::enableDevices(IOFlags t)
	{
		mEnabledTypes = t;
	}

	IOFlags InputManager::getEnabledDevices()
	{
		return mEnabledTypes;
	}


	void InputManager::update(TimeMS msElapsed)
	{
		clock_t curTime = clock() * 1000 / CLOCKS_PER_SEC;
		if (msElapsed == 0 && mLastTime != 0)
		{
			msElapsed = curTime - mLastTime;
		}
		mLastTime = curTime;

		// Update start
		FOR_LISTENERS(preUpdate(this));

		// Update io systems
		for (auto& it : mIOSubSystems)
		{
			if (it->getTypes() & mEnabledTypes)
			{
				it->updateDeviceList();
				it->update();
			}
		}

		// Update devices
		for (DeviceType devType : ALL_DEVICE_TYPES)
		{
			for (Index i = 0; i < maxDeviceIndex(devType); i++)
			{
				if (Device* device = getDevice(devType, i))
				{
					device->_update(msElapsed);
				}
			}
		}

		FOR_LISTENERS(postUpdate(this))
	}

#ifdef _WIN32
	void InputManager::handleMessage(const MSG* msg)
	{
		for (auto& it : mIOSubSystems)
		{
			if (it->getTypes() & mEnabledTypes)
				it->handleMessage(msg);
		}
	}
#endif


	KeyboardDevice* InputManager::getKeyboardDevice(Index slot)
	{
		return static_cast<KeyboardDevice*>(getDevice(DT_KEYBOARD, slot));
	}

	unsigned int InputManager::maxKeyboardDeviceIndex()
	{
		return maxDeviceIndex(DT_KEYBOARD);
	}

	MouseDevice* InputManager::getMouseDevice(Index slot)
	{
		return static_cast<MouseDevice*>(getDevice(DT_MOUSE, slot));
	}

	unsigned int InputManager::maxMouseDeviceIndex()
	{
		return maxDeviceIndex(DT_MOUSE);
	}

	GamepadDevice* InputManager::getGamepadDevice(Index slot)
	{
		return static_cast<GamepadDevice*>(getDevice(DT_GAMEPAD, slot));
	}

	unsigned int InputManager::maxGamepadDeviceIndex()
	{
		return maxDeviceIndex(DT_GAMEPAD);
	}

	XInputDevice* InputManager::getXInputDevice(Index slot)
	{
		return static_cast<XInputDevice*>(getDevice(DT_XINPUT, slot));
	}

	unsigned int InputManager::maxXInputDeviceIndex()
	{
		return maxDeviceIndex(DT_XINPUT);
	}

	DSDevice* InputManager::getDSDevice(Index slot)
	{
		return static_cast<DSDevice*>(getDevice(DT_DUALSHOCK, slot));
	}

	unsigned int InputManager::maxDSDeviceIndex()
	{
		return maxDeviceIndex(DT_DUALSHOCK);
	}

	NintendoDevice* InputManager::getNintendoDevice(Index slot)
	{
		return static_cast<NintendoDevice*>(getDevice(DT_NINTENDO, slot));
	}

	unsigned int InputManager::maxNintendoDeviceIndex()
	{
		return maxDeviceIndex(DT_NINTENDO);
	}

	IOSignal * InputManager::getIOSignal(const IOCode & ic, Index deviceSlot) const
	{
		Device* dev = getDevice(ic.deviceType, deviceSlot);
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
		DeviceEnumerator::registerDevice(dev);

		// Register to the global device
		Device* global = getDevice(dev->type(), 0);
		if (global)
		{
			for (DeviceComponent* comp : dev->getDeviceComponents())
			{
				for (DeviceComponent* globalComp : dev->getDeviceComponents())
				{
					if (globalComp->deviceType() == comp->deviceType())
						globalComp->_coverDeviceComponent(comp);
				}
			}
		}

		FOR_LISTENERS(deviceRegistered(this, dev));
	}

	void InputManager::unregisterDevice(Device * dev)
	{
		DeviceEnumerator::unregisterDevice(dev);

		// Unregister from the global device
		Device* global = getDevice(dev->type(), 0);
		if (global)
		{
			for (DeviceComponent* comp : dev->getDeviceComponents())
			{
				for (DeviceComponent* globalComp : dev->getDeviceComponents())
				{
					if (globalComp->deviceType() == comp->deviceType())
						globalComp->_uncoverDeviceComponent(comp);
				}
			}
		}

		FOR_LISTENERS(deviceUnregistered(this, dev));
	}
	
	void InputManager::registerIOSubSystem(IOSubSystem* dev)
	{
		mIOSubSystems.push_back(dev);
	}

	void InputManager::unregisterIOSubSystem(IOSubSystem* dev)
	{
		mIOSubSystems.remove(dev);
	}

	void InputManager::addListener(InputManagerListener* listener)
	{
		mInputManagerListeners.push_back(listener);
	}

	void InputManager::removeListener(InputManagerListener* listener)
	{
		mInputManagerListeners.remove(listener);
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