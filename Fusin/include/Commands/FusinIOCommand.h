#pragma once
#ifndef _FUSIN_IO_COMMAND_H
#define _FUSIN_IO_COMMAND_H

#include "Commands/FusinCommand.h"
#include "Utilities/FusinSlotArray.h"
#include <vector>
#include <map>
#include <list>

namespace Fusin
{
	class IOSignal;

	/*
	The most basic types of commands.

	Each IOCommand is indirectly mapped to IOSignals by
	assigning different IOCodes to different slots of an IOCommand.
	This system is meant to make IO mapping easier by connecting a single in-game action or event
	to one or more actual Device IOSignals.
	A common example would be assigning both the keyboard key 'E' and the gamepad button 4 to the action
	of player picking up an item or interacting with the environment.
	Of course, due to this system it is also much easier to implement input remapping functionality to the game.

	They are divided into InputCommands and OutputCommands
	and their detailed descriptions can be found in their respective files.
	*/
	class IOCommand : public Command
	{
	public:
		IOCommand(DeviceEnumerator* devEnum = nullptr);
		~IOCommand() = 0;

		/*
		Assigns the IOCode to the specified slot. Use -1 to assign it to the first free slot.
		*/
		void assignIOCode(const IOCode& ic, Index slot = -1);
		/*
		Copies the IOCodes of the source Command between the specified Indices using the specified position as the first slot.
		Use -1 to assign codes to the first free slots.
		*/
		void assign(Index position, const IOCommand& source, Index first, Index last);
		/*
		Returns the IOCode assigned to the specified slot
		*/
		IOCode getIOCode(Index slot) const;
		/*
		Returns the first slot with the specified IOCode found. Returns -1 if no such slot is found.
		*/
		Index findIOCode(const IOCode& ic) const;
		/*
		Unassigns the IOCode from the InputCommand by clearing all slots with the specified IOCode.
		*/
		void clearIOCode(const IOCode& ic);
		/*
		Unassigns all slots with IOCodes corresponding the specified filter.
		*/
		void clearIOCode(IOFlags filter);
		/*
		Unassigns the IOCode from the slot ind.
		*/
		void clearSlot(Index ind);
		/*
		Clears all slots, effectively disabling the InputCommand until an IOCode is assigned.
		*/
		void clearAll();
		size_t slotCount() const;

		/*
		When a Command is tracked, this Command's value will be  updated with the tracked Command's value
		*/
		void trackCommand(Command* a);
		void untrackCommand(Command* a);
		void untrackAll();
		bool isCommandTracked(Command* a) const;
		const std::list<Command*>& _accessTrackedCommands() const;

		/*
		Sets the Index of the Device whose signals will be connected, for the specified device type.
		Use Index 0 is for global devices.
		*/
		void setDeviceIndex(Index ind, DeviceType t = DT_ANY);
		/*
		Sets the Index of the Device whose signals will be connected, for the specified device type.
		Use Index 0 is for global devices.
		EXAMPLE: setDeviceIndex(1, IO_KEYBOARD | IO_MOUSE) <= Sets the index for keyboard and mouse device inputs to 1
		*/
		void setDeviceIndex(Index ind, IOFlags filter);
		Index getDeviceIndex(DeviceType t = DT_ANY) const;
		/*
		Sets the filter that specifies which IOSignals will be enabled. This is the fast way to enable/disable IOCode slots.
		*/
		void setEnabledInputTypes(IOFlags filter);
		IOFlags getEnabledInputTypes() const;

		// Sets the deadzone for the specified DeviceType and IOType
		void setDeadZone(float dz, DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY);
		// Sets the deadzone for all DeviceTypes and IOTypes corresponding to the filter DeviceType and IOType are checked separately.
		void setDeadZone(float dz, IOFlags filter);
		// If the value is set lower than the deadzone, it will be set to 0
		float getDeadZone(DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY) const;

		// Sets the max value for the specified DeviceType and IOType
		void setMaxValue(float val, DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY);
		// Sets the max value for all DeviceTypes and IOTypes corresponding to the filter. DeviceType and IOType are checked separately.
		void setMaxValue(float val, IOFlags filter);
		// If the value is set greater than the max value, it will be set to 0
		float getMaxValue(DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY) const;

		// Sets the factor for the specified DeviceType and IOType
		void setFactor(float f, DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY);
		// Sets the factor for all DeviceTypes and IOTypes corresponding to the filter DeviceType and IOType are checked separately.
		void setFactor(float f, IOFlags filter);
		// When the value is set, it will be multiplied by the factor
		float getFactor(DeviceType deviceType = DT_ANY, IOType ioType = IO_ANY) const;

		void update();
		void replug();

	protected:
		SlotArray<IOCode> mIOCodes;
		SlotArray<std::pair<IOSignal*, Index> > mSignalIndexPairs;
		IOFlags mEnabledInputFlags;
		std::map<DeviceType, std::map<IOType, float> > mDeadZones, mMaxValues, mFactors;
		std::map<DeviceType, Index> mDeviceIndices;
		std::list<Command*> mTrackedCommands;

		// Child classes should implement this based on whether the value depends on the signal or vice-versa
		virtual void syncValueSignal(IOSignal* sig, Index devInd);

		/*
		Chooses the appropriate signal for the slot with Index ind.
		If there is no appropriate signal, the corresponding mSignalIndexPairs will be free.
		*/
		void replugSlot(Index ind);
	};

	namespace ValueModifierUtilities
	{
		void setModifier(std::map<DeviceType, std::map<IOType, float> >& modMap, float val, DeviceType deviceType, IOType ioType);
		void setModifier(std::map<DeviceType, std::map<IOType, float> >& modMap, float val, IOFlags filter);
		float getModifier(const std::map<DeviceType, std::map<IOType, float> >& modMap, DeviceType deviceType, IOType ioType);
	}
}

#endif