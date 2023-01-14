#pragma once

#ifdef _WIN32
#define GHUD_PLATFORM_WINDOWS
#endif
#ifdef __linux__
#define GHUD_PLATFORM_LINUX
#pragma error "Linux is currently not supported!"
#endif
#ifdef __OSX__
#define GHUD_PLATFORM_MACOS
#pragma error "MacOS is currently not supported!"
#endif