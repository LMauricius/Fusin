#include "IOCodes/FusinIOCode.h"

#include "IOCodes/FusinAnyBattery.h"
#include "IOCodes/FusinAnyCursor.h"
#include "IOCodes/FusinAnyDPad.h"
#include "IOCodes/FusinAnyJoystick.h"
#include "IOCodes/FusinAnyKey.h"
#include "IOCodes/FusinAnyMotionTracker.h"
#include "IOCodes/FusinAnyRGB.h"
#include "IOCodes/FusinAnyTouch.h"
#include "IOCodes/FusinAnyVibration.h"
#include "IOCodes/FusinAnyWheel.h"
#include "IOCodes/FusinKey.h"
#include "IOCodes/FusinMouse.h"
#include "IOCodes/FusinGamepad.h"
#include "IOCodes/FusinDS.h"
#include "IOCodes/FusinXInput.h"
#include "IOCodes/FusinNintendo.h"

#include "Utilities/FusinAlgorithm.h"
#include "Utilities/FusinBitCast.h"

#include <sstream>

namespace Fusin
{
	const IOCode IOCode::NULLCODE = IOCode(DT_NONE, IO_NONE, 0);
	const IOCode IOCode::ANYCODE = IOCode(DT_ANY, IO_ANY, 0);
	const int32_t IOCode::SIGNED_INDEX_FLAG = 1 << 31;
	const int32_t IOCode::POSITIVITY_INDEX_FLAG = 1 << 30;
	const int32_t IOCode::INDEX_FLAGS = SIGNED_INDEX_FLAG | POSITIVITY_INDEX_FLAG;
	const int32_t IOCode::INDEX_VALUE = ~INDEX_FLAGS;

	String deviceTypeToName(DeviceType t)
	{
		if (t < DEVICE_TYPE_NAMES.size())
			return DEVICE_TYPE_NAMES[t];

		return DEVICE_TYPE_NAMES[DT_NONE];
	}

	DeviceType nameToDeviceType(String name)
	{
		auto it = std::find(DEVICE_TYPE_NAMES.begin(), DEVICE_TYPE_NAMES.end(), name);
		if (it != DEVICE_TYPE_NAMES.end())
		{
			return (DeviceType)(it - DEVICE_TYPE_NAMES.begin());
		}

		return DT_NONE;
	}

	String _FUSIN_EXPORT inputTypeToName(IOType t)
	{
		if (t < INPUT_TYPE_NAMES.size())
			return INPUT_TYPE_NAMES[t];

		return INPUT_TYPE_NAMES[IO_NONE];
	}

	IOType _FUSIN_EXPORT nameToInputType(String name)
	{
		auto it = std::find(INPUT_TYPE_NAMES.begin(), INPUT_TYPE_NAMES.end(), name);
		if (it != INPUT_TYPE_NAMES.end())
		{
			return (IOType)(it - INPUT_TYPE_NAMES.begin());
		}

		return IO_NONE;
	}

	IOCode::IOCode():
		deviceType(DT_NONE),
		ioType(IO_NONE),
		index(0)
	{
		
	}

	IOCode::IOCode(const IOCode& ic) :
		deviceType(ic.deviceType),
		ioType(ic.ioType),
		index(ic.index)
	{
		
	}

	IOCode::IOCode(DeviceType deviceType, IOType ioType, int32_t index) :
		deviceType(deviceType),
		ioType(ioType),
		index(index)
	{
	}

	IOCode::IOCode(uint64_t ic):
		deviceType((DeviceType)((ic & mask1(48, 64)) >> 48)),
		ioType((IOType)((ic & mask1(32, 48)) >> 32)),
		index(int32_t(ic & mask1(0, 32)))
	{
	}

	IOCode::IOCode(const String& name) :
		deviceType(DT_NONE),
		ioType(IO_NONE),
		index(0)
	{
		StringStream ss(name);
		String deviceStr, inputStr, indexStr, signStr;
		bool foundDevice = false;
		bool foundInput = false;
		bool foundIndex = false;
		bool foundModifier = false;
		size_t indexFactor = 1;

		if (!(ss >> deviceStr))
			return;

		// We have a special case for keys, which describe both a deviceType and ioType type
		if (deviceStr == FUSIN_STR("Key"))
		{
			deviceType = DT_KEYBOARD;
			ioType = IO_BUTTON;

			foundDevice = true;
			foundInput = true;
			foundModifier = true;// keys don't have signed versions
		}
		else if (deviceStr == FUSIN_STR("Typed"))
		{
			deviceType = DT_KEYBOARD;
			ioType = IO_TYPED_BUTTON;

			foundDevice = true;
			foundInput = true;
			foundModifier = true;
		}

		// Get deviceType
		if (!foundDevice)
			deviceType = nameToDeviceType(deviceStr);
		foundDevice = true;

		if (!foundInput)
		{
			if (!(ss >> inputStr))
				return;

			// Check specialized names per device type
			switch (deviceType)
			{
			case DT_COMPONENT_TYPING:
				if (inputStr == FUSIN_STR("Key"))
				{
					ioType = IO_BUTTON;

					foundInput = true;
					foundModifier = true;// keys don't have signed versions
				}
				else if (inputStr == FUSIN_STR("Typed"))
				{
					ioType = IO_TYPED_BUTTON;

					foundInput = true;
					foundModifier = true;
				}
				break;
			case DT_COMPONENT_CURSOR:
				if (quickFindIndex(CURSOR_MOVEMENT_NAMES, inputStr, &index))
				{
					ioType = IO_MOVEMENT;
					foundInput = true;
					foundIndex = true;
				}
				break;
			case DT_COMPONENT_WHEEL:
				if (quickFindIndex(WHEEL_ROTATION_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				break;
			case DT_MOUSE:
				if (quickFindIndex(MOUSE_BUTTON_NAMES, inputStr, &index))
				{
					ioType = IO_BUTTON;
					foundInput = true;
					foundIndex = true;
					foundModifier = true;
				}
				else if (quickFindIndex(WHEEL_ROTATION_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				else if (quickFindIndex(CURSOR_MOVEMENT_NAMES, inputStr, &index))
				{
					ioType = IO_MOVEMENT;
					foundInput = true;
					foundIndex = true;
				}
				break;
			case DT_COMPONENT_TOUCHPAD:
			case DT_TOUCHSCREEN:
				if (quickFindIndex(TOUCH_PRESSURE_NAMES, inputStr, &index))
				{
					ioType = IO_PRESSURE;
					foundInput = true;
					foundModifier = true;
					indexFactor = TOUCH_PRESSURE_NAMES.size();
				}
				else if (quickFindIndex(TOUCH_POSITION_NAMES, inputStr, &index))
				{
					ioType = IO_POSITION;
					foundInput = true;
					indexFactor = TOUCH_POSITION_NAMES.size();
				}
				else if (quickFindIndex(TOUCH_MOVEMENT_NAMES, inputStr, &index))
				{
					ioType = IO_MOVEMENT;
					foundInput = true;
					indexFactor = TOUCH_MOVEMENT_NAMES.size();
				}
				break;
			case DT_COMPONENT_JOYSTICK:
			case DT_GAMEPAD:
				if (quickFindIndex(JOYSTICK_AXIS_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				break;
			case DT_XINPUT:
				if (quickFindIndex(XINPUT_BUTTON_NAMES, inputStr, &index))
				{
					ioType = IO_BUTTON;
					foundInput = true;
					foundIndex = true;
					foundModifier = true;
				}
				else if (quickFindIndex(XINPUT_AXIS_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				break;
			case DT_DUALSHOCK:
				if (quickFindIndex(DS_BUTTON_NAMES, inputStr, &index))
				{
					ioType = IO_BUTTON;
					foundInput = true;
					foundIndex = true;
					foundModifier = true;
				}
				else if (quickFindIndex(DS_AXIS_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				break;
			case DT_NINTENDO:
				if (quickFindIndex(NINTENDO_BUTTON_NAMES, inputStr, &index))
				{
					ioType = IO_BUTTON;
					foundInput = true;
					foundIndex = true;
					foundModifier = true;
				}
				else if (quickFindIndex(NINTENDO_AXIS_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				break;
			}

			// Search for generic ioType names if we still haven't found the ioType type
			if (!foundInput)
			{
				ioType = nameToInputType(inputStr);
			}
		}

		if (!foundIndex)
		{
			if (!(ss >> indexStr))
				return;

			// Search for named indices
			switch (ioType)
			{
			case IO_BUTTON:
			case IO_TYPED_BUTTON:
				if (deviceType == DT_KEYBOARD || deviceType == DT_COMPONENT_TYPING)
				{
					index = nameToKey(indexStr);

					if (index != '\0')
						foundIndex = true;
				}

				break;
			case IO_ANGLE:
				if (quickFindIndex(DPAD_ANGLE_NAMES, inputStr, &index))
					foundIndex = true;
				break;
			case IO_DIRECTION:
				if (quickFindIndex(DPAD_DIRECTION_NAMES, inputStr, &index))
					foundIndex = true;
				break;
			case IO_MOVEMENT:
			case IO_POSITION:
			case IO_ACCELERATION:
			case IO_VELOCITY:
			case IO_WORLD_POSITION:
				if (quickFindIndex(POSITION_NAMES, inputStr, &index))
					foundIndex = true;
				break;
			case IO_ORIENTATION:
			case IO_GYRO:
				if (quickFindIndex(ROTATION_NAMES, inputStr, &index))
					foundIndex = true;
				break;
			case IO_VIBRATION:
				if (quickFindIndex(VIBRATION_NAMES, inputStr, &index))
					foundIndex = true;
				break;
			case IO_RGB:
				if (quickFindIndex(COLOR_NAMES, inputStr, &index))
					foundIndex = true;
				break;
			case IO_BATTERY:
				if (quickFindIndex(BATTERY_NAMES, inputStr, &index))
					foundIndex = true;
				break;
			}

			// Convert the index string to integer if we haven't found a named index
			if (!foundIndex)
			{
				StringStream ss2(indexStr);
				ss2 >> index;
				foundIndex = true;
			}
		}

		// Modifiers (+ and -)
		if (!foundModifier)
		{
			if (ss >> signStr)
			{
				if (signStr == FUSIN_STR("+"))
				{
					setFlags(index, SIGNED_INDEX_FLAG, true);
					setFlags(index, POSITIVITY_INDEX_FLAG, true);
				}
				else if (signStr == FUSIN_STR("-"))
				{
					setFlags(index, SIGNED_INDEX_FLAG, true);
					setFlags(index, POSITIVITY_INDEX_FLAG, false);
				}
			}
		}
	}


	IOCode& IOCode::operator=(const IOCode& ic)
	{
		deviceType = ic.deviceType;
		ioType = ic.ioType;
		index = ic.index;
		return *this;
	}

	bool IOCode::operator==(const IOCode& ic) const
	{
		return deviceType == ic.deviceType && ioType == ic.ioType && index == ic.index;
	}

	bool IOCode::operator>(const IOCode& ic) const
	{
		return (int64_t)(*this) > (int64_t)ic;
	}

	bool IOCode::operator<(const IOCode& ic) const
	{
		return (int64_t)(*this) < (int64_t)ic;
	}

	bool IOCode::operator>=(const IOCode& ic) const
	{
		return (int64_t)(*this) >= (int64_t)ic;
	}

	bool IOCode::operator<=(const IOCode& ic) const
	{
		return (int64_t)(*this) <= (int64_t)ic;
	}

	IOCode::operator uint64_t() const
	{
		return ((uint64_t)deviceType << 48) | ((uint64_t)ioType << 32) | index;
	}

	namespace
	{
		bool writeIndexName(StringStream& stream, const std::vector<String>& names, Index ind)
		{
			if ((ind & IOCode::INDEX_VALUE) < names.size()) stream << names[ind & IOCode::INDEX_VALUE];
			else return false;

			return true;
		}
	}

	String IOCode::name() const
	{
		StringStream ss;
		bool wroteDevice = false;
		bool wroteInput = false;
		bool wroteIndex = false;

		// We use a simplified naming scheme for keyboard 
		if (deviceType == DT_KEYBOARD)
		{
			wroteDevice = wroteInput = wroteIndex = true;

			if (ioType == IO_BUTTON)
				ss << "Key " << keyToName(index);
			else if (ioType == IO_TYPED_BUTTON)
				ss << "Typed " << keyToName(index);
			else
				wroteDevice = wroteInput = wroteIndex = false;
		}

		// Write <DEVICE NAME>
		if (!wroteDevice)
			ss << deviceTypeToName(deviceType);
		wroteDevice = true;

		if (!wroteInput)
		{
			ss << FUSIN_STR(" ");

			// Try to write <IOTYPE+INDEX NAME> for special device types
			switch (deviceType)
			{
			case DT_COMPONENT_TYPING:
				if (ioType == IO_BUTTON)
				{
					ss << "Key " << keyToName(index);
					wroteInput = true;
					wroteIndex = true;
				}
				else if (ioType == IO_TYPED_BUTTON)
				{
					ss << "Typed " << keyToName(index);
					wroteInput = true;
					wroteIndex = true;
				}

				break;
			case DT_COMPONENT_CURSOR:
				if (ioType == IO_MOVEMENT && writeIndexName(ss, CURSOR_MOVEMENT_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_COMPONENT_WHEEL:
				if (ioType == IO_AXIS && writeIndexName(ss, WHEEL_ROTATION_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_MOUSE:
				if (ioType == IO_BUTTON && writeIndexName(ss, MOUSE_BUTTON_NAMES, index) || 
					ioType == IO_AXIS && writeIndexName(ss, WHEEL_ROTATION_NAMES, index) ||
					ioType == IO_MOVEMENT && writeIndexName(ss, CURSOR_MOVEMENT_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_COMPONENT_TOUCHPAD:
			case DT_TOUCHSCREEN:
				if (ioType == IO_PRESSURE)
				{
					// For pressure inputs index denotes the index of the touch
					ss << TOUCH_PRESSURE_NAMES[0] << " " << index;
					wroteInput = true;
					wroteIndex = true;
				}
				else if (ioType == IO_POSITION && writeIndexName(ss, TOUCH_POSITION_NAMES, index % 2) ||
						 ioType == IO_MOVEMENT && writeIndexName(ss, TOUCH_MOVEMENT_NAMES, index % 2))
				{
					// For position and movement inputs there are 2 iocode indices per an index of the touch
					ss << " " << index / 2;
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_COMPONENT_JOYSTICK:
			case DT_GAMEPAD:
				if (ioType == IO_AXIS && writeIndexName(ss, JOYSTICK_AXIS_NAMES, index) )
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_XINPUT:
				if (ioType == IO_BUTTON && writeIndexName(ss, XINPUT_BUTTON_NAMES, index) ||
					ioType == IO_AXIS && writeIndexName(ss, XINPUT_AXIS_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_DUALSHOCK:
				if (ioType == IO_BUTTON && writeIndexName(ss, DS_BUTTON_NAMES, index) ||
					ioType == IO_AXIS && writeIndexName(ss, DS_AXIS_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_NINTENDO:
				if (ioType == IO_BUTTON && writeIndexName(ss, NINTENDO_BUTTON_NAMES, index) ||
					ioType == IO_AXIS && writeIndexName(ss, NINTENDO_AXIS_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			}

			// Write the <IOTYPE NAME> if we haven't already
			if (!wroteInput)
				ss << inputTypeToName(ioType);
			wroteInput = true;
		}

		if (!wroteIndex)
		{
			ss << FUSIN_STR(" ");

			// Try to write <INDEX NAME> for special input types
			switch (ioType)
			{
			case IO_ANGLE:
				if (writeIndexName(ss, DPAD_ANGLE_NAMES, index))
					wroteIndex = true;
				break;
			case IO_DIRECTION:
				if (writeIndexName(ss, DPAD_DIRECTION_NAMES, index))
					wroteIndex = true;
				break;
			case IO_MOVEMENT:
			case IO_POSITION:
			case IO_ACCELERATION:
			case IO_VELOCITY:
			case IO_WORLD_POSITION:
				if (writeIndexName(ss, POSITION_NAMES, index))
					wroteIndex = true;
				break;
			case IO_ORIENTATION:
			case IO_GYRO:
				if (writeIndexName(ss, ROTATION_NAMES, index))
					wroteIndex = true;
				break;
			case IO_BATTERY:
				if (writeIndexName(ss, BATTERY_NAMES, index))
					wroteIndex = true;
				break;
			case IO_VIBRATION:
				if (writeIndexName(ss, VIBRATION_NAMES, index))
					wroteIndex = true;
				break;
			case IO_RGB:
				if (writeIndexName(ss, COLOR_NAMES, index))
					wroteIndex = true;
				break;
			}

			// Write the numeric <INDEX> if no named index could be used
			if (!wroteIndex)
				ss << (index & INDEX_VALUE);
			wroteIndex = true;
		}

		// Write the <MODIFIERS>
		if (index & SIGNED_INDEX_FLAG)
		{
			if (index & POSITIVITY_INDEX_FLAG)
				ss << FUSIN_STR("+");
			else
				ss << FUSIN_STR("-");
		}

		return ss.str();
	}

	IOCode IOCode::withDeviceType(DeviceType newDeviceType) const
	{
		return IOCode(newDeviceType, ioType, index);
	}

	IOCode IOCode::withIOType(IOType newIOType) const
	{
		return IOCode(deviceType, newIOType, index);
	}

	IOCode IOCode::withIndex(int32_t newIndex) const
	{
		return IOCode(deviceType, ioType, (newIndex & INDEX_VALUE) | (index & INDEX_FLAGS));
	}

	IOCode IOCode::withIndexFlags(int32_t newIndexFlags) const
	{
		return IOCode(deviceType, ioType, (newIndexFlags & INDEX_FLAGS) | (index & INDEX_VALUE));
	}

	IOCode IOCode::positiveVersion() const
	{
		int ind2 = index;

		setFlags(ind2, SIGNED_INDEX_FLAG, true);
		setFlags(ind2, POSITIVITY_INDEX_FLAG, true);

		return IOCode(deviceType, ioType, ind2);
	}

	IOCode IOCode::negativeVersion() const
	{
		int ind2 = index;

		setFlags(ind2, SIGNED_INDEX_FLAG, true);
		setFlags(ind2, POSITIVITY_INDEX_FLAG, false);

		return IOCode(deviceType, ioType, ind2);
	}

	IOCode IOCode::unsignedVersion() const
	{
		int ind2 = index;

		setFlags(ind2, SIGNED_INDEX_FLAG, false);
		setFlags(ind2, POSITIVITY_INDEX_FLAG, false);

		return IOCode(deviceType, ioType, ind2);
	}

	bool IOCode::isValid() const
	{
		return (deviceType != DT_NONE && ioType != IO_NONE && deviceType < DT_LAST && ioType < IO_LAST);
	}

	bool IOCode::fitsFilter(IOFlags filter)
	{
		return (
			(filter & IOF_ANY_DEVICE & BASE_FLAGS_PER_DEVICE[deviceType]) &&
			(filter & (IOF_ANY_INPUT | IOF_ANY_OUTPUT) & FLAG_PER_IO[ioType])
			);
	}

}