#ifndef _FUSIN_INPUT_SIGNAL_H
#define _FUSIN_INPUT_SIGNAL_H

#include "IOCodes/FusinIOCode.h"

#include <list>
#include <set>

namespace Fusin
{
	class _FUSIN_EXPORT IOSignal
	{
	public:
		/*
		code: The code of the ioType whose value is contained in this IOSignal.
		positiveSignal: If specified, the positiveSignal's value is set to this signal's value if
						the value is greater than 0, otherwise it is set to 0.
						It is automatically updated during this signal's _update().
		negativeSignal: If specified, the positiveSignal's value is set to abs(this signal's value) if
						the value is less than 0, otherwise it is set to 0.
						It is automatically updated during this signal's _update().

		*/
		IOSignal(IOCode code, IOSignal *positiveSignal = nullptr, IOSignal *negativeSignal = nullptr);
		~IOSignal();

		inline float value() const { return mValue; }
		/*
		Absolute value i.e. the strength of the signal
		*/
		inline float distance() const { return std::abs(mValue); }
		/*
		Returns the value that will be set during the next _update.
		*/
		inline float nextValue() const { return mNextValue; }
		inline float nextDistance() const { return std::abs(mNextValue); }
		/*
		Returns whether the value has been changed since the last _update.
		*/
		inline bool changed() const { return mChanged; }
		/*
		Returns nullptr if there is no negative version.
		*/
		inline IOSignal* negativeSignal() const { return mNegativeSignal; }
		inline IOSignal* positiveSignal() const { return mPositiveSignal; }
		/*
		Changes the value of this signal. Keep in mind that this only "posts" the new value,
		so the change will only take effect after the next _update() call.
		If you need to retrieve the new value immediately, use nextValue() instead of value().
		*/
		void setValue(float val);
		/*
		Changes the value of this signal without taking note of the signal change.
		*/
		void quietlySetValue(float val);
		/*
		Sets the value to 1
		*/
		void press();
		/*
		Sets the value to 0
		*/
		void release();
		/*
		Updates the current value, positive/negative signals, listeners and the owner
		*/
		void update();
		inline IOCode ioCode() const { return mCode; }

	private:
		float mValue, mNextValue;
		bool mChanged;
		IOCode mCode;
		IOSignal *mPositiveSignal, *mNegativeSignal;
	};

}

#endif