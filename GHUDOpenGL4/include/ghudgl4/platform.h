#pragma once

#ifdef _WIN32
#define GHUDGL4_PLATFORM_WINDOWS
#ifdef GHUDGL4_DYNAMIC_LIBRARY
#ifdef _DLLEXPORT
#define GHUDGL4_API __declspec(dllexport)
#else
#define GHUDGL4_API __declspec(dllimport)
#endif
#else
#define GHUDGL4_API
#endif
#endif
#ifdef __linux__
#define GHUDGL4_PLATFORM_LINUX
#define GHUDGL4_API
#pragma warning "Linux might not be supported!"
#endif
#ifdef __OSX__
#define GHUDGL4_PLATFORM_MACOS
#define GHUDGL4_API
#pragma warning "MacOS might not be supported!"
#endif