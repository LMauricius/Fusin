#ifndef _FUSIN_CURSOR_COMPONENT_H
#define _FUSIN_CURSOR_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <vector>
#include <list>
#include <map>

namespace Fusin
{
	class CursorComponent : virtual public DeviceComponent
	{
	public:
		CursorComponent(DeviceType signalType);
		~CursorComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		void simulateMovement(float x, float y);
		String getStateString();

		IOSignal axisMoveX, axisMoveY;
		IOSignal axisMoveLeft, axisMoveRight, axisMoveUp, axisMoveDown;

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mCursorMovement;
	};

}

#endif