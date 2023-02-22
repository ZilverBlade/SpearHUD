#pragma once

#ifdef _WIN32
#define GHUD_PLATFORmWINDOWS
#endif
#ifdef __linux__
#define GHUD_PLATFORmLINUX
#pragma error "Linux is currently not supported!"
#endif
#ifdef __OSX__
#define GHUD_PLATFORmMACOS
#pragma error "MacOS is currently not supported!"
#endif