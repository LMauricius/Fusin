#include "Components/FusinTypingComponent.h"
#include "IOCodes/FusinAnyKey.h"

#include <set>

namespace Fusin
{
	TypingComponent::TypingComponent(DeviceType signalType, size_t keyNum, size_t funcKeyNum):
		DeviceComponent(
			{},
			{ {IO_BUTTON, &mKeySignals}, {IO_TYPED_BUTTON, &mTypedKeySignals} }),
		mKeyNum(keyNum),
		mFunctionKeyNum(funcKeyNum),
		mSignalDeviceType(signalType)
	{
	}

	TypingComponent::~TypingComponent()
	{
		for (auto& p : mKeySignals)
			delete p.second;
		for (auto& p : mTypedKeySignals)
			delete p.second;
	}

	DeviceType TypingComponent::deviceType() const
	{
		return DT_COMPONENT_TYPING;
	}

	IOFlags TypingComponent::flags() const
	{
		return IOF_TYPING | IOF_BUTTON | IOF_TYPED_BUTTON;
	}

	void TypingComponent::_setKeyCount(int keyNum, int functionKeyNum)
	{
		mKeyNum = keyNum;
		mFunctionKeyNum = functionKeyNum;
	}
	
	IOSignal& TypingComponent::operator[](Char c)
	{
		auto it = mKeySignals.find(c);
		if (it != mKeySignals.end()) {
			return *it->second;
		}
		else {
			auto p = mKeySignals.insert(std::make_pair(
				c,
				new IOSignal(AnyKey(c).withDeviceType(mSignalDeviceType))
			)).first;
			return *p->second;
		}
	}

	IOSignal& TypingComponent::getTypedKey(Char c)
	{
		auto it = mTypedKeySignals.find(c);
		if (it != mTypedKeySignals.end()) {
			return *it->second;
		}
		else {
			auto p = mTypedKeySignals.insert(std::make_pair(
				c,
				new IOSignal(AnyKeyTyped(c).withDeviceType(mSignalDeviceType))
			)).first;
			return *p->second;
		}
	}

	String TypingComponent::getKeyString(CharType keyTypes)
	{
		String str;

		str.reserve(mPressedKeys.size() + 1);
		for (Char c : mPressedKeys)
		{
			if (getCharType(c) | keyTypes)
			{
				str += c;
			}
		}

		return str;
	}

	String TypingComponent::getTypedKeyString(CharType keyTypes)
	{
		String str;

		str.reserve(mTypedKeys.size() + 1);
		for (Char c : mTypedKeys)
		{
			if (getCharType(c) | keyTypes)
			{
				str += c;
			}
		}

		return str;
	}

	void TypingComponent::_update(size_t msElapsed)
	{
		/*
		covered devices
		*/
		if (mCoveredComponents.size())
		{
			// build sets of keys pressed externally so that we can use this info to write those keys to covered devices
			std::set<Char> newlyPressedKeys, newlyTypedKeys;

			for (auto p : mKeySignals)
			{
				IOSignal* signal = p.second;

				if (signal->changed() && signal->value() > 0.0f)
					newlyPressedKeys.insert(signal->ioCode().index);
			}
			for (auto p : mTypedKeySignals)
			{
				IOSignal* signal = p.second;

				if (signal->changed() && signal->value() > 0.0f)
					newlyTypedKeys.insert(signal->ioCode().index);
			}

			// reset old pressed keys, some of these were read from the covered devices in the previous update; we don't need those
			for (Char c : mPressedKeys)
				mKeySignals[c]->release();
			for (Char c : mTypedKeys)
				mTypedKeySignals[c]->release();

			for (auto it : mCoveredComponents)
			{
				// read from devices
				for (auto& charSignalPair : static_cast<TypingComponent*>(it)->mKeySignals)
				{
					if (charSignalPair.second->value())
						getKey(charSignalPair.first).press();
				}
				for (auto& charSignalPair : static_cast<TypingComponent*>(it)->mTypedKeySignals)
				{
					if (charSignalPair.second->value())
						getTypedKey(charSignalPair.first).press();
				}

				// write to devices
				for (Char c : newlyPressedKeys)
					static_cast<TypingComponent*>(it)->getKey(c).press();
				for (Char c : newlyTypedKeys)
					static_cast<TypingComponent*>(it)->getTypedKey(c).press();
			}

			// Now that old pressed keys have been filtered out and new keys have been read from the covered devices,
			// we can re-press the keys that have been pressed externally that we have released above
			for (Char c : newlyPressedKeys)
				mKeySignals[c]->press();
			for (Char c : newlyTypedKeys)
				mTypedKeySignals[c]->press();
		}

		/*
		update signals
		*/
		mPressedKeys.clear();// we fill these sets only with currently active signals
		mTypedKeys.clear();
		for (auto p : mKeySignals)
		{
			IOSignal* signal = p.second;
			signal->update();
			if (signal->value() > 0.0f)
				mPressedKeys.push_back(signal->ioCode().index);
		}
		for (auto p : mTypedKeySignals)
		{
			IOSignal* signal = p.second;
			signal->update();
			if (signal->value() > 0.0f)
				mTypedKeys.push_back(signal->ioCode().index);
			signal->release();// TypedKey signals should be released in the next step
		}
	}

	void TypingComponent::clear()
	{
		for (Char c : mPressedKeys)
			mKeySignals[c]->release();

		for (Char c : mPressedKeys)
			mTypedKeySignals[c]->release();

		mPressedKeys.clear();
		mTypedKeys.clear();
	}

}