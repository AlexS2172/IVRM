/**
 *  @file	ActivRouter/External.h
 *
 *	@brief	Import rules for ActivRouter exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_ROUTER_EXTERNAL_H)
#define ACTIV_ROUTER_EXTERNAL_H

#if !defined (ACTIV_ROUTER_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_ROUTER_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_ROUTER_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_ROUTER_API)

#include "ActivMiddleware/ActivFramework/ActivFramework.h"

#endif // !defined (ACTIV_ROUTER_EXTERNAL_H)
