#pragma once

#ifdef _WIN32
#define GHUDVK_PLATFORM_WINDOWS
#ifdef GHUDVK_DYNAMIC_LIBRARY
#ifdef _DLLEXPORT
#define GHUDVK_API __declspec(dllexport)
#else
#define GHUDVK_API __declspec(dllimport)
#endif
#else
#define GHUDVK_API
#endif
#endif
#ifdef __linux__
#define GHUDVK_PLATFORM_LINUX
#define GHUDVK_API
#pragma warning "Linux might not be supported!"
#endif
#ifdef __OSX__
#define GHUDVK_PLATFORM_MACOS
#define GHUDVK_API
#pragma warning "MacOS might not be supported!"
#endif