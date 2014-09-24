/**
 *	@file	Posix.h
 *
 *	@brief	Posix configuration file.
 *
 *	$Log: $
 */

#if (!defined ACTIV_CONFIG_POSIX_H)
#define ACTIV_CONFIG_POSIX_H

#define ACTIV_PLATFORM_POSIX

#include <unistd.h>
#include <cerrno>
#include <netinet/in.h>

#define ACTIV_WAIT_INFINITE		0xffffffff

namespace Activ
{

typedef int SOCKET;
const SOCKET INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;

typedef int HANDLE;
const HANDLE INVALID_HANDLE_VALUE = -1;

/**
 *	@brief	Close a socket handle.
 *
 *	@param	socket the socket to close.
 *
 *	@return	0 on success or SOCKET_ERROR on error.
 */
inline int closesocket(const SOCKET socket)
{
	return ::close(socket);
}

} // namespace Activ

#endif // (!defined ACTIV_CONFIG_POSIX_H)
