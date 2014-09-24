/**
 *  @file	External.h
 *
 *	@brief	Import rules for ActivContentGatewayCommon exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_EXTERNAL_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_EXTERNAL_H

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_CONTENT_GATEWAY_COMMON_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_CONTENT_GATEWAY_COMMON_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_API)

#include "ActivMiddleware/ActivFramework/ActivFramework.h"

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_EXTERNAL_H)
