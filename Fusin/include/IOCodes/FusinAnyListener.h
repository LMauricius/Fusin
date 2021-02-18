#ifndef _FUSIN__ANY_LISTENER_H
#define _FUSIN__ANY_LISTENER_H

/*#include "Commands/FusinCommand.h"
#include "FusinInputManager.h"
#include "Devices/FusinDevice.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class _FUSIN_EXPORT AnyListener : public Command, InputManagerListener
	{
	public:
		AnyListener(InputManager* im = nullptr);
		~AnyListener();

		void setInputManager(InputManager* im);

		IOCode getLastIOCode() const;
		Index getLastDeviceIndex() const;

		void setTrackedInputTypes(IOType t = IO_ANY);
		IOType getTrackedInputTypes() const;
		void setTrackedDeviceIndex(unsigned int ind, IOType t = IO_ANY);
		unsigned int getTrackedDeviceIndex(IOType t = IO_ANY) const;

		void setDeadZone(float dz, IOType t = IO_ANY);
		float getDeadZone(IOType t = IO_ANY) const;
		void setMaxValue(float val, IOType t = IO_ANY);
		float getMaxValue(IOType t = IO_ANY) const;
		void setFactor(float f, IOType t = IO_ANY);
		float getFactor(IOType t = IO_ANY) const;

	protected:
		IOType mTrackedInputTypes;
		std::map < IOType, unsigned int > mTrackedDeviceIndices;
		std::map < IOType, float > mDeadZones, mMaxValues, mFactors;
		IOCode mLastIOCode;
		unsigned int mLastDeviceIndex;

		void trackProccess();
		void untrackProccess();
		void _beginUpdate();
		void _endUpdate();

		void deviceRegistered(InputManager* im, Device* d);
		void valueUpdated(Device* device, const IOCode& ic, float val);
	};
}

#endif*/