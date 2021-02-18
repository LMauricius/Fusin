#ifndef _FUSIN_INPUT_VALUE_H
#define _FUSIN_INPUT_VALUE_H

#include "FusinPrerequisites.h"
#include "Utilities/FusinBitCast.h"

#include <vector>
#include <list>
#include <map>
#include <stdint.h>
#include <functional>

namespace Fusin
{
	class InputManager;
	class InputSystem;
	class Command;

	/*
	Enum containing types of devices supported by Fusin.
	It covers both device components and actual devices.
	The value is stored in 16 bits so the IOCode would fit in 64 bits.
	*/
	enum DeviceType : uint16_t {
		DT_NONE,
		DT_ANY,

		DT_COMPONENT_TYPING,// Device that has buttons corresponding to characters, typed versions of button InputSignals (with a typing delay between signals), and a String value containing characters typed since last _update
		DT_COMPONENT_CURSOR,// Device with position and movement values
		DT_COMPONENT_BUTTONPAD,// Device containing buttons
		DT_COMPONENT_WHEEL,// Device containing horizontal and vertical rotation axes interpreted as direction buttons
		DT_COMPONENT_JOYSTICK,// Device containing joystick-like axes
		DT_COMPONENT_DPAD,// Device with 4 directional inputs and an angle
		DT_COMPONENT_TOUCHPAD,// Device with multiple position, movement and activation values
		DT_COMPONENT_MOTION_TRACKER,// Device with many values specifying real world position and orientation
		DT_COMPONENT_BATTERY,// Device with a battery
		DT_COMPONENT_LEDS,// Device with LEDs
		DT_COMPONENT_VIBRATION,// Device with vibration support
		DT_COMPONENT_RGB,// Device with RGB support (such as the DS4 controller)
		DT_FIRST_COMPONENT = DT_COMPONENT_TYPING,
		DT_LAST_COMPONENT = DT_COMPONENT_RGB,

		DT_KEYBOARD,// A typing device with LED outputs
		DT_MOUSE,// Mouse device, a cursor with at least 3 buttons and 2 wheel axes
		DT_TOUCHSCREEN,// Touchpad device with a screen
		DT_GAMEPAD,// A generic gamepad with buttons, axes and a DPad
		DT_XINPUT,// XInput compatible gamepad
		DT_DUALSHOCK,// Dualshock or SixAxis controller
		DT_NINTENDO,// Gamepad used for Nintendo consoles
		DT_LAST_DEVICE = DT_NINTENDO,
		DT_FIRST_DEVICE = DT_KEYBOARD,
		DT_LAST
	};

	/*
	Enum containing all types of inputs supported by Fusin
	The value is stored in 16 bits so the IOCode would fit in 64 bits.
	*/
	enum IOType : uint16_t {
		IO_NONE,
		IO_ANY,

		IO_FIRST_INPUT,
		IO_BUTTON = IO_FIRST_INPUT,// For physical buttons. Buttons usually have two states (0 and 1), exception being pressure-sensitive buttons. Name = Button IND
		IO_AXIS,// For axis values. Axes can hold any real values but it's recommended to scale them between -1.0 and 1.0. Name = Axis IND
		IO_DIRECTION,// For DPad buttons. DPads usually have four buttons for different directions. Name = Direction IND
		IO_ANGLE,// For directional axes, expressed in degrees. 0 means no direction, so 360 is used for 0 degrees angle instead. Name = Angle IND
		IO_MOVEMENT,// For movement axes, contained in devices such as a mouse or a touchpad. Name = Move IND
		IO_POSITION,// Position axes, expressed in pixels or other virtual values. For Real-world positions use IO_WORLD_POSITION. Name = Position IND
		IO_ORIENTATION,// Object's orientation, expressed in degrees. Name = Orientation IND
		IO_GYRO,// Object's angular velocity, expressed in degrees per second. Name = Gyro IND
		IO_ACCELERATION,// Object's acceleration, expressed in g (1g = 9.807m/s^2). Name = Accel IND
		IO_VELOCITY,// Object's velocity, expressed in meters per second. Name = Velocity IND
		IO_WORLD_POSITION,// Real world position, expressed in meters. Name = WPosition IND
		IO_PRESSURE,// For inputs that are based on different levels of pressure, such as touch devices. Keep in mind that many other inputs can have pressure sensitivity as a feature but aren't based on that. Name = Pressure IND
		IO_TYPED_BUTTON,// For typed versions of buttons, with typing delay like when holding a keyboard button down
		IO_BATTERY,// Battery value. 0-100
		IO_LAST_INPUT = IO_BATTERY,

		IO_FIRST_OUTPUT,
		IO_LEDS = IO_FIRST_OUTPUT,// Controllable LEDS
		IO_VIBRATION,// Vibration (At least left and right)
		IO_RGB,// Changeable color setting (such as LED light on DS4 controllers)
		IO_LAST_OUTPUT = IO_RGB,
		IO_LAST
	};

	/*
	Describes various properties of devices and their inputs
	Can be used to filter and find Devices with certain features or 
	inputs of multiple types.
	*/
	typedef uint64_t IOFlags;

	constexpr size_t IOF_INPUT_STARTING_BIT = 0;
	constexpr size_t IOF_OUTPUT_STARTING_BIT = 24;
	constexpr size_t IOF_DEVICE_STARTING_BIT = 32;
	constexpr size_t IOF_MODIFIER_STARTING_BIT = 56;

	const IOFlags IOF_NONE = 0;
	const IOFlags IOF_BUTTON         = 1ULL << (0 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_AXIS           = 1ULL << (1 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_DIRECTION      = 1ULL << (3 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_ANGLE          = 1ULL << (4 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_MOVEMENT       = 1ULL << (5 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_POSITION       = 1ULL << (6 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_ORIENTATION    = 1ULL << (7 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_GYRO           = 1ULL << (8 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_ACCELERATION   = 1ULL << (9 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_VELOCITY       = 1ULL << (10 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_WORLD_POSITION = 1ULL << (11 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_PRESSURE     = 1ULL << (12 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_TYPED_BUTTON   = 1ULL << (13 + IOF_INPUT_STARTING_BIT);
	const IOFlags IOF_BATTERY        = 1ULL << (14 + IOF_INPUT_STARTING_BIT);

	const IOFlags IOF_LEDS      = 1ULL << (0 + IOF_OUTPUT_STARTING_BIT);// Controllable LEDS
	const IOFlags IOF_VIBRATION = 1ULL << (1 + IOF_OUTPUT_STARTING_BIT);// Vibration (At least left and right)
	const IOFlags IOF_RGB       = 1ULL << (2 + IOF_OUTPUT_STARTING_BIT);// Changeable color setting (such as LED light on DS4 controllers)
	
	const IOFlags IOF_TYPING         = 1ULL << (0 + IOF_DEVICE_STARTING_BIT);// Device that has buttons corresponding to characters, typed versions of button InputSignals (with a typing delay between signals), and a String value containing characters typed since last _update
	const IOFlags IOF_CURSOR         = 1ULL << (1 + IOF_DEVICE_STARTING_BIT);// Device with position and movement values
	const IOFlags IOF_BUTTONPAD      = 1ULL << (2 + IOF_DEVICE_STARTING_BIT);// Device containing buttons
	const IOFlags IOF_WHEEL          = 1ULL << (3 + IOF_DEVICE_STARTING_BIT);// Device containing mouse wheel rotation axes interpreted as direction buttons
	const IOFlags IOF_JOYSTICK       = 1ULL << (4 + IOF_DEVICE_STARTING_BIT);// Device containing joystick-like axes
	const IOFlags IOF_DPAD           = 1ULL << (5 + IOF_DEVICE_STARTING_BIT);// Device with 4 directional inputs and an angle
	const IOFlags IOF_TOUCHPAD       = 1ULL << (6 + IOF_DEVICE_STARTING_BIT);// Device with multiple position, movement and activation values
	const IOFlags IOF_MOTION_TRACKER = 1ULL << (7 + IOF_DEVICE_STARTING_BIT);// Device with many values specifying real world position and orientation
	const IOFlags IOF_SIGNED_VERSION = 1ULL << (0 + IOF_MODIFIER_STARTING_BIT);// A filter modifier flag, for Devices it filters only those that have signed version of some of their inputs, for InputSignals it filters only those that are signed versions of unsigned InputSignals. Name = + OR -
	const IOFlags IOF_ANY_INPUT    = mask1(IOF_INPUT_STARTING_BIT, IOF_OUTPUT_STARTING_BIT);
	const IOFlags IOF_ANY_OUTPUT   = mask1(IOF_OUTPUT_STARTING_BIT, IOF_DEVICE_STARTING_BIT);
	const IOFlags IOF_ANY_DEVICE   = mask1(IOF_DEVICE_STARTING_BIT, IOF_MODIFIER_STARTING_BIT);
	const IOFlags IOF_ANY_MODIFIER = mask1(IOF_MODIFIER_STARTING_BIT, 64);
	const IOFlags IOF_ANY = -1;

	/*
	Vector containing all DeviceComponent types supported by Fusin,
	listed in the same order as the enum,
	so that ALL_DEVICE_TYPES[0] == DT_FIRST_COMPONENT
	*/
	const std::vector<DeviceType> ALL_COMPONENT_TYPES{
		DT_COMPONENT_TYPING,
		DT_COMPONENT_CURSOR,
		DT_COMPONENT_BUTTONPAD,
		DT_COMPONENT_WHEEL,
		DT_COMPONENT_JOYSTICK,
		DT_COMPONENT_DPAD,
		DT_COMPONENT_TOUCHPAD,
		DT_COMPONENT_MOTION_TRACKER,
		DT_COMPONENT_BATTERY,
		DT_COMPONENT_LEDS,
		DT_COMPONENT_VIBRATION,
		DT_COMPONENT_RGB
	};

	/*
	Vector containing all Device types supported by Fusin,
	listed in the same order as the enum,
	so that ALL_DEVICE_TYPES[0] == DT_FIRST_DEVICE
	*/
	const std::vector<DeviceType> ALL_DEVICE_TYPES {
		DT_KEYBOARD,
		DT_MOUSE,
		DT_TOUCHSCREEN,
		DT_GAMEPAD,
		DT_XINPUT,
		DT_DUALSHOCK,
		DT_NINTENDO
	}; 

	/*
	Vector containing all Input IOTypes supported by Fusin,
	listed in the same order as the enum but not necessarily starting at 0,
	so that ALL_INPUT_TYPES[0] == IO_FIRST_INPUT
	*/
	const std::vector<IOType> ALL_INPUT_TYPES {
		IO_BUTTON,
		IO_AXIS,
		IO_DIRECTION,
		IO_ANGLE,
		IO_MOVEMENT,
		IO_POSITION,
		IO_ORIENTATION,
		IO_GYRO,
		IO_ACCELERATION,
		IO_VELOCITY,
		IO_WORLD_POSITION,
		IO_PRESSURE,
		IO_TYPED_BUTTON,
		IO_BATTERY
	};

	/*
	Vector containing all Output IOTypes supported by Fusin,
	listed in the same order as the enum but not necessarily starting at 0,
	so that ALL_OUTPUT_TYPES[0] == IO_FIRST_OUTPUT
	*/
	const std::vector<IOType> ALL_OUTPUT_TYPES{
		IO_LEDS,
		IO_VIBRATION,
		IO_RGB
	};

	/*
	Vector containing names of all the DeviceTypes,
	listed in the same order as the enum, starting at 0,
	so that DEVICE_TYPE_NAMES[t] == <name of t>
	*/
	const std::vector<String> DEVICE_TYPE_NAMES{
		FUSIN_STR("No_Device"),
		FUSIN_STR("Any_Device"),
		FUSIN_STR("Any_Typing_Device"),
		FUSIN_STR("Any_Cursor_Device"),
		FUSIN_STR("Any_ButtonPad_Device"),
		FUSIN_STR("Any_Wheel_Device"),
		FUSIN_STR("Any_Joystick_Device"),
		FUSIN_STR("Any_DPad_Device"),
		FUSIN_STR("Any_TouchPad_Device"),
		FUSIN_STR("Any_Motion_Tracker_Device"),
		FUSIN_STR("Any_Battery_Device"),
		FUSIN_STR("Any_LED_Device"),
		FUSIN_STR("Any_Vibration_Device"),
		FUSIN_STR("Any_RGB_Device"),
		FUSIN_STR("Keyboard"),
		FUSIN_STR("Mouse"),
		FUSIN_STR("Touchscreen"),
		FUSIN_STR("Gamepad"),
		FUSIN_STR("XInput"),
		FUSIN_STR("DualShock"),
		FUSIN_STR("Nintendo")
	};

	/*
	Vector containing names of all the InputTypes,
	listed in the same order as the enum, starting at 0,
	so that INPUT_TYPE_NAMES[t] == <name of t>
	*/
	const std::vector<String> INPUT_TYPE_NAMES{
		FUSIN_STR("No_Input"),
		FUSIN_STR("Any_Input"),
		FUSIN_STR("Button"),
		FUSIN_STR("Axis"),
		FUSIN_STR("Direction"),
		FUSIN_STR("Angle"),
		FUSIN_STR("Movement"),
		FUSIN_STR("Position"),
		FUSIN_STR("Orientation"),
		FUSIN_STR("Gyro"),
		FUSIN_STR("Acceleration"),
		FUSIN_STR("Velocity"),
		FUSIN_STR("World_Position"),
		FUSIN_STR("Pressure"),
		FUSIN_STR("Typed"),
		FUSIN_STR("Battery"),

		FUSIN_STR("LEDs"),
		FUSIN_STR("Vibration"),
		FUSIN_STR("RGB")
	};

	/*
	Vector containing Device IOFlags that ALL Devices
	of a certain DeviceType have,
	listed in the same order as the enum, starting at 0,
	so that BASE_FLAGS_PER_DEVICE[t] == <flags of t>
	*/
	const std::vector<IOFlags> BASE_FLAGS_PER_DEVICE{
		IOF_NONE,//DT_NONE
		IOF_NONE,//DT_ANY

		IOF_TYPING,//DT_COMPONENT_TYPING
		IOF_CURSOR,//DT_COMPONENT_CURSOR
		IOF_BUTTONPAD,//DT_COMPONENT_BUTTONPAD
		IOF_WHEEL,//DT_COMPONENT_WHEEL
		IOF_JOYSTICK,//DT_COMPONENT_JOYSTICK
		IOF_DPAD,//DT_COMPONENT_DPAD
		IOF_TOUCHPAD,//DT_COMPONENT_TOUCHPAD
		IOF_MOTION_TRACKER,//DT_COMPONENT_MOTION_TRACKER
		IOF_BATTERY,//DT_COMPONENT_MOTION_TRACKER
		IOF_LEDS,//DT_COMPONENT_LEDS
		IOF_VIBRATION,//DT_COMPONENT_VIBRATION
		IOF_RGB,//DT_COMPONENT_RGB

		IOF_TYPING,//DT_KEYBOARD
		IOF_CURSOR | IOF_BUTTONPAD | IOF_WHEEL,//DT_MOUSE
		IOF_TOUCHPAD,//DT_TOUCHSCREEN
		IOF_JOYSTICK,//DT_GAMEPAD
		IOF_JOYSTICK | IOF_DPAD | IOF_BATTERY,//DT_XINPUT
		IOF_JOYSTICK | IOF_DPAD | IOF_MOTION_TRACKER | IOF_BATTERY,//DT_DUALSHOCK
		IOF_JOYSTICK | IOF_DPAD | IOF_BATTERY//DT_NINTENDO
	};

	/*
	Vector containing Device IOFlags that a Device
	of a certain DeviceType CAN POSSIBLY have,
	listed in the same order as the enum, starting at 0,
	so that POSSIBLE_FLAGS_PER_DEVICE[t] == <flags of t>
	*/
	const std::vector<IOFlags> POSSIBLE_FLAGS_PER_DEVICE{
		IOF_NONE,//DT_NONE
		IOF_ANY_DEVICE,//DT_ANY

		IOF_ANY_DEVICE,//DT_COMPONENT_TYPING
		IOF_ANY_DEVICE,//DT_COMPONENT_CURSOR
		IOF_ANY_DEVICE,//DT_COMPONENT_BUTTONPAD
		IOF_ANY_DEVICE,//DT_COMPONENT_WHEEL
		IOF_ANY_DEVICE,//DT_COMPONENT_JOYSTICK
		IOF_ANY_DEVICE,//DT_COMPONENT_DPAD
		IOF_ANY_DEVICE,//DT_COMPONENT_TOUCHPAD
		IOF_ANY_DEVICE,//DT_COMPONENT_MOTION_TRACKER
		IOF_ANY_DEVICE,//DT_COMPONENT_BATTERY
		IOF_ANY_DEVICE,//DT_COMPONENT_LEDS
		IOF_ANY_DEVICE,//DT_COMPONENT_VIBRATION
		IOF_ANY_DEVICE,//DT_COMPONENT_RGB

		IOF_TYPING,//DT_KEYBOARD
		IOF_CURSOR,//DT_MOUSE
		IOF_TOUCHPAD | IOF_CURSOR,//DT_TOUCHSCREEN
		IOF_JOYSTICK | IOF_DPAD,//DT_GAMEPAD
		IOF_JOYSTICK | IOF_DPAD | IOF_BATTERY | IOF_BATTERY,//DT_XINPUT
		IOF_JOYSTICK | IOF_DPAD | IOF_TOUCHPAD | IOF_MOTION_TRACKER | IOF_BATTERY,//DT_DUALSHOCK
		IOF_JOYSTICK | IOF_DPAD | IOF_TOUCHPAD | IOF_MOTION_TRACKER | IOF_BATTERY//DT_NINTENDO
	};

	/*
	Vector containing Input/Output IOFlags for each IOType,
	listed in the same order as the enum, starting at 0,
	so that FLAG_PER_IO[t] == <flags of t>
	*/
	const std::vector<IOFlags> FLAG_PER_IO{
		IOF_NONE,// IO_NONE,
		IOF_ANY,// IO_ANY,

		IOF_BUTTON,// IO_BUTTON,
		IOF_AXIS,// IO_AXIS,
		IOF_DIRECTION,// IO_DIRECTION,
		IOF_ANGLE,// IO_ANGLE,
		IOF_MOVEMENT,// IO_MOVEMENT,
		IOF_POSITION,// IO_POSITION,
		IOF_ORIENTATION,// IO_ORIENTATION,
		IOF_GYRO,// IO_GYRO,
		IOF_ACCELERATION,// IO_ACCELERATION,
		IOF_VELOCITY,// IO_VELOCITY,
		IOF_WORLD_POSITION,// IO_WORLD_POSITION,
		IOF_PRESSURE,// IO_PRESSURE,
		IOF_TYPED_BUTTON,// IO_TYPED_BUTTON,
		IOF_BATTERY,//IO_BATTERY

		IOF_LEDS,// IO_LEDS
		IOF_VIBRATION,// IO_VIBRATION
		IOF_RGB,// IO_RGB
	};

	/*
	Returns the name of the device type, such as "Keyboard" or "Mouse"
	*/
	String _FUSIN_EXPORT deviceTypeToName(DeviceType t);
	/*
	Returns the device type corresponding to the device name. Inverse of deviceTypeToName();
	*/
	DeviceType _FUSIN_EXPORT nameToDeviceType(String name);

	/*
	Returns the name of the ioType type, such as "Button" or "Axis"
	*/
	String _FUSIN_EXPORT inputTypeToName(IOType t);
	/*
	Returns the device type corresponding to the device name. Inverse of inputTypeToName();
	*/
	IOType _FUSIN_EXPORT nameToInputType(String name);

	class _FUSIN_EXPORT IOCode
	{
	public:
		IOCode();
		IOCode(const IOCode& ic);
		IOCode(DeviceType deviceType, IOType ioType, int32_t index);
		IOCode(uint64_t ic);
		IOCode(const String& name);

		IOCode& operator=(const IOCode& ic);
		bool operator==(const IOCode& ic) const;
		bool operator>(const IOCode& ic) const;
		bool operator<(const IOCode& ic) const;
		bool operator>=(const IOCode& ic) const;
		bool operator<=(const IOCode& ic) const;
		operator uint64_t() const;

		DeviceType deviceType;
		IOType ioType;
		int32_t index;
		
		/*
		Returns the name in one of following formats:
		General name format: <DEVICE NAME> <IOTYPE NAME> <INDEX>[<MODIFIERS>]
		Keyboard format: Key [Typed] <KEY NAME>
		Special device format: <DEVICE NAME> <IOTYPE+INDEX NAME>[<MODIFIERS>]
		Special input format: <DEVICE NAME> <IOTYPE NAME> <INDEX NAME>[<MODIFIERS>]
		*/
		String name() const;
		/*
		Returns the version of this code with the specified device type, and all other attributes unchanged
		*/
		IOCode withDeviceType(DeviceType newDeviceType) const;
		/*
		Returns the version of this code with the specified IO type, and all other attributes unchanged
		*/
		IOCode withIOType(IOType newIOType) const;
		/*
		Returns the version of this code with the specified index value,
		and all other attributes unchanged, including the index flags
		*/
		IOCode withIndex(int32_t newIndex) const;
		/*
		Returns the version of this code with the specified index value,
		and all other attributes unchanged, including the index flags
		*/
		IOCode withIndexFlags(int32_t newIndexFlags) const;
		/*
		Returns the signed positive version of this code
		*/
		IOCode positiveVersion() const;
		/*
		Returns the signed negative version of this code
		*/
		IOCode negativeVersion() const;
		/*
		Returns the unsigned version of this code
		*/
		IOCode unsignedVersion() const;
		/*
		Returns whether the deviceType and ioType are valid
		*/
		bool isValid() const;
		/*
		Returns whether the code fits the filter. The DeviceType and IOType are evaluated separately, 
		i.e. deviceType needs to fit 
		*/
		bool fitsFilter(IOFlags filter);

		// NULL Value
		static const IOCode NULLCODE;
		static const IOCode ANYCODE;
		static const int32_t SIGNED_INDEX_FLAG;
		static const int32_t POSITIVITY_INDEX_FLAG; 
		static const int32_t INDEX_FLAGS;// & this with index to get the index modifier flags
		static const int32_t INDEX_VALUE;// & this with index to get the actual value
	};





	/*
	------------------------------------------------------------
	Old, cluttered versions. Too much effort put to delete it.
	------------------------------------------------------------
	*/

	/*
	Vector containing IOFlags that ALL Devices
	of a certain DeviceType have
	*/
	/*const std::vector<IOFlags> BASE_FLAGS_PER_DEVICE{
		IOF_NONE,//DT_NONE
		IOF_NONE,//DT_ANY
		IOF_TYPING | IOF_HAS_BUTTON,//DT_KEYBOARD
		IOF_CURSOR | IOF_HAS_POSITION | IOF_HAS_MOVEMENT | IOF_HAS_BUTTON,//DT_MOUSE
		IOF_TOUCHPAD | IOF_HAS_POSITION | IOF_HAS_MOVEMENT | IOF_HAS_ACTIVATION,//DT_TOUCHSCREEN
		IOF_JOYSTICK,//DT_GAMEPAD
		IOF_JOYSTICK | IOF_DPAD | IOF_HAS_BUTTON | IOF_HAS_AXIS | IOF_HAS_DIRECTION | IOF_HAS_ANGLE | IOF_HAS_VIBRATION,//DT_XINPUT
		IOF_JOYSTICK | IOF_DPAD | IOF_TOUCHPAD | IOF_MOTION_TRACKER |
			IOF_HAS_BUTTON | IOF_HAS_AXIS | IOF_HAS_DIRECTION | IOF_HAS_ANGLE |
			IOF_HAS_ORIENTATION | IOF_HAS_GYRO | IOF_HAS_ACCELERATION | IOF_HAS_VELOCITY | IOF_HAS_WORLD_POSITION |
			IOF_HAS_POSITION | IOF_HAS_MOVEMENT | IOF_HAS_ACTIVATION |
			IOF_HAS_VIBRATION,//DT_DUALSHOCK
		IOF_HAS_BUTTON | IOF_HAS_AXIS | IOF_HAS_DIRECTION | IOF_HAS_ANGLE//DT_NINTENDO
	};*/

	/*
	Vector containing IOFlags that a Device
	of a certain DeviceType CAN POSSIBLY have
	*/
	/*const std::vector<IOFlags> POSSIBLE_FLAGS_PER_DEVICE{
		IOF_NONE,//DT_NONE
		IOF_ANY,//DT_ANY
		IOF_HAS_BUTTONS | IOF_HAS_LEDS,//DT_KEYBOARD
		IOF_HAS_POSITION | IOF_HAS_MOVEMENT | IOF_HAS_BUTTON | IOF_HAS_AXIS,//DT_MOUSE
		IOF_HAS_POSITION | IOF_HAS_MOVEMENT | IOF_HAS_ACTIVATION,//DT_TOUCHSCREEN
		IOF_JOYSTICK | IOF_HAS_BUTTON | IOF_HAS_AXIS | IOF_HAS_DIRECTION | IOF_HAS_ANGLE | IOF_HAS_VIBRATION,//DT_GAMEPAD
		IOF_JOYSTICK | IOF_DPAD | IOF_HAS_BUTTON | IOF_HAS_AXIS | IOF_HAS_DIRECTION | IOF_HAS_ANGLE | IOF_HAS_VIBRATION,//DT_XINPUT
		IOF_JOYSTICK | IOF_DPAD | IOF_TOUCHPAD | IOF_MOTION_TRACKER |
			IOF_HAS_BUTTON | IOF_HAS_AXIS | IOF_HAS_DIRECTION | IOF_HAS_ANGLE |
			IOF_HAS_ORIENTATION | IOF_HAS_GYRO | IOF_HAS_ACCELERATION | IOF_HAS_VELOCITY | IOF_HAS_WORLD_POSITION |
			IOF_HAS_POSITION | IOF_HAS_MOVEMENT | IOF_HAS_ACTIVATION |
			IOF_HAS_VIBRATION | IOF_HAS_LEDS | IOF_HAS_RGB,//DT_DUALSHOCK
		IOF_JOYSTICK | IOF_DPAD | IOF_TOUCHPAD | IOF_MOTION_TRACKER |
			IOF_HAS_BUTTON | IOF_HAS_AXIS | IOF_HAS_DIRECTION | IOF_HAS_ANGLE |
			IOF_HAS_ORIENTATION | IOF_HAS_GYRO | IOF_HAS_ACCELERATION | IOF_HAS_VELOCITY | IOF_HAS_WORLD_POSITION |
			IOF_HAS_POSITION | IOF_HAS_MOVEMENT | IOF_HAS_ACTIVATION |
			IOF_HAS_VIBRATION//DT_NINTENDO
	};*/

}

#endif