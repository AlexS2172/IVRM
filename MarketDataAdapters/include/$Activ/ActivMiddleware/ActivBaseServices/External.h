/**
 *  @file	ActivBaseServices/External.h
 *
 *	@brief	Import rules for ActivBaseServices exports.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_SERVICES_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_BASE_SERVICES_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_BASE_SERVICES_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_BASE_SERVICES_API)

#include "ActivMiddleware/ActivFramework/ActivFramework.h"
