/**
 *	@file	ActivIp/UdpSocket.h
 *
 *	@brief	Header file for udp socket class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_UDP_SOCKET_H)
#define ACTIV_IP_UDP_SOCKET_H

#include "External.h"

#if defined (ACTIV_PLATFORM_WINDOWS)
#include "Windows/UdpSocket.h"
#elif defined (ACTIV_PLATFORM_POSIX)
#include "Posix/UdpSocket.h"
#else
#error No UdpSocket.h for this platform yet.
#endif

#endif // !defined (ACTIV_IP_UDP_SOCKET_H)
