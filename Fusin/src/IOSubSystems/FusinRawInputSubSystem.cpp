#include "FusinRawInputSystem.h"
#include "FusinDeviceEnumerator.h"
#include "RawInput/FusinRawInputKeyboardHandler.h"
#include "RawInput/FusinRawInputMouseHandler.h"
#include "RawInput/FusinRawInputGamepadHandler.h"
#include "RawInput/FusinRawInputDS3Handler.h"
#include "RawInput/FusinRawInputDS4Handler.h"
#include "RawInput/FusinRawInputJoyConHandler.h"
/*#include "FusinKeyboardDevice.h"
#include "FusinMouseDevice.h"
#include "FusinGamepadDevice.h"
#include "FusinDSDevice.h"
#include "FusinXBoxDevice.h"
#include "FusinNintendoDevice.h"*/
#include "FusinKey.h"
#include "FusinMouse.h"
#include "FusinGamepad.h"
#include "FusinDS.h"
#include "FusinXBox.h"
#include "FusinNintendo.h"
#include <algorithm>
#include <locale>
#include <codecvt>
#include <iostream>

#define SET_BOOL_CFG(VAR, KEY, DEFAULT) cfgIt = config.find(FUSIN_STR(KEY)); if (cfgIt != config.cend()) VAR = (cfgIt->second == FUSIN_STR("True")); else VAR = DEFAULT;
#define SET_OTHER_CFG(VAR, KEY, DEFAULT) cfgIt = config.find(FUSIN_STR(KEY)); if (cfgIt != config.cend()) {ss << cfgIt->second; ss >> VAR; ss.str(String());} else VAR = DEFAULT;

namespace Fusin
{

	RawInputSystem::RawInputSystem(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
		: InputSystem(de)
		, mWindow((HWND)window)
	{
		mTypes = IT_ANY;

		// Set config
		std::map<String, String>::const_iterator cfgIt;
		StringStream ss;
		SET_BOOL_CFG(mReceiveInputOutsideFocus, "Receive input outside focus", false);
		SET_BOOL_CFG(mSupportDS, "Support DS", true);
		SET_BOOL_CFG(mSupportXBox, "Support XBox", true);
		SET_BOOL_CFG(mSupportNintendo, "Support Nintendo", true);
		SET_OTHER_CFG(mMaxGamepadMessages, "Max gamepad messages", 0);

		// create the window
		if (!mWindow)
		{
			WNDCLASS wc = { 0 };
			wc.lpfnWndProc = DefWindowProc;
			wc.hInstance = GetModuleHandle(0);
			wc.lpszClassName = L"RawInputWindowClass";
			RegisterClass(&wc);

			mWindow = CreateWindow(wc.lpszClassName, L"RawInputWindow", 0, 0, 0, 100, 100, HWND_MESSAGE/*(HWND)NULL*/, NULL, GetModuleHandle(0), NULL);
			ShowWindow(mWindow, SW_HIDE);//
			mCreatedHiddenWindow = true;
		}
		else
		{
			mCreatedHiddenWindow = false;
		}

		// register raw ioType devices
		mRIDevices = new RAWINPUTDEVICE[4];
		DWORD flag = RIDEV_INPUTSINK * (mReceiveInputOutsideFocus || mCreatedHiddenWindow);
		DWORD mkFlag = RIDEV_NOLEGACY * mCreatedHiddenWindow*0;

		mRIDevices[0].dwFlags = flag | mkFlag;	// ignore legacy messages and receive system wide keystrokes-RIDEV_NOLEGACY	 | flag
		mRIDevices[0].usUsagePage = 1;
		mRIDevices[0].usUsage = 6;    							// keyboard
		mRIDevices[0].hwndTarget = mWindow;

		mRIDevices[1].dwFlags = flag | mkFlag;
		mRIDevices[1].usUsagePage = 1;
		mRIDevices[1].usUsage = 2;    							// mouse
		mRIDevices[1].hwndTarget = mWindow;

		mRIDevices[2].dwFlags = flag;
		mRIDevices[2].usUsagePage = 1;
		mRIDevices[2].usUsage = 4;    							// joystick
		mRIDevices[2].hwndTarget = mWindow;

		mRIDevices[3].dwFlags = flag;
		mRIDevices[3].usUsagePage = 1;
		mRIDevices[3].usUsage = 5;    							// other gamepads (DS/XBox controller)
		mRIDevices[3].hwndTarget = mWindow;

		RegisterRawInputDevices(mRIDevices, 4, sizeof(RAWINPUTDEVICE));

	}

	RawInputSystem::~RawInputSystem()
	{
		if (mCreatedHiddenWindow)
		{
			DestroyWindow(mWindow);
		}

		delete[] mRIDevices;

		for (auto it = mHandleMap.begin(); it != mHandleMap.end(); it++)
		{
			if ((*it).second)
			{
				mDeviceEnumerator->unregisterDevice((*it).second->device());
				delete (*it).second->device();
				delete (*it).second;
			}
		}
	}

	void RawInputSystem::updateDeviceList()
	{
		PRAWINPUTDEVICELIST pRIDList;
		UINT deviceCount = 0;
		UINT ridListTypeSize = sizeof(RAWINPUTDEVICELIST);
		/* list containing the devices that are currently active.
		devices that are found in the mHandleMap but not in currentHandles will be removed at the end of the _update.
		*/
		std::list<HANDLE> activeHandles;

		// get the number of RI devices
		if (GetRawInputDeviceList(NULL, &deviceCount, ridListTypeSize) == -1)
			return;

		// create a raw ioType device list buffer of the correct size
		pRIDList = (PRAWINPUTDEVICELIST) new BYTE[deviceCount*ridListTypeSize];
		if (!pRIDList)
			return;

		// get the RI devices
		if (GetRawInputDeviceList(pRIDList, &deviceCount, ridListTypeSize) != -1)
		{
			for (int i = 0; i < deviceCount; i++)
			{
				PRAWINPUTDEVICELIST pRID = (pRIDList + i);
				HANDLE deviceHandle = pRID->hDevice;
				PRID_DEVICE_INFO pDInfo;
				UINT infoSize;

				// add it to the currently active list so it won't be deleted at the end of the _update
				activeHandles.push_back(deviceHandle);

				// if it hasn't been detected yet, create a DeviceHandler for it
				if (mHandleMap.find(deviceHandle) == mHandleMap.end())
				{
					// Get other device info
					if (GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICEINFO, NULL, &infoSize) != -1)
					{
						pDInfo = (PRID_DEVICE_INFO) new BYTE[infoSize];
						if (!pDInfo)
							return;
						if (GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICEINFO, pDInfo, &infoSize) != -1)
						{
							RawInputDeviceHandler *deviceHandler = nullptr;

							switch (pDInfo->dwType)
							{
							case RIM_TYPEKEYBOARD:
								deviceHandler = new RawInputKeyboardHandler(deviceHandle, pDInfo);
								break;
							case RIM_TYPEMOUSE:
								deviceHandler = new RawInputMouseHandler(deviceHandle, pDInfo);
								break;
							case RIM_TYPEHID:
								if (pDInfo->hid.usUsagePage == 1 && (pDInfo->hid.usUsage == 4 || pDInfo->hid.usUsage == 5))
								{
									// check the kind of the gamepad device
									if (pDInfo->hid.dwVendorId == DS_VENDOR_ID)// dualshock
									{
										if (std::find(DS3_PRODUCT_IDS.begin(), DS3_PRODUCT_IDS.end(), pDInfo->hid.dwProductId) != DS3_PRODUCT_IDS.end())// ds3
										{
											deviceHandler = new RawInputDS3Handler(deviceHandle, pDInfo);
										}
										else if (std::find(DS4_PRODUCT_IDS.begin(), DS4_PRODUCT_IDS.end(), pDInfo->hid.dwProductId) != DS4_PRODUCT_IDS.end())// ds4
										{
											deviceHandler = new RawInputDS4Handler(deviceHandle, pDInfo);
										}
									}
									else if (pDInfo->hid.dwVendorId == XBOX_VENDOR_ID)// xbox
									{

									}
									else if (pDInfo->hid.dwVendorId == NINTENDO_VENDOR_ID)// xbox
									{
										if (std::find(JOYCON_PRODUCT_IDS.begin(), JOYCON_PRODUCT_IDS.end(), pDInfo->hid.dwProductId) != JOYCON_PRODUCT_IDS.end())// joy-con
										{
											deviceHandler = new RawInputJoyConHandler(deviceHandle, pDInfo, pDInfo->hid.dwProductId == 0x2007);
										}
									}
									else// generic gamepad
									{
										deviceHandler = new RawInputGamepadHandler(deviceHandle, pDInfo);
									}
								}
								break;
							}

							// add it to the map
							if (deviceHandler)
							{
								if (deviceHandler->isSuccessful())
								{
									mHandleMap.insert(std::make_pair(deviceHandle, deviceHandler));
									mDeviceEnumerator->registerDevice(deviceHandler->device());

									//wchar_t serial[128];
									//if (!HidD_GetSerialNumberString(riDevHandler->hidDevice(), serial, 128 * sizeof(wchar_t))) serial[0] = '\0';
									//std::wcout << "    Ind: " << dev->index() << " RIHandle:" << deviceHandle /*<< " Serial:" << serial*/ << "\n";
								}
								else
								{
									delete deviceHandler;
									mHandleMap.insert(std::make_pair(deviceHandle, nullptr));// prevent it from recreating the device each _update
								}
							}
						}

						delete[] pDInfo;
					}
				}
			}
		}

		delete[] pRIDList;

		// Update Fusin Devices and remove those that can't be found on the handle list;
		for (auto it = mHandleMap.begin(); it != mHandleMap.end();)
		{
			if (std::find(activeHandles.begin(), activeHandles.end(), (*it).first) == activeHandles.end())
			{
				if ((*it).second)
				{
					mDeviceEnumerator->unregisterDevice((*it).second->device());
					delete (*it).second->device();
					delete (*it).second;
				}
				mHandleMap.erase(it++);
				continue;
			}
			else
			{
				if ((*it).second)
				{
					(*it).second->update();
				}
			}
			it++;
		}

	}

	void RawInputSystem::update()
	{
		MSG msg = { 0 };

		// Handle messages
		if (mCreatedHiddenWindow)
		{
			mHandleMessageCounter.clear();
			while (PeekMessage(&msg, mWindow, 0, 0, PM_REMOVE) != 0)
			{
				handleMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	void RawInputSystem::handleMessage(const MSG* msg)
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

			// create a raw ioType data buffer of the correct size
			pRawInput = (PRAWINPUT) new BYTE[bufferSize];
			if (!pRawInput)
				return;

			// get the data itself
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pRawInput, &bufferSize, sizeof(RAWINPUTHEADER));

			// let the correct device handler handle the message
			auto it = mHandleMap.find(pRawInput->header.hDevice);
			if (it != mHandleMap.end() && (*it).second)
			{
				(*it).second->handleRawInput(pRawInput);
			}

			// free memory
			delete[] pRawInput;
		}

	}

}