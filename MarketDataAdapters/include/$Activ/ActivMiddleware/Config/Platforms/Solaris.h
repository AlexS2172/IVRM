/**
 *	@file	ConfigSolaris.h
 *
 *	@brief	Solaris configuration file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONFIG_SOLARIS_H)
#define ACTIV_CONFIG_SOLARIS_H

// include this now to get definitions of SUN_LEN, CMSG_LEN etc if available (e.g. solaris 10)
// and if not we'll define our own later
#include <sys/socket.h>

#define ACTIV_PLATFORM_NAME "Solaris"

#define ACTIV_POSIX_HAS_CONTEXT_API
#define ACTIV_POSIX_HAS_PSHARED

#define ACTIV_HAS_INTTYPES_H
#define ACTIV_EXPLICIT_INSTANTIATE_SIZE_T

#if !defined (SUN_LEN)
#define SUN_LEN(p)		((size_t)(ACTIV_FIELD_OFFSET(sockaddr_un, sun_path)))
#endif

#if !defined (CMSG_SPACE)
#define CMSG_SPACE(len)	(_CMSG_HDR_ALIGN(sizeof(struct cmsghdr)) + _CMSG_DATA_ALIGN(len))
#endif

#if !defined (CMSG_LEN)
#define CMSG_LEN(len)	(_CMSG_HDR_ALIGN(sizeof(struct cmsghdr)) + (len))
#endif

#define ACTIV_MAX_PATH	MAXNAMELEN

#include "Posix.h"
#include <strings.h>

#endif // !defined (ACTIV_CONFIG_SOLARIS_H)
