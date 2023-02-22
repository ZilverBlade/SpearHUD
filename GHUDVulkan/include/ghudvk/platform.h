#pragma once

#ifdef _WIN32
#define GHUDVK_PLATFORmWINDOWS
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
#define GHUDVK_PLATFORmLINUX
#pragma error "Linux is currently not supported!"
#define GHUDVK_API
#endif
#ifdef __OSX__
#define GHUDVK_PLATFORmMACOS
#pragma error "MacOS is currently not supported!"
#define GHUDVK_API
#endif