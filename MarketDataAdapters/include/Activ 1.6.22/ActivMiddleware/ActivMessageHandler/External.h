/**
 *  @file	ActivMessageHandler/External.h
 *
 *	@brief	Import rules for ActivMessageHandler exports.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MESSAGE_HANDLER_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_MESSAGE_HANDLER_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_MESSAGE_HANDLER_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_MESSAGE_HANDLER_API)

#include "ActivMiddleware/Activ.h"
