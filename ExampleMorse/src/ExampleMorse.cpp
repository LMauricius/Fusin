/*
This example shows the basics of using Fusin.
It reads and prints the left/right key inputs, mouse movement, and gamepad stick and DPad inputs.
It shows how to setup the InputManager, assign several inputs, and read the input values.
*/

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cctype>
#include "Fusin.h"
#include "Devices/FusinKeyboardDevice.h"

using namespace std::chrono_literals;

std::string getMorse(char x)
{
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

	std::string text, morse;
    int pos = 0, morsePos = -1;
    bool spaceDelay = true;

    std::cout << "Enter your message: ";
	std::getline(std::cin, text);

	im.update();
	while (running)
	{
		Fusin::KeyboardDevice *globalKB = (Fusin::KeyboardDevice*)im.getDevice(Fusin::DT_KEYBOARD, 0);
		
        if (std::chrono::system_clock::now() - lastTime > nextStepDelay)
        {
            lastTime = std::chrono::system_clock::now();

            spaceDelay = !spaceDelay;

            if (!spaceDelay)
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

                if (morse[morsePos] == '.')// short
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
            else
            {
                nextStepDelay = 300ms;
            }
        }
        if (spaceDelay)
        {
            globalKB->leds.setLEDFlags(Fusin::LED_NONE);
        }
        else
        {
            globalKB->leds.setLEDFlags(Fusin::LED_CAPS | Fusin::LED_NUM | Fusin::LED_SCROLL);
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