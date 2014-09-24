/**
 *  @file	External.h
 *
 *	@brief	Import rules for AgentApplication exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_AGENT_APPLICATION_EXTERNAL_H)
#define ACTIV_AGENT_APPLICATION_EXTERNAL_H

#if !defined (ACTIV_AGENT_APPLICATION_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_AGENT_APPLICATION_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_AGENT_APPLICATION_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_AGENT_APPLICATION_API)

#include "ActivMiddleware/ActivFramework/ActivFramework.h"

#endif // !defined (ACTIV_AGENT_APPLICATION_EXTERNAL_H)
