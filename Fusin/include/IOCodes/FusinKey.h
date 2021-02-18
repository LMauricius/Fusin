#ifndef _FUSIN_KEY_H
#define _FUSIN_KEY_H

#include "IOCodes/FusinIOCode.h"

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

	/*
	Returns the name of the key, as specified by the constants below, or '<key>' for printable characters
	*/
	String _FUSIN_EXPORT keyToName(Char c);
	/*
	Returns the character corresponding to the name. Inverse of keyToName().
	*/
	Char _FUSIN_EXPORT nameToKey(String name);

	const IOCode KEY_BACKSPACE = Key(0x08);
	const IOCode KEY_TAB = Key(0x09);
	const IOCode KEY_NEW_LINE = Key(0x0A);// not used in modern keyboards
	const IOCode KEY_CLEAR = Key(0x0C);
	const IOCode KEY_RETURN = Key(0x0D);// Carriage return i.e. 'Enter' key in modern keyboards
	const IOCode KEY_ENTER = KEY_RETURN;// 'Enter' key usually produces the Carriage Return character (KEY_RETURN), but I also defined it as KEY_ENTER in case someone from modern times (like me) forgets this
	const IOCode KEY_SHIFT = Key(0x10);
	const IOCode KEY_CONTROL = Key(0x11);
	const IOCode KEY_ALT = Key(0x12);
	const IOCode KEY_PAUSE = Key(0x13);
	const IOCode KEY_CAPSLOCK = Key(0x14);
	const IOCode KEY_ESCAPE = Key(0x1B);
	const IOCode KEY_SPACE = Key(0x20);
	const IOCode KEY_PAGE_UP = Key(0x21);
	const IOCode KEY_PAGE_DOWN = Key(0x22);
	const IOCode KEY_END = Key(0x23);
	const IOCode KEY_HOME = Key(0x24);
	const IOCode KEY_LEFT = Key(0x25);
	const IOCode KEY_UP = Key(0x26);
	const IOCode KEY_RIGHT = Key(0x27);
	const IOCode KEY_DOWN = Key(0x28);
	const IOCode KEY_PRINT_SCREEN = Key(0x2C);
	const IOCode KEY_INSERT = Key(0x2D);
	const IOCode KEY_DELETE = Key(0x2E);
	const IOCode KEY_LWIN = Key(0x5B);
	const IOCode KEY_RWIN = Key(0x5C);
	const IOCode KEY_NUMPAD_0 = Key(0x60);
	const IOCode KEY_NUMPAD_1 = Key(0x61);
	const IOCode KEY_NUMPAD_2 = Key(0x62);
	const IOCode KEY_NUMPAD_3 = Key(0x63);
	const IOCode KEY_NUMPAD_4 = Key(0x64);
	const IOCode KEY_NUMPAD_5 = Key(0x65);
	const IOCode KEY_NUMPAD_6 = Key(0x66);
	const IOCode KEY_NUMPAD_7 = Key(0x67);
	const IOCode KEY_NUMPAD_8 = Key(0x68);
	const IOCode KEY_NUMPAD_9 = Key(0x69);
	const IOCode KEY_MULTIPLY = Key(0x6A);
	const IOCode KEY_ADD = Key(0x6B);
	const IOCode KEY_SUBTRACT = Key(0x6D);
	const IOCode KEY_DECIMAL = Key(0x6E);
	const IOCode KEY_DIVIDE = Key(0x6F);
	const IOCode KEY_F1 = Key(0x70);
	const IOCode KEY_F2 = Key(0x71);
	const IOCode KEY_F3 = Key(0x72);
	const IOCode KEY_F4 = Key(0x73);
	const IOCode KEY_F5 = Key(0x74);
	const IOCode KEY_F6 = Key(0x75);
	const IOCode KEY_F7 = Key(0x76);
	const IOCode KEY_F8 = Key(0x77);
	const IOCode KEY_F9 = Key(0x78);
	const IOCode KEY_F10 = Key(0x79);
	const IOCode KEY_F11 = Key(0x7A);
	const IOCode KEY_F12 = Key(0x7B);
	const IOCode KEY_NUM_LOCK = Key(0x90);
	const IOCode KEY_SCROLL_LOCK = Key(0x91);
	const IOCode KEY_LSHIFT = Key(0xA0);
	const IOCode KEY_RSHIFT = Key(0xA1);
	const IOCode KEY_LCONTROL = Key(0xA2);
	const IOCode KEY_RCONTROL = Key(0xA3);
	const IOCode KEY_LMENU = Key(0xA4);
	const IOCode KEY_RMENU = Key(0xA5);

	const unsigned int NUMPAD_KEYS_START = 0x60;
	const unsigned int NUMPAD_KEYS_END = 0x69;
	const unsigned int FUNCTION_KEYS_START = 0x70;
	const unsigned int FUNCTION_KEYS_END = 0x87;

}

#endif