#ifndef _FUSIN_SHAKE_ACTION_H
#define _FUSIN_SHAKE_ACTION_H

#include "FusinInputGesture.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class InputManager;

	class ShakeGesture : public Gesture
	{
		friend InputManager;

	public:
		ShakeGesture(InputManager* im = nullptr);
		~ShakeGesture();

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

		InputGesture axis1;
		InputGesture axis2;
		InputGesture axis3;

		void _beginUpdate();
		void _endUpdate();

	protected:
		IOType mEnabledInputTypes;
		std::map < IOType, float > mDeadZones, mMaxValues, mFactors;
		std::map < IOType, unsigned int > mDeviceIndices;
		std::map < IOType, float > mLastAxis1ByType;
		std::map < IOType, float > mLastAxis2ByType;
		std::map < IOType, float > mLastAxis3ByType;
	};
}

#endif