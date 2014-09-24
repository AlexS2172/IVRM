/**
 *  @file	ActivMonitor/Messages.h
 *
 *	@brief	Header file for monitor message classes.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_H)
#define ACTIV_MONITOR_H

#include "ActivMiddleware/ActivMessageHandler/MessageValidater.h"
#include "ActivMiddleware/ActivMessageHandler/MessageBuilder.h"

#include "ActivMiddleware/ActivIp/IpManager.h"

namespace Activ
{

namespace Monitor
{

// Message types
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_SYSTEM_STATE);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_MESSAGE_STATS);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_PROCESS_DIE);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_HEAP_STATS);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_TCP_CONNECTION_STATE);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_LOG);
ACTIV_DECLARE_MESSAGE_TYPE(6,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_PROCESS_STATS);
ACTIV_DECLARE_MESSAGE_TYPE(7,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_ENDPOINT_STATS);
ACTIV_DECLARE_MESSAGE_TYPE(8,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_ENDPOINT_STATS_EOD);
ACTIV_DECLARE_MESSAGE_TYPE(9,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_HEAP_STATS_EOD);
ACTIV_DECLARE_MESSAGE_TYPE(10,	MESSAGE_CLASS_MONITOR,	MONITOR_MESSAGE_UDP_RECEIVER_STATE);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Subject for monitor messages.
 */
class Subject : public Activ::Subject
{
public:
	/**
	 *	@brief	Constructor.
	 */
	Subject();
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Subject::Subject() :
	Activ::Subject("/system/monitor")
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Base monitor message.
 */
class Message
{
public:
	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	std::string	m_systemName;
	std::string	m_hostName;

	static const byte_t CURRENT_VERSION = 2;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Message::Serialize(MessageBuilder &messageBuilder, const std::string &systemName)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(CURRENT_VERSION, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(IpManager::GetHostName()));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Message::Deserialize(MessageValidater &messageValidater)
{
	byte_t version;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(version), MessageHandler::ENDIAN_LITTLE, &version));

	if (version > CURRENT_VERSION)
		return STATUS_CODE_INVALID_VERSION;

	messageValidater.SetMessageVersion(version);

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_hostName));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_H)
