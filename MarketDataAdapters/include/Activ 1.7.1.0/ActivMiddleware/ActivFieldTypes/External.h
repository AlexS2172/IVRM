/**
 *  @file	ActivFieldTypes/External.h
 *
 *	@brief	Import rules for ActivFieldTypes exports.
 *
 *	$Log: $
 */

/**
 *	@def	ACTIV_FIELD_TYPES_API
 *
 *	@brief	This defines the library's interface to the client.
 */
#if !defined (ACTIV_FIELD_TYPES_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_FIELD_TYPES_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_FIELD_TYPES_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_FIELD_TYPES_API)

#include "ActivMiddleware/Activ.h"
