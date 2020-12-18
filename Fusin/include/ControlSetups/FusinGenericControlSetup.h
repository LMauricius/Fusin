#ifndef _FUSIN_GENERIC_CONTROL_SETUP
#define _FUSIN_GENERIC_CONTROL_SETUP

#include "Commands/FusinInputCommand.h"
#include "Commands/FusinPositionCommand.h"
#include "Commands/FusinRangeCommand.h"
#include "Utilities/FusinColor.h"

namespace Fusin
{

	class InputManager;

	class GenericControlSetup
	{
	public:
		GenericControlSetup(InputManager* im = nullptr);
		void setInputManager(InputManager* im);
		// set index to -1 to disable affection this type of device
		void setDeviceIndex(int ind, DeviceType t);
		int getDeviceIndex(DeviceType t);
		void setEnabledInputTypes(IOFlags t = IOF_ANY);
		IOFlags getEnabledInputTypes();

		void setDefaultXInputCodes(int slot);
		void setDefaultDSCodes(int slot);
		void assign(int position, const GenericControlSetup& source, unsigned int first, unsigned int last);

		InputCommand any;
		InputCommand anyButton;

		PositionCommand lStick;
		PositionCommand rStick;

		InputCommand dpadUp;
		InputCommand dpadDown;
		InputCommand dpadLeft;
		InputCommand dpadRight;

		/*
		Face buttons layout:
		    face4               Y        ^
		face3   face2       = X   B or ■   O
		    face1               A        X
		*/
		InputCommand face1;
		InputCommand face2;
		InputCommand face3;
		InputCommand face4;
		InputCommand start;
		InputCommand select;
		InputCommand shoulderL1;
		InputCommand shoulderR1;
		InputCommand shoulderL2;
		InputCommand shoulderR2;
		InputCommand lStickPress;
		InputCommand rStickPress;

		// Menu controls

		InputCommand ok;
		InputCommand cancel;
		InputCommand up;
		InputCommand down;
		InputCommand left;
		InputCommand right;

		void setLeftVibration(float strength = 0, unsigned int msDuration = 0);
		void setRightVibration(float strength = 0, unsigned int msDuration = 0);
		void setVibration(float strength = 0, unsigned int msDuration = 0);
		void setColor(ColorRGB c);

	protected:
		InputManager *mInputManager;
		IOFlags mEnabledInputTypes;
		std::map < DeviceType, unsigned int > mDeviceIndices;
	};

}

#endif