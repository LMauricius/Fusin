#pragma once
#ifndef _FUSIN_INPUT_SIGNAL_MANIPULATION_H
#define _FUSIN_INPUT_SIGNAL_MANIPULATION_H

#include "FusinIOSignal.h"
#include <vector>

namespace Fusin
{
	/*
	Covers the InputSignals by taking each signal's value only if it is larger
	than the value of all other corresponding covered signals and sets it as the coverer signal's next value.
	If the coverer signal has been changed since the last _update, it applies the change to the covered signals instead.
	@param whoOwnerPtrs The vector of pointers to owners of the signals that are being covered
	@param whoOffsetPtr The offset of the IOSignals that are being covered from their owners
	@param by The IOSignal that covers the other one
	*/
	template<class OwnerT1List, class OwnerT2>
	void coverInputSignal(const OwnerT1List& whoOwnerPtrs, IOSignal OwnerT2::* whoMemberPtr, IOSignal& by)
	{
		// If the cover ('by') signal has changed since the last update,
		// apply that change to the covered ('who') signals.
		// If not, take the strongest covered signal's value and
		// set it to the cover signal
		if (by.changed())
		{
			for (auto whoOwner : whoOwnerPtrs)
			{
				IOSignal& who = ((OwnerT2*)whoOwner)->*whoMemberPtr;

				who.setValue(by.value());
			}
		}
		else if (whoOwnerPtrs.size())
		{
			by.setValue(0.0f);

			for (auto whoOwner : whoOwnerPtrs)
			{
				IOSignal& who = ((OwnerT2*)whoOwner)->*whoMemberPtr;

				if (who.nextDistance() >= by.nextDistance())
					by.setValue(who.nextValue());
			}
		}
	}

	/*
	Covers the InputSignals by taking only those covered InputSignals whose absolute values are larger
	than the values of coverer signals and setting them as the respective signal's next value.
	If the coverer signal has been changed since the last _update, it applies the change to the covered signals instead.
	Increases the size of destination vector if necessary, sets the IOType to the newly created InputSignals.
	@param whoOwnerPtrs The vector of pointers to owners of the signals that are being covered
	@param whoOffsetPtrs The vector of offsets of the IOSignals that are being covered from their owners
	@param by The vector of IOSignals that cover the covered signals
	@param deviceType The type of device that will be assigned to
		the newly created IOSignals of the 'by' vector
	@param ioType The type of IO that will be assigned to
		the newly created IOSignals of the 'by' vector
	*/
	template<class OwnerT1List, class OwnerT2>
	void coverInputSignalVector(const OwnerT1List& whoOwnerPtrs, std::vector<IOSignal*> OwnerT2::* whoOffsetPtr, std::vector<IOSignal*>& by,
		DeviceType deviceType, IOType ioType)
	{
		for (int i = 0; i < by.size(); i++)
		{
			// Same as above, except we have to do some reinterpretations differently since
			// whoOffsetPtr is now an offset to the vector of IOSignals, not an offset to IOSignal itself
			if (by[i]->changed())
			{
				for (auto whoOwner : whoOwnerPtrs)
				{
					std::vector<IOSignal*>& who = *((OwnerT2*)whoOwner)->*whoOffsetPtr;

					if (i < who.size())
						who[i]->setValue(by[i]->value());
				}
			}
			else if (whoOwnerPtrs.size())
			{
				by.setValue(0.0f);

				for (auto whoOwner : whoOwnerPtrs)
				{
					std::vector<IOSignal*>& who = *((OwnerT2*)whoOwner)->*whoOffsetPtr;

					if (i < who.size() && who[i]->nextDistance() >= by[i]->nextDistance())
						by[i]->setValue(who[i]->nextValue());
				}
			}
		}
	}

	/*
	Same as above, except it also handles positive and negative versions of IOSignals
	*/
	template<class OwnerT1List, class OwnerT2>
	void coverInputSignalVector(const OwnerT1List& whoOwnerPtrs, std::vector<IOSignal*> OwnerT2::* whoOffsetPtr,
		std::vector<IOSignal*>& by, std::vector<IOSignal*>& byNegative, std::vector<IOSignal*>& byPositive,
		DeviceType deviceType, IOType ioType)
	{
		for (auto whoOwner : whoOwnerPtrs)
		{
			std::vector<IOSignal*>& who = *((OwnerT2*)whoOwner)->*whoOffsetPtr;

			if (who.size() > by.size())
			{
				by.reserve(who.size());
				for (int i = by.size(); i < who.size(); i++) {
					by.push_back(new IOSignal(IOCode(deviceType, ioType, i)));
				}
			}
		}

		// this overload does the rest
		coverInputSignalVector(whoOwnerPtrs, whoOffsetPtr, by);
	}

	/*
	Same as above, except it does not change the sizes of any vectors.
	@param whoOwnerPtrs The vector of pointers to owners of the signals that are being covered
	@param whoOffsetPtrs The vector of offsets of the IOSignals that are being covered from their owners
	@param by The vector of IOSignals that cover the covered signals
	*/
	template<class OwnerT1List, class OwnerT2>
	void coverInputSignalVector(const OwnerT1List& whoOwnerPtrs, std::vector<IOSignal*> OwnerT2::* whoOffsetPtr,
		std::vector<IOSignal*>& by)
	{
		for (auto whoOwner : whoOwnerPtrs)
		{
			std::vector<IOSignal*>& who = *((OwnerT2*)whoOwner)->*whoOffsetPtr;

			if (who.size() > by.size())
			{
				by.reserve(who.size());
				byNegative.reserve(who.size());
				byPositive.reserve(who.size());

				for (int i = by.size(); i < who.size(); i++)
				{
					byNegative.push_back(new IOSignal(IOCode(deviceType, ioType, i).negativeVersion()));
					byPositive.push_back(new IOSignal(IOCode(deviceType, ioType, i).positiveVersion()));
					by.push_back(new IOSignal(IOCode(deviceType, ioType, i), byPositive.back(), byNegative.back()));
				}
			}
		}

		// this overload does the rest
		coverInputSignalVector(whoOwnerPtrs, whoOffsetPtr, by);
	}

	/*
	Just performs _update() on all IOSignals in the vector
	*/
	void updateInputSignalVector(std::vector<IOSignal*>& signals);

	void releaseInputSignalVector(std::vector<IOSignal*>& signals);

	void pressInputSignalVector(std::vector<IOSignal*>& signals);
}

#endif