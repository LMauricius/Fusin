#pragma once
#ifndef _FUSIN_INPUT_SIGNAL_MANIPULATION_H
#define _FUSIN_INPUT_SIGNAL_MANIPULATION_H

#include "Components/FusinIOSignal.h"
#include <vector>

namespace Fusin
{
	/*
	Covers the InputSignals by taking each signal's value only if it is larger
	than the value of all other corresponding covered signals and sets it as the coverer signal's next value.
	If the coverer signal has been changed since the last _update, it applies the change to the covered signals instead.
	@param puppetOwnerPtrs The vector of pointers to owners of the signals that are being covered
	@param puppetMemberPtr The offset of the IOSignals that are being covered from their owners
	@param master The IOSignal that covers the other one
	*/
	template<class PuppetOwnerListT, class MasterOwnerT>
	void coverInputSignal(const PuppetOwnerListT& puppetOwnerPtrs, IOSignal MasterOwnerT::* puppetMemberPtr, IOSignal& master)
	{
		// If the cover ('master') signal has changed since the last update,
		// apply that change to the covered ('who') signals.
		// If not, take the strongest covered signal's value and
		// set it to the cover signal
		if (master.changed())
		{
			for (auto puppetOwner : puppetOwnerPtrs)
			{
				IOSignal& puppet = ((MasterOwnerT*)puppetOwner)->*puppetMemberPtr;

				puppet.setValue(master.value());
			}
		}
		else if (puppetOwnerPtrs.size())
		{
			master.setValue(0.0f);

			for (auto puppetOwner : puppetOwnerPtrs)
			{
				IOSignal& puppet = ((MasterOwnerT*)puppetOwner)->*puppetMemberPtr;

				if (puppet.nextDistance() >= master.nextDistance())
					master.setValue(puppet.nextValue());
			}
		}
	}

	/*
	Covers the InputSignals by taking only those covered InputSignals whose absolute values are larger
	than the values of coverer signals and setting them as the respective signal's next value.
	If the coverer signal has been changed since the last _update, it applies the change to the covered signals instead.
	
	It's called 'static' because the vector size doesn't change

	@param puppetOwnerPtrs The vector of pointers to owners of the signals that are being covered
	@param whoOffsetPtrs The vector of offsets of the IOSignals that are being covered from their owners
	@param master The vector of IOSignals that cover the covered signals
	*/
	template<class PuppetOwnerListT, class MasterOwnerT>
	void coverInputSignalVectorStatic(
		const PuppetOwnerListT& puppetOwnerPtrs,
		std::vector<IOSignal*> MasterOwnerT::* puppetOffsetPtr,
		std::vector<IOSignal*>& master
	)
	{
		for (int i = 0; i < master.size(); i++)
		{
			// Same as above, except we have to do some reinterpretations differently since
			// puppetOffsetPtr is now an offset to the vector of IOSignals, not an offset to IOSignal itself
			if (master[i]->changed())
			{
				for (auto puppetOwner : puppetOwnerPtrs)
				{
					std::vector<IOSignal*>& puppet = ((MasterOwnerT*)puppetOwner)->*puppetOffsetPtr;

					if (i < puppet.size())
						puppet[i]->setValue(master[i]->value());
				}
			}
			else if (puppetOwnerPtrs.size())
			{
				master[i]->setValue(0.0f);

				for (auto puppetOwner : puppetOwnerPtrs)
				{
					std::vector<IOSignal*>& puppet = ((MasterOwnerT*)puppetOwner)->*puppetOffsetPtr;

					if (i < puppet.size() && puppet[i]->nextDistance() >= master[i]->nextDistance())
						master[i]->setValue(puppet[i]->nextValue());
				}
			}
		}
	}

	/*
	Same as above, except it increases the size of destination vector if necessary,
	sets the IOType to the newly created InputSignals.

	@param puppetOwnerPtrs The vector of pointers to owners of the signals that are being covered
	@param whoOffsetPtrs The vector of offsets of the IOSignals that are being covered from their owners
	@param master The vector of IOSignals that cover the covered signals
	@param deviceType The type of device that will be assigned to
		the newly created IOSignals of the 'master' vector
	@param ioType The type of IO that will be assigned to
		the newly created IOSignals of the 'master' vector
	*/
	template<class PuppetOwnerListT, class MasterOwnerT>
	void coverInputSignalVectorDynamic(
		const PuppetOwnerListT& puppetOwnerPtrs, std::vector<IOSignal*> MasterOwnerT::* puppetOffsetPtr,
		std::vector<IOSignal*>& master,
		DeviceType deviceType, IOType ioType
	)
	{
		// Ensure the vector is big enough
		for (auto puppetOwner : puppetOwnerPtrs)
		{
			std::vector<IOSignal*>& puppet = ((MasterOwnerT*)puppetOwner)->*puppetOffsetPtr;

			if (puppet.size() > master.size())
			{
				master.reserve(puppet.size());

				for (size_t i = master.size(); i < puppet.size(); i++)
				{
					master.push_back(new IOSignal(IOCode(deviceType, ioType, i)));
				}
			}
		}

		// this overload does the rest
		coverInputSignalVectorStatic(puppetOwnerPtrs, puppetOffsetPtr, master);
	}

	/*
	Same as above, except it also handles positive and negative versions of IOSignals
	*/
	template<class PuppetOwnerListT, class MasterOwnerT>
	void coverInputSignalVectorDynamic(
		const PuppetOwnerListT& puppetOwnerPtrs, std::vector<IOSignal*> MasterOwnerT::* puppetOffsetPtr,
		std::vector<IOSignal*>& master, std::vector<IOSignal*>& masterNegative, std::vector<IOSignal*>& masterPositive,
		DeviceType deviceType, IOType ioType
	)
	{
		// Ensure the vector is big enough
		for (auto puppetOwner : puppetOwnerPtrs)
		{
			std::vector<IOSignal*>& puppet = ((MasterOwnerT*)puppetOwner)->*puppetOffsetPtr;

			if (puppet.size() > master.size())
			{
				master.reserve(puppet.size());
				masterNegative.reserve(puppet.size());
				masterPositive.reserve(puppet.size());

				for (int i = master.size(); i < puppet.size(); i++)
				{
					masterNegative.push_back(new IOSignal(IOCode(deviceType, ioType, i).negativeVersion()));
					masterPositive.push_back(new IOSignal(IOCode(deviceType, ioType, i).positiveVersion()));
					master.push_back(new IOSignal(IOCode(deviceType, ioType, i), masterPositive.back(), masterNegative.back()));
				}
			}
		}

		// this overload does the rest
		coverInputSignalVectorStatic(puppetOwnerPtrs, puppetOffsetPtr, master);
	}

	/*
	Just performs _update() on all IOSignals in the vector
	*/
	void updateInputSignalVector(std::vector<IOSignal*>& signals);

	void releaseInputSignalVector(std::vector<IOSignal*>& signals);

	void pressInputSignalVector(std::vector<IOSignal*>& signals);
}

#endif