#include "IOSystems/FusinXInputIOSystem.h"

#ifdef FUSIN_BUILD_XINPUT

	#include "FusinInputManager.h"
	#include "IOCodes/FusinXInput.h"
	#include "Devices/FusinXInputDevice.h"
	#include <Windows.h>
	#include <XInput.h>
	#include "Utilities/FusinLog.h"

	namespace Fusin
	{

		XInputIOSystem::XInputIOSystem():
			IOSystem(IOF_ANY)
		{
		}

		XInputIOSystem::~XInputIOSystem()
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

		void XInputIOSystem::initialize(DeviceEnumerator* de, const std::map<String, String>& config, void* window)
		{
			IOSystem::initialize(de, config, window);
			mDevices.assign(XUSER_MAX_COUNT, nullptr);
		}

		void XInputIOSystem::updateDeviceList()
		{
			for (int i = 0; i < XUSER_MAX_COUNT; i++)
			{
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));
				DWORD result = XInputGetState(i, &state);

				if (result == ERROR_SUCCESS)
				{
					if (!mDevices[i])
					{
						StringStream ss;
						ss << "XInput Controller " << (i+1);
						
						XINPUT_BATTERY_INFORMATION batteryInfo;
						XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);

						mDevices[i] = new XInputDevice(ss.str(), batteryInfo.BatteryType != BATTERY_TYPE_WIRED);
						mDeviceEnumerator->registerDevice(mDevices[i]);

						Log::singleton() << "XInput Device found:" <<
							"\n    name: " << "" << "\n";
					}
				}
				else if (result == ERROR_DEVICE_NOT_CONNECTED)
				{
					if (mDevices[i])
					{
						mDeviceEnumerator->unregisterDevice(mDevices[i]);
						delete mDevices[i];
						mDevices[i] = nullptr;
					}
				}
			}

		}

		void XInputIOSystem::update()
		{
			for (int i = 0; i < XUSER_MAX_COUNT; i++)
			{
				if (mDevices[i])
				{
					XINPUT_STATE state;
					ZeroMemory(&state, sizeof(XINPUT_STATE));
					DWORD result = XInputGetState(i, &state);

					if (result == ERROR_SUCCESS)
					{
						XInputDevice& xbD = *mDevices[i];

						xbD.axisLeftStickX.setValue((float)state.Gamepad.sThumbLX / 32767);
						xbD.axisLeftStickY.setValue((float)state.Gamepad.sThumbLY / 32767);
						xbD.axisRightStickX.setValue((float)state.Gamepad.sThumbRX / 32767);
						xbD.axisRightStickY.setValue((float)state.Gamepad.sThumbRY / 32767);
						xbD.axisLT.setValue((float)state.Gamepad.bLeftTrigger / 255);
						xbD.axisRT.setValue((float)state.Gamepad.bRightTrigger / 255);
						xbD.buttonA.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_A));
						xbD.buttonB.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_B));
						xbD.buttonX.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_X));
						xbD.buttonY.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_Y));
						xbD.buttonMenu.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_START));
						xbD.buttonView.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK));
						xbD.buttonLB.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER));
						xbD.buttonRB.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER));
						xbD.buttonLeftStick.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB));
						xbD.buttonRightStick.setValue((float)(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB));
						xbD.buttonGuide.setValue((float)(state.Gamepad.wButtons & 0x400));//?

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
						xbD.dPad.angle.setValue(angle);

						XINPUT_BATTERY_INFORMATION batteryInfo;
						XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);
						if (batteryInfo.BatteryType != BATTERY_TYPE_WIRED)
						{
							xbD.battery.charging.setValue(batteryInfo.BatteryType != BATTERY_TYPE_DISCONNECTED);
							switch (batteryInfo.BatteryLevel)
							{
							case BATTERY_LEVEL_EMPTY:
								xbD.battery.energy.setValue(BAT_EMPTY);
								break;
							case BATTERY_LEVEL_LOW:
								xbD.battery.energy.setValue(BAT_LOW);
								break;
							case BATTERY_LEVEL_MEDIUM:
								xbD.battery.energy.setValue(BAT_MEDIUM);
								break;
							case BATTERY_LEVEL_FULL:
								xbD.battery.energy.setValue(BAT_FULL);
								break;
							}
						}

						XINPUT_VIBRATION vibration;
						vibration.wLeftMotorSpeed = (WORD)(65535 * xbD.vibration.leftForce.value());
						vibration.wRightMotorSpeed = (WORD)(65535 * xbD.vibration.rightForce.value());
						XInputSetState(i, &vibration);
					}
				}
			}

		}

	}

#endif // FUSIN_BUILD_XINPUT