#pragma once


#ifdef _WIN32
	#ifdef _WIN64
		#define EG_PLATFORM_WINDOWS
	#else
		#error "Not supported x86"
	#endif // _WIN64
#elif __ANDROID__
	#define EG_PLATFORM_ANDROID
	#error "Androi is not supporte"
#elif __linux__
	#define EG_PLATFORM_LINUX
	#error "Linux is not supporte"
#endif
