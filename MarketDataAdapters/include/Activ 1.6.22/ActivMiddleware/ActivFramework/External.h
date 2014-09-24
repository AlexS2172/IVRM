/**
 *  @file	External.h
 *
 *	@brief	Import rules for ActivFramework exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_EXTERNAL_H)
#define ACTIV_FRAMEWORK_EXTERNAL_H

#if !defined (ACTIV_FRAMEWORK_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_FRAMEWORK_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_FRAMEWORK_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_FRAMEWORK_API)

#include "ActivMiddleware/Activ.h"
#include "ActivMiddleware/ActivSystem/ActivSystem.h"

namespace Activ
{

/**
 *	@brief	List of agent scopes.
 */
#define ACTIV_AGENT_SCOPE_LIST(d)			\
	d(AGENT_SCOPE_LOCAL, "Local")			\
	d(AGENT_SCOPE_GLOBAL, "Global")			\
	d(AGENT_SCOPE_REMOTE, "Remote")

/**
 *	@brief	Agent scope type.
 *
 *	AGENT_SCOPE_LOCAL within the agent space
 *	AGENT_SCOPE_GLOBAL local & remote
 *	AGENT_SCOPE_REMOTE outside the agent space
 */
typedef byte_t AgentScope;

enum
{
	ACTIV_AGENT_SCOPE_LIST(ACTIV_DECLARE_HIDDEN_ELEMENT_1)
	NUM_AGENT_SCOPES
};

#define ACTIV_DECLARE_AGENT_SCOPE(a, b)	ACTIV_DECLARE_CONSTANT_ELEMENT_1(AgentScope, a, b)
ACTIV_AGENT_SCOPE_LIST(ACTIV_DECLARE_AGENT_SCOPE)
#undef ACTIV_DECLARE_AGENT_SCOPE

/**
 *	@brief	Return a scope as a string.
 */
ACTIV_FRAMEWORK_API std::string AgentScopeToString(const AgentScope agentScope);

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_EXTERNAL_H)
