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
#define GHUDVK_API
#pragma error "Linux is currently not supported!"
#endif
#ifdef __OSX__
#define GHUDVK_PLATFORM_MACOS
#define GHUDVK_API
#pragma error "MacOS is currently not supported!"
#endif