/**
 *  @file	ActivCompress/External.h
 *
 *	@brief	Import rules for ActivCompress exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_COMPRESS_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_COMPRESS_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_COMPRESS_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_COMPRESS_API)

#include "ActivMiddleware/Activ.h"
