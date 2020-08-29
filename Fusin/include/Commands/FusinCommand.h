#ifndef _FUSIN_ACTION_H
#define _FUSIN_ACTION_H

#include "Devices/FusinDeviceEnumerator.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{

	/*
	The abstraction used to make communication between the user and Devices simpler.
	Instead of directly manipulating the Devices and their signals,
	you can use Commands to read, modify and connect to multiple IOSignals at once.
	
	The primary idea behind Commands is making IO remapping easier,
	but they also provide many other helpful functions for IO management.
	
	A single Command usually corresponds to a single abstract IO event, 
	such as moving a character or vibration feedback to the player.
	More specific descriptions for Command usage are given
	in the respective files for each Command type.
	*/
	class Command : protected DeviceEnumeratorListener
	{
	public:
		Command(DeviceEnumerator* devEnum = nullptr);
		~Command();

		virtual void setDeviceEnumerator(DeviceEnumerator* devEnum);
		/*
		The DeviceEnumerator whose Devices' IOSignals will be connected to this Command.
		The Command is also automatically updated during the DeviceEnumerator's update.
		*/
		inline DeviceEnumerator* deviceEnumerator() { return mDeviceEnumerator; }

		/*
		Returns the value of this Command, used to either retrieve IOSignal values or change them
		*/
		float value() const { return mValue; }
		/*
		Returns the IOCode that influenced the value the most
		*/
		IOCode strongestIOCode() const { return mStrongestIOCode; }
		/*
		Returns the index of the Device that influenced the value the most
		*/
		Index strongestDeviceIndex() const { return mStrongestDeviceIndex; }
		/*
		Returns the absolute value
		*/
		float distance();
		/*
		Returns whether the value has become non-0 since the last _update
		*/
		inline bool pressed() const { return mPressed; }
		/*
		Returns whether the value has become 0 since the last _update
		*/
		inline bool released() const { return mReleased; }
		/*
		Returns whether the value's distance is greater than the threshold
		*/
		bool check();
		/*
		The value before the last _update
		*/
		inline float prevValue() const { return mPrevValue; }
		/*
		The distance before the last _update
		*/
		float prevDistance();

		/*
		Sets the threshold at which the value is considered 'pressed'
		*/
		void setThreshold(float threshold);
		inline float threshold() { return mThreshold; }

		/*
		Synchronizes the relations between the Command and IOSignals and updates whether the value is pressed or released
		*/
		virtual void update();
		/*
		Called when deviceEnumerator state is changed, i.e. a Device has been registered or unregistered
		*/
		virtual void replug();

	protected:
		float mValue, mPrevValue, mThreshold;
		IOCode mStrongestIOCode;
		Index mStrongestDeviceIndex;
		bool mPressed, mReleased;
		DeviceEnumerator* mDeviceEnumerator;
		bool mUpdateAfterDeviceUpdate;// Should the command be updated before or after the Device update? (default true = after)

		// Replugged
		void deviceRegistered(DeviceEnumerator* de, Device* d);
		// Replugged
		void deviceUnregistered(DeviceEnumerator* de, Device* d);
		// Called before the IOSubSystems are updated, OutputCommands are updated here
		void preUpdate();
		// Called after the IOSubSystems are updated, InputCommands are updated here
		void postUpdate();

	};
}

#endif