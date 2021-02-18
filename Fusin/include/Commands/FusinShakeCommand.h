#ifndef _FUSIN_SHAKE_ACTION_H
#define _FUSIN_SHAKE_ACTION_H

#include "FusinInputCommand.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	/*
	The more abstract Command used to interpret simple InputCommands' movement (in multiple axes)
	as shaking.
	
	*/
	class _FUSIN_EXPORT ShakeCommand : public Command
	{
	public:
		ShakeCommand(DeviceEnumerator* devEnum = nullptr);
		~ShakeCommand();

		/*
		Also sets the DeviceEnumerator for all sub-Commands
		*/
		void setDeviceEnumerator(DeviceEnumerator* devEnum);

		/*
		Sets the threshold for axis vector length at which shake is detected.
		Higher setting lowers the sensitivity.
		Be careful not to put the threshold higher than the max axis vector length as
		shaking couldn't be detected then.
		Default is 0.5
		*/
		void setShakeDetectionThreshold(float threshold);
		float getShakeDetectionThreshold() const { return mShakeThreshold; }

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
		Returns the InputCommand for the axis with index ind.
		You can setup these axes to receive input,
		and values from multi-dimensional axes will be used to detect shaking.
		*/
		InputCommand& getAxis(Index ind);

		void update();

	protected:
		float mShakeThreshold;
		std::vector<InputCommand> mAxes;
		std::vector<float> mPrevStretchValues;
	};
}

#endif