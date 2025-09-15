#pragma once

// Platform abstraction layer for CodeFrame.
// Handles all OS-specific functionality in one place.

#if defined(_WIN64) || defined(_WIN32) // WINDOWS
#include "windows/cf-windows.h"

#define PLATFORM "windows"

#elif defined(__linux__)
#include "linux/cf-linux.h"

#define PLATFORM "linux"

#elif defined(__APPLE__)
#include "apple/cf-apple.h"

#define PLATFORM "apple"

#else
#error "OS not supported!"
#endif

// class Platform {
// public:
//   static Platform &getInstance() {
//     static Platform instance; // Constructed once, thread-safe (since C++11)
//     return instance;
//   }

//   enum OS { Windows, MacOS, Linux, Unknown };

//   cconst OS getPlatform() { return currentPlatform; };

//   cconst bool isWindows() { return getPlatform() == OS::Windows; };
//   cconst bool isMacOS() { return getPlatform() == OS::MacOS; };
//   cconst bool isLinux() { return getPlatform() == OS::Linux; };

// private:
//   Platform() {
// #if defined(_WIN32) || defined(_WIN64)
//     cconst OS currentPlatform = OS::Windows;
// #elif defined(__APPLE__) && defined(__MACH__)
//     cconst OS currentPlatform = OS::MacOS;
// #elif defined(__linux__)
//     cconst OS currentPlatform = OS::Linux;
// #else
//     cconst OS currentPlatform = OS::Unknown;
// #endif
//   }

//   Platform(const Platform &) = delete;
//   Platform &operator=(const Platform &) = delete;
//   OS currentPlatform;
// };