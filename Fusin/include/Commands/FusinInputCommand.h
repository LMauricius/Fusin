#ifndef _FUSIN_INPUT_GESTURE_H
#define _FUSIN_INPUT_GESTURE_H

#include "FusinGesture.h"
#include "FusinSlotArray.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class InputManager;
	class Device;
	class IOSignal;

	class InputGesture : public Gesture
	{
	public:
		InputGesture(InputManager* im = nullptr);
		~InputGesture();

		void setInputManager(InputManager* im);
		void _beginUpdate();
		void _updateValue(float value, IOType t = IT_ANY);
		void _endUpdate();
		virtual void _plug(Device * d);
		virtual void _unplug(Device * d);

		/*
		Assigns the IOCode to the specified slot. Use -1 to assign it to the first free slot.
		*/
		void assignInputCode(const IOCode& ic, Index slot = -1);
		/*
		Copies the InputCodes of the source Gesture between the specified Indices using the specified position as the first slot.
		Use -1 to assign codes to the first free slots.
		*/
		void assign(int position, const InputGesture& source, Index first, Index last);
		/*
		Returns the IOCode assigned to the specified slot
		*/
		IOCode getInputCode(Index slot) const;
		/*
		Returns the first slot with the specified IOCode found. Returns -1 if no such slot is found.
		*/
		Index findInputCode(const IOCode& ic) const;
		/*
		Unassigns the IOCode from the InputGesture by clearing all slots with the specified IOCode.
		*/
		void clearInputCode(const IOCode& ic);
		void clearSlot(Index slot);
		/*
		Clears all slots, effectively disabling the InputGesture until an IOCode is assigned.
		*/
		void clearAll();
		size_t slotCount() const;

		void trackGesture(Gesture* a);
		void untrackGesture(Gesture* a);
		void untrackAll();
		bool isGestureTracked(Gesture* a) const;
		const std::list<Gesture*>& _accessTrackedGestures() const;

		/*
		Sets the Index of the Device whose values will be read, for the specified device types. Use Index 0 is for global devices.
		EXAMPLE: setDeviceIndex(1, IT_KEYBOARD | IT_MOUSE) <= Sets the index for keyboard and mouse device inputs to 1
		*/
		void setDeviceIndex(Index ind, IOType t = IT_ANY);
		Index getDeviceIndex(IOType t = IT_ANY) const;
		void setEnabledInputTypes(IOType t);
		IOType getEnabledInputTypes() const;
		
		void setDeadZone(float dz, IOType t = IT_ANY);
		float getDeadZone(IOType t = IT_ANY) const;
		void setMaxValue(float val, IOType t = IT_ANY);
		float getMaxValue(IOType t = IT_ANY) const;
		void setFactor(float f, IOType t = IT_ANY);
		float getFactor(IOType t = IT_ANY) const;

	protected:
		SlotArray<IOCode> mInputCodes;
		IOType mEnabledInputTypes;
		std::map<IOType, float> mDeadZones, mMaxValues, mFactors;
		std::map<IOType, Index> mDeviceIndices;
		std::list<Gesture*> mTrackedGestures;

		Device * findDevice(const IOCode& ic);
		IOSignal * findSignal(const IOCode& ic);
		void assignProccess(const IOCode& ic);
		void clearProccess(const IOCode& ic);
	};
}

#endif