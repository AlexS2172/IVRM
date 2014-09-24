/**
 *	@file	ActivIp/IpSocket.h
 *
 *	@brief	Header file for ip socket class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_SOCKET_H)
#define ACTIV_IP_SOCKET_H

#include "External.h"

#if defined (ACTIV_PLATFORM_WINDOWS)
#include "Windows/IpSocket.h"
#elif defined (ACTIV_PLATFORM_POSIX)
#include "Posix/IpSocket.h"
#else
#error No IpSocket.h for this platform yet.
#endif

namespace Activ
{

template<typename SOCKET_TYPE>
typename SOCKET_TYPE::Stats IpSocket::Stats::Get(const SOCKET_TYPE &socket)
{
	typedef typename SOCKET_TYPE::Stats StatsType;
	const IpSocket &ipSocket = socket;
	MutexHelper mutexHelper(ipSocket.m_pMutex);

#if defined (ACTIV_COMPILER_VISUAL_C)
	return StatsType::Stats(socket);
#else
	return typename SOCKET_TYPE::Stats(socket);
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpSocket::AppendRxBuffer(void *pData, const size_t dataLength)
{
	return m_rxBuffers.Append(pData, dataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpSocket::AppendTxBuffer(const void *pData, const size_t dataLength)
{
	return m_txBuffers.Append(const_cast<void *>(pData), dataLength);
}

} // namespace Activ

#endif // !defined (ACTIV_IP_SOCKET_H)
