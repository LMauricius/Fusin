#ifndef _FUSIN_CHAR_CODE_H
#define _FUSIN_CHAR_CODE_H

#include "FusinPrerequisites.h"

namespace Fusin
{
	/*
	Defines portable keyboard input codes used by Fusin, called Charcodes. 

	The charcodes for each key are defined as follows:
		* If the key is a special key defined by Fusin in this file,
			the defined Charcode is always given for that key 
			and it is portable between platforms.
		* If the key is not defined in this file and it's printable, 
			its charcode is equivalent to its uppercase Unicode value.
		* If the key is a special key NOT defined by Fusin, 
			the only guarantee is that it's consistent on the local system, 
			i.e. pressing the same key always gives the same Charcode. 
			Such special Charcodes could possibly conflict with other Charcodes.
	
	Implementation note:
		To add a new special Charcode you must add the constant in this file,
		add its Keyboard IOCode equivalent in FusinKey.h,
		define its name in keyToName() and add it to nameToKey()

		The values available for special Charcodes are 
		Unicode control characters and lowercase letters that fit in one byte. 
			00-0f
			10-1f
			61-6f
			70-7a
			80-8f
			90-9f
			e0-ef
			f0-f6, f8-ff
		All other Unicode values are used for printable Charcodes.
	*/



	/*
	Returns the name of the key, as specified by the constants below, or '<key>' for printable characters
	*/
	String _FUSIN_EXPORT keyToName(Char c);
	/*
	Returns the character corresponding to the name. Inverse of keyToName().
	*/
	Char _FUSIN_EXPORT nameToKey(String name);

	const Char CC_BACKSPACE = 0x08;
	const Char CC_TAB = 0x09;
	const Char CC_NEW_LINE = 0x0A;// not used in modern keyboards
	const Char CC_CLEAR = 0x0C;
	const Char CC_RETURN = 0x0D;// Carriage return i.e. 'Enter' key in modern keyboards
	const Char CC_ENTER = CC_RETURN;// 'Enter' key usually produces the Carriage Return character (CC_RETURN), but I also defined it as CC_ENTER in case someone from modern times (like me) forgets this
	const Char CC_SHIFT = 0x10;
	const Char CC_CONTROL = 0x11;
	const Char CC_ALT = 0x12;
	const Char CC_PAUSE = 0x13;
	const Char CC_CAPSLOCK = 0x14;
	const Char CC_ESCAPE = 0x1B;
	const Char CC_SPACE = 0x20;
	const Char CC_PAGE_UP = 0x21;
	const Char CC_PAGE_DOWN = 0x22;
	const Char CC_END = 0x23;
	const Char CC_HOME = 0x24;
	const Char CC_LEFT = 0x25;
	const Char CC_UP = 0x26;
	const Char CC_RIGHT = 0x27;
	const Char CC_DOWN = 0x28;
	const Char CC_PRINT_SCREEN = 0x2C;
	const Char CC_INSERT = 0x2D;
	const Char CC_DELETE = 0x2E;
	const Char CC_LSUPER = 0x5B;
	const Char CC_RSUPER = 0x5C;
	const Char CC_LWIN = CC_LSUPER;
	const Char CC_RWIN = CC_RSUPER;
	const Char CC_NUMPAD_0 = 0x60;
	const Char CC_NUMPAD_1 = 0x61;
	const Char CC_NUMPAD_2 = 0x62;
	const Char CC_NUMPAD_3 = 0x63;
	const Char CC_NUMPAD_4 = 0x64;
	const Char CC_NUMPAD_5 = 0x65;
	const Char CC_NUMPAD_6 = 0x66;
	const Char CC_NUMPAD_7 = 0x67;
	const Char CC_NUMPAD_8 = 0x68;
	const Char CC_NUMPAD_9 = 0x69;
	const Char CC_MULTIPLY = 0x6A;
	const Char CC_ADD = 0x6B;
	const Char CC_SUBTRACT = 0x6D;
	const Char CC_DECIMAL = 0x6E;
	const Char CC_DIVIDE = 0x6F;
	const Char CC_F1 = 0x70;
	const Char CC_F2 = 0x71;
	const Char CC_F3 = 0x72;
	const Char CC_F4 = 0x73;
	const Char CC_F5 = 0x74;
	const Char CC_F6 = 0x75;
	const Char CC_F7 = 0x76;
	const Char CC_F8 = 0x77;
	const Char CC_F9 = 0x78;
	const Char CC_F10 = 0x79;
	const Char CC_F11 = 0x7A;
	const Char CC_F12 = 0x7B;
	const Char CC_NUM_LOCK = 0x90;
	const Char CC_SCROLL_LOCK = 0x91;
	const Char CC_LSHIFT = 0xA0;
	const Char CC_RSHIFT = 0xA1;
	const Char CC_LCONTROL = 0xA2;
	const Char CC_RCONTROL = 0xA3;
	const Char CC_LMENU = 0xA4;
	const Char CC_RMENU = 0xA5;

	const Char CC_NUMPAD_FIRST = CC_NUMPAD_0;
	const Char CC_NUMPAD_LAST = CC_NUMPAD_9;
	const Char CC_FUNCTION_FIRST = CC_F1;
	const Char CC_FUNCTION_LAST = 0x87;
}

#endif