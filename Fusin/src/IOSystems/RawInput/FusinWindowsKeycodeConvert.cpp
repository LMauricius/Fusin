#include "IOSystems/RawInput/FusinWindowsKeycodeConvert.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	#include "IOCodes/FusinKey.h"

	#include <vector>

	#define EXTENDED_KEYMASK (1<<24)

	namespace Fusin
	{
        Char virtualKeycodeToVirtualKey(int vkey, int scancode, USHORT flags)
		{
			// special codes
			switch (vkey)
			{
			case VK_HOME: return CC_HOME;
			case VK_END: return CC_END;
			case VK_SHIFT:
				if (MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX) == VK_LSHIFT)
					return CC_LSHIFT;
				else
					return CC_RSHIFT;
			case VK_LSHIFT: return CC_LSHIFT;
			case VK_RSHIFT: return CC_RSHIFT;
			case VK_CONTROL:
				if ( flags & RI_KEY_E0 )
					return CC_RCONTROL;
				else
					return CC_LCONTROL;
			case VK_LCONTROL: return CC_LCONTROL;
			case VK_RCONTROL: return CC_RCONTROL;

			case VK_BACK: return CC_BACKSPACE;
			case VK_TAB: return CC_TAB;
			//case VK_LINE: return CC_NEW_LINE;// not used in modern keyboards
			case VK_CLEAR: return CC_CLEAR;
			case VK_RETURN: return CC_RETURN;
			case VK_PRIOR: return CC_PAGE_UP;
			case VK_NEXT: return CC_PAGE_DOWN;

			case VK_PAUSE: return CC_PAUSE;
			case VK_NUMLOCK: return CC_NUM_LOCK;
			case VK_SCROLL: return CC_SCROLL_LOCK;
			case VK_CAPITAL: return CC_CAPSLOCK;

			case VK_MENU: 
				if ( flags & RI_KEY_E0 )
					return CC_RALT;
				else
					return CC_LALT;
			case VK_LMENU: return CC_LALT;
			case VK_RMENU: return CC_RALT;
			case VK_APPS: return CC_MENU;
			case VK_LWIN: return CC_LWIN;
			case VK_RWIN: return CC_RWIN;

			case VK_ESCAPE: return CC_ESCAPE;
			case VK_PRINT:
			case VK_SNAPSHOT: return CC_PRINT_SCREEN;
			case VK_INSERT: return CC_INSERT;

			case VK_SPACE: return CC_SPACE;

			case VK_MULTIPLY: return CC_MULTIPLY;
			case VK_ADD: return CC_ADD;
			case VK_SUBTRACT: return CC_SUBTRACT;
			case VK_DECIMAL: return CC_DECIMAL;
			case VK_DIVIDE: return CC_DIVIDE;
			
			case VK_LEFT: return CC_LEFT;
			case VK_UP: return CC_UP;
			case VK_RIGHT: return CC_RIGHT;
			case VK_DOWN: return CC_DOWN;
			
			case VK_DELETE: return CC_DELETE;
			}

			// Numpad
			if (vkey >= VK_NUMPAD0 && vkey <= VK_NUMPAD9)
			{
				return CC_NUMPAD_FIRST + (vkey - VK_NUMPAD0);
			}

			// Function
			if (vkey >= VK_F1 && vkey <= VK_F24)
			{
				return CC_FUNCTION_FIRST + (vkey - VK_F1);
			}

			// try unicode
			std::vector<BYTE> keyBuf(256, 0);
			int sc = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
			
			wchar_t unicodeC;
			int retVal = ToUnicode(
					vkey, sc, 
					keyBuf.data(),
					&unicodeC, 1, 0
				);

			if (retVal > 0)
			{
				return towupper(unicodeC);
			}

			// If not converted return the raw vkey, even if it's wrong
			return vkey; 
		}
	}

#endif // FUSIN_BUILD_RAW_INPUT