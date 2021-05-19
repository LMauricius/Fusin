# Fusin
Fusin is a C++ input library oriented towards game development that combines many input sources under one API.
It's made to be simpler than XInput, yet provide the rich features of a high-level IO API and support for many devices, while giving the programmer as much power down to the metal as one could ever want. 
The modular approach makes it easy to extend functionality without changing the library usage whatsoever.
It is multiplatform, although joystick support for Unix is WIP.

- [Fusin](#fusin)
- [General concepts](#general-concepts)
- [Supported platforms](#supported-platforms)
- [Installing the library](#installing-the-library)
- [Including Fusin in your project](#including-fusin-in-your-project)
  - [Visual studio projects](#visual-studio-projects)
  - [CMake projects](#cmake-projects)
- [Using Fusin](#using-fusin)

# General concepts
High-level usage is based on three concepts that provide full control over common features:
* **IOCode** - The code that identifies a unique type of input or output. It is defined by 3 values: the type of device, the type of IO and the index of the IO. The index value can hold additional binary flags which will make it seem larger than it really is. For convenience use the pre-defined IOCodes located in the headers in the IOCodes/ folder.
* **Command** - The interface between player actions and Device inputs/outputs. It allows you to track Device inputs by using IOCodes only, or track other Commands if neccessary. Each InputCommand can correspond to a player command, while an OutputCommand can correspond to one type of feedback to the player. Commands for reading various gestures, such as the ShakeCommand or 2D analog PositionCommand are also included. Examples of commands for menu input would be *cursorMovement* (PositionCommand), *selectAction* and *cancelAction* (InputCommands), while an example of an OutputCommand would be *vibration* command. Each command is then connected to the device inputs by assigning IOCodes.
* **InputManager** - Encapsulates everything that is needed for the library to function without making the user worry about low-level stuff. Enumerates Devices, registers SubSystems and updates EVERYTHING with a single call, while still giving access to low-level objects in case you need finer control - the low-level stuff is only two function calls away.

If you are not satisfied by using such high level concepts feel free to dvelve deeper into the library and use the SubSystems, IOSignals, Devices and DeviceComponents, without ever worying about not knowing what is hidden deep inside. You can control the flow of data manually instead of letting Manager objects reign, while still retaining a great deal of abstraction over the raw metal that should make porting to different platforms easier.

# Supported platforms
* **Windows** - Full support for keyboards, mice and gamepads. Implemented using RawInput and XInput.
* **Linux** - Keyboards and mice fully supported, gamepads WIP. Implemented using Xlib and XI2.
* **Mac** - Not tested, but the code should be compatible
* **Custom platforms** - Adding implementation for other platforms is relatively easy as all it takes is to make a new backend that provides the rest of Fusin input data. Just do what you'd do anyways, but modify Fusin's devices and signals so that you can easily remap the controls when needed.

# Installing the library
First install [CMake](https://cmake.org/download/) if you haven't already.
* **Windows** - Run `buildFusin_Windows.bat`. If you want to save the Fusin path globally also run `setFUSIN_HOME.bat`. You can use the environment variable `$(FUSIN_HOME)` in your include paths to avoid searching for paths later.
* **Linux/Unix** - Make `buildFusin_Linux.sh` executable with `chmod +x buildFusin_Linux.sh`. Then run `buildFusin_Linux.sh`. If you want to install Fusin in your system's standard location also run `cmake --build ./build --targets install`. By default CMake will install the files in /usr/local, so remember to use `sudo` with the command.
* **Custom/Out of src builds** - Fusin requires no fancy terminal magic to be built. Just use typical CMake commands to configure and build the library how and where you see fit. User configurable build variables are all located at the start of CMakeLists.txt in the source's root directory.

# Including Fusin in your project
All you need to include are the directories where the headers are (`${FUSIN_HOME}/include`), the directory where the library is (`${FUSIN_HOME}/bin`) and the library itself (`Fusin` or `Fusin_d` if we are using a debug configuration)

## Visual studio projects
* Open your project settings. Select All configurations and add the following:
* In `C++/General/Additional Include Directories` add: `$(FUSIN_HOME)Fusin/include`
* In `Linker/General/Additional Library Directories` add: `$(FUSIN_HOME)bin/$(Configuration);`
* Now select the Release configuration. In `Linker/Input/Additional Dependencies:` add `Fusin.lib;hid.lib;xinput.lib;`
* Now select the Debug configuration. In `Linker/Input/Additional Dependencies:` add `Fusin_d.lib;hid.lib;xinput.lib;`

If you don't have ${FUSIN_HOME} set as an environment variable manually replace $(FUSIN_HOME) with Fusin's source path

## CMake projects
Add the following to your CMakeLists.txt:
```
target_include_directories(${TARGET_NAME} PRIVATE "${FUSIN_HOME}Fusin/include")
target_link_directories(ExampleBasics PUBLIC "${FUSIN_HOME}bin")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(${TARGET_NAME} PUBLIC Fusin_d)
else()
    target_link_libraries(${TARGET_NAME} PUBLIC Fusin)
endif()
```
Replace ${TARGET_NAME} and ${FUSIN_HOME} with the target name and Fusin's source path. If you have ${FUSIN_HOME} set as an environment variable you can replace it with ` $ENV{VAR}`.

# Using Fusin

Using Fusin is pretty straightforward. For the usual functionality just `#include "Fusin.h"`. A typical setup will have this in the `main` program:
```C++
int main()
{
	bool running = true;
	Fusin::InputManager im;
	im.initialize(true);
    
	while (running)
    {
		im.update();
    }
}
```

Obviously, this doesn't do anything, and the program never ends. So how do we use the InputCommands?
```C++
int main()
{
	bool running = true;
	Fusin::InputManager im;
	Fusin::InputCommand quitCommand(&im), jumpCommand(&im);

	im.initialize(true);
	quitCommand.assignIOCode(Fusin::KEY_ESCAPE);
	jumpCommand.assignIOCode(Fusin::KEY_SPACE);
    
	while (running)
    {
		im.update();
		if (quitCommand.check())
        {
            running = false;
        }
		if (jumpCommand.check())
        {
            // Jump or smthg
            std::cout << "Jump" << std::endl;
        }
    }

    return 0;
}
```
Now that's better! This program can exit by pressing the Escape key and tell us that the player has 'jumped' when pressing the Spacebar.

Fusin also supports listening to input to only one window. To connect the manager to an existing external window use `InputManager::setInputWindow(windowHandle)`. It supports handles for different platforms. If you use an external window you will also have to manually pass the window messages to the InputManager, with `InputManager::handleMessage(msg)`. 

To get into more detailed examples check the Example folders in the source code. 