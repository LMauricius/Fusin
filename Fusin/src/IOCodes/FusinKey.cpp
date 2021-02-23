#include "IOCodes/FusinKey.h"
#include <sstream>

namespace Fusin
{

	IOCode Key(Char c)
	{
		return IOCode(DT_KEYBOARD, IO_BUTTON, c);
	}

	IOCode Key(const IOCode& ic)
	{
		return IOCode(DT_KEYBOARD, IO_BUTTON, ic.index);
	}

	IOCode NumpadKey(int n)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, CC_NUMPAD_FIRST + n);
	}

	IOCode FunctionKey(int n)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, CC_FUNCTION_FIRST + n);
	}

	IOCode KeyTyped(Char c)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, c);
	}

	IOCode KeyTyped(const IOCode& ic)
	{
		return IOCode(DT_KEYBOARD, IO_TYPED_BUTTON, ic.index);
	}

}