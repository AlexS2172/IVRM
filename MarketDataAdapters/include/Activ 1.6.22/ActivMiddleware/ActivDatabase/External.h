/**
 *  @file	ActivDatabase/External.h
 *
 *	@brief	Import rules for ActivDatabase exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_DATABASE_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_DATABASE_API ACTIV_IMPORT_DLL				///< Export as a dll
#	else
#		define ACTIV_DATABASE_API ACTIV_IMPORT_LIB				///< Export as a lib
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_DATABASE_API)

#include "ActivMiddleware/Activ.h"
