#ifndef _FUSIN__ANY_LISTENER_H
#define _FUSIN__ANY_LISTENER_H

#include "FusinGesture.h"
#include "FusinInputManager.h"
#include "FusinDevice.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class AnyListener : public Gesture, InputManagerListener, DeviceListener
	{
	public:
		AnyListener(InputManager* im = nullptr);
		~AnyListener();

		void setInputManager(InputManager* im);

		IOCode getLastInputCode() const;
		unsigned int getLastDeviceIndex() const;

		void setTrackedInputTypes(IOType t = IT_ANY);
		IOType getTrackedInputTypes() const;
		void setTrackedDeviceIndex(unsigned int ind, IOType t = IT_ANY);
		unsigned int getTrackedDeviceIndex(IOType t = IT_ANY) const;

		void setDeadZone(float dz, IOType t = IT_ANY);
		float getDeadZone(IOType t = IT_ANY) const;
		void setMaxValue(float val, IOType t = IT_ANY);
		float getMaxValue(IOType t = IT_ANY) const;
		void setFactor(float f, IOType t = IT_ANY);
		float getFactor(IOType t = IT_ANY) const;

	protected:
		IOType mTrackedInputTypes;
		std::map < IOType, unsigned int > mTrackedDeviceIndices;
		std::map < IOType, float > mDeadZones, mMaxValues, mFactors;
		IOCode mLastInputCode;
		unsigned int mLastDeviceIndex;

		void trackProccess();
		void untrackProccess();
		void _beginUpdate();
		void _endUpdate();

		void deviceRegistered(InputManager* im, Device* d);
		void valueUpdated(Device* device, const IOCode& ic, float val);
	};
}

#endif