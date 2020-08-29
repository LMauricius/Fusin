#include "Commands/FusinPositionCommand.h"
#include <algorithm>

#define FOR_SUBACTIONS(EXP) xAxis.EXP; yAxis.EXP; angleAxis.EXP; leftDirection.EXP; rightDirection.EXP; upDirection.EXP; downDirection.EXP;
#define FOR_SUBACTIONS_EXCEPT_DIRAXIS(EXP) xAxis.EXP; yAxis.EXP; leftDirection.EXP; rightDirection.EXP; upDirection.EXP; downDirection.EXP;

namespace Fusin
{

	PositionCommand::PositionCommand(DeviceEnumerator* devEnum) :
		Command(devEnum),
		xAxis(devEnum),
		yAxis(devEnum),
		angleAxis(devEnum),
		leftDirection(devEnum),
		rightDirection(devEnum),
		upDirection(devEnum),
		downDirection(devEnum)
	{

	}

	PositionCommand::~PositionCommand()
	{
	}

	float PositionCommand::x()
	{
		return mX;
	}

	float PositionCommand::y()
	{
		return mY;
	}

	float PositionCommand::angle()
	{
		return mAngle;
	}

	void PositionCommand::setDeviceEnumerator(DeviceEnumerator* devEnum)
	{
		Command::setDeviceEnumerator(devEnum);
		FOR_SUBACTIONS(setDeviceEnumerator(devEnum));
	}

	void PositionCommand::setDeviceIndex(Index ind, DeviceType t) {
		FOR_SUBACTIONS(setDeviceIndex(ind, t));
	}

	void PositionCommand::setDeviceIndex(Index ind, IOFlags filter) {
		FOR_SUBACTIONS(setDeviceIndex(ind, filter));
	}

	void PositionCommand::setEnabledInputTypes(IOFlags filter) {
		FOR_SUBACTIONS(setEnabledInputTypes(filter));
	}

	void PositionCommand::setDeadZone(float dz, DeviceType deviceType, IOType ioType) {
		FOR_SUBACTIONS(setDeadZone(dz, deviceType, ioType));
	}

	void PositionCommand::setDeadZone(float dz, IOFlags filter) {
		FOR_SUBACTIONS(setDeadZone(dz, filter));
	}

	void PositionCommand::setMaxValue(float val, DeviceType deviceType, IOType ioType) {
		FOR_SUBACTIONS(setMaxValue(val, deviceType, ioType));
	}

	void PositionCommand::setMaxValue(float val, IOFlags filter) {
		FOR_SUBACTIONS(setDeadZone(val, filter));
	}

	void PositionCommand::setFactor(float f, DeviceType deviceType, IOType ioType)
	{
		FOR_SUBACTIONS(setFactor(f, deviceType, ioType));
		ValueModifierUtilities::setModifier(mFactors, f, deviceType, ioType);
	}

	void PositionCommand::setFactor(float f, IOFlags filter)
	{
		FOR_SUBACTIONS(setDeadZone(f, filter));
		ValueModifierUtilities::setModifier(mFactors, f, filter);
	}

	void PositionCommand::update()
	{
		/*
			(x, y, value=distance, angle=(x,y)direction)

		Position calculating algorithm:
			First we try to use the angleAxis' value to determine the angle of this Command.
			If the angle is non-zero (Use 360 for 0° angle) we set
			the value to the factor specified for the angleAxis' strongestIOCode and
			the (x,y) position to the one calculated from the angle and value.
			Note that the value of 1 is multiplied by the factor, not the angle itself!

			Then, xAxis and yAxis' values are used to determine their angle and distance. If the distance is greater than
			the previously calculated value, the value is set to that distance and angle to the calculated angle.
			The position itself is directly derived from the xAxis and yAxis' values.

			After that we check the direction axes (leftDirection,rightDirection,upDirection,downDirection).
			First, the position is calculated from the directions' values by subtracting opposite directions,
			i.e. if both leftDirection and rightDirection have equal values, they are cancelled.
			The angle and distance are calculated the same as in the previous step.
			If the direction axes give a greater value than the angle or position axes, this one is used.
		*/

		// default
		mValue = 0;

		// by angleAxis
		if (angleAxis.value())
		{
			mAngle = angleAxis.value();
			mValue = ValueModifierUtilities::getModifier(
				mFactors,
				angleAxis.strongestIOCode().deviceType,
				angleAxis.strongestIOCode().ioType
			);
			mX = cos((mAngle - 90) * PI / 180.0) * mValue;
			mY = sin((mAngle - 90) * PI / 180.0) * mValue;
		}

		// by position axes
		float xv = xAxis.value();
		float yv = yAxis.value();
		float dist = sqrt(xv * xv + yv * yv);
		if (dist > mValue)
		{
			mAngle = atan2(yv, xv) * 180 / PI + 90;
			mValue = dist;
			mX = xv;
			mY = yv;
		}

		// by direction axes
		xv = rightDirection.value() - leftDirection.value();
		yv = downDirection.value() - upDirection.value();
		dist = sqrt(xv * xv + yv * yv);
		if (dist > mValue)
		{
			mAngle = atan2(yv, xv) * 180 / PI + 90;
			mValue = dist;
			mX = xv;
			mY = yv;
		}

		// parent update
		Command::update();
	}

}