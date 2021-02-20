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

#include <algorithm>
#include <locale>
#include <codecvt>
#include <iostream>
#include <iostream>
#include <string.h>
#include <set>

#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>

namespace Fusin
{
	#define GET_CFG(VAR, KEY, DEFAULT) VAR = getCfgValue(config, FUSIN_STR(KEY), DEFAULT)

	X11IOSystem::X11IOSystem():
		IOSystem(IOF_ANY)
	{
	}

	void X11IOSystem::initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
	{
		IOSystem::initialize(de, config, window);
		mWindow = ((Window)window);

		// create the window
		if (!mWindow)
		{
			mDisplay = XOpenDisplay(nullptr);
			int scrId = DefaultScreen(mDisplay);

			mWindow = XCreateSimpleWindow(mDisplay, XRootWindow(mDisplay, scrId),
				10, 10, 400, 200, 1, BlackPixel(mDisplay, scrId), WhitePixel(mDisplay, scrId));
  			/*XSelectInput(mDisplay, mWindow, 
				ExposureMask | KeyPressMask | KeyReleaseMask | 
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask
			);*/
    		/*XSelectInput(mDisplay, mWindow,
				ExposureMask | 
				KeyPressMask | KeyReleaseMask | 
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask
			);*/
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
				XIEventMask masks[1];
				unsigned char mask[(XI_LASTEVENT + 7)/8];
				//unsigned long long mask;

				memset(mask, 0, sizeof(mask));
				/*XISetMask(mask, XI_RawKeyPress);
				XISetMask(mask, XI_RawKeyRelease);
				XISetMask(mask, XI_RawMotion);
				XISetMask(mask, XI_Motion);
				XISetMask(mask, XI_RawButtonPress);
				XISetMask(mask, XI_RawButtonRelease);
				XISetMask(mask, XI_DeviceChanged);*/
				XISetMask(mask, XI_KeyPress);
				XISetMask(mask, XI_KeyRelease);
				XISetMask(mask, XI_Motion);
				XISetMask(mask, XI_ButtonPress);
				XISetMask(mask, XI_ButtonRelease);
				XISetMask(mask, XI_DeviceChanged);
				/*mask = XI_KeyPressMask | XI_KeyReleaseMask |
					XI_RawMotionMask | XI_ButtonPressMask | XI_ButtonReleaseMask |
					XI_DeviceChangedMask;*/

				masks[0].deviceid = XIAllMasterDevices;
				masks[0].mask_len = sizeof(mask);
				masks[0].mask = mask;

				//XISelectEvents(mDisplay, DefaultRootWindow(mDisplay), masks, 1);
				XISelectEvents(mDisplay, mWindow, masks, 1);
    			XFlush(mDisplay);

				mUsingXInput = true;
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
		int deviceNum;
		XIDeviceInfo *devices = XIQueryDevice(mDisplay, XIAllDevices, &deviceNum);

		for (int i=0; i<deviceNum; i++)
		{
			if (devices[i].enabled)
			{
				XIDeviceInfo& dev = devices[i];

				deviceIDs.insert(dev.deviceid);

				// ---- Add new devices ---- 
				switch (dev.use)
				{
				case XISlavePointer:
					if (auto it = mMouseDevicesPerID.find(dev.deviceid) == mMouseDevicesPerID.end())
					{
						StringStream ss;
						ss << dev.name;

						MouseDevice *newKbD = new MouseDevice(ss.str(), 5);
						mDeviceEnumerator->registerDevice(newKbD);
						mMouseDevicesPerID[dev.deviceid] = newKbD;
										
						Log::singleton() << "Mouse Device found:" << dev.name <<
							"\n    Xi index: " << dev.deviceid <<
							//"\n    button number: " << mpDeviceInfo->mouse.dwNumberOfButtons <<
							//"\n    has horizontal wheel: " << mpDeviceInfo->mouse.fHasHorizontalWheel <<
							"\n";
					}
					break;
				case XISlaveKeyboard:
					if (auto it = mKeyboardDevicesPerID.find(dev.deviceid) == mKeyboardDevicesPerID.end())
					{
						StringStream ss;
						ss << dev.name;

						KeyboardDevice *newKbD = new KeyboardDevice(ss.str());
						mDeviceEnumerator->registerDevice(newKbD);
						mKeyboardDevicesPerID[dev.deviceid] = newKbD;
										
						Log::singleton() << "Keyboard Device found:" << dev.name <<
							"\n    Xi index: " << dev.deviceid <<
							//"\n    key number: " << mpDeviceInfo->keyboard.dwNumberOfKeysTotal <<
							//"\n    function key number: " << mpDeviceInfo->keyboard.dwNumberOfFunctionKeys <<
							//"\n    LED number: " << mpDeviceInfo->keyboard.dwNumberOfIndicators << "\n";
							"\n";
					}
					break;
				case XIFloatingSlave:
					break;
				}
			}
		}

		// ---- Delete unused devices ---- 
		for (auto it = mKeyboardDevicesPerID.begin(); it != mKeyboardDevicesPerID.end(); )
		{
			if (deviceIDs.find(it->first) == deviceIDs.end())// If not found in the current device list
			{
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
	}

	void X11IOSystem::handleMessage(XEvent* msg)
	{
		if (!mUsingXInput)
		{
			KeyboardDevice* kbD = mKeyboardDevicesPerID[0];
			MouseDevice* mD = mMouseDevicesPerID[0];
			int a, b, c;
			switch (msg->type)
			{
			case KeyPress:
				a = XkbKeycodeToKeysym(mDisplay, msg->xkey.keycode, 0, 1);
				b = XkbKeycodeToKeysym(mDisplay, msg->xkey.keycode, 1, 1);
				c = XKeysymToString(XKeycodeToKeysym(mDisplay, msg->xkey.keycode, 0))[0];
				Log::singleton() << a << " " << b << " " << c << " " << XKeysymToString(XKeycodeToKeysym(mDisplay, msg->xkey.keycode, 0)) << " " << "\n";
				kbD->keys[msg->xkey.keycode].press();
				break;
			case KeyRelease:
				kbD->keys[msg->xkey.keycode].release();
				break;
			case ButtonPress:
				switch (msg->xbutton.button)
				{
				case Button1:
					(*mD).buttonLeft.press();
					break;
				case Button2:
					(*mD).buttonRight.press();
					break;
				case Button3:
					(*mD).buttonMiddle.press();
					break;
				case Button4:
					(*mD).wheel.scrollUp.press();
					break;
				case Button5:
					(*mD).wheel.scrollDown.press();
					break;
				case 6:
					(*mD).wheel.scrollLeft.press();
					break;
				case 7:
					(*mD).wheel.scrollRight.press();
					break;
				case 8:
					(*mD).button4.press();
					break;
				case 9:
					(*mD).button5.press();
					break;
				}
				break;
			case ButtonRelease:
				switch (msg->xbutton.button)
				{
				case Button1:
					(*mD).buttonLeft.release();
					break;
				case Button2:
					(*mD).buttonRight.release();
					break;
				case Button3:
					(*mD).buttonMiddle.release();
					break;
				case 8:
					(*mD).button4.release();
					break;
				case 9:
					(*mD).button5.release();
					break;
				}
				break;
			case EnterNotify:
				(*mD).movement.setPosition(msg->xcrossing.x, msg->xmotion.y);
				mLastMouseX = msg->xcrossing.x;
				mLastMouseY = msg->xcrossing.y;
				break;
			case MotionNotify:
				(*mD).movement.simulateMovement(
					msg->xmotion.x - mLastMouseX,
					msg->xmotion.y - mLastMouseY
				);
				(*mD).movement.setPosition(msg->xmotion.x, msg->xmotion.y);
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
					{
                		XIRawEvent *keyEvent = (XIRawEvent*)cookie->data;
						int a, b, c;
						a = XkbKeycodeToKeysym(mDisplay, keyEvent->detail, 0, 1);
						b = XkbKeycodeToKeysym(mDisplay, keyEvent->detail, 1, 1);
						c = XKeysymToString(XKeycodeToKeysym(mDisplay, keyEvent->detail, 0))[0];
						Log::singleton() << a << " " << b << " " << c << " " << XKeysymToString(XKeycodeToKeysym(mDisplay, keyEvent->detail, 0)) << " " << "\n";
						//(*kbD).keys[msg->xkey.keycode].press();
						break;
					}
					case XI_KeyRelease:
                		{XIRawEvent *motionEvent = (XIRawEvent*)cookie->data;
						//(*kbD).keys[msg->xkey.keycode].release();
						break;}
					case XI_ButtonPress:
                		{XIDeviceEvent *motionEvent = (XIDeviceEvent*)cookie->data;
						/*switch (msg->xbutton.button)
						{
						case Button1:
							(*mD).buttonLeft.press();
							break;
						case Button2:
							(*mD).buttonRight.press();
							break;
						case Button3:
							(*mD).buttonMiddle.press();
							break;
						case Button4:
							(*mD).wheel.scrollUp.press();
							break;
						case Button5:
							(*mD).wheel.scrollDown.press();
							break;
						case 6:
							(*mD).wheel.scrollLeft.press();
							break;
						case 7:
							(*mD).wheel.scrollRight.press();
							break;
						case 8:
							(*mD).button4.press();
							break;
						case 9:
							(*mD).button5.press();
							break;
						}*/
						break;}
					case XI_ButtonRelease:
                		{XIDeviceEvent *motionEvent = (XIDeviceEvent*)cookie->data;
						/*switch (msg->xbutton.button)
						{
						case Button1:
							(*mD).buttonLeft.release();
							break;
						case Button2:
							(*mD).buttonRight.release();
							break;
						case Button3:
							(*mD).buttonMiddle.release();
							break;
						case 8:
							(*mD).button4.release();
							break;
						case 9:
							(*mD).button5.release();
							break;
						}*/
						break;}
					case XI_Motion:
					{
						XWindowAttributes xwa;
						Window child;
						int x, y;
						XTranslateCoordinates(mDisplay, mWindow, RootWindow(mDisplay, 0), 0, 0, &x, &y, &child );
						XGetWindowAttributes(mDisplay, mWindow, &xwa);
						x -= xwa.x;
						y -= xwa.y;

                		XIDeviceEvent *motionEvent = (XIDeviceEvent*)cookie->data;
						Log::singleton() << motionEvent->sourceid << " " << (motionEvent->root_x - x) << "\n";
						/*(*mD).movement.simulateMovement(
							msg->xmotion.x - mLastMouseX,
							msg->xmotion.y - mLastMouseY
						);
						(*mD).movement.setPosition(msg->xmotion.x, msg->xmotion.y);
						mLastMouseX = msg->xcrossing.x;
						mLastMouseY = msg->xcrossing.y;*/
						break;
					}
					case XI_DeviceChanged:
					{
                		XIDeviceChangedEvent *chngEvent = (XIDeviceChangedEvent*)cookie->data;
						Log::singleton() << chngEvent->sourceid << "\n";
						/*(*mD).movement.simulateMovement(
							msg->xmotion.x - mLastMouseX,
							msg->xmotion.y - mLastMouseY
						);
						(*mD).movement.setPosition(msg->xmotion.x, msg->xmotion.y);
						mLastMouseX = msg->xcrossing.x;
						mLastMouseY = msg->xcrossing.y;*/
						break;
					}
					}
				}
			}
			
			XFreeEventData(mDisplay, &msg->xcookie);
		}
	}

}

#endif//FUSIN_BUILD_X11