#ifndef _FUSIN_POSITION_ACTION_H
#define _FUSIN_POSITION_ACTION_H

#include "FusinInputGesture.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class InputManager;

	class PositionGesture : public Gesture
	{
		friend InputManager;

	public:
		PositionGesture(InputManager* im = nullptr);
		~PositionGesture();

		float x();
		float y();
		float direction();

		/*
		Sets the InputManager for the PositionGesture and all subGestures
		*/
		void setInputManager(InputManager* im);

		/*
		Sets the device indices for the PositionGesture and all subGestures
		*/
		void setDeviceIndex(unsigned int ind, IOType t = IT_ANY);
		unsigned int getDeviceIndex(IOType t = IT_ANY);
		/*
		Sets the enabled ioType types for the PositionGesture and all subGestures
		*/
		void setEnabledInputTypes(IOType t = IT_ANY);
		IOType getEnabledInputTypes();

		void setDeadZone(float dz, IOType t = IT_ANY);
		float getDeadZone(IOType t = IT_ANY);
		void setMaxValue(float val, IOType t = IT_ANY);
		float getMaxValue(IOType t = IT_ANY);
		void setFactor(float f, IOType t = IT_ANY);
		float getFactor(IOType t = IT_ANY);

		/*
		Sub Gestures
		*/

		InputGesture xAxis;
		InputGesture yAxis;
		InputGesture directionAxis;
		InputGesture leftDirection;
		InputGesture rightDirection;
		InputGesture upDirection;
		InputGesture downDirection;

		void _beginUpdate();
		void _endUpdate();

	protected:
		float mX, mY, mDirection, mPrevX, mPrevY, mPrevDirection;
		IOType mEnabledInputTypes;
		std::map < IOType, float > mDeadZones, mMaxValues, mFactors;
		std::map < IOType, unsigned int > mDeviceIndices;

	};
}

#endif