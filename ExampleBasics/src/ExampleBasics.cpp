#include <iostream>
#include <iomanip>
//#include <windows.h>
#include "Fusin.h"

int main()
{
	bool running = true;
	Fusin::InputManager im;
	im.initialize(true);
	Fusin::InputCommand cLeft(&im), cRight(&im);

	im.update();
	cLeft.assignIOCode(Fusin::KEY_LEFT);
	cRight.assignIOCode(Fusin::KEY_RIGHT);
	cLeft.assignIOCode(Fusin::GAMEPAD_X_NEGATIVE);
	cRight.assignIOCode(Fusin::GAMEPAD_X_POSITIVE);
	cLeft.assignIOCode(Fusin::MOUSE_MOVE_LEFT);
	cRight.assignIOCode(Fusin::MOUSE_MOVE_RIGHT);

	//std::wcout << std::fixed << std::setprecision(2);

	while (running)
	{
		im.update();
		if (cLeft.check()) std::wcout << "Left: "<< cLeft.value() << " ";
		if (cRight.check()) std::wcout << "Right: " << cRight.value() << " ";
		std::wcout << std::endl;
	}

	return 0;
}