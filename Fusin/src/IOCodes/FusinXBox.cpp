#include "FusinXBox.h"

namespace Fusin
{

	InputCode XBoxButton(Index b)
	{
		return InputCode(IT_XBOX_BUTTON, b);
	}

	InputCode XBoxAxis(Index a)
	{
		return InputCode(IT_XBOX_AXIS, a);
	}

	InputCode XBoxDPadButton(Index a)
	{
		return InputCode(IT_XBOX_DPAD_BUTTON, a);
	}

	InputCode XBoxSignedAxis(Index a)
	{
		return InputCode(IT_XBOX_SIGNED_AXIS, a);
	}

	InputCode XBoxPositiveAxis(Index a)
	{
		return XBoxSignedAxis(a * 2 + 1);
	}

	InputCode XBoxNegativeAxis(Index a)
	{
		return XBoxSignedAxis(a * 2);
	}

}