#include "IOCodes/FusinAnyKey.h"
#include "IOCodes/FusinKey.h"

namespace Fusin
{
	IOCode AnyKey(Char c)
	{
		return IOCode(DT_COMPONENT_TYPING, IO_TYPED_BUTTON, c);
	}

	IOCode AnyKey(const IOCode& ic)
	{
		return IOCode(DT_COMPONENT_TYPING, IO_BUTTON, ic.index);
	}

	IOCode AnyNumpadKey(int n)
	{
		return IOCode(DT_COMPONENT_TYPING, IO_TYPED_BUTTON, CC_NUMPAD_FIRST + n);
	}

	IOCode AnyFunctionKey(int n)
	{
		return IOCode(DT_COMPONENT_TYPING, IO_TYPED_BUTTON, CC_FUNCTION_FIRST + n);
	}

	IOCode AnyKeyTyped(Char c)
	{
		return IOCode(DT_COMPONENT_TYPING, IO_TYPED_BUTTON, c);
	}

	IOCode AnyKeyTyped(const IOCode& ic)
	{
		return IOCode(DT_COMPONENT_TYPING, IO_TYPED_BUTTON, ic.index);
	}

}