#include "Components/FusinTouchComponent.h"
#include "IOCOdes/FusinAnyTouch.h"
#include "Utilities/FusinInputSignalManipulation.h"

namespace Fusin
{
	TouchComponent::TouchSignals::TouchSignals(DeviceType signalType, Index ind):
		pressure(AnyTouchActivation(ind).withDeviceType(signalType)),
		positionX(AnyTouchPositionX(ind).withDeviceType(signalType)),
		positionY(AnyTouchPositionY(ind).withDeviceType(signalType)),
		moveX(AnyTouchMoveX(ind).withDeviceType(signalType), &moveRight, &moveLeft),
		moveY(AnyTouchMoveY(ind).withDeviceType(signalType), &moveDown, &moveUp),
		moveLeft(AnyTouchMoveLeft(ind).withDeviceType(signalType)),
		moveRight(AnyTouchMoveRight(ind).withDeviceType(signalType)),
		moveUp(AnyTouchMoveUp(ind).withDeviceType(signalType)),
		moveDown(AnyTouchMoveDown(ind).withDeviceType(signalType))
	{
	}

	TouchComponent::TouchComponent(DeviceType signalType, size_t touchNum):
		DeviceComponent(
			{ {IO_PRESSURE, &mTouchActivations}, {IO_POSITION, &mTouchPositions}, {IO_MOVEMENT, &mTouchMovement} },
			{}),
		mSignalDeviceType(signalType)
	{
		_setTouchCount(touchNum);
	}

	TouchComponent::~TouchComponent()
	{
		for (int i = 0; i < mTouches.size(); i++)
		{
			delete mTouches[i];
		}
	}

	DeviceType TouchComponent::deviceType() const
	{
		return DT_COMPONENT_TOUCHPAD;
	}

	IOFlags TouchComponent::flags() const
	{
		return IOF_TOUCHPAD | IOF_POSITION | IOF_MOVEMENT | IOF_PRESSURE;
	}

	String TouchComponent::getStateString()
	{
		StringStream ss;

		for (int i = 0; i < mTouches.size(); i++)
		{
			if (mTouches[i]->pressure.value())
				ss << "Touch" << i << "(" << mTouches[i]->positionX.value() << ", " << mTouches[i]->positionX.value() << ") ";
		}

		return ss.str();
	}

	void TouchComponent::_update(size_t msElapsed)
	{
		/*
		covered devices
		*/
		coverInputSignalVector(mCoveredComponents, &TouchComponent::mTouchActivations, mTouchActivations);
		coverInputSignalVector(mCoveredComponents, &TouchComponent::mTouchPositions, mTouchPositions);
		coverInputSignalVector(mCoveredComponents, &TouchComponent::mTouchMovement, mTouchMovement);

		/*
		Calc new values
		*/
		for (auto touch : mTouches)
		{
			if (!touch->positionX.changed() && touch->moveX.changed())
				touch->positionX.setValue(touch->positionX.value() + touch->moveX.nextValue());
			else if (!touch->positionX.changed())
				touch->moveX.setValue(0.0f);

			if (!touch->positionY.changed() && touch->moveY.changed())
				touch->positionY.setValue(touch->positionY.value() + touch->moveY.nextValue());
			else if (!touch->positionY.changed())
				touch->moveY.setValue(0.0f);
		}

		/*
		_update
		*/
		updateInputSignalVector(mTouchActivations);
		updateInputSignalVector(mTouchPositions);
		updateInputSignalVector(mTouchMovement);

	}

	void TouchComponent::_setTouchCount(size_t touchNum)
	{
		mTouches.reserve(touchNum);
		mTouchActivations.reserve(touchNum);
		mTouchPositions.reserve(touchNum * 2);
		mTouchMovement.reserve(touchNum * 2);

		// create if needed
		for (int i = mTouches.size(); i < touchNum; i++)
		{
			mTouches.emplace_back(mSignalDeviceType, i);
			mTouchActivations.push_back(&mTouches.back()->pressure);
			mTouchPositions.push_back(&mTouches.back()->positionX);
			mTouchPositions.push_back(&mTouches.back()->positionY);
			mTouchMovement.push_back(&mTouches.back()->moveX);
			mTouchMovement.push_back(&mTouches.back()->moveY);
		}

		// delete if needed
		for (int i = touchNum; i < mTouches.size(); i++)
			delete mTouches[i];
		mTouches.resize(touchNum);
		mTouchActivations.resize(touchNum);
		mTouchPositions.resize(touchNum * 2);
		mTouchMovement.resize(touchNum * 2);
	}

	TouchComponent::TouchSignals& TouchComponent::operator[](Index ind)
	{
		if (ind >= mTouches.size())
			throw std::out_of_range((std::stringstream() <<
				"Trying to access touch #" << ind << " but this device only recognizes " << mTouches.size() << " touches."
				).str());

		return *mTouches[ind];
	}

}