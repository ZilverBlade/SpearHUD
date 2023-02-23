#pragma once


#ifdef _WIN32
#define GHUD_PLATFORM_WINDOWS
#ifdef GHUD_DYNAMIC_LIBRARY
#ifdef _DLLEXPORT
#define GHUD_API __declspec(dllexport)
#else
#define GHUD_API __declspec(dllimport)
#endif
#else
#define GHUD_API
#endif
#endif
#ifdef __linux__
#define GHUD_PLATFORM_LINUX
#define GHUD_API
#pragma warning "Linux might not be supported!"
#endif
#ifdef __OSX__
#define GHUD_PLATFORM_MACOS
#define GHUD_API
#pragma warning "MacOS might not be supported!"
#endif