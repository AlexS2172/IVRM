/**
 *  @file	ActivMq/External.h
 *
 *	@brief	Import rules for ActivMq exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_MQ_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_MQ_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_MQ_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_MQ_API)

#include "ActivMiddleware/Activ.h"
