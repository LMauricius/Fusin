#include "Components/FusinCursorComponent.h"
#include "IOCodes/FusinAnyCursor.h"
#include "Utilities/FusinStringBuilder.h"
#include <algorithm>

namespace Fusin
{

	CursorComponent::CursorComponent(DeviceType signalType):
		DeviceComponent(
			{
				{IO_POSITION, &mCursorPosition},
				{IO_MOVEMENT, &mCursorMovement}
			}, 
			{}),
		positionX(ANY_CURSOR_POSITION_X.withDeviceType(signalType)),
		positionY(ANY_CURSOR_POSITION_Y.withDeviceType(signalType)),
		moveX(ANY_CURSOR_MOVE_X.withDeviceType(signalType), &moveRight, &moveLeft),
		moveY(ANY_CURSOR_MOVE_Y.withDeviceType(signalType), &moveDown, &moveUp),
		moveLeft(ANY_CURSOR_MOVE_LEFT.withDeviceType(signalType)),
		moveRight(ANY_CURSOR_MOVE_RIGHT.withDeviceType(signalType)),
		moveUp(ANY_CURSOR_MOVE_UP.withDeviceType(signalType)),
		moveDown(ANY_CURSOR_MOVE_DOWN.withDeviceType(signalType)),
		mCursorPosition{&positionX, &positionY},
		mCursorMovement{&moveX, &moveY}
	{
	}

	CursorComponent::~CursorComponent()
	{
	}

	DeviceType CursorComponent::deviceType() const
	{
		return DT_COMPONENT_CURSOR;
	}

	IOFlags CursorComponent::flags() const
	{
		return IOF_CURSOR | IOF_POSITION | IOF_MOVEMENT;
	}

	void CursorComponent::setPosition(float x, float y)
	{
		positionX.setValue(x);
		positionY.setValue(y);
	}


	void CursorComponent::simulateMovement(float x, float y)
	{
		moveX.setValue(moveX.nextValue() + x);
		moveY.setValue(moveY.nextValue() + y);
	}

	String CursorComponent::getStateString()
	{
		StringStream ss;
		ss << "Pos(>" << positionX.value() << ",v" << positionY.value() << ")" <<
			"Move(>" << moveX.value() << ",v" << moveY.value() << ")";
		return ss.str();
	}


	void CursorComponent::_update(size_t msElapsed)
	{
		DeviceComponent::_update(msElapsed);

		// _update signals
		if (mCoveredComponents.size() == 0)
		{
			moveX.release();// The cursor's movement shouldn't have memory, i.e. it should stop if not specified otherwise
			moveY.release();
		}
	}

}