/**
 *  @file	ActivSystem/External.h
 *
 *	@brief	Import rules for ActivSystem exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_EXTERNAL_H)
#define ACTIV_SYSTEM_EXTERNAL_H

#if !defined (ACTIV_SYSTEM_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_SYSTEM_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_SYSTEM_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_SYSTEM_API)

#include "ActivMiddleware/Activ.h"
#include "ActivMiddleware/ActivSystem/BasicTypes.h"

#endif // !defined (ACTIV_SYSTEM_EXTERNAL_H)
