/*
This example shows Fusin's output capabilities.
It allows you to enter a sentence, which will then be spelled back using the Morse code,
by flashing the LEDs and using vibration.
*/

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cctype>
#include "Fusin.h"
#include "Devices/FusinKeyboardDevice.h"
#include "Utilities/FusinLog.h"

using namespace std::chrono_literals;

std::string getMorse(char x)
{
	Fusin::Log::singleton().pipe(std::wcout);

	switch (tolower(x))
	{
    case 'a':
        return ".-";
    case 'b':
        return "-...";
    case 'c':
        return "-.-.";
    case 'd':
        return "-..";
    case 'e':
        return ".";
    case 'f':
        return "..-.";
    case 'g':
        return "--.";
    case 'h':
        return "....";
    case 'i':
        return "..";
    case 'j':
        return ".---";
    case 'k':
        return "-.-";
    case 'l':
        return ".-..";
    case 'm':
        return "--";
    case 'n':
        return "-.";
    case 'o':
        return "---";
    case 'p':
        return ".--.";
    case 'q':
        return "--.-";
    case 'r':
        return ".-.";
    case 's':
        return "...";
    case 't':
        return "-";
    case 'u':
        return "..-";
    case 'v':
        return "...-";
    case 'w':
        return ".--";
    case 'x':
        return "-..-";
    case 'y':
        return "-.--";
    case 'z':
        return "--..";
    case '1':
        return ".----";
    case '2':
        return "..---";
    case '3':
        return "...--";
    case '4':
        return "....-";
    case '5':
        return ".....";
    case '6':
        return "-....";
    case '7':
        return "--...";
    case '8':
        return "---..";
    case '9':
        return "----.";
    case '0':
        return "-----";
	default:
        return "";
	}
}

int main()
{
	bool running = true;
    std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();
    std::chrono::system_clock::duration nextStepDelay = 0ms;

    Fusin::InputManager im;
	im.initialize(true);
    
    Fusin::OutputCommand cMorse(&im);

    cMorse.assignIOCode(Fusin::KEYBOARD_LED_CAPS);
    cMorse.assignIOCode(Fusin::KEYBOARD_LED_NUM);
    cMorse.assignIOCode(Fusin::KEYBOARD_LED_SCROLL);
    cMorse.assignIOCode(Fusin::GAMEPAD_VIBRATION_LEFT_FORCE);
    cMorse.assignIOCode(Fusin::GAMEPAD_VIBRATION_RIGHT_FORCE);
    cMorse.assignIOCode(Fusin::NINTENDO_VIBRATION_LEFT_FORCE);
    cMorse.assignIOCode(Fusin::NINTENDO_VIBRATION_RIGHT_FORCE);
    cMorse.assignIOCode(Fusin::DS_VIBRATION_LEFT_FORCE);
    cMorse.assignIOCode(Fusin::DS_VIBRATION_RIGHT_FORCE);
    cMorse.assignIOCode(Fusin::XINPUT_VIBRATION_LEFT_FORCE);
    cMorse.assignIOCode(Fusin::XINPUT_VIBRATION_RIGHT_FORCE);

	std::string text, morse;
    int pos = -1, morsePos = -1;
    bool spaceDelay = true;

    std::cout << "Enter your message: ";
	std::getline(std::cin, text);

	im.update();
	while (running)
	{
		//Fusin::KeyboardDevice *globalKB = (Fusin::KeyboardDevice*)im.getDevice(Fusin::DT_KEYBOARD, 0);
		
        if (std::chrono::system_clock::now() - lastTime > nextStepDelay)
        {
            lastTime = std::chrono::system_clock::now();

            spaceDelay = !spaceDelay;

            if (spaceDelay)
            {
                if (morsePos+1 == morse.size() && pos == text.length()-1)// sentence wait
                {
                    nextStepDelay = 1500ms;
                }
                else
                {
                    nextStepDelay = 300ms;
                }
            }
            else
            {
                morsePos++;
                if (morsePos >= morse.length())
                {
                    morsePos = 0;
                    pos++;
                    if (pos >= text.length())
                    {
                        pos = 0;
                    }
                    morse = getMorse(text[pos]);
                    
                    std::cout << "\n(" << text[pos] << ") ";
                    std::cout.flush();
                }

                if (morse.size() == 0)// space
                {
                    nextStepDelay = 900ms;
                    spaceDelay = true;
                }
                else if (morse[morsePos] == '.')// short
                {
                    nextStepDelay = 300ms;
                }
                else// long
                {
                    nextStepDelay = 900ms;
                }
                
                std::cout << morse[morsePos];
                std::cout.flush();
            }
        }
        if (spaceDelay)
        {
            cMorse.setValue(0);
            //globalKB->leds.setLEDFlags(Fusin::LED_NONE);
        }
        else
        {
            cMorse.setValue(1);
            //globalKB->leds.setLEDFlags(Fusin::LED_CAPS | Fusin::LED_NUM | Fusin::LED_SCROLL);
        }

		im.update();
		/*Fusin::KeyboardDevice *globalKB = (Fusin::KeyboardDevice*)im.getDevice(Fusin::DT_KEYBOARD, 0);
		
        std::cout << "(" << text[pos] << ") ";
        std::cout.flush();
        morse = getMorse(text[pos]);
        for (char c : morse)
		{
		    std::cout << c;
            std::cout.flush();

			globalKB->leds.setLEDFlags(Fusin::LED_CAPS);
			im.update();
			if (c == '.')// short
			{
				std::this_thread::sleep_for(300ms);
			}
			else// long
			{
				std::this_thread::sleep_for(900ms);
			}

			// pause
			globalKB->leds.setLEDFlags(Fusin::LED_NONE);
			im.update();
			std::this_thread::sleep_for(300ms);
		}
		std::cout << std::endl;

		pos++;
		if (pos >= text.size())
			pos = 0;

		std::this_thread::sleep_for(1200ms);*/
	}

	return 0;
}