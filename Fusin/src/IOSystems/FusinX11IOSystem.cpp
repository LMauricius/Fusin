#include "IOSystems/FusinX11IOSystem.h"

#ifdef FUSIN_BUILD_X11

#include "Devices/FusinDeviceEnumerator.h"

#include "Devices/FusinKeyboardDevice.h"
#include "Devices/FusinMouseDevice.h"

#include "IOCodes/FusinKey.h"
#include "IOCodes/FusinMouse.h"

#include "Utilities/FusinAlgorithm.h"
#include "Utilities/FusinConfigMap.h"
#include "Utilities/FusinVendors.h"
#include "Utilities/FusinLog.h"

#include "IOSystems/X11/FusinX11KeycodeConvert.h"

#include <algorithm>
#include <locale>
#include <codecvt>
#include <iostream>
#include <iostream>
#include <string.h>
#include <set>

#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <xkbcommon/xkbcommon.h>
#include <X11/extensions/XInput2.h>

namespace Fusin
{
	#define X11_SCROLL_LOCK 1
	#define X11_CAPSLOCK 2
	#define X11_NUMLOCK 16
	#define GET_CFG(VAR, KEY, DEFAULT) VAR = getCfgValue(config, FUSIN_STR(KEY), DEFAULT)

	X11IOSystem::X11IOSystem():
		IOSystem(IOF_ANY)
	{
	}

	void X11IOSystem::initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
	{
		IOSystem::initialize(de, config, window);
		mWindow = ((Window)window);

		// Set config
		GET_CFG(mReceiveInputOutsideFocus, "Receive input outside focus", false);

		// create the window
		if (!mWindow)
		{
			mDisplay = XOpenDisplay(nullptr);
			int scrId = DefaultScreen(mDisplay);

			mWindow = XCreateSimpleWindow(mDisplay, XRootWindow(mDisplay, scrId),
				10, 10, 400, 200, 1, BlackPixel(mDisplay, scrId), WhitePixel(mDisplay, scrId));
  			XStoreName(mDisplay, mWindow, "Fusin X11 Window");
			XMapWindow(mDisplay, mWindow);

			mCreatedWindow = true;
		}
		else
		{
			mDisplay = XOpenDisplay(nullptr);
			mCreatedWindow = false;
		}

		// initialize XInput
		mUsingXInput = false;
		int ev, err;
		if (!XQueryExtension(mDisplay, "XInputExtension", &mXIExtOpcode, &ev, &err))
		{
			Log::singleton() << "XInput extension not found; using global X11 inputs";
			mUsingXInput = false;
		}
		else
		{
			int major_op = 2;
			int minor_op = 2;
			int result = XIQueryVersion(mDisplay, &major_op, &minor_op);
			if (result != Success)
			{
				Log::singleton() << "Required version of XInput not found; using global X11 inputs";
				mUsingXInput = false;
			}
			else
			{
				XIEventMask globalMasks[1];
				XIEventMask localMasks[1];
				unsigned char globalMask[(XI_LASTEVENT + 7)/8];
				unsigned char localMask[(XI_LASTEVENT + 7)/8];

				memset(globalMask, 0, sizeof(globalMask));
				if (mReceiveInputOutsideFocus)
				{
					// if we listen globally, receive all inputs with root wnd raw inputs
					XISetMask(globalMask, XI_RawKeyPress);
					XISetMask(globalMask, XI_RawKeyRelease);
					XISetMask(globalMask, XI_RawButtonPress);
					XISetMask(globalMask, XI_RawButtonRelease);
				}
				XISetMask(globalMask, XI_RawMotion);// mouse speed is only available as global raw input

				memset(localMask, 0, sizeof(localMask));
				if (!mReceiveInputOutsideFocus)
				{
					// If we listen only locally, receive all available inputs with local window
					XISetMask(localMask, XI_KeyPress);
					XISetMask(localMask, XI_KeyRelease);
					XISetMask(localMask, XI_Motion);
					XISetMask(localMask, XI_ButtonPress);
					XISetMask(localMask, XI_ButtonRelease);
				}

				globalMasks[0].deviceid = XIAllMasterDevices;
				globalMasks[0].mask_len = sizeof(globalMask);
				globalMasks[0].mask = globalMask;

				localMasks[0].deviceid = XIAllMasterDevices;
				localMasks[0].mask_len = sizeof(localMask);
				localMasks[0].mask = localMask;

				XISelectEvents(mDisplay, DefaultRootWindow(mDisplay), globalMasks, 1);
				XISelectEvents(mDisplay, mWindow, localMasks, 1);
    			XFlush(mDisplay);

				mUsingXInput = true;
				mRawMotionEnabled = false;// enable on first RawMotion event
			}
		}

		// Create 'global' devices if not using XInput
		if (!mUsingXInput)
		{
			KeyboardDevice *kbD = new KeyboardDevice(FUSIN_STR("X11 Keyboard"));
			mDeviceEnumerator->registerDevice(kbD);
			mKeyboardDevicesPerID[0] = kbD;
			
			MouseDevice *mD = new MouseDevice(FUSIN_STR("X11 Mouse"), 5);
			mDeviceEnumerator->registerDevice(mD);
			mMouseDevicesPerID[0] = mD;
			
    		XSelectInput(mDisplay, mWindow,
				ExposureMask | 
				KeyPressMask | KeyReleaseMask | 
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask
			);
		}

	}

	X11IOSystem::~X11IOSystem()
	{
		if (mCreatedWindow)
		{
			XDestroyWindow(mDisplay, mWindow);
		}

		for (auto& idKbPair : mMouseDevicesPerID)
		{
			mDeviceEnumerator->unregisterDevice(idKbPair.second);
			delete idKbPair.second;
		}
		for (auto& idMPair : mMouseDevicesPerID)
		{
			mDeviceEnumerator->unregisterDevice(idMPair.second);
			delete idMPair.second;
		}
	}

	void X11IOSystem::updateDeviceList()
	{
		// ---- Find current devies ---- 
		std::set<int> deviceIDs;
		int deviceNum=0;
		XIDeviceInfo *devices = XIQueryDevice(mDisplay, XIAllDevices, &deviceNum);

		for (int i=0; i<deviceNum; i++)
		{
			XIDeviceInfo *dev = &devices[i];
			XIAnyClassInfo **classes = dev->classes;

			if (dev->enabled)
			{
				//Log::singleton() << dev->name << ", index: " << dev->deviceid << "\n";

				deviceIDs.insert(dev->deviceid);

				

				// ---- Add new devices ----
				switch (dev->use)
				{
				case XISlavePointer:
					//Log::singleton() << ", Mouse";
					if (auto it = mMouseDevicesPerID.find(dev->deviceid) == mMouseDevicesPerID.end())
					{
						StringStream ss;
						ss << dev->name;

						MouseDevice *newKbD = new MouseDevice(ss.str(), 5);
						mDeviceEnumerator->registerDevice(newKbD);
						mMouseDevicesPerID[dev->deviceid] = newKbD;

						Log::singleton() << "Mouse Device found:" << dev->name << "\n    Xi index: " << dev->deviceid <<
							//"\n    button number: " << mpDeviceInfo->mouse.dwNumberOfButtons <<
							//"\n    has horizontal wheel: " << mpDeviceInfo->mouse.fHasHorizontalWheel <<
							"\n";
					}
					break;
				//case XIMasterKeyboard:
				case XISlaveKeyboard:
					//Log::singleton() << ", KB";
					if (auto it = mKeyboardDevicesPerID.find(dev->deviceid) == mKeyboardDevicesPerID.end())
					{
						// Get name
						StringStream ss;
						ss << dev->name;

						// Get capabilities
						int keynum, fkeynum=0, lednum=3;
						for (int j = 0; j < dev->num_classes; j++)
						{
							XIAnyClassInfo *cls = classes[j];
							switch(cls->type)
							{
							case XIKeyClass:
							{
								XIKeyClassInfo *keyCls = (XIKeyClassInfo *)cls;
								keyCls->num_keycodes;
								break;
							}
							case XIButtonClass:
							{
								XIButtonClassInfo *butCls = (XIButtonClassInfo *)cls;
								butCls->num_buttons;
								break;
							}
							case XIValuatorClass:
							{
								XIValuatorClassInfo *valCls = (XIValuatorClassInfo *)cls;
								valCls->number;
								break;
							}
							}
						}

						KeyboardDevice *newKbD = new KeyboardDevice(ss.str(), keynum, fkeynum, lednum);
						mDeviceEnumerator->registerDevice(newKbD);
						mKeyboardDevicesPerID[dev->deviceid] = newKbD;

						Log::singleton() << "Keyboard Device found:" << dev->name << "\n    Xi index: " << dev->deviceid <<
							//"\n    key number: " << mpDeviceInfo->keyboard.dwNumberOfKeysTotal <<
							//"\n    function key number: " << mpDeviceInfo->keyboard.dwNumberOfFunctionKeys <<
							//"\n    LED number: " << mpDeviceInfo->keyboard.dwNumberOfIndicators << "\n";
							"\n";
					}
					break;
				/*case XIMasterPointer:

					Log::singleton() << ", Master Mouse";
					break;
				case XIMasterKeyboard:

					Log::singleton() << ", Master KB";
					break;
				case XIFloatingSlave:

					Log::singleton() << ", Floating";
					break;*/
				}
				//Log::singleton() << "\n";
			}
		}

		// ---- Delete unused devices ---- 
		for (auto it = mKeyboardDevicesPerID.begin(); it != mKeyboardDevicesPerID.end(); )
		{
			if (deviceIDs.find(it->first) == deviceIDs.end())// If not found in the current device list
			{
				Log::singleton() << "Keyboard Device removed:" << it->first << "\n";
				mDeviceEnumerator->unregisterDevice(it->second);
				delete it->second;
				mKeyboardDevicesPerID.erase(it++);
				continue;
			}
			it++;
		}
		for (auto it = mMouseDevicesPerID.begin(); it != mMouseDevicesPerID.end(); )
		{
			if (deviceIDs.find(it->first) == deviceIDs.end())// If not found in the current device list
			{				
				Log::singleton() << "Mouse Device removed:" << it->first << "\n";
				mDeviceEnumerator->unregisterDevice(it->second);
				delete it->second;
				mMouseDevicesPerID.erase(it++);
				continue;
			}
			it++;
		}

		// ---- Free memory ---- 
		XIFreeDeviceInfo(devices);
	}

	void X11IOSystem::update()
	{
		// ---- Handle messages ---- 
		if (mCreatedWindow)
		{
			XEvent ev;

			/*while (XCheckWindowEvent(mDisplay, mWindow, -1, &ev))
			{
				handleMessage(&ev);
			}*/
			while (XPending(mDisplay))
			{
				XNextEvent(mDisplay, &ev);
				handleMessage(&ev);
			}
		}
		

		// ---- Update devices ----
		for (auto& idDevPair : mKeyboardDevicesPerID)
		{
			LEDFlags fl = idDevPair.second->leds.nextLedFlags();
			if (fl != LED_AUTO)
			{
				int enabled = 0;
				if (fl & LED_CAPS) enabled |= X11_CAPSLOCK;
				if (fl & LED_NUM) enabled |= X11_NUMLOCK;
				if (fl & LED_SCROLL) enabled |= X11_SCROLL_LOCK;
				XkbLockModifiers(
					mDisplay, idDevPair.first,
					X11_SCROLL_LOCK | X11_CAPSLOCK | X11_NUMLOCK,
					enabled
				);
			}
			else
			{
				XkbLockModifiers(
					mDisplay, idDevPair.first,
					0,
					0
				);
			}
		}
		for (auto& idDevPair : mMouseDevicesPerID)
		{
		}
	}

	void X11IOSystem::handleMessage(XEvent* msg)
	{
		if (!mUsingXInput)
		{
			KeyboardDevice &kbD = *mKeyboardDevicesPerID[0];
			MouseDevice &mD = *mMouseDevicesPerID[0];
			int a, b, c;
			switch (msg->type)
			{
			case KeyPress:
				a = XkbKeycodeToKeysym(mDisplay, msg->xkey.keycode, 0, 1);
				b = XkbKeycodeToKeysym(mDisplay, msg->xkey.keycode, 1, 1);
				c = XKeysymToString(XKeycodeToKeysym(mDisplay, msg->xkey.keycode, 0))[0];
				Log::singleton() << a << " " << b << " " << c << " " << XKeysymToString(XKeycodeToKeysym(mDisplay, msg->xkey.keycode, 0)) << " " << "\n";
				kbD.keys[msg->xkey.keycode].press();
				break;
			case KeyRelease:

				kbD.keys[msg->xkey.keycode].release();
				break;
			case ButtonPress:
				switch (msg->xbutton.button)
				{
				case Button1: 	mD.buttonLeft.press(); break;
				case Button2: 	mD.buttonRight.press(); break;
				case Button3: 	mD.buttonMiddle.press(); break;
				case Button4: 	mD.wheel.scrollUp.press(); break;
				case Button5: 	mD.wheel.scrollDown.press(); break;
				case 6: 		mD.wheel.scrollLeft.press(); break;
				case 7: 		mD.wheel.scrollRight.press(); break;
				case 8: 		mD.button4.press(); break;
				case 9: 		mD.button5.press(); break;
				}
				break;
			case ButtonRelease:
				switch (msg->xbutton.button)
				{
				case Button1: 	mD.buttonLeft.release(); break;
				case Button2: 	mD.buttonRight.release(); break;
				case Button3: 	mD.buttonMiddle.release(); break;
				case 8: 		mD.button4.release(); break;
				case 9: 		mD.button5.release(); break;
				}
				break;
			case EnterNotify:
				mD.movement.setPosition(msg->xcrossing.x, msg->xmotion.y);
				mLastMouseX = msg->xcrossing.x;
				mLastMouseY = msg->xcrossing.y;
				break;
			case MotionNotify:
				mD.movement.simulateMovement(
					msg->xmotion.x - mLastMouseX,
					msg->xmotion.y - mLastMouseY
				);
				mD.movement.setPosition(msg->xmotion.x, msg->xmotion.y);
				mLastMouseX = msg->xcrossing.x;
				mLastMouseY = msg->xcrossing.y;
				break;
			}
		}
		else
		{
			XEvent nonConstEv = *msg;
			if (XGetEventData(mDisplay, &msg->xcookie))
			{
				XGenericEventCookie *cookie = &msg->xcookie;
				if (cookie->type == GenericEvent && cookie->extension == mXIExtOpcode)
				{
					switch (cookie->evtype)
					{
					case XI_KeyPress:
					case XI_RawKeyPress:
					{
						XIRawEvent *keyEvent = (XIRawEvent*)cookie->data;
						auto it = mKeyboardDevicesPerID.find(keyEvent->sourceid);
						if (it != mKeyboardDevicesPerID.end())
						{
							KeyboardDevice &kbD = *it->second;
							KeySym keysym = XkbKeycodeToKeysym(mDisplay, keyEvent->detail, 0, 0);
							Char key = keysymToVirtualKey(keysym);
							Log::singleton() << std::hex << keysym << std::dec << "\n";
							kbD.keys[key].press();
						}
                		
						KeySym keysym = XkbKeycodeToKeysym(mDisplay, keyEvent->detail, 0, 0);
						
						//char cName[20];
						//int overflow;
						/*int a, b, c;
						//char *str = XKeysymToString(keysym);
						//XkbTranslateKeySym(mDisplay, &keysym, 0, cName, sizeof(cName), &overflow);
						a = keysymToVirtualKey(keysym);
						//b = XkbKeycodeToKeysym(mDisplay, keyEvent->detail, 0, 0);
						//c = XkbKeycodeToKeysym(mDisplay, keyEvent->detail, 0, 1);
						Log::singleton() << std::hex << 
							a << '-' << (wchar_t)a << " " << 
							//str << " " <<
							std::dec << "\n";*/
						break;
					}
					case XI_KeyRelease:
					case XI_RawKeyRelease:
					{
						XIRawEvent *keyEvent = (XIRawEvent*)cookie->data;
						auto it = mKeyboardDevicesPerID.find(keyEvent->sourceid);
						if (it != mKeyboardDevicesPerID.end())
						{
							KeyboardDevice &kbD = *it->second;
							KeySym keysym = XkbKeycodeToKeysym(mDisplay, keyEvent->detail, 0, 0);
							kbD.keys[keysymToVirtualKey(keysym)].release();
						}
                		
						//Log::singleton() << keyEvent->sourceid << "\n";
						break;
					}
					case XI_ButtonPress:
					case XI_RawButtonPress:
					{
						XIRawEvent *butEvent = (XIRawEvent*)cookie->data;
						auto it = mMouseDevicesPerID.find(butEvent->sourceid);
						if (it != mMouseDevicesPerID.end())
						{
							MouseDevice &mD = *it->second;
							
							switch (butEvent->detail)
							{
							case Button1: 	mD.buttonLeft.press(); break;
							case Button2: 	mD.buttonRight.press(); break;
							case Button3: 	mD.buttonMiddle.press(); break;
							case Button4: 	mD.wheel.scrollUp.press(); break;
							case Button5: 	mD.wheel.scrollDown.press(); break;
							case 6: 		mD.wheel.scrollLeft.press(); break;
							case 7: 		mD.wheel.scrollRight.press(); break;
							case 8: 		mD.button4.press(); break;
							case 9: 		mD.button5.press(); break;
							}

						}
                		
						break;
					}
					case XI_ButtonRelease:
					case XI_RawButtonRelease:
					{
						XIRawEvent *butEvent = (XIRawEvent*)cookie->data;
						auto it = mMouseDevicesPerID.find(butEvent->sourceid);
						if (it != mMouseDevicesPerID.end())
						{
							MouseDevice &mD = *it->second;
							
							switch (butEvent->detail)
							{
							case Button1: 	mD.buttonLeft.release(); break;
							case Button2: 	mD.buttonRight.release(); break;
							case Button3: 	mD.buttonMiddle.release(); break;
							case 8: 		mD.button4.release(); break;
							case 9: 		mD.button5.release(); break;
							}
						}
						break;
					}
					case XI_Motion:// Normal motion is for position change inside window
					{
						if (!mRawMotionEnabled)// ignore if using raw motion
						{
							XIDeviceEvent *motionEvent = (XIDeviceEvent*)cookie->data;
							
							auto it = mMouseDevicesPerID.find(motionEvent->sourceid);
							if (it != mMouseDevicesPerID.end())
							{
								MouseDevice &mD = *it->second;
								
								mD.movement.simulateMovement(
									motionEvent->event_x - mLastMouseX,
									motionEvent->event_y - mLastMouseY
								);
								mD.movement.setPosition(motionEvent->event_x, motionEvent->event_y);
								mLastMouseX = motionEvent->event_x;
								mLastMouseY = motionEvent->event_y;
								Log::singleton() << motionEvent->sourceid << " " << (motionEvent->event_x ) << "\n";
							}
						}
						break;
					}
					case XI_RawMotion:// Raw motion is for pointer speed
					{
						mRawMotionEnabled = true;
						XIDeviceEvent *motionEvent = (XIDeviceEvent*)cookie->data;
							
						// Get window pos
						/*XWindowAttributes xwa;
						Window child;
						int x, y, w, h;
						XTranslateCoordinates(mDisplay, mWindow, RootWindow(mDisplay, 0), 0, 0, &x, &y, &child );
						XGetWindowAttributes(mDisplay, mWindow, &xwa);
						x -= xwa.x;
						y -= xwa.y;
						w = xwa.width;
						h = xwa.height;*/

						auto it = mMouseDevicesPerID.find(motionEvent->sourceid);
						if (it != mMouseDevicesPerID.end())
						{
							MouseDevice &mD = *it->second;

							// Get window geometry
							XWindowAttributes xwa;
							XGetWindowAttributes(mDisplay, mWindow, &xwa);
						
							// Get cursor position
							Window childWnd, rootWnd, parentDummy;
							int root_x, root_y, win_x, win_y;
							unsigned int maskDummy;
							XQueryPointer(mDisplay, mWindow, &rootWnd, &childWnd,
								&root_x, &root_y, &win_x, &win_y, &maskDummy);
							
							// Event
							if (mReceiveInputOutsideFocus || 
								(win_x >= 0 && win_x <= xwa.width && win_y >= 0 && win_y <= xwa.height))
							{
								mD.movement.setPosition(win_x, win_y);
								mD.movement.simulateMovement(motionEvent->event_x, motionEvent->event_y);
								Log::singleton() << motionEvent->sourceid << " " << (win_x) << "\n";
							}
						}
						break;
					}
					/*case XI_DeviceChanged:
					{
                		XIDeviceChangedEvent *chngEvent = (XIDeviceChangedEvent*)cookie->data;
						Log::singleton() << chngEvent->sourceid << "\n";
						break;
					}*/
					}
				}
			}
			
			XFreeEventData(mDisplay, &msg->xcookie);
		}
	}

}

#endif//FUSIN_BUILD_X11