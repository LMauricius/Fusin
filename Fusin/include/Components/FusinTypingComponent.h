#ifndef _FUSIN_TYPING_COMPONENT_H
#define _FUSIN_TYPING_COMPONENT_H

#include "FusinDeviceComponent.h"
#include "Utilities/FusinCharType.h"
#include <list>
#include <vector>
#include <map>

namespace Fusin
{

	class TypingComponent : public DeviceComponent
	{
		friend InputManager;

	public:
		TypingComponent(DeviceType signalType, size_t keyNum, size_t funcKeyNum);
		~TypingComponent();

		DeviceType deviceType() const;
		IOFlags flags() const;

		// Typing device info
		inline size_t keyNumber() { return mKeyNum; }
		inline size_t functionKeyNumber() { return mFunctionKeyNum; }

		IOSignal& getKey(Char c);
		IOSignal& getTypedKey(Char c);

		/*
		Returns the string of all keys that are currently held down of the specified character type
		*/
		String getKeyString(CharType charTypes = CT_ALL);
		/*
		Returns the string of all keys that have currently been typed of the specified character type
		*/
		String getTypedKeyString(CharType charTypes = CT_ALL);

		void clear();
		void _update(size_t msElapsed = 0);
		void _setKeyCount(int keyNum = 0, int functionKeyNum = 0);

	protected:
		DeviceType mSignalDeviceType;
		int mKeyNum, mFunctionKeyNum;
		std::map<Index, IOSignal*> mKeySignals;
		std::map<Index, IOSignal*> mTypedKeySignals;
		std::vector<Char> mPressedKeys;
		std::vector<Char> mTypedKeys;
	};

}

#endif