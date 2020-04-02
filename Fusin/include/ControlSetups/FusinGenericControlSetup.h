#ifndef _FUSIN_GENERIC_CONTROL_SETUP
#define _FUSIN_GENERIC_CONTROL_SETUP

#include "FusinPrerequisites.h"
#include "FusinInputGesture.h"
#include "FusinPositionGesture.h"
#include "FusinRangeGesture.h"
#include "FusinColor.h"

namespace Fusin
{

	class InputManager;

	class GenericControlSetup
	{
	public:
		GenericControlSetup(InputManager* im = nullptr);
		void setInputManager(InputManager* im);
		void setDeviceIndex(unsigned int ind, IOType t = IT_ANY);
		unsigned int getDeviceIndex(IOType t = IT_ANY);
		void setEnabledInputTypes(IOType t = IT_ANY);
		IOType getEnabledInputTypes();

		void setDefaultXBoxCodes(int slot);
		void setDefaultDSCodes(int slot);
		void assign(int position, const GenericControlSetup& source, unsigned int first, unsigned int last);

		InputGesture any;
		InputGesture anyButton;

		PositionGesture lStick;
		PositionGesture rStick;

		InputGesture dpadUp;
		InputGesture dpadDown;
		InputGesture dpadLeft;
		InputGesture dpadRight;

		/*
		Face buttons layout:
		    face4               Y        ^
		face3   face2       = X   B or ■   O
		    face1               A        X
		*/
		InputGesture face1;
		InputGesture face2;
		InputGesture face3;
		InputGesture face4;
		InputGesture start;
		InputGesture select;
		InputGesture shoulderL1;
		InputGesture shoulderR1;
		InputGesture shoulderL2;
		InputGesture shoulderR2;
		InputGesture lStickPress;
		InputGesture rStickPress;

		// Menu controls

		InputGesture ok;
		InputGesture cancel;
		InputGesture up;
		InputGesture down;
		InputGesture left;
		InputGesture right;

		void setLeftVibration(float strength = 0, unsigned int msDuration = 0);
		void setRightVibration(float strength = 0, unsigned int msDuration = 0);
		void setVibration(float strength = 0, unsigned int msDuration = 0);
		void setColor(ColorRGB c);

	protected:
		InputManager *mInputManager;
		IOType mEnabledInputTypes;
		std::map < IOType, unsigned int > mDeviceIndices;
	};

}

#endif