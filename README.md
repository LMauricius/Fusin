# Fusin
(WIP) An input library oriented towards game development that combines many input sources under one API.
The idea is to make it even simpler than XInput, yet provide the rich features of a high-level IO API and support for any imaginable device, while giving the programmer as much power down to the metal as one could ever want. 
Its pure modular approach makes it easy to extend functionality without changing the library usage whatsoever.
The aim is to make it multiplatform, althought it currently only supports Windows via the Raw Input SubSystem.

# General concepts
High-level usage is based on three concepts that should provide full control over common features:
* **IOCode** - The code that identifies a unique type of input or output. It is defined by 3 values: the type of device, the type of IO and the index of the IO. Be careful though - the index can hold additional binary flags which will make it seem larger than it really is. It is best to use the pre-defined IOCodes located in the headers in the IOCodes/ folder.
* **Command** - The interface between player actions and Device inputs/outputs. It allows you to bind Device signals by using IOCodes only, or track other Commands if neccessary. A single InputCommand should correspond to a single player command, while an Output command should correspond toa single type of feedback to the player. Commands for reading various gestures, such as ShakeCommand or 2D analog PositionCOmmand are also implemented. Examples of commands for menu input would be left, right, up, down, select and cancel InputCommands, whilean example of OutputCommand would be vibration command.
* **InputManager** - Encapsulates everything that is needed for the library to function without making the user worry about lower-level stuff. Enumerates Devices, registers SubSystems and updates EVERYTHING with a single call, while still giving access to low-level objects in case you need finer control - the low-level stuff is only two function calls away.

If you are not satisfied by using such high level concepts feel free to dvelve deeper into the library and use the SubSystems, IOSignals, Devices and DeviceComponents, without ever worying about not knowing what is hidden deep inside. You can control the flow of data manually instead of letting Manager objects reign, while still retaining a great deal of abstraction over the raw metal that should make porting to different platforms easier.

# Supported platforms
Only windows currently. Currently WIP due to a refactoring, but it will be implemented using RawInput. XInput is considered as a separate SubSystem, but will probably be added to the RawInput just to make things simpler.
Adding implementation for other platforms should be relatively easy as all it takes is to make a new backend that provides the rest of Fusin input data.
