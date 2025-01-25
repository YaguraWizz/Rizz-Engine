#pragma once
// Platform detection using predefined macros
#ifdef _WIN32
    /* Windows x64/x86 */
    #ifdef _WIN64
        /* Windows x64 */
        #ifndef RZ_PLATFORM_WINDOWS
            #define RZ_PLATFORM_WINDOWS
        #endif
    #else
        /* Windows x86 */
        #error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all the platforms
     * so we must check all of them (in this order)
     * to ensure that we're running on macOS
     * and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "iOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define RZ_PLATFORM_IOS
        #error "iOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define RZ_PLATFORM_MACOS
        /* MacOS is defined here */
    #else
        #error "Unknown Apple platform!"
    #endif
#elif defined(__ANDROID__)
    #define RZ_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define RZ_PLATFORM_LINUX
    #error "Linux is not supported!"
#else
    /* Unknown compiler/platform */
    #error "Unknown platform!"
#endif // End of platform detection
