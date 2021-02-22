#include "IOSystems/RawInput/FusinWindowsKeycodeConvert.h"

#ifdef FUSIN_BUILD_RAW_INPUT

	#include "IOCodes/FusinKey.h"

	#include <vector>

	#include <windows.h>

	namespace Fusin
	{
        Char virtualKeycodeToVirtualKey(int vkey)
		{
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
				return unicodeC;
			}

			// special codes
			switch (vkey)
			{
			case VK_BACK: return KEY_BACKSPACE.index; break;
			case VK_TAB: return KEY_TAB.index; break;
			//case VK_LINE: return KEY_NEW_LINE.index; break;// not used in modern keyboards
			case VK_CLEAR: return KEY_CLEAR.index; break;
			case VK_RETURN: return KEY_RETURN.index; break;
			case VK_MENU: return KEY_ALT.index; break;
			case VK_PAUSE: return KEY_PAUSE.index; break;
			case VK_CAPITAL: return KEY_CAPSLOCK.index; break;
			case VK_ESCAPE: return KEY_ESCAPE.index; break;
			case VK_SPACE: return KEY_SPACE.index; break;
			case VK_PRIOR: return KEY_PAGE_UP.index; break;
			case VK_NEXT: return KEY_PAGE_DOWN.index; break;
			case VK_END: return KEY_END.index; break;
			case VK_HOME: return KEY_HOME.index; break;
			case VK_LEFT: return KEY_LEFT.index; break;
			case VK_UP: return KEY_UP.index; break;
			case VK_RIGHT: return KEY_RIGHT.index; break;
			case VK_DOWN: return KEY_DOWN.index; break;
			case VK_PRINT:
			case VK_SNAPSHOT: return KEY_PRINT_SCREEN.index; break;
			case VK_INSERT: return KEY_INSERT.index; break;
			case VK_DELETE: return KEY_DELETE.index; break;
			case VK_LWIN: return KEY_LWIN.index; break;
			case VK_RWIN: return KEY_RWIN.index; break;
			case VK_MULTIPLY: return KEY_MULTIPLY.index; break;
			case VK_ADD: return KEY_ADD.index; break;
			case VK_SUBTRACT: return KEY_SUBTRACT.index; break;
			case VK_DECIMAL: return KEY_DECIMAL.index; break;
			case VK_DIVIDE: return KEY_DIVIDE.index; break;
			case VK_NUMLOCK: return KEY_NUM_LOCK.index; break;
			case VK_SCROLL: return KEY_SCROLL_LOCK.index; break;
			case VK_SHIFT: return KEY_SHIFT.index; break;
			case VK_LSHIFT: return KEY_LSHIFT.index; break;
			case VK_RSHIFT: return KEY_RSHIFT.index; break;
			case VK_CONTROL: return KEY_CONTROL.index; break;
			case VK_LCONTROL: return KEY_LCONTROL.index; break;
			case VK_RCONTROL: return KEY_RCONTROL.index; break;
			case VK_LMENU: return KEY_LMENU.index; break;
			case VK_RMENU: return KEY_RMENU.index; break;
			}

			// Numpad
			if (vkey >= VK_NUMPAD0 && vkey <= VK_NUMPAD9)
			{
				return KEY_NUMPAD_0.index + (vkey - VK_NUMPAD0);
			}

			// Function
			if (vkey >= VK_F1 && vkey <= VK_F24)
			{
				return KEY_F1.index + (vkey - VK_F1);
			}

			// If not converted return the raw vkey, even if it's wrong
			return vkey; 
		}
	}

#endif // FUSIN_BUILD_RAW_INPUT