#ifndef _FUSIN_NINTENDO_H
#define _FUSIN_NINTENDO_H

#include "IOCodes/FusinIOCode.h"
#include <vector>

namespace Fusin
{
	/*
	Returns the DT_NINTENDO, IO_BUTTON IOCode for the specified button
	*/
	IOCode _FUSIN_EXPORT NintendoButton(Index b);

	/*
	Returns the DT_NINTENDO, IO_AXIS IOCode for the specified gamepad axis.
	*/
	IOCode _FUSIN_EXPORT NintendoAxis(Index a);

	/*
	Returns the DT_NINTENDO, IO_DIRECTION IOCode for the specified value.
	*/
	IOCode _FUSIN_EXPORT NintendoDPadButton(Index a);

	/*
	Returns the DT_NINTENDO, IO_AXIS IOCode for the specified gamepad axis' positive direction.
	*/
	IOCode _FUSIN_EXPORT NintendoPositiveAxis(Index a);

	/*
	Returns the DT_NINTENDO, IO_AXIS IOCode for the specified gamepad axis' negative direction.
	*/
	IOCode _FUSIN_EXPORT NintendoNegativeAxis(Index a);

	const IOCode NINTENDO_A = NintendoButton(0);
	const IOCode NINTENDO_B = NintendoButton(1);
	const IOCode NINTENDO_X = NintendoButton(2);
	const IOCode NINTENDO_Y = NintendoButton(3);
	const IOCode NINTENDO_L = NintendoButton(4);
	const IOCode NINTENDO_R = NintendoButton(5);
	const IOCode NINTENDO_ZL = NintendoButton(6);
	const IOCode NINTENDO_ZR = NintendoButton(7);
	const IOCode NINTENDO_MINUS = NintendoButton(8);
	const IOCode NINTENDO_PLUS = NintendoButton(9);
	const IOCode NINTENDO_LEFT_STICK_PRESS = NintendoButton(10);
	const IOCode NINTENDO_RIGHT_STICK_PRESS = NintendoButton(11);
	const IOCode NINTENDO_HOME = NintendoButton(12);
	const IOCode NINTENDO_CAPTURE = NintendoButton(13);
	const IOCode NINTENDO_SL = NintendoButton(14);
	const IOCode NINTENDO_SR = NintendoButton(15);

	const IOCode NINTENDO_LEFT_X_AXIS = NintendoAxis(0);
	const IOCode NINTENDO_LEFT_Y_AXIS = NintendoAxis(1);
	const IOCode NINTENDO_RIGHT_X_AXIS = NintendoAxis(2);
	const IOCode NINTENDO_RIGHT_Y_AXIS = NintendoAxis(3);

	const IOCode NINTENDO_DPAD_ANGLE = IOCode(DT_NINTENDO, IO_ANGLE, 0);
	const IOCode NINTENDO_DPAD_UP = NintendoDPadButton(0);
	const IOCode NINTENDO_DPAD_DOWN = NintendoDPadButton(1);
	const IOCode NINTENDO_DPAD_LEFT = NintendoDPadButton(2);
	const IOCode NINTENDO_DPAD_RIGHT = NintendoDPadButton(3);

	const IOCode NINTENDO_LEFT_STICK_RIGHT = NintendoPositiveAxis(0);
	const IOCode NINTENDO_LEFT_STICK_LEFT = NintendoNegativeAxis(0);
	const IOCode NINTENDO_LEFT_STICK_DOWN = NintendoPositiveAxis(1);
	const IOCode NINTENDO_LEFT_STICK_UP = NintendoNegativeAxis(1);
	const IOCode NINTENDO_RIGHT_STICK_RIGHT = NintendoPositiveAxis(2);
	const IOCode NINTENDO_RIGHT_STICK_LEFT = NintendoNegativeAxis(2);
	const IOCode NINTENDO_RIGHT_STICK_DOWN = NintendoPositiveAxis(3);
	const IOCode NINTENDO_RIGHT_STICK_UP = NintendoNegativeAxis(3);

	const IOCode NINTENDO_GYRO_YAW = IOCode(DT_NINTENDO, IO_GYRO, 0);
	const IOCode NINTENDO_GYRO_PITCH = IOCode(DT_NINTENDO, IO_GYRO, 1);
	const IOCode NINTENDO_GYRO_ROLL = IOCode(DT_NINTENDO, IO_GYRO, 2);
	const IOCode NINTENDO_ACCELERATION_X = IOCode(DT_NINTENDO, IO_ACCELERATION, 0);
	const IOCode NINTENDO_ACCELERATION_Y = IOCode(DT_NINTENDO, IO_ACCELERATION, 1);
	const IOCode NINTENDO_ACCELERATION_Z = IOCode(DT_NINTENDO, IO_ACCELERATION, 2);
	const IOCode NINTENDO_ORIENTATION_YAW = IOCode(DT_NINTENDO, IO_ORIENTATION, 0);
	const IOCode NINTENDO_ORIENTATION_PITCH = IOCode(DT_NINTENDO, IO_ORIENTATION, 1);
	const IOCode NINTENDO_ORIENTATION_ROLL = IOCode(DT_NINTENDO, IO_ORIENTATION, 2);
	const IOCode NINTENDO_VELOCITY_X = IOCode(DT_NINTENDO, IO_VELOCITY, 0);
	const IOCode NINTENDO_VELOCITY_Y = IOCode(DT_NINTENDO, IO_VELOCITY, 1);
	const IOCode NINTENDO_VELOCITY_Z = IOCode(DT_NINTENDO, IO_VELOCITY, 2);
	const IOCode NINTENDO_WORLD_POSITION_X = IOCode(DT_NINTENDO, IO_WORLD_POSITION, 0);
	const IOCode NINTENDO_WORLD_POSITION_Y = IOCode(DT_NINTENDO, IO_WORLD_POSITION, 1);
	const IOCode NINTENDO_WORLD_POSITION_Z = IOCode(DT_NINTENDO, IO_WORLD_POSITION, 2);

	const IOCode NINTENDO_GYRO_YAW_NEGATIVE = NINTENDO_GYRO_YAW.negativeVersion();
	const IOCode NINTENDO_GYRO_PITCH_NEGATIVE = NINTENDO_GYRO_PITCH.negativeVersion();
	const IOCode NINTENDO_GYRO_ROLL_NEGATIVE = NINTENDO_GYRO_ROLL.negativeVersion();
	const IOCode NINTENDO_ACCELERATION_X_NEGATIVE = NINTENDO_ACCELERATION_X.negativeVersion();
	const IOCode NINTENDO_ACCELERATION_Y_NEGATIVE = NINTENDO_ACCELERATION_Y.negativeVersion();
	const IOCode NINTENDO_ACCELERATION_Z_NEGATIVE = NINTENDO_ACCELERATION_Z.negativeVersion();
	const IOCode NINTENDO_ORIENTATION_YAW_NEGATIVE = NINTENDO_ORIENTATION_YAW.negativeVersion();
	const IOCode NINTENDO_ORIENTATION_PITCH_NEGATIVE = NINTENDO_ORIENTATION_PITCH.negativeVersion();
	const IOCode NINTENDO_ORIENTATION_ROLL_NEGATIVE = NINTENDO_ORIENTATION_ROLL.negativeVersion();
	const IOCode NINTENDO_VELOCITY_X_NEGATIVE = NINTENDO_VELOCITY_X.negativeVersion();
	const IOCode NINTENDO_VELOCITY_Y_NEGATIVE = NINTENDO_VELOCITY_Y.negativeVersion();
	const IOCode NINTENDO_VELOCITY_Z_NEGATIVE = NINTENDO_VELOCITY_Z.negativeVersion();
	const IOCode NINTENDO_WORLD_POSITION_X_NEGATIVE = NINTENDO_WORLD_POSITION_X.negativeVersion();
	const IOCode NINTENDO_WORLD_POSITION_Y_NEGATIVE = NINTENDO_WORLD_POSITION_Y.negativeVersion();
	const IOCode NINTENDO_WORLD_POSITION_Z_NEGATIVE = NINTENDO_WORLD_POSITION_Z.negativeVersion();

	const IOCode NINTENDO_GYRO_YAW_POSITIVE = NINTENDO_GYRO_YAW.positiveVersion();
	const IOCode NINTENDO_GYRO_PITCH_POSITIVE = NINTENDO_GYRO_PITCH.positiveVersion();
	const IOCode NINTENDO_GYRO_ROLL_POSITIVE = NINTENDO_GYRO_ROLL.positiveVersion();
	const IOCode NINTENDO_ACCELERATION_X_POSITIVE = NINTENDO_ACCELERATION_X.positiveVersion();
	const IOCode NINTENDO_ACCELERATION_Y_POSITIVE = NINTENDO_ACCELERATION_Y.positiveVersion();
	const IOCode NINTENDO_ACCELERATION_Z_POSITIVE = NINTENDO_ACCELERATION_Z.positiveVersion();
	const IOCode NINTENDO_ORIENTATION_YAW_POSITIVE = NINTENDO_ORIENTATION_YAW.positiveVersion();
	const IOCode NINTENDO_ORIENTATION_PITCH_POSITIVE = NINTENDO_ORIENTATION_PITCH.positiveVersion();
	const IOCode NINTENDO_ORIENTATION_ROLL_POSITIVE = NINTENDO_ORIENTATION_ROLL.positiveVersion();
	const IOCode NINTENDO_VELOCITY_X_POSITIVE = NINTENDO_VELOCITY_X.positiveVersion();
	const IOCode NINTENDO_VELOCITY_Y_POSITIVE = NINTENDO_VELOCITY_Y.positiveVersion();
	const IOCode NINTENDO_VELOCITY_Z_POSITIVE = NINTENDO_VELOCITY_Z.positiveVersion();
	const IOCode NINTENDO_WORLD_POSITION_X_POSITIVE = NINTENDO_WORLD_POSITION_X.positiveVersion();
	const IOCode NINTENDO_WORLD_POSITION_Y_POSITIVE = NINTENDO_WORLD_POSITION_Y.positiveVersion();
	const IOCode NINTENDO_WORLD_POSITION_Z_POSITIVE = NINTENDO_WORLD_POSITION_Z.positiveVersion();

	const std::vector<String> NINTENDO_BUTTON_NAMES{
		FUSIN_STR("A"), FUSIN_STR("B"), FUSIN_STR("X"), FUSIN_STR("Y"),
		FUSIN_STR("L"), FUSIN_STR("R"), FUSIN_STR("ZL"), FUSIN_STR("ZR"), FUSIN_STR("Minus"), FUSIN_STR("Plus"),
		FUSIN_STR("Left Stick"), FUSIN_STR("Right Stick"), FUSIN_STR("Home"), FUSIN_STR("Capture"), FUSIN_STR("SL"), FUSIN_STR("SR")
	};
	const std::vector<String> NINTENDO_AXIS_NAMES{
		FUSIN_STR("Left Stick X"), FUSIN_STR("Left Stick Y"), FUSIN_STR("Right Stick X"), FUSIN_STR("Right Stick Y")
	};

	const unsigned int NINTENDO_BUTTON_NUMBER = 16;
	const unsigned int NINTENDO_AXIS_NUMBER = 11;
}

#endif