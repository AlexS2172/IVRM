/**
 *  @file	ActivMonitor/ProcessDieMessage.h
 *
 *	@brief	Header file for monitor process die class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_PROCESS_DIE_MESSAGE_H)
#define ACTIV_MONITOR_PROCESS_DIE_MESSAGE_H

#include "ActivMiddleware/ActivMonitor/ActivMonitor.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	System state message.
 */
class ProcessDieMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_PROCESS_DIE;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	processName.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const std::string &processName);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	std::string	m_processName;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode ProcessDieMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const std::string &processName)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(processName));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode ProcessDieMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_processName));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_PROCESS_DIE_MESSAGE_H)
