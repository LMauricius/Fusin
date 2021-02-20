#include "FusinInputManager.h"

#include "IOSystems/FusinIOSystem.h"

#include "Devices/FusinDevice.h"
#include "Devices/FusinKeyboardDevice.h"
#include "Devices/FusinMouseDevice.h"
#include "Devices/FusinGamepadDevice.h"
#include "Devices/FusinXInputDevice.h"
#include "Devices/FusinDSDevice.h"
#include "Devices/FusinXInputDevice.h"
#include "Devices/FusinNintendoDevice.h"

// These won't always be built, only if needed macros are defined
#include "IOSystems/FusinRawInputIOSystem.h"
#include "IOSystems/FusinXInputIOSystem.h"
#include "IOSystems/FusinX11IOSystem.h"

#include "Utilities/FusinConfigMap.h"

#include <stdexcept>
#include <ctime>

#define FOR_LISTENERS(EXP) for (auto& it : mInputManagerListeners) {it->EXP;} for (auto& it : mDeviceEnumeratorListeners) {it->EXP;}
#define PREINIT(FUNC_NAME) if (mInitialized) throw std::runtime_error("You need to call " FUNC_NAME " *before* initializing InputManager!");

namespace Fusin
{

	InputManager::InputManager()
		: mEnabledTypes(IOF_ANY_DEVICE)
		, mWindowHandle(0)
		, mInitialized(false)
		, mLastTime(0)
		, mDeviceEnumerationTimer(0)
	{

	}

	InputManager::~InputManager()
	{
		for (auto it = mIOSystems.begin(); it != mIOSystems.end(); it++)
		{
			delete (*it);
		}
	}

	#define GET_CFG(VAR, KEY, DEFAULT) VAR = getCfgValue(config, FUSIN_STR(KEY), DEFAULT)

	void InputManager::initialize(bool registerDefaultIOSystems, const std::map<String, String>& config)
	{
		GET_CFG(mDeviceEnumerationPeriod, "Device enumeration period", 1000);

		// Default input Systems
		if (registerDefaultIOSystems)
		{
			#ifdef FUSIN_BUILD_RAW_INPUT
				mIOSystems.push_back(new RawInputIOSystem());
			#endif
			#ifdef FUSIN_BUILD_XINPUT
				mIOSystems.push_back(new XInputIOSystem());
			#endif
			#ifdef FUSIN_BUILD_X11
				mIOSystems.push_back(new X11IOSystem());
			#endif
		}

		// Init IO systems
		for (IOSystem* ioSys : mIOSystems) {
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
		// default timer
		clock_t curTime = clock() * 1000 / CLOCKS_PER_SEC;
		if (msElapsed == 0 && mLastTime != 0)
		{
			msElapsed = curTime - mLastTime;
		}
		mLastTime = curTime;

		// device listing update timer
		mDeviceEnumerationTimer += msElapsed;

		// Update start
		FOR_LISTENERS(preUpdate(this));

		// Update io systems
		for (auto& it : mIOSystems)
		{
			if (it->getFlags() & mEnabledTypes)
			{
				if (mDeviceEnumerationTimer > mDeviceEnumerationPeriod)
				{
					it->updateDeviceList();
				}
				it->update();
			}
		}

		// Reset timer
		if (mDeviceEnumerationTimer > mDeviceEnumerationPeriod)
		{
			mDeviceEnumerationTimer -= mDeviceEnumerationPeriod;
			
			// If 2 or more periods have accumulated reset it to 0, to prevent enumerating on each update() 
			// in case the timing goes bonkers.
			if (mDeviceEnumerationTimer > mDeviceEnumerationPeriod)
				mDeviceEnumerationTimer = 0;
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

		for (auto& it : mInputManagerListeners) {it->postUpdate(this);}
		for (auto& it : mDeviceEnumeratorListeners) {
			it->postUpdate(this);
		}
		//FOR_LISTENERS(postUpdate(this))
	}

#ifdef _WIN32
	void InputManager::handleMessage(const MSG* msg)
	{
		for (auto& it : mIOSystems)
		{
			if (it->getFlags() & mEnabledTypes)
				it->handleMessage(msg);
		}
	}
#endif


	KeyboardDevice* InputManager::getKeyboardDevice(Index slot)
	{
		return static_cast<KeyboardDevice*>(getDevice(DT_KEYBOARD, slot));
	}

	size_t InputManager::maxKeyboardDeviceIndex()
	{
		return maxDeviceIndex(DT_KEYBOARD);
	}

	MouseDevice* InputManager::getMouseDevice(Index slot)
	{
		return static_cast<MouseDevice*>(getDevice(DT_MOUSE, slot));
	}

	size_t InputManager::maxMouseDeviceIndex()
	{
		return maxDeviceIndex(DT_MOUSE);
	}

	GamepadDevice* InputManager::getGamepadDevice(Index slot)
	{
		return static_cast<GamepadDevice*>(getDevice(DT_GAMEPAD, slot));
	}

	size_t InputManager::maxGamepadDeviceIndex()
	{
		return maxDeviceIndex(DT_GAMEPAD);
	}

	XInputDevice* InputManager::getXInputDevice(Index slot)
	{
		return static_cast<XInputDevice*>(getDevice(DT_XINPUT, slot));
	}

	size_t InputManager::maxXInputDeviceIndex()
	{
		return maxDeviceIndex(DT_XINPUT);
	}

	DSDevice* InputManager::getDSDevice(Index slot)
	{
		return static_cast<DSDevice*>(getDevice(DT_DUALSHOCK, slot));
	}

	size_t InputManager::maxDSDeviceIndex()
	{
		return maxDeviceIndex(DT_DUALSHOCK);
	}

	NintendoDevice* InputManager::getNintendoDevice(Index slot)
	{
		return static_cast<NintendoDevice*>(getDevice(DT_NINTENDO, slot));
	}

	size_t InputManager::maxNintendoDeviceIndex()
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

	Index InputManager::registerDevice(Device * dev, bool registerComponents)
	{
		Index ind = DeviceEnumerator::registerDevice(dev);

		// Register to the global device
		if (ind != 0)// if the current device isn't the global one
		{
			Device* global = getDevice(dev->type(), 0);
			if (global)
			{
				for (DeviceComponent* comp : dev->getDeviceComponents())
				{
					for (DeviceComponent* globalComp : global->getDeviceComponents())
					{
						if (globalComp->deviceType() == comp->deviceType())
							globalComp->_coverDeviceComponent(comp);
					}
				}
			}
		}

		FOR_LISTENERS(deviceRegistered(this, dev));
		return ind;
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
	
	void InputManager::registerIOSystem(IOSystem* dev)
	{
		mIOSystems.push_back(dev);
	}

	void InputManager::unregisterIOSystem(IOSystem* dev)
	{
		mIOSystems.remove(dev);
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