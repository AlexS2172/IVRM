/**
 *	@file	Windows.h
 *
 *	@brief	Microsoft Windows configuration file.
 *
 *	$Log: $
 */

#if (!defined ACTIV_CONFIG_WINDOWS_H)
#define ACTIV_CONFIG_WINDOWS_H

#if (ACTIV_PLATFORM_WINDOWS == 64)
#define ACTIV_PLATFORM_NAME		"Win64"
#else
#define ACTIV_PLATFORM_NAME		"Win32"
#endif

#define ACTIV_PLATFORM_TYPE		Activ::PLATFORM_TYPE_WINDOWS

#if (!defined _WIN32_WINNT)
#define _WIN32_WINNT 0x500
#endif

// prevent definition of max and min macros in windef.h. Must be defined before windows.h is included.
#if (!defined NOMINMAX)
#define NOMINMAX
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define ACTIV_WAIT_INFINITE	INFINITE
#define ACTIV_MAX_PATH		MAX_PATH

#if (ACTIV_PLATFORM_WINDOWS == 32) && (!defined ACTIV_COMPILER_GCC)
#define ACTIV_EXPLICIT_INSTANTIATE_SIZE_T
#endif

#endif // (!defined ACTIV_CONFIG_WINDOWS_H)
