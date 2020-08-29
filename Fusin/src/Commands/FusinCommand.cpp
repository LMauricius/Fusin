#include "Commands/FusinCommand.h"

namespace Fusin
{

	Command::Command(DeviceEnumerator* devEnum):
		mValue(0.0f),
		mPrevValue(0.0f),
		mThreshold(0.5f),
		mStrongestIOCode(IOCode::NULLCODE),
		mStrongestDeviceIndex(-1),
		mPressed(false),
		mReleased(false),
		mUpdateAfterDeviceUpdate(true)
	{
		setDeviceEnumerator(devEnum);
	}

	Command::~Command()
	{
		if (mDeviceEnumerator)
			mDeviceEnumerator->removeListener(this);
	}

	void Command::setDeviceEnumerator(DeviceEnumerator* devEnum)
	{
		if (mDeviceEnumerator)
			mDeviceEnumerator->removeListener(this);

		mDeviceEnumerator = devEnum;

		if (mDeviceEnumerator)
			mDeviceEnumerator->addListener(this);

		replug();
	}

	float Command::distance()
	{
		return std::abs(mValue);
	}

	bool Command::check()
	{
		return std::abs(mValue) > mThreshold;
	}

	float Command::prevDistance()
	{
		return std::abs(mPrevValue);
	}

	void Command::setThreshold(float threshold)
	{
		mThreshold = threshold;
	}

	void Command::update()
	{
		mPressed = (prevDistance() <= mThreshold && check());
		mReleased = (prevDistance() > mThreshold && !check());
		mPrevValue = mValue;
	}

	void Command::replug()
	{
	}

	void Command::deviceRegistered(DeviceEnumerator* de, Device* d)
	{
		replug();
	}

	void Command::deviceUnregistered(DeviceEnumerator* de, Device* d)
	{
		replug();
	}

	void Command::preUpdate()
	{
		if (!mUpdateAfterDeviceUpdate)
			update();
	}

	void Command::postUpdate()
	{
		if (mUpdateAfterDeviceUpdate)
			update();
	}

}