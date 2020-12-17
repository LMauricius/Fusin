#ifndef _FUSIN_DS_H
#define _FUSIN_DS_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{

	/*
	Returns the DT_DUALSHOCK, IO_BUTTON IOCode for the specified button
	*/
	IOCode DSButton(Index b);

	/*
	Returns the DT_DUALSHOCK, IO_AXIS IOCode for the specified axis.
	*/
	IOCode DSAxis(Index a);

	/*
	Returns the DT_DUALSHOCK, IO_DIRECTION IOCode for the specified value.
	*/
	IOCode DSDPadButton(Index a);

	/*
	Returns the DT_DUALSHOCK, IO_AXIS IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode DSPositiveAxis(Index a);

	/*
	Returns the DT_DUALSHOCK, IO_AXIS IOCode for the specified gamepad axis' negative direction.
	*/
	IOCode DSNegativeAxis(Index a);

	const IOCode DS_SQUARE = DSButton(0);
	const IOCode DS_CROSS = DSButton(1);
	const IOCode DS_CIRCLE = DSButton(2);
	const IOCode DS_TRIANGLE = DSButton(3);
	const IOCode DS_L1 = DSButton(4);
	const IOCode DS_R1 = DSButton(5);
	const IOCode DS_L2_BUTTON = DSButton(6);
	const IOCode DS_R2_BUTTON = DSButton(7);
	const IOCode DS_CREATE = DSButton(8);
	const IOCode DS_OPTIONS = DSButton(9);
	const IOCode DS_L3 = DSButton(10);
	const IOCode DS_R3 = DSButton(11);
	const IOCode DS_PS = DSButton(12);
	const IOCode DS_TOUCHPAD_BUTTON = DSButton(13);
	const IOCode DS_SELECT = DS_CREATE;
	const IOCode DS_START = DS_TOUCHPAD_BUTTON;

	const IOCode DS_LEFT_X_AXIS = DSAxis(0);
	const IOCode DS_LEFT_Y_AXIS = DSAxis(1);
	const IOCode DS_RIGHT_X_AXIS = DSAxis(2);
	const IOCode DS_RIGHT_Y_AXIS = DSAxis(3);
	const IOCode DS_L2 = DSAxis(4);
	const IOCode DS_R2 = DSAxis(5);

	const IOCode DS_DPAD_ANGLE = IOCode(DT_DUALSHOCK, IO_ANGLE, 0);
	const IOCode DS_DPAD_UP = DSDPadButton(0);
	const IOCode DS_DPAD_DOWN = DSDPadButton(1);
	const IOCode DS_DPAD_LEFT = DSDPadButton(2);
	const IOCode DS_DPAD_RIGHT = DSDPadButton(3);

	const IOCode DS_LEFT_STICK_RIGHT = DSPositiveAxis(0);
	const IOCode DS_LEFT_STICK_LEFT = DSNegativeAxis(0);
	const IOCode DS_LEFT_STICK_DOWN = DSPositiveAxis(1);
	const IOCode DS_LEFT_STICK_UP = DSNegativeAxis(1);
	const IOCode DS_RIGHT_STICK_RIGHT = DSPositiveAxis(2);
	const IOCode DS_RIGHT_STICK_LEFT = DSNegativeAxis(2);
	const IOCode DS_RIGHT_STICK_DOWN = DSPositiveAxis(3);
	const IOCode DS_RIGHT_STICK_UP = DSNegativeAxis(3);

	const IOCode DS_TOUCH1 = IOCode(DT_DUALSHOCK, IO_ACTIVATION, 0);
	const IOCode DS_TOUCH2 = IOCode(DT_DUALSHOCK, IO_ACTIVATION, 1);
	const IOCode DS_TOUCH1_X = IOCode(DT_DUALSHOCK, IO_POSITION, 0);
	const IOCode DS_TOUCH1_Y = IOCode(DT_DUALSHOCK, IO_POSITION, 1);
	const IOCode DS_TOUCH2_X = IOCode(DT_DUALSHOCK, IO_POSITION, 2);
	const IOCode DS_TOUCH2_Y = IOCode(DT_DUALSHOCK, IO_POSITION, 3);

	const IOCode DS_GYRO_YAW = IOCode(DT_DUALSHOCK, IO_GYRO, 0);
	const IOCode DS_GYRO_PITCH = IOCode(DT_DUALSHOCK, IO_GYRO, 1);
	const IOCode DS_GYRO_ROLL = IOCode(DT_DUALSHOCK, IO_GYRO, 2);
	const IOCode DS_ACCELERATION_X = IOCode(DT_DUALSHOCK, IO_ACCELERATION, 0);
	const IOCode DS_ACCELERATION_Y = IOCode(DT_DUALSHOCK, IO_ACCELERATION, 1);
	const IOCode DS_ACCELERATION_Z = IOCode(DT_DUALSHOCK, IO_ACCELERATION, 2);
	const IOCode DS_ORIENTATION_YAW = IOCode(DT_DUALSHOCK, IO_ORIENTATION, 0);
	const IOCode DS_ORIENTATION_PITCH = IOCode(DT_DUALSHOCK, IO_ORIENTATION, 1);
	const IOCode DS_ORIENTATION_ROLL = IOCode(DT_DUALSHOCK, IO_ORIENTATION, 2);
	const IOCode DS_VELOCITY_X = IOCode(DT_DUALSHOCK, IO_VELOCITY, 0);
	const IOCode DS_VELOCITY_Y = IOCode(DT_DUALSHOCK, IO_VELOCITY, 1);
	const IOCode DS_VELOCITY_Z = IOCode(DT_DUALSHOCK, IO_VELOCITY, 2);
	const IOCode DS_WORLD_POSITION_X = IOCode(DT_DUALSHOCK, IO_WORLD_POSITION, 0);
	const IOCode DS_WORLD_POSITION_Y = IOCode(DT_DUALSHOCK, IO_WORLD_POSITION, 1);
	const IOCode DS_WORLD_POSITION_Z = IOCode(DT_DUALSHOCK, IO_WORLD_POSITION, 2);

	const IOCode DS_GYRO_YAW_NEGATIVE = DS_GYRO_YAW.negativeVersion();
	const IOCode DS_GYRO_PITCH_NEGATIVE = DS_GYRO_PITCH.negativeVersion();
	const IOCode DS_GYRO_ROLL_NEGATIVE = DS_GYRO_ROLL.negativeVersion();
	const IOCode DS_ACCELERATION_X_NEGATIVE = DS_ACCELERATION_X.negativeVersion();
	const IOCode DS_ACCELERATION_Y_NEGATIVE = DS_ACCELERATION_Y.negativeVersion();
	const IOCode DS_ACCELERATION_Z_NEGATIVE = DS_ACCELERATION_Z.negativeVersion();
	const IOCode DS_ORIENTATION_YAW_NEGATIVE = DS_ORIENTATION_YAW.negativeVersion();
	const IOCode DS_ORIENTATION_PITCH_NEGATIVE = DS_ORIENTATION_PITCH.negativeVersion();
	const IOCode DS_ORIENTATION_ROLL_NEGATIVE = DS_ORIENTATION_ROLL.negativeVersion();
	const IOCode DS_VELOCITY_X_NEGATIVE = DS_VELOCITY_X.negativeVersion();
	const IOCode DS_VELOCITY_Y_NEGATIVE = DS_VELOCITY_Y.negativeVersion();
	const IOCode DS_VELOCITY_Z_NEGATIVE = DS_VELOCITY_Z.negativeVersion();
	const IOCode DS_POSITION_X_NEGATIVE = DS_POSITION_X.negativeVersion();
	const IOCode DS_POSITION_Y_NEGATIVE = DS_POSITION_Y.negativeVersion();
	const IOCode DS_POSITION_Z_NEGATIVE = DS_POSITION_Z.negativeVersion();

	const IOCode DS_GYRO_YAW_POSITIVE = DS_GYRO_YAW.positiveVersion();
	const IOCode DS_GYRO_PITCH_POSITIVE = DS_GYRO_PITCH.positiveVersion();
	const IOCode DS_GYRO_ROLL_POSITIVE = DS_GYRO_ROLL.positiveVersion();
	const IOCode DS_ACCELERATION_X_POSITIVE = DS_ACCELERATION_X.positiveVersion();
	const IOCode DS_ACCELERATION_Y_POSITIVE = DS_ACCELERATION_Y.positiveVersion();
	const IOCode DS_ACCELERATION_Z_POSITIVE = DS_ACCELERATION_Z.positiveVersion();
	const IOCode DS_ORIENTATION_YAW_POSITIVE = DS_ORIENTATION_YAW.positiveVersion();
	const IOCode DS_ORIENTATION_PITCH_POSITIVE = DS_ORIENTATION_PITCH.positiveVersion();
	const IOCode DS_ORIENTATION_ROLL_POSITIVE = DS_ORIENTATION_ROLL.positiveVersion();
	const IOCode DS_VELOCITY_X_POSITIVE = DS_VELOCITY_X.positiveVersion();
	const IOCode DS_VELOCITY_Y_POSITIVE = DS_VELOCITY_Y.positiveVersion();
	const IOCode DS_VELOCITY_Z_POSITIVE = DS_VELOCITY_Z.positiveVersion();
	const IOCode DS_POSITION_X_POSITIVE = DS_POSITION_X.positiveVersion();
	const IOCode DS_POSITION_Y_POSITIVE = DS_POSITION_Y.positiveVersion();
	const IOCode DS_POSITION_Z_POSITIVE = DS_POSITION_Z.positiveVersion();

	const std::vector<String> DS_BUTTON_NAMES {
		FUSIN_STR("Square"), FUSIN_STR("Cross"), FUSIN_STR("Circle"), FUSIN_STR("Triangle"),
		FUSIN_STR("L1"), FUSIN_STR("R1"), FUSIN_STR("L2_Button"), FUSIN_STR("R2_Button"), FUSIN_STR("L3"), FUSIN_STR("R3"), FUSIN_STR("PS"),
		FUSIN_STR("Share"), FUSIN_STR("Options"), FUSIN_STR("TouchPad_Button")
	};
	const std::vector<String> DS_AXIS_NAMES {
		FUSIN_STR("Left_Stick_X"), FUSIN_STR("Left_Stick_Y"), FUSIN_STR("Right_Stick_X"), FUSIN_STR("Right_Stick_Y"),
		FUSIN_STR("L2"), FUSIN_STR("R2")
	};

	const unsigned int DS_BUTTON_NUMBER = 22;
	const unsigned int DS_AXIS_NUMBER = 22;

}

#endif