C++/General/Additional Include Directories:
	$(FUSIN_INCLUDE_DIR);

Linker/General/Additional Library Directories:
	$(FUSIN_LIB_DIR)\vs$(PlatformToolsetVersion)_$(Platform);

Linker/Input/Additional Dependencies:
	Fusin.lib;hid.lib;xinput.lib; (Release, Windows)
	Fusin_d.lib;hid.lib;xinput.lib; (Debug, Windows)