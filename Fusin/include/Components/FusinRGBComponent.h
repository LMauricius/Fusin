#ifndef _FUSIN_RGB_COMPONENT_H
#define _FUSIN_RGB_COMPONENT_H

#include "FusinDeviceComponent.h"
#include "Utilities/FusinColor.h"

#include <vector>

namespace Fusin
{
	/*
	Component for devices with RGB changing lights.
	*/
	class RGBComponent : public DeviceComponent
	{
	public:
		RGBComponent(DeviceType signalType);
		~RGBComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		String getStateString();

		IOSignal red;
		IOSignal green;
		IOSignal blue;

		/*
		A quick method of setting red, green, and blue color.
		*/
		void setColor(const ColorRGB& c);
		ColorRGB getColor();

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mColorSignals;
	};

}

#endif