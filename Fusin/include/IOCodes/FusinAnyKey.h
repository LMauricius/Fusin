/*
This file contains definitions for IOCodes corresponding to Typing DeviceComponent (DT_COMPONENT_TYPING).
For KeyboardDevice versions of the IOCode definitions below, refer to 'FusinKey.h' file.
Some additional constant definitions are also included in 'FusinKey.h' file,
including many IOCodes for commonly used but non-printable keys.
To use DT_COMPONENT_TYPING variations of those IOCodes, you can include both files and use 
AnyKey(const IOCode& ic) or AnyKeyTyped(const IOCode& ic) functions on those IOCodes.
For example, AnyKey(KEY_BACKSPACE)
*/

#ifndef _FUSIN_ANY_KEY_H
#define _FUSIN_ANY_KEY_H

#include "IOCodes/FusinIOCode.h"

namespace Fusin
{
	/*
	Returns the DT_COMPONENT_TYPING, IO_BUTTON IOCode for the specified character
	*/
	IOCode _FUSIN_EXPORT AnyKey(Char c);

	/*
	Returns the DT_COMPONENT_TYPING, IO_BUTTON version of the specified IOCode
	*/
	IOCode _FUSIN_EXPORT AnyKey(const IOCode& ic);

	/*
	Returns the DT_COMPONENT_TYPING, IO_BUTTON IOCode for the specified numpad key
	*/
	IOCode _FUSIN_EXPORT AnyNumpadKey(int n);

	/*
	Returns the DT_COMPONENT_TYPING, IO_BUTTON IOCode for the specified function key
	*/
	IOCode _FUSIN_EXPORT AnyFunctionKey(int n);

	/*
	Returns the DT_COMPONENT_TYPING, IO_TYPED_BUTTON IOCode for the specified character
	*/
	IOCode _FUSIN_EXPORT AnyKeyTyped(Char c);

	/*
	Returns the DT_COMPONENT_TYPING, IO_TYPED_BUTTON version of the specified IOCode
	*/
	IOCode _FUSIN_EXPORT AnyKeyTyped(const IOCode& ic);
}

#endif