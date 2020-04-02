#include <iostream>
#include <iomanip>
//#include <windows.h>
#include "Fusin.h"

int main()
{
	bool running = true;
	Fusin::InputManager im;
	im.initialize();
	Fusin::InputGesture gestLeft(&im), gestRight(&im);

	im.update();
	gestLeft.assignInputCode(Fusin::KEY_LEFT);
	gestRight.assignInputCode(Fusin::KEY_RIGHT);
	gestLeft.assignInputCode(Fusin::GAMEPAD_X_NEGATIVE);
	gestRight.assignInputCode(Fusin::GAMEPAD_X_POSITIVE);
	gestLeft.assignInputCode(Fusin::MOUSE_MOVE_LEFT);
	gestRight.assignInputCode(Fusin::MOUSE_MOVE_RIGHT);

	//std::wcout << std::fixed << std::setprecision(2);

	while (running)
	{
		im.update();
		if (gestLeft.check()) std::wcout << "Left: "<< gestLeft.value() << " ";
		if (gestRight.check()) std::wcout << "Right: " << gestRight.value() << " ";
		std::wcout << std::endl;
	}

	return 0;
}