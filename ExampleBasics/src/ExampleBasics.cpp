/*
This example shows the basics of using Fusin.
It reads and prints the left/right key inputs, mouse movement, and gamepad stick and DPad inputs.
It shows how to setup the InputManager, assign several inputs, and read the input values.
*/

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
//#include <windows.h>
#include "Fusin.h"

using namespace std::chrono_literals;

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
	cLeft.assignIOCode(Fusin::GAMEPAD_DPAD_LEFT);
	cRight.assignIOCode(Fusin::GAMEPAD_DPAD_RIGHT);
	
	cLeft.assignIOCode(Fusin::XINPUT_LEFT_STICK_LEFT);
	cRight.assignIOCode(Fusin::XINPUT_LEFT_STICK_RIGHT);
	cLeft.assignIOCode(Fusin::XINPUT_DPAD_LEFT);
	cRight.assignIOCode(Fusin::XINPUT_DPAD_RIGHT);

	cLeft.assignIOCode(Fusin::MOUSE_MOVE_LEFT);
	cRight.assignIOCode(Fusin::MOUSE_MOVE_RIGHT);

	std::wcout << std::fixed << std::setprecision(2);
	while (running)
	{
		im.update();
		if (cLeft.check()) std::wcout << "Left: "<< cLeft.value() << " ";
		if (cRight.check()) std::wcout << "Right: " << cRight.value() << " ";
		std::wcout << std::endl;

		std::this_thread::sleep_for(33ms);
	}

	return 0;
}