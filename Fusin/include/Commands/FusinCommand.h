#ifndef _FUSIN_ACTION_H
#define _FUSIN_ACTION_H

#include "FusinInputCode.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class InputManager;
	class Device;

	class Gesture
	{
	public:
		~Gesture();

		virtual void setInputManager(InputManager* im);
		inline InputManager* inputManager() { return mInputManager; }
		virtual void _beginUpdate();
		virtual void _updateValue(float value, DeviceType deviceType = DT_ANY, IOType ioType = IT_ANY);
		/*
		Starts tracking the specified device if it corresponds to the ioType codes and flags of this Gesture,
		called when a device has been plugged into the InputManager
		*/
		virtual void _plug(Device* d);
		/*
		Stops tracking the specified device, called when a device has been unplugged from the InputManager
		*/
		virtual void _unplug(Device* d);
		virtual void _endUpdate();

		float value();
		float value(IOType t);
		/*
		Returns the absolute value
		*/
		float distance();
		/*
		Returns whether the value has become non-0 since the last _update
		*/
		bool pressed();
		/*
		Returns whether the value has become 0 since the last _update
		*/
		bool released();
		/*
		Returns whether the value is non-zero
		*/
		bool check();
		/*
		The value before the last _update
		*/
		float prevValue();
		/*
		The distance before the last _update
		*/
		float prevDistance();

		void setThreshold(float threshold);
		inline float threshold() { return mThreshold; }

	protected:
		Gesture();

		bool mPressed, mReleased;
		float mValue, mPrevValue, mThreshold;
		std::map<DeviceType, std::map<IOType, float > > mValuesByType;
		InputManager* mInputManager;

	};

	typedef std::vector<Gesture*> GesturePtrVector;
	typedef std::list<Gesture*> GesturePtrList;
}

#endif