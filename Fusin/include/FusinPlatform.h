#ifndef _FUSIN_PLATFORM_H
#define _FUSIN_PLATFORM_H

#include "FusinConfig.h"

// The old __declspec trick for windows...
#ifdef _WIN32

    #ifdef FUSIN_STATIC_LIB
        #define _FUSIN_EXPORT
    #else
        #ifdef FUSIN_BUILD
            #define _FUSIN_EXPORT __declspec( dllexport )
        #else
            #define _FUSIN_EXPORT __declspec( dllimport )
        #endif
    #endif

#else// For more sane platforms
    #define _FUSIN_EXPORT
#endif


#endif//_FUSIN_PLATFORM_H