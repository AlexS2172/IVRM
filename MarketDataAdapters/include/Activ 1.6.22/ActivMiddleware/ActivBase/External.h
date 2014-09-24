/**
 *  @file	ActivBase/External.h
 *
 *	@brief	Import rules for ActivBase exports.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_BASE_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_BASE_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_BASE_API)

#include "ActivMiddleware/Activ.h"
