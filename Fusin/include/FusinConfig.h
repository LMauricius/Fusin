#pragma once
#ifndef _FUSIN_CONFIG_H
#define _FUSIN_CONFIG_H

#ifndef FUSIN_USE_WCHAR
#	define FUSIN_USE_WCHAR 1
#endif
/*
#ifndef FUSIN_USE_LONG_INDEX_TYPE
#	define FUSIN_USE_LONG_INDEX_TYPE 0
#endif

#ifndef FUSIN_USE_DOUBLE_PRECISION
#	define FUSIN_USE_DOUBLE_PRECISION 0
#endif*/

#ifndef FUSIN_NO_DEFAULT_SUBSYSTEMS

	#ifdef _WIN32
		#define FUSIN_BUILD_RAW_INPUT
		#define FUSIN_BUILD_XINPUT
	#endif
	
	#ifdef unix
		#define FUSIN_BUILD_X11
	#endif

#endif


#endif