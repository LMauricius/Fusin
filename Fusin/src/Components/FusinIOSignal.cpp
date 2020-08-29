#include "Components/FusinIOSignal.h"

namespace Fusin
{

	IOSignal::IOSignal(IOCode code, IOSignal *positiveSignal, IOSignal *negativeSignal):
		mCode (code),
		mValue(0.0f),
		mNextValue(0.0f),
		mPositiveSignal(positiveSignal),
		mNegativeSignal(negativeSignal),
		mChanged(false)
	{
	}

	IOSignal::~IOSignal()
	{
	}

	void IOSignal::setValue(float val)
	{
		if (val != mNextValue)
		{
			mNextValue = val;
		}
		mChanged = true;
	}

	void IOSignal::quietlySetValue(float val)
	{
		if (val != mNextValue)
		{
			mNextValue = val;
		}
	}

	void IOSignal::press()
	{
		setValue(1);
	}

	void IOSignal::release()
	{
		setValue(0);
	}

	void IOSignal::update()
	{
		if (mValue != mNextValue || mValue != 0.0f)
		{
			mValue = mNextValue;

			if (mPositiveSignal)
			{
				if (mValue > 0.0f)
					mPositiveSignal->setValue(mValue);
				else
					mPositiveSignal->setValue(0.0f);
				mPositiveSignal->update();
			}

			if (mNegativeSignal)
			{
				if (mValue < 0.0f)
					mNegativeSignal->setValue(-mValue);
				else
					mNegativeSignal->setValue(0.0f);
				mNegativeSignal->update();
			}
		}
		mChanged = false;
	}

}
