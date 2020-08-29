#pragma once
#ifndef _FUSIN_POSITION_ACTION_H
#define _FUSIN_POSITION_ACTION_H

#include "FusinInputCommand.h"
#include <map>

namespace Fusin
{

	/*
	The more abstract Command used to interpret simple InputCommands
	as axis position, direction and strength.

	An example would be interpreting an analog stick as the position
	or a D-Pad as the angle.
	*/
	class PositionCommand : public Command
	{
	public:
		PositionCommand(DeviceEnumerator* devEnum = nullptr);
		~PositionCommand();

		float x();
		float y();
		float angle();

		/*
		Also sets the DeviceEnumerator for all sub-Commands
		*/
		void setDeviceEnumerator(DeviceEnumerator* devEnum);

		/*
		Setters for all sub-Commands
		*/

		void setDeviceIndex(Index ind, DeviceType t = DT_ANY);
		void setDeviceIndex(Index ind, IOFlags filter);
		void setEnabledInputTypes(IOFlags filter);
		void setDeadZone(float dz, DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY);
		void setDeadZone(float dz, IOFlags filter);
		void setMaxValue(float val, DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY);
		void setMaxValue(float val, IOFlags filter);
		void setFactor(float f, DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY);
		void setFactor(float f, IOFlags filter);
		

		/*
		Sub-Commands
		*/

		InputCommand xAxis;
		InputCommand yAxis;
		InputCommand angleAxis;
		InputCommand leftDirection;
		InputCommand rightDirection;
		InputCommand upDirection;
		InputCommand downDirection;

		void update();

	protected:
		float mX, mY, mAngle, mPrevX, mPrevY, mPrevDirection;
		std::map<DeviceType, std::map<IOType, float> > mFactors;
	};
}

#endif