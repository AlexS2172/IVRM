/**
 *	@file	Messages.h
 *
 *	@brief	Header file for exported framework message classes.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_MESSAGES_H)
#define ACTIV_FRAMEWORK_MESSAGES_H

#include "ActivMiddleware/ActivFramework/ServiceTypes.h"
#include "ActivMiddleware/ActivMessageHandler/ISerializableMessage.h"

namespace Activ
{

/**
 *	@brief	Open service request.
 */
class OpenServiceRequest : public ISerializableMessage
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	m_version is set to m_currentVersion, but can be overriden if required.
	 */
	ACTIV_FRAMEWORK_API OpenServiceRequest();

	ACTIV_FRAMEWORK_API virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;
	ACTIV_FRAMEWORK_API virtual StatusCode Deserialize(MessageValidater &messageValidater);

	static const byte_t m_highestVersion = 2;					///< Highest supported version
	static const byte_t m_currentVersion = 1;					///< Current version used by client side

	byte_t		m_version;
	ServiceId	m_serviceId;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Open service response. The response implicitly matches the version of the request.
 */
class OpenServiceResponse : public ISerializableMessage
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API OpenServiceResponse();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	serviceId id for the service.
	 */
	ACTIV_FRAMEWORK_API OpenServiceResponse(const ServiceId &serviceId);

	ACTIV_FRAMEWORK_API virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;
	ACTIV_FRAMEWORK_API virtual StatusCode Deserialize(MessageValidater &messageValidater);

	ServiceId	m_serviceId;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	SYSTEM_MESSAGE_ENDPOINT_SHUTDOWN, SYSTEM_MESSAGE_ENDPOINT_DIE.
 *
 *	Message received by subscribing to EndPointSubject.
 */
class SystemMessageEndPointIdSet
{
public:
	/**
	 *	@brief	Serialize the message.
	 *
	 *	@param	messageBuilder builder to use.
	 *	@param	endPointIdSet set of endpoints shutting down.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode Serialize(MessageBuilder &messageBuilder, const EndPointIdSet &endPointIdSet);

	/**
	 *	@brief	Deserialize the message.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Deserialize(MessageValidater &messageValidater);

	EndPointIdSet	m_endPointIdSet;							///< set of endpoints that are shutting down

private:
	typedef SerializableObjectContainerHelper<EndPointIdSet, EndPointId> Serializer;
};

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_MESSAGES_H)
