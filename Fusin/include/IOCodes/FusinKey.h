#ifndef _FUSIN_KEY_H
#define _FUSIN_KEY_H

#include "IOCodes/FusinIOCode.h"
#include "Utilities/FusinCharCode.h"

namespace Fusin
{
	/*
	Returns the DT_KEYBOARD, IO_BUTTON IOCode for the specified character
	*/
	IOCode _FUSIN_EXPORT Key(Char c);

	/*
	Returns the DT_KEYBOARD, IO_BUTTON version of the specified IOCode
	*/
	IOCode _FUSIN_EXPORT Key(const IOCode& ic);

	/*
	Returns the DT_KEYBOARD, IO_BUTTON IOCode for the specified numpad key
	*/
	IOCode _FUSIN_EXPORT NumpadKey(int n);

	/*
	Returns the DT_KEYBOARD, IO_BUTTON IOCode for the specified function key
	*/
	IOCode _FUSIN_EXPORT FunctionKey(int n);

	/*
	Returns the DT_KEYBOARD, IO_TYPED_BUTTON IOCode for the specified character
	*/
	IOCode _FUSIN_EXPORT KeyTyped(Char c);

	/*
	Returns the DT_KEYBOARD, IO_TYPED_BUTTON version of the specified IOCode
	*/
	IOCode _FUSIN_EXPORT KeyTyped(const IOCode& ic);

	const IOCode KEY_HOME = Key(CC_HOME);
	const IOCode KEY_END = Key(CC_END);
	const IOCode KEY_LSHIFT = Key(CC_LSHIFT);
	const IOCode KEY_RSHIFT = Key(CC_RSHIFT);
	const IOCode KEY_LCONTROL = Key(CC_LCONTROL);
	const IOCode KEY_RCONTROL = Key(CC_RCONTROL);

	const IOCode KEY_BACKSPACE = Key(CC_BACKSPACE);
	const IOCode KEY_TAB = Key(CC_TAB);
	const IOCode KEY_NEW_LINE = Key(CC_NEW_LINE);// not used in modern keyboards
	const IOCode KEY_CLEAR = Key(CC_CLEAR);
	const IOCode KEY_RETURN = KEY_CLEAR;// Carriage return i.e. 'Enter' key in modern keyboards
	const IOCode KEY_ENTER = Key(CC_ENTER);// 'Enter' key usually produces the Carriage Return character (CC_RETURN), but I also defined it as CC_ENTER in case someone from modern times (like me) forgets this
	const IOCode KEY_PAGE_UP = Key(CC_PAGE_UP);
	const IOCode KEY_PAGE_DOWN = Key(CC_PAGE_DOWN);
	
	const IOCode KEY_PAUSE = Key(CC_PAUSE);
	const IOCode KEY_NUM_LOCK = Key(CC_NUM_LOCK);
	const IOCode KEY_SCROLL_LOCK = Key(CC_SCROLL_LOCK);
	const IOCode KEY_CAPSLOCK = Key(CC_CAPSLOCK);

	const IOCode KEY_LALT = Key(CC_LALT);
	const IOCode KEY_RALT = Key(CC_RALT);
	const IOCode KEY_LMENU = Key(CC_LMENU);
	const IOCode KEY_RMENU = Key(CC_RMENU);
	const IOCode KEY_LSUPER = Key(CC_LSUPER);
	const IOCode KEY_RSUPER = Key(CC_RSUPER);
	const IOCode KEY_LWIN = KEY_LSUPER;
	const IOCode KEY_RWIN = KEY_RSUPER;

	const IOCode KEY_ESCAPE = Key(CC_ESCAPE);
	const IOCode KEY_PRINT_SCREEN = Key(CC_PRINT_SCREEN);
	const IOCode KEY_INSERT = Key(CC_INSERT);

	const IOCode KEY_SPACE = Key(CC_SPACE);

	const IOCode KEY_NUMPAD_0 = NumpadKey(0);
	const IOCode KEY_NUMPAD_1 = NumpadKey(1);
	const IOCode KEY_NUMPAD_2 = NumpadKey(2);
	const IOCode KEY_NUMPAD_3 = NumpadKey(3);
	const IOCode KEY_NUMPAD_4 = NumpadKey(4);
	const IOCode KEY_NUMPAD_5 = NumpadKey(5);
	const IOCode KEY_NUMPAD_6 = NumpadKey(6);
	const IOCode KEY_NUMPAD_7 = NumpadKey(7);
	const IOCode KEY_NUMPAD_8 = NumpadKey(8);
	const IOCode KEY_NUMPAD_9 = NumpadKey(9);
	const IOCode KEY_MULTIPLY = Key(CC_MULTIPLY);
	const IOCode KEY_ADD = Key(CC_ADD);
	const IOCode KEY_SUBTRACT = Key(CC_SUBTRACT);
	const IOCode KEY_DECIMAL = Key(CC_DECIMAL);
	const IOCode KEY_DIVIDE = Key(CC_DIVIDE);

	const IOCode KEY_LEFT = Key(CC_LEFT);
	const IOCode KEY_UP = Key(CC_UP);
	const IOCode KEY_RIGHT = Key(CC_RIGHT);
	const IOCode KEY_DOWN = Key(CC_DOWN);

	const IOCode KEY_DELETE = Key(CC_DELETE);

	const IOCode KEY_F1 = FunctionKey(0);
	const IOCode KEY_F2 = FunctionKey(1);
	const IOCode KEY_F3 = FunctionKey(2);
	const IOCode KEY_F4 = FunctionKey(3);
	const IOCode KEY_F5 = FunctionKey(4);
	const IOCode KEY_F6 = FunctionKey(5);
	const IOCode KEY_F7 = FunctionKey(6);
	const IOCode KEY_F8 = FunctionKey(7);
	const IOCode KEY_F9 = FunctionKey(8);
	const IOCode KEY_F10 = FunctionKey(9);
	const IOCode KEY_F11 = FunctionKey(10);
	const IOCode KEY_F12 = FunctionKey(11);


	const IOCode KEYBOARD_LED_CAPS   = IOCode(DT_KEYBOARD, IO_LEDS, 0);
	const IOCode KEYBOARD_LED_NUM    = IOCode(DT_KEYBOARD, IO_LEDS, 1);
	const IOCode KEYBOARD_LED_SCROLL = IOCode(DT_KEYBOARD, IO_LEDS, 2);
}

#endif