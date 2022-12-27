#pragma once

#include <memory>


#ifdef _WIN32
	#ifdef _WIN64
		#define BG_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator is not supported!"
	#elif TAGET_OS_IPHONE == 1
		#define BG_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define BG_PLATFORM_MACOS
		#error "MacOs is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif

#elif defined(__ANDROID__)
	#define BG_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define BG_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown Platform"
#endif


#ifdef BG_PLATFORM_WINDOWS
	#if BG_DYNAMIC_LINK
		#ifdef BG_BUILD_DLL
			#define BG_API __declspec(dllexport)
		#else
			#define BG_API __declspec(dllimport)
		#endif
	#else
		#define BG_API 
	#endif
#else
	#error Bagel only supports windows!
#endif

#if BG_DEBUG
	#define BG_ENABLE_ASSERTS	
#endif

#ifdef BG_ENABLE_ASSERTS
	#define BG_ASSERT(x, ...) { if(!(x)) { BG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define BG_CORE_ASSERT(x, ...) { if(!(x)) { BG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define BG_ASSERT(x, ...)
	#define BG_CORE_ASSERT(x, ...)
#endif

#define BG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)

namespace Bagel {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}