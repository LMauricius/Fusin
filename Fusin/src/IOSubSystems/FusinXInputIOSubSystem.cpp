#include "FusinXInputSystem.h"
#include "FusinInputManager.h"
#include "FusinXInput.h"
#include "FusinXInputDevice.h"
#include <Windows.h>
#include <XInput.h>
#include "Utilities/FusinLog.h"

namespace Fusin
{

	XInputSystem::XInputSystem(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
		: InputSystem(de)
	{
		mTypes = IO_XInput;
		mDevices.assign(XUSER_MAX_COUNT, nullptr);
	}

	XInputSystem::~XInputSystem()
	{
		for (auto dev : mDevices)
		{
			if (dev)
			{
				//mInputManager->enumerator.unregisterDevice(dev);
				delete dev;
			}
		}
	}

	void XInputSystem::update()
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (mDevices[i] || true)
			{
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));
				DWORD result = XInputGetState(i, &state);

				if (result == ERROR_SUCCESS)
				{
					if (!mDevices[i])
					{
						mDevices[i] = new XInputDevice();
						//mInputManager->enumerator.registerDevice(mDevices[i]);

						Log::singleton() << "XInput Device found:" <<
							"\n    name: " << "" << "\n";
					}

					XInputDevice& xbD = *mDevices[i];

					/*xbD.setAxisValue(XINPUT_LEFT_X_AXIS.data, (float)state.Gamepad.sThumbLX / 32767);
					xbD.setAxisValue(XINPUT_LEFT_Y_AXIS.data, (float)state.Gamepad.sThumbLY / 32767);
					xbD.setAxisValue(XINPUT_RIGHT_X_AXIS.data, (float)state.Gamepad.sThumbRX / 32767);
					xbD.setAxisValue(XINPUT_RIGHT_Y_AXIS.data, (float)state.Gamepad.sThumbRY / 32767);
					xbD.setAxisValue(XINPUT_LT.data, (float)state.Gamepad.bLeftTrigger / 255);
					xbD.setAxisValue(XINPUT_RT.data, (float)state.Gamepad.bRightTrigger / 255);
					xbD.setButtonState(XINPUT_A.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
					xbD.setButtonState(XINPUT_B.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
					xbD.setButtonState(XINPUT_X.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
					xbD.setButtonState(XINPUT_Y.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
					xbD.setButtonState(XINPUT_START.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
					xbD.setButtonState(XINPUT_BACK.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
					xbD.setButtonState(XINPUT_LB.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
					xbD.setButtonState(XINPUT_RB.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
					xbD.setButtonState(XINPUT_LSTICK.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
					xbD.setButtonState(XINPUT_RSTICK.data, state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
					xbD.setButtonState(XINPUT_GUIDE.data, state.Gamepad.wButtons & 0x400);//?*/

					bool up = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
					bool down = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
					bool left = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
					bool right = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
					float angle = 0;
					if (up)
					{
						if (right) angle = 45;
						else if (left) angle = 315;
						else angle = 360;
					}
					else if (down)
					{
						if (right) angle = 135;
						else if (left) angle = 225;
						else angle = 180;
					}
					else if (left)
					{
						angle = 270;
					}
					else if (right)
					{
						angle = 90;
					}
					//xbD.setAxisValue(XINPUT_DPAD_ANGLE.data, angle);

					XINPUT_BATTERY_INFORMATION batteryInfo;
					XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);
					if (batteryInfo.BatteryType == BATTERY_TYPE_WIRED)
					{
						xbD._setWired(true);
						xbD._setBattery(BL_FULL);
					}
					else
					{
						xbD._setWired(false);
						switch (batteryInfo.BatteryLevel)
						{
						case BATTERY_LEVEL_EMPTY:
							xbD._setBattery(BL_EMPTY);
							break;
						case BATTERY_LEVEL_LOW:
							xbD._setBattery(BL_LOW);
							break;
						case BATTERY_LEVEL_MEDIUM:
							xbD._setBattery(BL_MEDIUM);
							break;
						case BATTERY_LEVEL_FULL:
							xbD._setBattery(BL_FULL);
							break;
						}
						
					}

					XINPUT_VIBRATION vibration;
					vibration.wLeftMotorSpeed = 65535 * xbD.getLeftVibration();
					vibration.wRightMotorSpeed = 65535 * xbD.getRightVibration();
					XInputSetState(i, &vibration);
				}
				else
				{
					//if (mDevices[i]) mInputManager->enumerator.unregisterDevice(mDevices[i]);
					delete mDevices[i];
					mDevices[i] = nullptr;
				}
			}
		}

	}

}