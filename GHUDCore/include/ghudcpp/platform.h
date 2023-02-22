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
#pragma error "Linux is currently not supported!"
#define GHUD_API
#endif
#ifdef __OSX__
#define GHUD_PLATFORM_MACOS
#pragma error "MacOS is currently not supported!"
#define GHUD_API
#endif