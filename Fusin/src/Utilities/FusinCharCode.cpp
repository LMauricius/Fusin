#include "Utilities/FusinCharCode.h"
#include <sstream>
#include <map>

namespace Fusin
{

	namespace
	{
		const std::map<String, Char> NameToKeyMap = {
			{FUSIN_STR("Home"), CC_HOME},
			{FUSIN_STR("End"), CC_END},
			{FUSIN_STR("L_Shift"), CC_LSHIFT},
			{FUSIN_STR("R_Shift"), CC_RSHIFT},
			{FUSIN_STR("L_Ctrl"), CC_LCONTROL},
			{FUSIN_STR("R_Ctrl"), CC_RCONTROL},

			{FUSIN_STR("Backspace"), CC_BACKSPACE},
			{FUSIN_STR("Tab"), CC_TAB},
			{FUSIN_STR("New_Line"), CC_NEW_LINE},
			{FUSIN_STR("Clear"), CC_CLEAR},
			{FUSIN_STR("Return"), CC_RETURN},
			{FUSIN_STR("Page_Up"), CC_PAGE_UP},
			{FUSIN_STR("Page_Down"), CC_PAGE_DOWN},

			{FUSIN_STR("Pause"), CC_PAUSE},
			{FUSIN_STR("Num_Lock"), CC_NUM_LOCK},
			{FUSIN_STR("Scroll_Lock"), CC_SCROLL_LOCK},
			{FUSIN_STR("Caps_Lock"), CC_CAPSLOCK},
			
			{FUSIN_STR("L_Alt"), CC_LALT},
			{FUSIN_STR("R_Alt"), CC_RALT},
			{FUSIN_STR("L_Menu"), CC_LMENU},
			{FUSIN_STR("R_Menu"), CC_RMENU},
			{FUSIN_STR("L_Win"), CC_LSUPER},
			{FUSIN_STR("R_Win"), CC_RSUPER},

			{FUSIN_STR("Esc"), CC_ESCAPE},
			{FUSIN_STR("Print_Scr"), CC_PRINT_SCREEN},
			{FUSIN_STR("Insert"), CC_INSERT},

			{FUSIN_STR("Space"), CC_SPACE},
			
			{FUSIN_STR("Multiply"), CC_MULTIPLY},
			{FUSIN_STR("Add"), CC_ADD},
			{FUSIN_STR("Subtract"), CC_SUBTRACT},
			{FUSIN_STR("Decimal"), CC_DECIMAL},
			{FUSIN_STR("Divide"), CC_DIVIDE},

			{FUSIN_STR("Left"), CC_LEFT},
			{FUSIN_STR("Up"), CC_UP},
			{FUSIN_STR("Right"), CC_RIGHT},
			{FUSIN_STR("Down"), CC_DOWN},
			{FUSIN_STR("Del"), CC_DELETE},
		};
	}

	String keyToName(Char c)
	{
		StringStream ss;

		if (c >= CC_NUMPAD_FIRST && c <= CC_NUMPAD_LAST)
		{
			ss << "Num_" << (int)(c - CC_NUMPAD_FIRST);
		}
		else if (c >= CC_FUNCTION_FIRST && c <= CC_FUNCTION_LAST)
		{
			ss << "F" << (int)(c - CC_FUNCTION_FIRST + 1);
		}
		else
		{
			switch (c)
			{
			case CC_HOME: ss << "Home"; break;
			case CC_END: ss << "End"; break;
			case CC_LSHIFT: ss << "L_Shift"; break;
			case CC_RSHIFT: ss << "R_Shift"; break;
			case CC_LCONTROL: ss << "L_Ctrl"; break;
			case CC_RCONTROL: ss << "R_Ctrl"; break;

			case CC_BACKSPACE: ss << "Backspace"; break;
			case CC_TAB: ss << "Tab"; break;
			case CC_NEW_LINE: ss << "New_Line"; break;
			case CC_CLEAR: ss << "Clear"; break;
			case CC_RETURN: ss << "Return"; break;
			case CC_PAGE_UP: ss << "Page_Up"; break;
			case CC_PAGE_DOWN: ss << "Page_Down"; break;

			case CC_PAUSE: ss << "Pause"; break;
			case CC_NUM_LOCK: ss << "Num_Lock"; break;
			case CC_SCROLL_LOCK: ss << "Scroll_Lock"; break;
			case CC_CAPSLOCK: ss << "Caps_Lock"; break;
			
			case CC_LALT: ss << "L_Alt"; break;
			case CC_RALT: ss << "R_Alt"; break;
			case CC_LMENU: ss << "L_Menu"; break;
			case CC_RMENU: ss << "R_Menu"; break;
			case CC_LSUPER: ss << "L_Win"; break;
			case CC_RSUPER: ss << "R_Win"; break;

			case CC_ESCAPE: ss << "Esc"; break;
			case CC_PRINT_SCREEN: ss << "Print_Scr"; break;
			case CC_INSERT: ss << "Insert"; break;

			case CC_SPACE: ss << "Space"; break;
			
			case CC_MULTIPLY: ss << "Multiply"; break;
			case CC_ADD: ss << "Add"; break;
			case CC_SUBTRACT: ss << "Subtract"; break;
			case CC_DECIMAL: ss << "Decimal"; break;
			case CC_DIVIDE: ss << "Divide"; break;
			
			case CC_LEFT: ss << "Left"; break;
			case CC_UP: ss << "Up"; break;
			case CC_RIGHT: ss << "Right"; break;
			case CC_DOWN: ss << "Down"; break;
			case CC_DELETE: ss << "Del"; break;

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
			return CC_NUMPAD_FIRST + n;
		}
		else if (name[0] == 'F')
		{
			StringStream ss(name.substr(1));
			int n;
			ss >> n;
			return CC_FUNCTION_FIRST + n - 1;
		}
		else 
		{
			auto it = NameToKeyMap.find(name);// Add new Charcodes to NameToKeyMap
			if (it != NameToKeyMap.end())
				return it->second;
			else if (name[0] == '\'')
				return name[1];//'c'
		}

		return '\0';
	}

	/*
	List of Charcodes
	
	CC_BACKSPACE
	CC_TAB
	CC_NEW_LINE
	CC_CLEAR
	CC_RETURN
	CC_ENTER
	CC_SHIFT
	CC_CONTROL
	CC_ALT
	CC_PAUSE
	CC_CAPSLOCK
	CC_ESCAPE
	CC_SPACE
	CC_PAGE_UP
	CC_PAGE_DOWN
	CC_END
	CC_HOME
	CC_LEFT
	CC_UP
	CC_RIGHT
	CC_DOWN
	CC_PRINT_SCREEN
	CC_INSERT
	CC_DELETE
	CC_LSUPER
	CC_RSUPER
	CC_MULTIPLY
	CC_ADD
	CC_SUBTRACT
	CC_DECIMAL
	CC_DIVIDE
	CC_NUM_LOCK
	CC_SCROLL_LOCK
	CC_LSHIFT
	CC_RSHIFT
	CC_LCONTROL
	CC_RCONTROL
	CC_LMENU
	CC_RMENU
	*/

}