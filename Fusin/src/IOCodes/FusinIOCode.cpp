#include "FusinIOCode.h"

#include "FusinInputManager.h"
#include "FusinKey.h"
#include "FusinMouse.h"
#include "FusinGamepad.h"
#include "FusinTouch.h"
#include "FusinMotionTracker.h"
#include "FusinDS.h"
#include "FusinXBox.h"
#include "FusinNintendo.h"
#include "FusinDPad.h"

#include "FusinAlgorithm.h"

#include <sstream>

namespace Fusin
{
	const IOCode IOCode::NULLCODE;
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
		deviceType((DeviceType)((ic & FUSIN_MASK1(48, 64)) >> 48)),
		ioType((IOType)((ic & FUSIN_MASK1(32, 48)) >> 32)),
		index(ic & FUSIN_MASK1(0, 32))
	{
	}

	// NOT NEEDED
	/*
	returns the index of codeStr in the list of names, -1 if the code isn't on the list 
	*/
	//Index findTypeCode(const String& codeStr, const std::vector<String>& names);
	//String restOfSStream(StringStream& ss);

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
		int indexFactor = 1;

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
			case DT_MOUSE:
				if (quickFindIndex(MOUSE_BUTTON_NAMES, inputStr, &index))
				{
					ioType = IO_BUTTON;
					foundInput = true;
					foundIndex = true;
					foundModifier = true;
				}
				else if (quickFindIndex(MOUSE_WHEEL_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				else if (quickFindIndex(MOUSE_MOVEMENT_NAMES, inputStr, &index))
				{
					ioType = IO_MOVEMENT;
					foundInput = true;
					foundIndex = true;
				}
				break;
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
			case DT_GAMEPAD:
				if (quickFindIndex(GAMEPAD_AXIS_NAMES, inputStr, &index))
				{
					ioType = IO_AXIS;
					foundInput = true;
					foundIndex = true;
				}
				break;
			case DT_XINPUT:
				if (quickFindIndex(XBOX_BUTTON_NAMES, inputStr, &index))
				{
					ioType = IO_BUTTON;
					foundInput = true;
					foundIndex = true;
					foundModifier = true;
				}
				else if (quickFindIndex(XBOX_AXIS_NAMES, inputStr, &index))
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
		}
		// Search for generic ioType names if we still haven't found the ioType type
		if (!foundInput)
		{
			ioType = nameToInputType(inputStr);
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
				if (deviceType == DT_KEYBOARD)
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
			}
		}
		// Convert the index string to integer if we haven't found a named index
		if (!foundIndex)
		{
			StringStream ss2(indexStr);
			ss2 >> index;
			foundIndex = true;
		}

		// Modifiers (+ and -)
		if (!foundModifier)
		{
			if (ss >> signStr)
			{
				if (signStr == FUSIN_STR("+"))
				{
					SET_FLAG(index, SIGNED_INDEX_FLAG, true);
					SET_FLAG(index, POSITIVITY_INDEX_FLAG, true);
				}
				else if (signStr == FUSIN_STR("-"))
				{
					SET_FLAG(index, SIGNED_INDEX_FLAG, true);
					SET_FLAG(index, POSITIVITY_INDEX_FLAG, false);
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
				ss << FUSIN_STR("Key ") << keyToName(index);
			else if (ioType == IO_TYPED_BUTTON)
				ss << FUSIN_STR("Typed ") << keyToName(index);
			else
				wroteDevice = wroteInput = wroteIndex = false;
		}

		if (!wroteDevice)
			ss << deviceTypeToName(deviceType);
		wroteDevice = true;

		if (!wroteInput)
		{
			ss << FUSIN_STR(" ");

			// Check specialized names per device type
			switch (deviceType)
			{
			case DT_MOUSE:
				if (ioType == IO_BUTTON && writeIndexName(ss, MOUSE_BUTTON_NAMES, index) || 
					ioType == IO_AXIS && writeIndexName(ss, MOUSE_WHEEL_NAMES, index) ||
					ioType == IO_MOVEMENT && writeIndexName(ss, MOUSE_MOVEMENT_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_TOUCHSCREEN:
				if (ioType == IO_PRESSURE && writeIndexName(ss, TOUCH_PRESSURE_NAMES, index) ||
					ioType == IO_POSITION && writeIndexName(ss, TOUCH_POSITION_NAMES, index) ||
					ioType == IO_MOVEMENT && writeIndexName(ss, TOUCH_MOVEMENT_NAMES, index))
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_GAMEPAD:
				if (ioType == IO_AXIS && writeIndexName(ss, GAMEPAD_AXIS_NAMES, index) )
				{
					wroteInput = true;
					wroteIndex = true;
				}
				break;
			case DT_XINPUT:
				if (ioType == IO_BUTTON && writeIndexName(ss, XBOX_BUTTON_NAMES, index) ||
					ioType == IO_AXIS && writeIndexName(ss, XBOX_AXIS_NAMES, index))
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
		}
		// Write the ioType type if we haven't already
		if (!wroteInput)
			ss << inputTypeToName(ioType);
		wroteInput = true;

		if (!wroteIndex)
		{
			ss << FUSIN_STR(" ");

			// Check named indices
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
			}
		}
		// Write the numeric index if no named index could be used
		if (!wroteIndex)
			ss << (index & INDEX_VALUE);
		wroteIndex = true;

		// Write the modifiers
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

		SET_FLAG(ind2, SIGNED_INDEX_FLAG, true);
		SET_FLAG(ind2, POSITIVITY_INDEX_FLAG, true);

		return IOCode(deviceType, ioType, ind2);
	}

	IOCode IOCode::negativeVersion() const
	{
		int ind2 = index;

		SET_FLAG(ind2, SIGNED_INDEX_FLAG, true);
		SET_FLAG(ind2, POSITIVITY_INDEX_FLAG, false);

		return IOCode(deviceType, ioType, ind2);
	}

	IOCode IOCode::unsignedVersion() const
	{
		int ind2 = index;

		SET_FLAG(ind2, SIGNED_INDEX_FLAG, false);
		SET_FLAG(ind2, POSITIVITY_INDEX_FLAG, false);

		return IOCode(deviceType, ioType, ind2);
	}

	bool IOCode::isValid() const
	{
		return (deviceType != DT_NONE && ioType != IO_NONE && deviceType < DT_LAST && ioType < IO_LAST);
	}

	/*String restOfSStream(StringStream& is)
	{
		String str;
		std::getline(is, str);
		str.erase(0, str.find_first_not_of(L" \n\r\t"));
		size_t lastC = str.find_last_of(L" \n\r\t");
		if (lastC != String::npos) str.erase(lastC + 1);
		return str;
	}*/

}