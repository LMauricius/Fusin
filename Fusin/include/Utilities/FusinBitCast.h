#ifndef _FUSIN_BIT_CAST_H
#define _FUSIN_BIT_CAST_H

#include <inttypes.h>

namespace Fusin
{
    /*
    Cast from an integer describing a range using certain number of bits to
    float between 0.0-1.0 or -1.0-1.0
    */
    inline float fromBitRange8(const int8_t bits) {
        return ((float)(bits) / 255.0f * 2.0f - 1.0f);
    }
    inline float fromBitRange8U(const uint8_t bits) {
        return ((float)(bits) / 255.0f);
    }
    inline float fromBitRange16(const int16_t bits) {
        return ((float)(bits) / 65535.0f * 2.0f - 1.0f);
    }
    inline float fromBitRange16U(const uint16_t bits) {
        return ((float)(bits) / 65535.0f);
    }
    inline float fromBitRange12(const int16_t bits) {
        return ((float)(bits) / 4095.0f * 2.0f - 1.0f);
    }
    inline float fromBitRange12U(const uint16_t bits) {
        return ((float)(bits) / 4095.0f);
    }

    /*
    Returns the specified flag from a flag container, presumably integer
    */
    template<typename _TF, typename _TI>
    inline bool getFlag(const _TF flags, const _TI index) {
        return (flags & (_TI(1) << index));
    }

    /*
    Sets or removes (depending on 'enable' param) the specified flags from the variable output
    */
    template<typename _TF>
    inline void setFlags(_TF& output, const _TF flags, bool enable) {
        output = (output & ~flags) | (flags* enable);
    }

    // Mask of 1s, start and end are 0-based
    inline uint64_t mask1(uint8_t start, uint8_t end) {
        return (((1ULL << (end-start))-1)<<start);
    }

    // Mask of 0s, start and end are 0-based
    inline uint64_t mask0(uint8_t start, uint8_t end) {
        return ~mask1(start, end);
    }
}

#endif