#pragma once

#ifdef _WIN32
#define SHUDGL4_PLATFORM_WINDOWS
#ifdef SHUDGL4_DYNAMIC_LIBRARY
#ifdef _DLLEXPORT
#define SHUDGL4_API __declspec(dllexport)
#else
#define SHUDGL4_API __declspec(dllimport)
#endif
#else
#define SHUDGL4_API
#endif
#endif
#ifdef __linux__
#define SHUDGL4_PLATFORM_LINUX
#define SHUDGL4_API
#pragma warning "Linux might not be supported!"
#endif
#ifdef __OSX__
#define SHUDGL4_PLATFORM_MACOS
#define SHUDGL4_API
#pragma warning "MacOS might not be supported!"
#endif