#include "IOSystems/FusinRawInputIOSystem.h"

#ifdef FUSIN_BUILD_RAW_INPUT

#include "Devices/FusinDeviceEnumerator.h"

#include "IOCodes/FusinKey.h"
#include "IOCodes/FusinMouse.h"
#include "IOCodes/FusinGamepad.h"
#include "IOCodes/FusinDS.h"
#include "IOCodes/FusinXInput.h"
#include "IOCodes/FusinNintendo.h"

#include "IOSystems/RawInput/FusinRawInputKeyboardHandler.h"
#include "IOSystems/RawInput/FusinRawInputMouseHandler.h"
#include "IOSystems/RawInput/FusinRawInputGamepadHandler.h"
#include "IOSystems/RawInput/FusinRawInputDS3Handler.h"
#include "IOSystems/RawInput/FusinRawInputDS4Handler.h"
#include "IOSystems/RawInput/FusinRawInputJoyConHandler.h"

#include "Utilities/FusinAlgorithm.h"
#include "Utilities/FusinConfigMap.h"
#include "Utilities/FusinVendors.h"

#include <algorithm>
#include <locale>
#include <codecvt>
#include <iostream>

namespace Fusin
{
	#define GET_CFG(VAR, KEY, DEFAULT) VAR = getCfgValue(config, FUSIN_STR(KEY), DEFAULT)

	RawInputIOSystem::RawInputIOSystem():
		IOSystem(IO_ANY)
	{
	}

	void RawInputIOSystem::initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
	{
		IOSystem::initialize(de, config, window);
		mWindow = ((HWND)window);

		// Set config
		GET_CFG(mReceiveInputOutsideFocus, "Receive input outside focus", false);
		GET_CFG(mSupportDS, "Support DS", true);
		GET_CFG(mSupportXInput, "Support XInput", false);
		GET_CFG(mSupportNintendo, "Support Nintendo", true);
		GET_CFG(mMaxGamepadMessages, "Max gamepad messages", 0);

		// create the window
		if (!mWindow)
		{
			WNDCLASS wc = { 0 };
			wc.lpfnWndProc = DefWindowProc;
			wc.hInstance = GetModuleHandle(0);
			wc.lpszClassName = "RawInputWindowClass";
			RegisterClass(&wc);

			mWindow = CreateWindow(wc.lpszClassName, "RawInputWindow", 0, 0, 0, 100, 100, HWND_MESSAGE/*(HWND)NULL*/, NULL, GetModuleHandle(0), NULL);
			ShowWindow(mWindow, SW_HIDE);//
			mCreatedHiddenWindow = true;
		}
		else
		{
			mCreatedHiddenWindow = false;
		}

		// register raw input devices
		mRIDevices = new RAWINPUTDEVICE[4];
		DWORD flag = RIDEV_INPUTSINK * (mReceiveInputOutsideFocus || mCreatedHiddenWindow);
		DWORD mkFlag = RIDEV_NOLEGACY * mCreatedHiddenWindow*0;// ignore legacy messages and receive system wide keystrokes

		mRIDevices[0].dwFlags = flag | mkFlag;
		mRIDevices[0].usUsagePage = 1;
		mRIDevices[0].usUsage = 6;    			// keyboard
		mRIDevices[0].hwndTarget = mWindow;

		mRIDevices[1].dwFlags = flag | mkFlag;
		mRIDevices[1].usUsagePage = 1;
		mRIDevices[1].usUsage = 2;    			// mouse
		mRIDevices[1].hwndTarget = mWindow;

		mRIDevices[2].dwFlags = flag;
		mRIDevices[2].usUsagePage = 1;
		mRIDevices[2].usUsage = 4;    			// joystick
		mRIDevices[2].hwndTarget = mWindow;

		mRIDevices[3].dwFlags = flag;
		mRIDevices[3].usUsagePage = 1;
		mRIDevices[3].usUsage = 5;    			// other gamepads (DS/XInput controller)
		mRIDevices[3].hwndTarget = mWindow;

		RegisterRawInputDevices(mRIDevices, 4, sizeof(RAWINPUTDEVICE));

	}

	RawInputIOSystem::~RawInputIOSystem()
	{
		if (mCreatedHiddenWindow)
		{
			DestroyWindow(mWindow);
		}

		delete[] mRIDevices;

		for (auto it = mHandlerPerHandle.begin(); it != mHandlerPerHandle.end(); it++)
		{
			if ((*it).second)
			{
				mDeviceEnumerator->unregisterDevice((*it).second->fusinDevice());
				delete (*it).second->fusinDevice();
				delete (*it).second;
			}
		}
	}

	void RawInputIOSystem::updateDeviceList()
	{
		PRAWINPUTDEVICELIST pRIDList;
		UINT deviceCount = 0;
		UINT ridListTypeSize = sizeof(RAWINPUTDEVICELIST);
		/*
		list containing the devices that are currently active.
		handles that are found in the mHandlerPerHandle but not in activeHandles will be removed at the end of the _update.
		*/
		std::list<HANDLE> activeHandles;

		// get the number of RI devices
		if (GetRawInputDeviceList(NULL, &deviceCount, ridListTypeSize) == -1)
			return;

		// create a raw input device list buffer of the correct size
		pRIDList = (PRAWINPUTDEVICELIST) new BYTE[deviceCount*ridListTypeSize];
		if (!pRIDList)
			return;

		// get the RI devices
		if (GetRawInputDeviceList(pRIDList, &deviceCount, ridListTypeSize) != -1)
		{
			for (size_t i = 0; i < deviceCount; i++)
			{
				PRAWINPUTDEVICELIST pRID = (pRIDList + i);
				HANDLE handle = pRID->hDevice;
				PRID_DEVICE_INFO pDInfo;
				UINT infoSize;

				// add it to the currently active list so it won't be deleted at the end of the _update
				activeHandles.push_back(handle);

				// if it hasn't been detected yet, create a DeviceHandler for it
				if (mHandlerPerHandle.find(handle) == mHandlerPerHandle.end())
				{
					// Get other device info - size
					if (GetRawInputDeviceInfo(handle, RIDI_DEVICEINFO, NULL, &infoSize) == -1)
						continue;// just ignore the device if we can't find info
						
					pDInfo = (PRID_DEVICE_INFO) new BYTE[infoSize];
					if (!pDInfo)
						return;// likely a bigger problem
						
					// Actually get other device info and pass it to the new handler
					if (GetRawInputDeviceInfo(handle, RIDI_DEVICEINFO, pDInfo, &infoSize) != -1)
					{
						RawInputDeviceHandler *newDeviceHandler = nullptr;

						// Create the new device handler
						switch (pDInfo->dwType)
						{
						case RIM_TYPEKEYBOARD:
							newDeviceHandler = new RawInputKeyboardHandler(handle, pDInfo);
							break;
						case RIM_TYPEMOUSE:
							newDeviceHandler = new RawInputMouseHandler(handle, pDInfo);
							break;
						case RIM_TYPEHID:
							if (pDInfo->hid.usUsagePage == 1 && (pDInfo->hid.usUsage == 4 || pDInfo->hid.usUsage == 5))
							{
								// check the kind of the gamepad device
								switch (pDInfo->hid.dwVendorId)
								{
								case DS_VENDOR_ID:// dualshock
									if (quickFind(DS3_PRODUCT_IDS, pDInfo->hid.dwProductId))// ds3
									{
										newDeviceHandler = new RawInputDS3Handler(handle, pDInfo);
									}
									else if (quickFind(DS4_PRODUCT_IDS, pDInfo->hid.dwProductId))// ds4
									{
										newDeviceHandler = new RawInputDS4Handler(handle, pDInfo);
									}
									break;

								case XINPUT_VENDOR_ID:// XInput
									break;

								case NINTENDO_VENDOR_ID:// Nintendo
									if (quickFind(JOYCON_PRODUCT_IDS, pDInfo->hid.dwProductId))// joy-con
									{
										newDeviceHandler = new RawInputJoyConHandler(handle, pDInfo, pDInfo->hid.dwProductId == JOYCON_PRODUCT_ID_RIGHT);
									}
									break;

								default:// generic gamepad
									newDeviceHandler = new RawInputGamepadHandler(handle, pDInfo);
									break;
								}
							}
							break;
						}

						// if we created a device handler
						if (newDeviceHandler)
						{
						// add it to the map
							if (newDeviceHandler->initialize())
							{
								mHandlerPerHandle.insert(std::make_pair(handle, newDeviceHandler));
								mDeviceEnumerator->registerDevice(newDeviceHandler->fusinDevice());

								//wchar_t serial[128];
								//if (!HidD_GetSerialNumberString(riDevHandler->hidDevice(), serial, 128 * sizeof(wchar_t))) serial[0] = '\0';
								//std::wcout << "    Ind: " << dev->index() << " RIHandle:" << deviceHandle /*<< " Serial:" << serial*/ << "\n";
							}
							else
							{
								delete newDeviceHandler;
								mHandlerPerHandle.insert(std::make_pair(handle, nullptr));// prevent it from recreating the device each _update
							}
						}
					}

					delete[] pDInfo;
				}
			}
		}

		delete[] pRIDList;

		// Remove device handlers that don't have a handle anymore on the handle list;
		for (auto it = mHandlerPerHandle.begin(); it != mHandlerPerHandle.end();)
		{
			if (!quickFind(activeHandles, (*it).first))
			{
				if ((*it).second)
				{
					mDeviceEnumerator->unregisterDevice((*it).second->fusinDevice());
					delete (*it).second->fusinDevice();
					delete (*it).second;
				}
				mHandlerPerHandle.erase(it++);
			}
			else
			{
				it++;
			}
		}

	}

	void RawInputIOSystem::update()
	{
		MSG msg = { 0 };

		// Handle messages
		if (mCreatedHiddenWindow)
		{
			mMessageCounterPerHandle.clear();
			while (PeekMessage(&msg, mWindow, 0, 0, PM_REMOVE) != 0)
			{
				handleMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// Update Fusin Devices through the device handlers
		for (auto& handleHandlerPair : mHandlerPerHandle)
		{
			if (handleHandlerPair.second)
			{
				handleHandlerPair.second->update();
			}
		}
	}

	void RawInputIOSystem::handleMessage(const MSG* msg)
	{
		UINT message = msg->message;
		WPARAM wParam = msg->wParam;
		LPARAM lParam = msg->lParam;

		if (message == WM_INPUT)
		{
			PRAWINPUT pRawInput;
			UINT      bufferSize;

			// get the buffer size
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));

			// create a raw input data buffer of the correct size
			pRawInput = (PRAWINPUT) new BYTE[bufferSize];
			if (!pRawInput)
				return;

			// get the data itself
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pRawInput, &bufferSize, sizeof(RAWINPUTHEADER));

			// let the correct device handler handle the message
			auto it = mHandlerPerHandle.find(pRawInput->header.hDevice);
			if (it != mHandlerPerHandle.end() && (*it).second)
			{
				(*it).second->handleRawInput(pRawInput);
			}

			// free memory
			delete[] pRawInput;
		}

	}

}

#endif//FUSIN_BUILD_RAW_INPUT