#pragma once

#ifdef _WIN32
#define SHUD_PLATFORM_WINDOWS
#ifdef SHUD_DYNAMIC_LIBRARY
#ifdef _DLLEXPORT
#define SHUD_API __declspec(dllexport)
#else
#define SHUD_API __declspec(dllimport)
#endif
#else
#define SHUD_API
#endif
#endif
#ifdef __linux__
#define SHUD_PLATFORM_LINUX
#define SHUD_API
#pragma warning "Linux might not be supported!"
#endif
#ifdef __OSX__
#define SHUD_PLATFORM_MACOS
#define SHUD_API
#pragma warning "MacOS might not be supported!"
#endif