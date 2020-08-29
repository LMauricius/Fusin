#ifndef _FUSIN_RGB_COMPONENT_H
#define _FUSIN_RGB_COMPONENT_H

#include "FusinDeviceComponent.h"
#include "Utilities/FusinColor.h"

#include <vector>

namespace Fusin
{
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

		void setColor(const ColorRGB& c);
		ColorRGB getColor();

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mColorSignals;
	};

}

#endif