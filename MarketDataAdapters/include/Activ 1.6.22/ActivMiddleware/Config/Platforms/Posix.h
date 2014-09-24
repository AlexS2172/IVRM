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
#include <pthread.h>
#include <netinet/in.h>

#define closesocket				close
#define ACTIV_WAIT_INFINITE		0xffffffff

namespace Activ
{

typedef pid_t OsProcessId;
typedef pthread_t ThreadId;

typedef int SOCKET;
const SOCKET INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;

typedef int HANDLE;
const HANDLE INVALID_HANDLE_VALUE = -1;

} // namespace Activ

#endif // (!defined ACTIV_CONFIG_POSIX_H)
