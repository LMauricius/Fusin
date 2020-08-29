#include "IOCodes/FusinKey.h"
#include <sstream>

namespace Fusin
{

	IOCode Key(Char c)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, c);
	}

	IOCode Key(const IOCode& ic)
	{
		return IOCode(DT_KEYBOARD, IO_BUTTON, ic.index);
	}

	IOCode NumpadKey(int n)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, NUMPAD_KEYS_START + n);
	}

	IOCode FunctionKey(int n)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, FUNCTION_KEYS_START + n);
	}

	IOCode KeyTyped(Char c)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, c);
	}

	IOCode KeyTyped(const IOCode& ic)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, ic.index);
	}

	String keyToName(Char c)
	{
		StringStream ss;

		if (c >= NUMPAD_KEYS_START && c <= NUMPAD_KEYS_END)
		{
			ss << "Num_" << (int)(c - NUMPAD_KEYS_START);
		}
		else if (c >= FUNCTION_KEYS_START && c <= FUNCTION_KEYS_END)
		{
			ss << "F" << (int)(c - FUNCTION_KEYS_START + 1);
		}
		else
		{
			switch (c)
			{
			case 0x08:
				ss << "Backspace";
				break;
			case 0x09:
				ss << "Tab";
				break;
			case 0x0A:
				ss << "New_Line";
				break;
			case 0x0C:
				ss << "Clear";
				break;
			case 0x0D:
				ss << "Return";
				break;
			case 0x10:
				ss << "Shift";
				break;
			case 0x11:
				ss << "Ctrl";
				break;
			case 0x12:
				ss << "Alt";
				break;
			case 0x13:
				ss << "Pause";
				break;
			case 0x14:
				ss << "Caps_Lock";
				break;
			case 0x1B:
				ss << "Esc";
				break;
			case 0x20:
				ss << "Space";
				break;
			case 0x21:
				ss << "Page_Up";
				break;
			case 0x22:
				ss << "Page_Down";
				break;
			case 0x23:
				ss << "End";
				break;
			case 0x24:
				ss << "Home";
				break;
			case 0x25:
				ss << "Left";
				break;
			case 0x26:
				ss << "Up";
				break;
			case 0x27:
				ss << "Right";
				break;
			case 0x28:
				ss << "Down";
				break;
			case 0x2C:
				ss << "Print_Scr";
				break;
			case 0x2D:
				ss << "Insert";
				break;
			case 0x2E:
				ss << "Del";
				break;
			case 0x5B:
				ss << "L_Win";
				break;
			case 0x5C:
				ss << "R_Win";
				break;
			case 0x6A:
				ss << "'*'";
				break;
			case 0x6B:
				ss << "'+'";
				break;
			case 0x6C:
				ss << "'-'";
				break;
			case 0x6E:
				ss << "'.'";
				break;
			case 0x6F:
				ss << "'/'";
				break;
			case 0x90:
				ss << "Num_Lock";
				break;
			case 0x91:
				ss << "Scroll_Lock";
				break;
			case 0xA0:
				ss << "L_Shift";
				break;
			case 0xA1:
				ss << "R_Shift";
				break;
			case 0xA2:
				ss << "L_Ctrl";
				break;
			case 0xA3:
				ss << "R_Ctrl";
				break;
			case 0xA4:
				ss << "L_Menu";
				break;
			case 0xA5:
				ss << "R_Menu";
				break;
			default:
				ss << "'" << c << "'";
			}
		}

		return ss.str();
	}

	Char nameToKey(String name)
	{
		if (name.length() >= 4 && name.substr(0, 4) == FUSIN_STR("Num_"))
		{
			StringStream ss(name.substr(4));
			int n;
			ss >> n;
			return NUMPAD_KEYS_START + n;
		}
		else if (name[0] == 'F')
		{
			StringStream ss(name.substr(1));
			int n;
			ss >> n;
			return FUNCTION_KEYS_START + n - 1;
		}
		else if (name == L"Backspace")
			return 0x08;
		else if (name == L"Tab")
			return 0x09;
		else if (name == L"New_Line")
			return 0x0A;
		else if (name == L"Clear")
			return 0x0C;
		else if (name == L"Return")
			return 0x0D;
		else if (name == L"Shift")
			return 0x10;
		else if (name == L"Ctrl")
			return 0x11;
		else if (name == L"Alt")
			return 0x12;
		else if (name == L"Pause")
			return 0x13;
		else if (name == L"Caps_Lock")
			return 0x14;
		else if (name == L"Esc")
			return 0x1B;
		else if (name == L"Space")
			return 0x20;
		else if (name == L"Page_Up")
			return 0x21;
		else if (name == L"Page_Down")
			return 0x22;
		else if (name == L"End")
			return 0x23;
		else if (name == L"Home")
			return 0x24;
		else if (name == L"Left")
			return 0x25;
		else if (name == L"Up")
			return 0x26;
		else if (name == L"Right")
			return 0x27;
		else if (name == L"Down")
			return 0x28;
		else if (name == L"Print_Scr")
			return 0x2C;
		else if (name == L"Insert")
			return 0x2D;
		else if (name == L"Del")
			return 0x2E;
		else if (name == L"L_Win")
			return 0x5B;
		else if (name == L"R_Win")
			return 0x5C;
		else if (name == L"'*'")
			return 0x6A;
		else if (name == L"'+'")
			return 0x6B;
		else if (name == L"'-'")
			return 0x6C;
		else if (name == L"'.'")
			return 0x6E;
		else if (name == L"'/'")
			return 0x6F;
		else if (name == L"Num_Lock")
			return 0x90;
		else if (name == L"Scroll_Lock")
			return 0x91;
		else if (name == L"L_Shift")
			return 0xA0;
		else if (name == L"R_Shift")
			return 0xA1;
		else if (name == L"L_Ctrl")
			return 0xA2;
		else if (name == L"R_Ctrl")
			return 0xA3;
		else if (name == L"L_Menu")
			return 0xA4;
		else if (name == L"R_Menu")
			return 0xA5;
		else if (name[0] == '\'')
			return name[1];//'c'

		return '\0';
	}

}