#include "FusinAnyKey.h"
#include "FusinKey.h"

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
		return IOCode(DT_COMPONENT_TYPING, IO_TYPED_BUTTON, NUMPAD_KEYS_START + n);
	}

	IOCode AnyFunctionKey(int n)
	{
		return IOCode(DT_COMPONENT_TYPING, IO_TYPED_BUTTON, FUNCTION_KEYS_START + n);
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