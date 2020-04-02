#include "FusinCursorComponent.h"
#include "FusinAnyCursor.h"
#include <algorithm>

namespace Fusin
{

	CursorComponent::CursorComponent(DeviceType signalType):
		DeviceComponent(
			{ {IO_MOVEMENT, &mCursorMovement} }, 
			{}),
		axisMoveX(ANY_CURSOR_MOVE_X.withDeviceType(signalType), &axisMoveRight, &axisMoveLeft),
		axisMoveY(ANY_CURSOR_MOVE_Y.withDeviceType(signalType), &axisMoveDown, &axisMoveUp),
		axisMoveLeft(ANY_CURSOR_MOVE_LEFT.withDeviceType(signalType)),
		axisMoveRight(ANY_CURSOR_MOVE_RIGHT.withDeviceType(signalType)),
		axisMoveUp(ANY_CURSOR_MOVE_UP.withDeviceType(signalType)),
		axisMoveDown(ANY_CURSOR_MOVE_DOWN.withDeviceType(signalType)),
		mCursorMovement{&axisMoveX, &axisMoveY}
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
		return IOF_CURSOR | IOF_MOVEMENT;
	}


	void CursorComponent::simulateMovement(float x, float y)
	{
		axisMoveX.setValue(axisMoveX.nextValue() + x);
		axisMoveY.setValue(axisMoveY.nextValue() + y);
	}

	String CursorComponent::getStateString()
	{
		return (StringStream() << "Move(>" << axisMoveX.value() << ",v" << axisMoveY.value() << ")").str();
	}


	void CursorComponent::_update(size_t msElapsed)
	{
		DeviceComponent::_update(msElapsed);

		// _update signals
		axisMoveX.release();// The cursor's movement shouldn't have memory, i.e. it should stop if not specified otherwise
		axisMoveY.release();
	}

}