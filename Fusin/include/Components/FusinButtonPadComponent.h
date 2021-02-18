#ifndef _FUSIN_BUTTON_COMPONENT_H
#define _FUSIN_BUTTON_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	/*
	Component for devices with normal buttons.
	Each button has its own index, with indices starting at 0 and
	continuing until the value of buttonCount()-1.
	*/
	class _FUSIN_EXPORT ButtonPadComponent : public DeviceComponent
	{
	public:
		ButtonPadComponent(DeviceType signalType, size_t buttonNum);
		~ButtonPadComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		size_t buttonCount() const { return mButtons.size(); }

		/*
		Returns the string with indices of buttons that are currently held
		*/
		String getStateString();

		/*
		Returns the IOSignal of the button with the specified index
		*/
		IOSignal& operator[](Index ind);

		void _setButtonCount(size_t buttonNum);
		/*
		Inserts external IOSignals into the component's signals.
		The external IOSignals' ownership won't be taken.
		The IOSignals are inserted after the previously inserted external IOSignals and
		before the IOSignals owned by this ButtonPadComponent.
		Keep in mind that the IOSignals' IOCodes won't be changed, so you need to keep
		track of their indices yourself.
		*/
		void _registerExternalButtons(const std::vector<IOSignal*>& buttons);
		void _update(size_t msElapsed = 0);

	protected:
		DeviceType mSignalDeviceType;
		std::vector<IOSignal*> mButtons;
		size_t mReservedButtonCount;
	};

}

#endif