/**
 *	@file	Messages.h
 *
 *	@brief	Header file for pubsub service messages.
 *
 *  $Log: $
 */

#if !defined (ACTIV_BASE_SERVIES_PUB_SUB_MESSAGES_H)
#define ACTIV_BASE_SERVIES_PUB_SUB_MESSAGES_H

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
 *	@brief	Subscription message.
 */
class SubscribeMessage
{
public:
	/**
	 *	@brief	Deserialize the message.
	 *
	 *	@param	messageValidater message validater from which the data will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

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

inline StatusCode SubscribeMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_agentScope), 0, NUM_AGENT_SCOPES - 1, MessageHandler::ENDIAN_LITTLE, &m_agentScope));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_subject.Deserialize(messageValidater));

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // !defined (ACTIV_BASE_SERVIES_PUB_SUB_MESSAGES_H)
