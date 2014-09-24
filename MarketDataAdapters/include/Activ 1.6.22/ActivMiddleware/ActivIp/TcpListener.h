/**
 *	@file	ActivIp/TcpListener.h
 *
 *	@brief	Header file for tcp listener class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_TCP_LISTENER_H)
#define ACTIV_IP_TCP_LISTENER_H

#include "External.h"

#if defined (ACTIV_PLATFORM_WINDOWS)
#include "Windows/TcpListener.h"
#elif defined (ACTIV_PLATFORM_POSIX)
#include "Posix/TcpListener.h"
#else
#error No TcpListener.h for this platform yet.
#endif

#endif // !defined (ACTIV_IP_TCP_LISTENER_H)
