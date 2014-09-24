/**
 *	@file	ActivIp/TcpSocket.h
 *
 *	@brief	Header file for tcp socket class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_TCP_SOCKET_H)
#define ACTIV_IP_TCP_SOCKET_H

#include "External.h"

#if defined (ACTIV_PLATFORM_WINDOWS)
#include "Windows/TcpSocket.h"
#elif defined (ACTIV_PLATFORM_POSIX)
#include "Posix/TcpSocket.h"
#else
#error No TcpSocket.h for this platform yet.
#endif

#endif // !defined (ACTIV_IP_TCP_SOCKET_H)
