#pragma once

#ifdef BG_PLATFORM_WINDOWS
	#ifdef BG_BUILD_DLL
		#define BG_API __declspec(dllexport)
	#else
		#define BG_API __declspec(dllimport)
	#endif
#else
	#error Bagel only supports windows!
#endif


#define BIT(x) (1 << x)