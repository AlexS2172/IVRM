/**
 *  @file	ActivMonitor/MessageStatsMessage.h
 *
 *	@brief	Header file for monitor message stats message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_MESSAGE_STATS_MESSAGE_H)
#define ACTIV_MONITOR_MESSAGE_STATS_MESSAGE_H

#include "ActivMonitor.h"

#include "ActivMiddleware/ActivFieldTypes/Time.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	Message stats message.
 */
class MessageStatsMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_MESSAGE_STATS;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	feedName.
	 *	@param	userData.
	 *	@param	receivedCount.
	 *	@param	validatedCount.
	 *	@param	processedCount.
	 *	@param	ignoredCount.
	 *	@param	invalidCount.
	 *	@param	processingFailedCount.
	 *	@param	lastReceiveTime.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder,
								const std::string &systemName,
								const std::string &processName,
								const std::string &feedName,
								const std::string &userData,
								const uint32_t receivedCount,
								const uint32_t validatedCount,
								const uint32_t processedCount,
								const uint32_t ignoredCount,
								const uint32_t invalidCount,
								const uint32_t processingFailedCount,
								const Time &lastReceiveTime);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Serialize(MessageBuilder &messageBuilder) const;

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
	std::string	m_feedName;
	std::string	m_userData;
	uint32_t	m_receivedCount;
	uint32_t	m_validatedCount;
	uint32_t	m_processedCount;
	uint32_t	m_ignoredCount;
	uint32_t	m_invalidCount;
	uint32_t	m_processingFailedCount;
	Time		m_lastReceiveTime;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageStatsMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const std::string &processName, const std::string &feedName, const std::string &userData, const uint32_t receivedCount, const uint32_t validatedCount, const uint32_t processedCount, const uint32_t ignoredCount, const uint32_t invalidCount, const uint32_t processingFailedCount, const Time &lastReceiveTime)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(processName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(feedName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(userData));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(receivedCount, MessageHandler::ENDIAN_LITTLE));
	
	if (receivedCount > 0)
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(validatedCount, MessageHandler::ENDIAN_LITTLE));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(processedCount, MessageHandler::ENDIAN_LITTLE));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(ignoredCount, MessageHandler::ENDIAN_LITTLE));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(invalidCount, MessageHandler::ENDIAN_LITTLE));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(processingFailedCount, MessageHandler::ENDIAN_LITTLE));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, lastReceiveTime.SerializeLengthAndBody(messageBuilder));
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageStatsMessage::Serialize(MessageBuilder &messageBuilder) const
{
	return Serialize(messageBuilder, m_systemName, m_processName, m_feedName, m_userData, m_receivedCount, m_validatedCount, m_processedCount, m_ignoredCount, m_ignoredCount, m_ignoredCount, m_lastReceiveTime);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageStatsMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_processName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_feedName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_userData));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_receivedCount), MessageHandler::ENDIAN_LITTLE, &m_receivedCount));

	// HACK the IsEndOfMessage() will go
	if ((0 == m_receivedCount) && messageValidater.IsEndOfMessage())
	{
		m_validatedCount = 0;
		m_processedCount = 0;
		m_ignoredCount = 0;
		m_invalidCount = 0;
		m_processingFailedCount = 0;
		m_lastReceiveTime.Reset();

		return STATUS_CODE_SUCCESS;
	}

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_validatedCount), MessageHandler::ENDIAN_LITTLE, &m_validatedCount));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_processedCount), MessageHandler::ENDIAN_LITTLE, &m_processedCount));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_ignoredCount), MessageHandler::ENDIAN_LITTLE, &m_ignoredCount));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_invalidCount), MessageHandler::ENDIAN_LITTLE, &m_invalidCount));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(m_processingFailedCount), MessageHandler::ENDIAN_LITTLE, &m_processingFailedCount));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_lastReceiveTime.DeserializeLengthAndBody(messageValidater));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_MESSAGE_STATS_MESSAGE_H)
