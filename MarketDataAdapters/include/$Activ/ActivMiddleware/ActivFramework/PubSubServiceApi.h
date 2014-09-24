/**
 *	@file	PubSubServiceApi.h
 *
 *	@brief	Header file for the pubsub service api.
 *
 *	$Log: $
 */

#if !defined (ACTIV_FRAMEWORK_PUBSUB_SERVICE_API_H)
#define ACTIV_FRAMEWORK_PUBSUB_SERVICE_API_H

#include "ActivMiddleware/ActivFramework/External.h"

namespace Activ
{

class Component;

/**
 *	@brief	Api to the agent's pubsub service.
 */
class PubSubServiceApi
{
public:
	/**
	 *	@brief	Subscribe to a subject.
	 *
	 *	Note subscriptions from a component are not cumulative at the same scope level. However, subscribing to the
	 *	same subject at more than one scope requires unsubscribing at each scope.
	 *
	 *	@param	component the component wishing to subscribe.
	 *	@param	agentScope AGENT_SCOPE_LOCAL only receive messages published within this agent space,
	 *			AGENT_SCOPE_GLOBAL receive both local and messages published onto the network, AGENT_SCOPE_REMOTE only receive
	 *			messages published remotely.
	 *	@param	subject subject to subscribe to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode Subscribe(const Component &component, const AgentScope agentScope, const Subject &subject);

	/**
	 *	@brief	Unsubscribe from a subject.
	 *
	 *	@param	component the component wishing to unsubscribe.
	 *	@param	agentScope the scope from which to unsubscribe.
	 *	@param	subject the subject to unsubscribe from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode Unsubscribe(const Component &component, const AgentScope agentScope, const Subject &subject);

	/**
	 *	@brief	Publish a message.
	 *
	 *	@param	component the component wishing to publish.
	 *	@param	agentScope AGENT_SCOPE_LOCAL only publish locally within this agent space, AGENT_SCOPE_REMOTE
	 *			only publish onto the network, AGENT_SCOPE_GLOBAL publish locally & remotely.
	 *	@param	subject subject of published message.
	 *	@param	type type of published message.
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to publish.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode Publish(const Component &component, const AgentScope agentScope, const Subject &subject,
												  const MessageType type, const MessageBuilder &messageBuilder);

	/**
	 *	@brief	Publish a message.
	 *
	 *	@param	component the component wishing to publish.
	 *	@param	agentScope AGENT_SCOPE_LOCAL only publish locally within this agent space, AGENT_SCOPE_REMOTE
	 *			only publish onto the network, AGENT_SCOPE_GLOBAL publish locally & remotely.
	 *	@param	subject subject of published message.
	 *	@param	type type of published message.
	 *	@param	pData pointer to message body.
	 *	@param	dataLength length of message body.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode Publish(const Component &component, const AgentScope agentScope, const Subject &subject,
												  const MessageType type, const void *pData = 0, const size_t dataLength = 0);

	/**
	 *	@brief	Publish a message.
	 *
	 *	@param	component the component wishing to publish.
	 *	@param	agentScope AGENT_SCOPE_LOCAL only publish locally within this agent space, AGENT_SCOPE_REMOTE
	 *			only publish onto the network, AGENT_SCOPE_GLOBAL publish locally & remotely.
	 *	@param	subject subject of published message.
	 *	@param	type type of published message.
	 *	@param	heapMessage HeapMessage containing message body to publish.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode Publish(const Component &component, const AgentScope agentScope, const Subject &subject,
												  const MessageType type, HeapMessage &heapMessage);
};

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_PUBSUB_SERVICE_API_H)
