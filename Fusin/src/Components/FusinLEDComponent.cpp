#include "Components/FusinLEDComponent.h"
#include "Utilities/FusinInputSignalManipulation.h"
#include "Utilities/FusinBitCast.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{
	LEDComponent::LEDComponent(DeviceType signalType, size_t LEDNum) :
		DeviceComponent(
			{ {IO_LEDS, &mLEDs} },
			{}),
		mSignalDeviceType(signalType),
		mUseDefaults(true)
	{
		_setLEDCount(LEDNum);
	}

	LEDComponent::~LEDComponent()
	{
		for (int i = 0; i < mLEDs.size(); i++)
		{
			delete mLEDs[i];
		}
	}

	DeviceType LEDComponent::deviceType() const
	{
		return DT_COMPONENT_LEDS;
	}

	IOFlags LEDComponent::flags() const
	{
		return IOF_LEDS;
	}

	void LEDComponent::_update(size_t msElapsed)
	{
		for (int i=0; i<mLEDs.size(); i++)
		{
			if (mLEDs[i]->changed())
			{
				mUseDefaults = false;
			}
		}

		// read from covered devices
		coverInputSignalVectorDynamic(mCoveredComponents, &LEDComponent::mLEDs, mLEDs, mSignalDeviceType, IO_LEDS);

		// update
		updateInputSignalVector(mLEDs);
	}

	IOSignal& LEDComponent::operator[](size_t ind)
	{
		if (ind >= mLEDs.size())
		{
			std::stringstream ss;
			ss << "LED index out of range. Trying to access LED " << ind <<
					" but this device only has " << mLEDs.size() << " LEDs.";
			throw std::out_of_range(ss.str());
		}
		return *mLEDs[ind];
	}
	
	void LEDComponent::setLEDFlags(LEDFlags leds)
	{
		if (leds == LED_AUTO)
		{
			mUseDefaults = true;
		}
		else
		{
			for (int i=0; i<mLEDs.size(); i++)
			{
				mLEDs[i]->setValue(getFlag(leds, i));
			}
		}
	}

	LEDFlags LEDComponent::ledFlags()
	{
		if (mUseDefaults)
		{
			return LED_AUTO;
		}
		else
		{
			LEDFlags ret = LED_NONE;
			for (int i=0; i<mLEDs.size(); i++)
			{
				setFlag(ret, i, mLEDs[i]->value());
			}
			return ret;
		}
	}

	LEDFlags LEDComponent::nextLedFlags()
	{
		LEDFlags ret = LED_NONE;
		for (int i=0; i<mLEDs.size(); i++)
		{
			setFlag(ret, i, mLEDs[i]->nextValue());
		}
		return ret;
	}
	
	void LEDComponent::setUseDefaults(bool enable)
	{
		mUseDefaults = enable;
	}

	bool LEDComponent::useDefaults()
	{
		return mUseDefaults;
	}


	String LEDComponent::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		for (auto led : mLEDs)
		{
			if (led->value())
				ss << "*";
			else
				ss << ".";
		}

		return ss.str();
	}


	void LEDComponent::_setLEDCount(size_t LEDNum)
	{
		if (LEDNum > mLEDs.size())
		{
			mLEDs.reserve(LEDNum);
			for (int i = mLEDs.size(); i < LEDNum; i++)
				mLEDs.push_back(new IOSignal(IOCode(mSignalDeviceType, IO_LEDS, i)));
		}
		else
		{
			for (int i = LEDNum; i < mLEDs.size(); i++)
				delete mLEDs[i];
			mLEDs.erase(mLEDs.begin() + LEDNum, mLEDs.end());
		}
	}
}