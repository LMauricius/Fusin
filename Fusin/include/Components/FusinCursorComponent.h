#ifndef _FUSIN_CURSOR_COMPONENT_H
#define _FUSIN_CURSOR_COMPONENT_H

#include "FusinDeviceComponent.h"
#include <vector>
#include <list>
#include <map>

namespace Fusin
{
	/*
	Component for pointing devices.
	While it provides both the position and movement values,
	keep in mind their values aren't connected. 
	The pointer's position can 'teleport', i.e. it can change without
	the movement values being afected.
	Similarly, due to mouse movement being filtered through the OS,
	the change in movement doesn't have to correspond 1:1 to
	the acceleration of the position change
	*/
	class CursorComponent : virtual public DeviceComponent
	{
	public:
		CursorComponent(DeviceType signalType);
		~CursorComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		void setPosition(float x, float y);
		void simulateMovement(float x, float y);
		String getStateString();

		IOSignal positionX, positionY;
		IOSignal moveX, moveY;
		IOSignal moveLeft, moveRight, moveUp, moveDown;

		void _update(size_t msElapsed = 0);

	protected:
		std::vector<IOSignal*> mCursorPosition;
		std::vector<IOSignal*> mCursorMovement;
	};

}

#endif