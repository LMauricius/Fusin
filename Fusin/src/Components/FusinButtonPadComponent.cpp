#include "Components/FusinButtonPadComponent.h"
#include "Utilities/FusinInputSignalManipulation.h"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdexcept>

namespace Fusin
{
	ButtonPadComponent::ButtonPadComponent(DeviceType signalType, size_t buttonNum):
		DeviceComponent(
			{ {IO_BUTTON, &mButtons} },
			{}),
		mSignalDeviceType(signalType),
		mReservedButtonCount(0)
	{
		_setButtonCount(buttonNum);
	}

	ButtonPadComponent::~ButtonPadComponent()
	{
		for (int i = mReservedButtonCount; i < mButtons.size(); i++)
		{
			delete mButtons[i];
		}
	}

	DeviceType ButtonPadComponent::deviceType() const
	{
		return DT_COMPONENT_BUTTONPAD;
	}

	IOFlags ButtonPadComponent::flags() const
	{
		return IOF_BUTTONPAD | IOF_BUTTON;
	}

	void ButtonPadComponent::_update(size_t msElapsed)
	{
		// read from covered devices
		coverInputSignalVector(mCoveredComponents, &ButtonPadComponent::mButtons, mButtons, mSignalDeviceType, IO_BUTTON);

		// update
		updateInputSignalVector(mButtons);
	}

	IOSignal& ButtonPadComponent::operator[](Index ind)
	{
		if (ind >= mButtons.size()) throw std::out_of_range((std::stringstream() <<
			"Axis index out of range. Trying to acces button " << ind <<
			" but this device only has " << mButtons.size() << " buttons.").str());
		return *mButtons[ind];
	}

	// Input

	String ButtonPadComponent::getStateString()
	{
		StringStream ss;
		ss << std::fixed << std::setprecision(2);

		ss << "buttons: ";
		bool somePressed = false;
		for (int i = 0; i < mButtons.size(); i++)
		{
			if (mButtons[i]->value())
			{
				ss << i << " ";
				somePressed = true;
			}
		}
		if (!somePressed)
			ss << "none";

		return ss.str();
	}


	void ButtonPadComponent::_setButtonCount(size_t buttonNum)
	{
		if (buttonNum > mButtons.size())
		{
			mButtons.reserve(buttonNum);
			for (int i = mButtons.size(); i < buttonNum; i++) {
				mButtons.push_back(new IOSignal(IOCode(mSignalDeviceType, IO_BUTTON, i)));
			}
		}
		else if (buttonNum > mReservedButtonCount)
		{
			for (int i = buttonNum; i < mButtons.size(); i++) {
				delete mButtons[i];
			}
			mButtons.erase(mButtons.begin() + buttonNum, mButtons.end());
		}
		else
		{
			throw std::length_error("Trying to set the button count lower than the reserved button num, which would delete non-owned IOSignals.");
		}
	}

	void ButtonPadComponent::_registerExternalButtons(const std::vector<IOSignal*>& buttons)
	{
		mButtons.insert(mButtons.begin() + mReservedButtonCount, buttons.begin(), buttons.end());
		mReservedButtonCount += buttons.size();
	}
}