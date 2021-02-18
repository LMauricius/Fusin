#pragma once
#ifndef _FUSIN_OUTPUT_COMMAND_H
#define _FUSIN_OUTPUT_COMMAND_H

#include "Commands/FusinIOCommand.h"

namespace Fusin
{
	/*
	The Command that WRITES its values to IOSignals.

	Used to provide feedback to the user,
	such as an explosion being interpreted as vibration.
	*/
	class _FUSIN_EXPORT OutputCommand : public IOCommand
	{
	public:
		OutputCommand(DeviceEnumerator* devEnum = nullptr);
		~OutputCommand();

		/*
		Sets the value that will be sent via assigned device signals on the next update()
		*/
		void setValue(float value);

	protected:
		void syncValueSignal(IOSignal* sig, Index devInd);
	};
}

#endif