#include "Components/FusinDeviceComponent.h"
#include "Utilities/FusinStringBuilder.h"

#define FOR_LISTENERS(EXP) for (auto it : mDeviceListeners) {it->EXP;}

namespace Fusin
{

	DeviceComponent::DeviceComponent(
		const std::map<IOType, std::vector<IOSignal*>*>& orderedSignalsMap,
		const std::map<IOType, std::map<Index, IOSignal*>*>& mappedSignalsMap
	):
		mOrderedSignalsByType(orderedSignalsMap),
		mMappedSignalsByType(mappedSignalsMap)
	{
	}

	DeviceComponent::~DeviceComponent()
	{
	}


	// Values
	float DeviceComponent::getValue(const IOCode& ic) const
	{
		IOSignal* signal = getIOSignal(ic);
		if (signal)
		{
			return signal->value();
		}
		else
		{
			return 0;
		}
	}

	IOSignal* DeviceComponent::getIOSignal(const IOCode& ic) const
	{
		if (ic.deviceType != DT_ANY && ic.deviceType != deviceType())
			return nullptr;

		Index absInd = ic.index & IOCode::INDEX_VALUE;
		IOSignal* signal = nullptr;

		// check ordered signals (listed in a vector)
		auto it1 = mOrderedSignalsByType.find(ic.ioType);
		if (it1 != mOrderedSignalsByType.end())
		{
			if (absInd < it1->second->size())
			{
				signal = (*it1->second)[absInd];
			}
		}

		// check mapped signals
		if (signal == nullptr)
		{
			auto it2 = mMappedSignalsByType.find(ic.ioType);
			if (it2 != mMappedSignalsByType.end())
			{
				auto it3 = it2->second->find(absInd);
				if (it3 != it2->second->end())
				{
					signal = it3->second;
				}
			}
		}

		// Apply index modifiers
		if (signal)
		{
			if (ic.index & IOCode::SIGNED_INDEX_FLAG)
			{
				if (ic.index & IOCode::POSITIVITY_INDEX_FLAG)
					signal = signal->positiveSignal();
				else
					signal = signal->negativeSignal();
			}
		}

		return signal;
	}

	IOSignal* DeviceComponent::getFirstIOSignal(const IOFlags filter) const
	{
		if (!(filter & flags() & IOF_ANY_DEVICE) || !(filter & flags() & IOF_ANY_INPUT))
			return nullptr;

		// check ordered signals (listed in a vector)
		for (auto& typeVectorPair : mOrderedSignalsByType)
		{
			if (FLAG_PER_IO[typeVectorPair.first] & filter)
			{
				/*
				A for loop isn't needed, but we're using it to avoid checking the size of the vector
				and because we will need one in the next method
				*/
				for (auto signal : *typeVectorPair.second)
				{
					// If we're NOT checking the signed versions
					if (
						!(filter & IOF_SIGNED_VERSION) ||
						(signal->positiveSignal() == nullptr && signal->negativeSignal() == nullptr)
						)
						return signal;
					else if (signal->positiveSignal())
						return signal->positiveSignal();
					else if (signal->negativeSignal())
						return signal->negativeSignal();
				}
			}
		}

		// check mapped signals
		for (auto& typeMapPair : mMappedSignalsByType)
		{
			if (FLAG_PER_IO[typeMapPair.first] & filter)
			{
				/*
				Same as above
				*/
				for (auto& indexSignalPair : *typeMapPair.second)
				{
					// If we're NOT checking the signed versions
					if (
						!(filter & IOF_SIGNED_VERSION) ||
						(
							indexSignalPair.second->positiveSignal() == nullptr &&
							indexSignalPair.second->negativeSignal() == nullptr)
						)
						return indexSignalPair.second;
					else if (indexSignalPair.second->positiveSignal())
						return indexSignalPair.second->positiveSignal();
					else if (indexSignalPair.second->negativeSignal())
						return indexSignalPair.second->negativeSignal();
				}
			}
		}

		return nullptr;
	}

	IOSignal* DeviceComponent::getStrongestIOSignal(const IOFlags filter) const
	{
		if (!(filter & flags() & IOF_ANY_DEVICE) || !(filter & flags() & IOF_ANY_INPUT))
			return nullptr;

		float max = 0.0f;
		IOSignal* result = nullptr;

		// check ordered signals (listed in a vector)
		for (auto& typeVectorPair : mOrderedSignalsByType)
		{
			if (FLAG_PER_IO[typeVectorPair.first] & filter)
			{
				for (auto signal : *typeVectorPair.second)
				{
					// If we're NOT checking the signed versions
					if (
						!(filter & IOF_SIGNED_VERSION) ||
						(signal->positiveSignal() == nullptr && signal->negativeSignal() == nullptr)
						)
					{
						if (std::abs(signal->value()) > max)
						{
							max = std::abs(signal->value());
							result = signal;
						}
					}
					else if (signal->positiveSignal())
					{
						if (signal->positiveSignal()->value() > max)
						{
							max = signal->positiveSignal()->value();
							result = signal->positiveSignal();
						}
					}
					else if (signal->negativeSignal())
					{
						if (signal->negativeSignal()->value() > max)
						{
							max = signal->negativeSignal()->value();
							result = signal->negativeSignal();
						}
					}
				}
			}
		}

		// check mapped signals
		for (auto& typeMapPair : mMappedSignalsByType)
		{
			if (FLAG_PER_IO[typeMapPair.first] & filter)
			{
				for (auto& indexSignalPair : *typeMapPair.second)
				{
					auto signal = indexSignalPair.second;

					// If we're NOT checking the signed versions
					if (
						!(filter & IOF_SIGNED_VERSION) ||
						(signal->positiveSignal() == nullptr && signal->negativeSignal() == nullptr)
						)
					{
						if (std::abs(signal->value()) > max)
						{
							max = std::abs(signal->value());
							result = signal;
						}
					}
					else if (signal->positiveSignal())
					{
						if (signal->positiveSignal()->value() > max)
						{
							max = signal->positiveSignal()->value();
							result = signal->positiveSignal();
						}
					}
					else if (signal->negativeSignal())
					{
						if (signal->negativeSignal()->value() > max)
						{
							max = signal->negativeSignal()->value();
							result = signal->negativeSignal();
						}
					}
				}
			}
		}

		return result;
	}

	void DeviceComponent::clear()
	{
		// clear ordered signals (listed in a vector)
		for (auto& typeStructPair : mOrderedSignalsByType)
		{
			for (auto signal : *typeStructPair.second)
			{
				signal->release();
			}
		}

		// clear mapped signals
		for (auto& typeStructPair : mMappedSignalsByType)
		{
			for (auto& indexSignalPair : *typeStructPair.second)
			{
				indexSignalPair.second->release();
			}
		}

		for (auto comp : mCoveredComponents) comp->clear();
	}

	String DeviceComponent::getStateString()
	{
		return String();
	}


	// Internal

	void DeviceComponent::_update(size_t msElapsed)
	{
		/*
		Read from covered components
		*/
		// ordered signals
		for (auto& typeVecPair : mOrderedSignalsByType)
		{
			for (auto signal : *typeVecPair.second)
			{
				if (signal->changed())
				{
					// change covered components' values
					for (auto comp : mCoveredComponents)
						if (auto toSig = comp->getIOSignal(signal->ioCode()))
							toSig->setValue(signal->value());
				}
				else if (mCoveredComponents.size())
				{
					signal->release();// default is 0

					// change values based on the largest covered signal
					for (auto comp : mCoveredComponents)
						if (auto fromSig = comp->getIOSignal(signal->ioCode()))
							if (fromSig->distance() > signal->distance())
								signal->setValue(fromSig->value());
				}
			}
		}

		// mapped signals
		for (auto& typeMapPair : mMappedSignalsByType)
		{
			for (auto& indexSignalPair : *typeMapPair.second)
			{
				auto signal = indexSignalPair.second;

				if (signal->changed())
				{
					// change covered components' values
					for (auto comp : mCoveredComponents)
						if (auto toSig = comp->getIOSignal(signal->ioCode()))
							toSig->setValue(signal->value());
				}
				else if (mCoveredComponents.size())
				{
					signal->release();

					// change values based on the largest covered signal
					for (auto comp : mCoveredComponents)
						if (auto fromSig = comp->getIOSignal(signal->ioCode()))
							if (fromSig->distance() > signal->distance())
								signal->setValue(fromSig->value());
				}
			}
		}

		/*
		Update signals
		*/
		// ordered signals
		for (auto& typeVecPair : mOrderedSignalsByType)
		{
			for (auto signal : *typeVecPair.second)
			{
				signal->update();
			}
		}

		// mapped signals
		for (auto& typeMapPair : mMappedSignalsByType)
		{
			for (auto& indexSignalPair : *typeMapPair.second)
			{
				indexSignalPair.second->update();
			}
		}
	}

	void DeviceComponent::_coverDeviceComponent(DeviceComponent* component)
	{
		if (typeid(*this) != typeid(*component)) {
			throw std::invalid_argument(
					(CStringBuilder() << 
					"The component to be covered must be of the same type as the cover component, i.e " <<
					typeid(*this).name()
					).get()
				);
		}
		else {
			mCoveredComponents.insert(component);
		}
	}

	void DeviceComponent::_uncoverDeviceComponent(DeviceComponent* component)
	{
		mCoveredComponents.erase(component);
	}
}