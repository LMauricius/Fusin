#include "IOSystems/X11/FusinX11KeycodeConvert.h"

#include "IOCodes/FusinKey.h"

#include <X11/keysym.h>
#include <xkbcommon/xkbcommon.h>

#ifdef FUSIN_BUILD_X11

	namespace Fusin
	{
        Char keysymToVirtualKey(int keysym)
		{
			// try unicode
			Char ret = xkb_keysym_to_utf32(xkb_keysym_to_upper(keysym));
			if (ret != 0) return (ret);

			// special codes
			switch (keysym)
			{
			case XK_BackSpace: return KEY_BACKSPACE.index; break;
			case XK_Tab: return KEY_TAB.index; break;
			case XK_Linefeed: return KEY_NEW_LINE.index; break;// not used in modern keyboards
			case XK_Clear: return KEY_CLEAR.index; break;
			case XK_Return: return KEY_RETURN.index; break;
			case XK_Alt_L: return KEY_ALT.index; break;
			case XK_Pause: return KEY_PAUSE.index; break;
			case XK_Caps_Lock: return KEY_CAPSLOCK.index; break;
			case XK_Escape: return KEY_ESCAPE.index; break;
			case XK_space: return KEY_SPACE.index; break;
			case XK_Page_Up: return KEY_PAGE_UP.index; break;
			case XK_Page_Down: return KEY_PAGE_DOWN.index; break;
			case XK_End: return KEY_END.index; break;
			case XK_Home: return KEY_HOME.index; break;
			case XK_Left: return KEY_LEFT.index; break;
			case XK_Up: return KEY_UP.index; break;
			case XK_Right: return KEY_RIGHT.index; break;
			case XK_Down: return KEY_DOWN.index; break;
			case XK_Print: return KEY_PRINT_SCREEN.index; break;
			case XK_Insert: return KEY_INSERT.index; break;
			case XK_Delete: return KEY_DELETE.index; break;
			case XK_Super_L: return KEY_LWIN.index; break;
			case XK_Super_R: return KEY_RWIN.index; break;
			case XK_KP_Multiply: return KEY_MULTIPLY.index; break;
			case XK_KP_Add: return KEY_ADD.index; break;
			case XK_KP_Subtract: return KEY_SUBTRACT.index; break;
			case XK_KP_Decimal: return KEY_DECIMAL.index; break;
			case XK_KP_Divide: return KEY_DIVIDE.index; break;
			case XK_Num_Lock: return KEY_NUM_LOCK.index; break;
			case XK_Scroll_Lock: return KEY_SCROLL_LOCK.index; break;
			case XK_Shift_L: return KEY_LSHIFT.index; break;
			case XK_Shift_R: return KEY_RSHIFT.index; break;
			case XK_Control_L: return KEY_LCONTROL.index; break;
			case XK_Control_R: return KEY_RCONTROL.index; break;
			case XK_Menu: return KEY_LMENU.index; break;

			case XK_KP_Insert: return KEY_NUMPAD_0.index; break;
			case XK_KP_End: return KEY_NUMPAD_1.index; break;
			case XK_KP_Down: return KEY_NUMPAD_2.index; break;
			case XK_KP_Page_Down: return KEY_NUMPAD_3.index; break;
			case XK_KP_Left: return KEY_NUMPAD_4.index; break;
			case XK_KP_Begin: return KEY_NUMPAD_5.index; break;
			case XK_KP_Right: return KEY_NUMPAD_6.index; break;
			case XK_KP_Home: return KEY_NUMPAD_7.index; break;
			case XK_KP_Up: return KEY_NUMPAD_8.index; break;
			case XK_KP_Page_Up: return KEY_NUMPAD_9.index; break;
			}

			// Numpad
			if (keysym >= XK_KP_0 && keysym <= XK_KP_9)
			{
				return KEY_NUMPAD_0.index + (keysym - XK_KP_0);
			}

			// Function
			if (keysym >= XK_F1 && keysym <= XK_F35)
			{
				return KEY_F1.index + (keysym - XK_F1);
			}

			// If not converted return the raw keysym, even if it's wrong
			return keysym; 
		}
	}

#endif // FUSIN_BUILD_X11