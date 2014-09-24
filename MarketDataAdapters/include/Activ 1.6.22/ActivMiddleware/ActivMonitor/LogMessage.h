/**
 *  @file	ActivMonitor/LogMessage.h
 *
 *	@brief	Header file for monitor log message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_LOG_MESSAGE_H)
#define ACTIV_MONITOR_LOG_MESSAGE_H

#include "ActivMiddleware/ActivMonitor/ActivMonitor.h"
#include "ActivMiddleware/ActivFieldTypes/DateTime.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	System state message.
 */
class LogMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_LOG;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	dateTime time of log message.
	 *	@param	logType type of log message.
	 *	@param	endPointPortName name of endpoint & port originally sending the log message.
	 *	@param	message log message body.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const DateTime &dateTime, const LogType logType, const std::string &endPointPortName, const std::string &message);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	DateTime	m_dateTime;
	LogType		m_logType;
	std::string	m_endPointPortName;
	std::string	m_message;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode LogMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const DateTime &dateTime, const LogType logType, const std::string &endPointPortName, const std::string &message)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, dateTime.SerializeLengthAndBody(messageBuilder));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(logType, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(endPointPortName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(message));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode LogMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_dateTime.DeserializeLengthAndBody(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_logType), MessageHandler::ENDIAN_LITTLE, &m_logType));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_endPointPortName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_message));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_LOG_MESSAGE_H)
