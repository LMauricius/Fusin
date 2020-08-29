#pragma once
#ifndef _FUSIN_INPUT_COMMAND_H
#define _FUSIN_INPUT_COMMAND_H

#include "Commands/FusinIOCommand.h"

namespace Fusin
{
	/*
	The Command that READS IOSignal values and stores them, along with some info about the device that sent it.

	Used to receive inputs from the user and interpret them as abstract simple commands,
	such as a button press being interpreted as player jumping
	*/
	class InputCommand : public IOCommand
	{
	public:
		InputCommand(DeviceEnumerator* devEnum = nullptr);
		~InputCommand();

		/*
		Sets the value, strongestIOCode and strongestDeviceIndex
		*/
		virtual void setValue(float value, IOCode ioCode = IOCode::ANYCODE, Index deviceIndex = 0);
		/*
		Sets the value, strongestDeviceType and strongestIOType only if the new value is greater than the current one
		*/
		virtual void updateValue(float value, IOCode ioCode = IOCode::ANYCODE, Index deviceIndex = 0);

	protected:
		void syncValueSignal(IOSignal* sig, Index devInd);
	};
}

#endif