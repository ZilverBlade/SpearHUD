#pragma once

#ifdef _WIN32
#define GHUDVK_PLATFORM_WINDOWS
#ifdef _DLLEXPORT
#define GHUDVK_API __declspec(dllexport)
#else
#define GHUDVK_API __declspec(dllimport)
#endif
#endif
#ifdef __linux__
#define GHUDVK_PLATFORM_LINUX
#pragma error "Linux is currently not supported!"
#define GHUDVK_API
#endif
#ifdef __OSX__
#define GHUDVK_PLATFORM_MACOS
#pragma error "MacOS is currently not supported!"
#define GHUDVK_API
#endif