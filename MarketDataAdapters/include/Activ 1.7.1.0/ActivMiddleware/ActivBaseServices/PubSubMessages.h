/**
 *	@file	Messages.h
 *
 *	@brief	Header file for pubsub service messages.
 *
 *  $Log: $
 */

#if !defined (ACTIV_BASE_SERVIES_PUB_SUB_MESSAGES_H)
#define ACTIV_BASE_SERVIES_PUB_SUB_MESSAGES_H

#include "ActivMiddleware/ActivFramework/PubSubServiceApi.h"

namespace Activ
{

const PortId pubSubDefaultPort = 0;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Subject used when publishing (un)subscription messages.
 */
class SubscriptionSubject : public Subject
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	SubscriptionSubject();
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline SubscriptionSubject::SubscriptionSubject() :
	Subject("/system/pubsub/subscription")
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_PUBSUB message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_PUBSUB,	PUBSUB_MESSAGE_SUBSCRIBE);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_PUBSUB,	PUBSUB_MESSAGE_UNSUBSCRIBE);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_PUBSUB request types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_PUBSUB,	PUBSUB_REQUEST_GET_SUBSCRIPTIONS);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Subscription message.
 */
class SubscribeMessage
{
public:
	/**
	 *	@brief	Deserialize the message.
	 *
	 *	@param	messageValidator message validator from which the data will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief	Serialize the message.
	 *
	 *	@param	messageBuilder message builder into which data will be placed.
	 *	@param	agentScope.
	 *	@param	subject.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const AgentScope agentScope, const Subject &subject);

	AgentScope	m_agentScope;
	Subject		m_subject;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SubscribeMessage::Serialize(MessageBuilder &messageBuilder, const AgentScope agentScope, const Subject &subject)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(agentScope, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, subject.Serialize(messageBuilder));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SubscribeMessage::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(m_agentScope), 0, NUM_AGENT_SCOPES - 1, MessageHandler::ENDIAN_LITTLE, &m_agentScope));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_subject.Deserialize(messageValidator));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get subscriptions request.
 */
class GetSubscriptionsRequest
{
public:
	/**
	 *	@brief	Deserialize the message.
	 *
	 *	@param	messageValidator message validator from which the data will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief	Serialize the message.
	 *
	 *	@param	messageBuilder message builder into which data will be placed.
	 *	@param	subject.
	 *	@param	shouldRecurseTree.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const Subject &subject, const bool shouldRecurseTree);

	Subject		m_subject;
	bool		m_shouldRecurseTree;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode GetSubscriptionsRequest::Serialize(MessageBuilder &messageBuilder, const Subject &subject, const bool shouldRecurseTree)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, subject.Serialize(messageBuilder));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.AppendBoolean(shouldRecurseTree));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode GetSubscriptionsRequest::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_subject.Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateBoolean(&m_shouldRecurseTree));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get subscriptions response.
 */
class GetSubscriptionsResponse
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	subscriptionMap reference to map to take the result.
	 */
	GetSubscriptionsResponse(PubSubServiceApi::SubscriptionMap &subscriptionMap);

	/**
	 *	@brief	Deserialize the message.
	 *
	 *	@param	messageValidator message validator from which the data will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief	Serialize the message.
	 *
	 *	@param	messageBuilder message builder into which data will be placed.
	 *	@param	subscriptionMap.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const PubSubServiceApi::SubscriptionMap &subscriptionMap);

	PubSubServiceApi::SubscriptionMap &	m_subscriptionMap;		///< The subscription map.

private:
	typedef SerializableObjectContainerHelper<PubSubServiceApi::AddressSet, uint32_t>	AddressSetSerializer;	///< Address set list serializer type.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline GetSubscriptionsResponse::GetSubscriptionsResponse(PubSubServiceApi::SubscriptionMap &subscriptionMap) :
	m_subscriptionMap(subscriptionMap)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode GetSubscriptionsResponse::Serialize(MessageBuilder &messageBuilder, const PubSubServiceApi::SubscriptionMap &subscriptionMap)
{
	for (PubSubServiceApi::SubscriptionMap::const_iterator iterator = subscriptionMap.begin(), iteratorEnd = subscriptionMap.end(); iteratorEnd != iterator; ++iterator)
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, iterator->first.Serialize(messageBuilder));

		const PubSubServiceApi::AddressSetList &addressSetList = iterator->second;

		if (addressSetList.size() != NUM_AGENT_SCOPES)
			return STATUS_CODE_FAILURE;

		for (size_t agentScope = 0; agentScope < NUM_AGENT_SCOPES; ++agentScope)
		{
			ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AddressSetSerializer::Serialize(messageBuilder, addressSetList[agentScope]));
		}
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode GetSubscriptionsResponse::Deserialize(MessageValidator &messageValidator)
{
	while (!messageValidator.IsEndOfMessage())
	{
		Subject subject;
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, subject.Deserialize(messageValidator));

		PubSubServiceApi::AddressSetList addressSetList;

		for (size_t agentScope = 0; agentScope < NUM_AGENT_SCOPES; ++agentScope)
		{
			PubSubServiceApi::AddressSet addressSet;
			ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AddressSetSerializer::Deserialize(messageValidator, addressSet));
			addressSetList.push_back(addressSet);
		}

		if (!m_subscriptionMap.insert(PubSubServiceApi::SubscriptionMap::value_type(subject, addressSetList)).second)
			return STATUS_CODE_FAILURE;
	}

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // !defined (ACTIV_BASE_SERVIES_PUB_SUB_MESSAGES_H)
