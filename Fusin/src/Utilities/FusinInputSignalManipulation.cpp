#include "FusinInputSignalManipulation.h"

#include <algorithm>

namespace Fusin
{

	void releaseInputSignalVector(std::vector<IOSignal*>& signals)
	{
		for (auto signal : signals)
			signal->release();
	}

	void pressInputSignalVector(std::vector<IOSignal*>& signals)
	{
		for (auto signal : signals)
			signal->press();
	}

	void updateInputSignalVector(std::vector<IOSignal*>& signals)
	{
		for (auto signal : signals)
			signal->update();
	}

}