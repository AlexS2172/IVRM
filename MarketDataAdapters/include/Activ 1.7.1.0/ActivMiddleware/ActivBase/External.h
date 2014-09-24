/**
 *  @file	ActivBase/External.h
 *
 *	@brief	Import/export rules for ActivBase.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_EXTERNAL_H)
#define ACTIV_BASE_EXTERNAL_H

#if (!defined ACTIV_BASE_API)
#	if (!defined ACTIV_STATIC)
#		define ACTIV_BASE_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_BASE_API ACTIV_IMPORT_LIB
#	endif // (!defined ACTIV_STATIC)
#endif // (!defined ACTIV_BASE_API)

#include "ActivMiddleware/Activ.h"

#endif // (!defined ACTIV_BASE_EXTERNAL_H)
