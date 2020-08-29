#ifndef _FUSIN_VENDORS_H
#define _FUSIN_VENDORS_H

#include <vector>

namespace Fusin
{
    // Sony
	const unsigned long DS_VENDOR_ID = 0x054C;
	const std::vector<unsigned long> DS3_PRODUCT_IDS = {
		0x0268
	};
	const std::vector<unsigned long> DS4_PRODUCT_IDS = {
		0xBA0, 0x5C4, 0x09CC
	};
    
    // Nintendo
	const unsigned long NINTENDO_VENDOR_ID = 0x057E;
	const std::vector<unsigned long> JOYCON_PRODUCT_IDS{
		0x2006, 0x2007
	};
	const unsigned long JOYCON_PRODUCT_ID_LEFT = 0x2006;
	const unsigned long JOYCON_PRODUCT_ID_RIGHT = 0x2007;

    // XBox
	const unsigned long XINPUT_VENDOR_ID = 0x045E;
	const std::vector<unsigned long> XINPUT_PRODUCT_IDS{
        0x028E
    };
}

#endif