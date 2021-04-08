#include "IOSystems/X11/FusinX11KeycodeConvert.h"

#ifdef FUSIN_BUILD_X11

	#include "IOCodes/FusinKey.h"

	#include <X11/keysym.h>
	#include <xkbcommon/xkbcommon.h>

	namespace Fusin
	{
        Char keysymToVirtualKey(int keysym)
		{
			// special codes
			switch (keysym)
			{
			case XK_Home: return CC_HOME;
			case XK_End: return CC_END;
			case XK_Shift_L: return CC_LSHIFT;
			case XK_Shift_R: return CC_RSHIFT;
			case XK_Control_L: return CC_LCONTROL;
			case XK_Control_R: return CC_RCONTROL;

			case XK_BackSpace: return CC_BACKSPACE;
			case XK_Tab: return CC_TAB;
			case XK_Linefeed: return CC_NEW_LINE;// not used in modern keyboards
			case XK_Clear: return CC_CLEAR;
			case XK_Return: return CC_RETURN;
			case XK_Page_Up: return CC_PAGE_UP;
			case XK_Page_Down: return CC_PAGE_DOWN;

			case XK_Pause: return CC_PAUSE;
			case XK_Num_Lock: return CC_NUM_LOCK;
			case XK_Scroll_Lock: return CC_SCROLL_LOCK;
			case XK_Caps_Lock: return CC_CAPSLOCK;
			
			case XK_Alt_L: return CC_LALT;
			case XK_ISO_Level3_Shift:
			case XK_Alt_R: return CC_RALT;
			case XK_Menu: return CC_MENU;
			case XK_Super_L: return CC_LSUPER;
			case XK_Super_R: return CC_RSUPER;

			case XK_Escape: return CC_ESCAPE;
			case XK_Print: return CC_PRINT_SCREEN;
			case XK_Insert: return CC_INSERT;

			case XK_space: return CC_SPACE;

			case XK_KP_Multiply: return CC_MULTIPLY;
			case XK_KP_Add: return CC_ADD;
			case XK_KP_Subtract: return CC_SUBTRACT;
			case XK_KP_Delete:
			case XK_KP_Decimal: return CC_DECIMAL;
			case XK_KP_Divide: return CC_DIVIDE;

			case XK_KP_Insert: return CC_NUMPAD_0;
			case XK_KP_End: return CC_NUMPAD_1;
			case XK_KP_Down: return CC_NUMPAD_2;
			case XK_KP_Page_Down: return CC_NUMPAD_3;
			case XK_KP_Left: return CC_NUMPAD_4;
			case XK_KP_Begin: return CC_NUMPAD_5;
			case XK_KP_Right: return CC_NUMPAD_6;
			case XK_KP_Home: return CC_NUMPAD_7;
			case XK_KP_Up: return CC_NUMPAD_8;
			case XK_KP_Page_Up: return CC_NUMPAD_9;
			
			case XK_Left: return CC_LEFT;
			case XK_Up: return CC_UP;
			case XK_Right: return CC_RIGHT;
			case XK_Down: return CC_DOWN;
			case XK_Delete: return CC_DELETE;
			}

			// Numpad
			if (keysym >= XK_KP_0 && keysym <= XK_KP_9)
			{
				return CC_NUMPAD_FIRST + (keysym - XK_KP_0);
			}

			// Function
			if (keysym >= XK_F1 && keysym <= XK_F35)
			{
				return CC_FUNCTION_FIRST + (keysym - XK_F1);
			}

			// try unicode
			Char ret = xkb_keysym_to_utf32(xkb_keysym_to_upper(keysym));
			if (ret != 0) return (ret);

			// If not converted return the raw keysym, even if it's wrong
			return keysym; 
		}
	}

#endif // FUSIN_BUILD_X11