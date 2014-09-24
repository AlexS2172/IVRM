/**
 *  @file	ActivIp/External.h
 *
 *	@brief	Import rules for ActivIp exports.
 *
 *  $Log: $
 */
 
#if !defined (ACTIV_IP_EXTERNAL_H)
#define ACTIV_IP_EXTERNAL_H

#if !defined (ACTIV_IP_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_IP_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_IP_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_IP_API)

#include "ActivMiddleware/Activ.h"

#endif // !defined (ACTIV_IP_EXTERNAL_H)
