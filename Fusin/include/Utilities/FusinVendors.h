#ifndef _FUSIN_VENDORS_H
#define _FUSIN_VENDORS_H

#include "FusinPrerequisites.h"
#include <vector>

namespace Fusin
{
    // Sony
	const unsigned long DS_VENDOR_ID = 0x054C;
	_FUSIN_EXPORT extern const std::vector<unsigned long> DS3_PRODUCT_IDS;
	_FUSIN_EXPORT extern const std::vector<unsigned long> DS4_PRODUCT_IDS;
    
    // Nintendo
	const unsigned long NINTENDO_VENDOR_ID = 0x057E;
	_FUSIN_EXPORT extern const std::vector<unsigned long> JOYCON_PRODUCT_IDS;
	_FUSIN_EXPORT extern const unsigned long JOYCON_PRODUCT_ID_LEFT;
	_FUSIN_EXPORT extern const unsigned long JOYCON_PRODUCT_ID_RIGHT;

    // XBox
	const unsigned long XINPUT_VENDOR_ID = 0x045E;
	_FUSIN_EXPORT extern const std::vector<unsigned long> XINPUT_PRODUCT_IDS;
}

#endif