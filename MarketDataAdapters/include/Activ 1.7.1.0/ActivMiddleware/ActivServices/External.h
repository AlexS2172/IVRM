/**
 *  @file	ActivServices/External.h
 *
 *	@brief	Import rules for ActivServices exports.
 *
 *	$Log: $
 */

#if !defined (ACTIV_SERVICES_EXTERNAL_H)
#define ACTIV_SERVICES_EXTERNAL_H

#if !defined (ACTIV_SERVICES_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_SERVICES_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_SERVICES_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_SERVICES_API)

#include "ActivMiddleware/Activ.h"
#include "ActivMiddleware/ActivFramework/ActivFramework.h"

#endif // !defined (ACTIV_SERVICES_EXTERNAL_H)
