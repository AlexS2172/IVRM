/**
 *  @file	ActivUpdate/External.h
 *
 *	@brief	Import rules for ActivUpdate exports.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UPDATE_EXTERNAL_H)
#define ACTIV_UPDATE_EXTERNAL_H

#if !defined (ACTIV_UPDATE_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_UPDATE_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_UPDATE_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_UPDATE_API)

#include "ActivMiddleware/Activ.h"
#include "ActivMiddleware/ActivFramework/ActivFramework.h"

#endif // !defined (ACTIV_UPDATE_EXTERNAL_H)
