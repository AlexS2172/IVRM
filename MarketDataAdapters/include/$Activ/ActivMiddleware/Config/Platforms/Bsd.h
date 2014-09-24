/**
 *	@file	Bsd.h
 *
 *	@brief	BSD configuration file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONFIG_BSD_H)
#define ACTIV_CONFIG_BSD_H

#define ACTIV_PLATFORM_NAME "BSD"

#define ACTIV_POSIX_HAS_CONTEXT_API
#define ACTIV_HAS_INTTYPES_H

#define ACTIV_MAX_PATH	NAME_MAX

#include "Posix.h"

#endif // !defined (ACTIV_CONFIG_BSD_H)
