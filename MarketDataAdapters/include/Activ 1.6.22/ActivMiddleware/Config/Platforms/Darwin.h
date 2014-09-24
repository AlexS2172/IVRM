/**
 *	@file	Darwin.h
 *
 *	@brief	Darwin configuration file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONFIG_DARWIN_H)
#define ACTIV_CONFIG_DARWIN_H

#define ACTIV_PLATFORM_NAME "Darwin"

#define __STDC_LIBRARY_SUPPORTED__

#define ACTIV_HAS_INTTYPES_H
#define ACTIV_EXPLICIT_INSTANTIATE_SIZE_T

#define ACTIV_MAX_PATH	NAME_MAX

#include "Posix.h"

#endif // !defined (ACTIV_CONFIG_DARWIN_H)
