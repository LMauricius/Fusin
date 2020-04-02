#pragma once
#ifndef _FUSIN_TOUCH_DEVICE_H
#define _FUSIN_TOUCH_DEVICE_H

#include "FusinDeviceComponent.h"
#include <list>
#include <map>
#include <vector>

namespace Fusin
{
	class TouchComponentListener;

	class TouchComponent : public DeviceComponent
	{
	public:
		TouchComponent(DeviceType signalType, size_t touchNum);
		~TouchComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		String getStateString();

		struct TouchSignals {
			IOSignal pressure;
			IOSignal positionX, positionY;
			IOSignal moveX, moveY;
			IOSignal moveLeft, moveRight, moveUp, moveDown;
			TouchSignals(DeviceType signalType, Index ind);
		};
		TouchSignals& operator[](Index ind);

		void _update(size_t msElapsed = 0);
		void _setTouchCount(size_t touchNum);

	protected:
		DeviceType mSignalDeviceType;
		std::vector<TouchSignals*> mTouches;
		std::vector<IOSignal*> mTouchActivations;
		std::vector<IOSignal*> mTouchPositions;
		std::vector<IOSignal*> mTouchMovement;
	};

}

#endif