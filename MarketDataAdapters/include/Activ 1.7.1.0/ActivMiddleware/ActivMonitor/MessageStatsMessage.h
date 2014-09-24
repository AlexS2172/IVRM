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
	 *	@param	latency.
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
								const Time &lastReceiveTime,
								const uint32_t latency);

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
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

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
	uint32_t	m_latency;

private:
	static const uint8_t FLAG_BITS_RECEIVE_TIME = 0x01;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageStatsMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const std::string &processName, const std::string &feedName, const std::string &userData, const uint32_t receivedCount, const uint32_t validatedCount, const uint32_t processedCount, const uint32_t ignoredCount, const uint32_t invalidCount, const uint32_t processingFailedCount, const Time &lastReceiveTime, const uint32_t latency)
{
	uint8_t flag = 0;

	if (lastReceiveTime.IsInitialized())
		flag |= FLAG_BITS_RECEIVE_TIME;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(processName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(feedName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(userData));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(flag));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(receivedCount, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(validatedCount, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(processedCount, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(ignoredCount, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(invalidCount, MessageHandler::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(processingFailedCount, MessageHandler::ENDIAN_LITTLE));
	
	if ((flag & FLAG_BITS_RECEIVE_TIME) == FLAG_BITS_RECEIVE_TIME)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, lastReceiveTime.SerializeLengthAndBody(messageBuilder));

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(latency, MessageHandler::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageStatsMessage::Serialize(MessageBuilder &messageBuilder) const
{
	return Serialize(messageBuilder, m_systemName, m_processName, m_feedName, m_userData, m_receivedCount, m_validatedCount, m_processedCount, m_ignoredCount, m_ignoredCount, m_ignoredCount, m_lastReceiveTime, m_latency);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageStatsMessage::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&m_processName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&m_feedName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&m_userData));

	if (messageValidator.GetMessageVersion() <= 3)
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_receivedCount));
		
		// HACK the IsEndOfMessage() will go
		if ((0 == m_receivedCount) && messageValidator.IsEndOfMessage())
		{
			m_validatedCount = 0;
			m_processedCount = 0;
			m_ignoredCount = 0;
			m_invalidCount = 0;
			m_processingFailedCount = 0;
			m_lastReceiveTime.Reset();
			m_latency = 0;

			return STATUS_CODE_SUCCESS;
		}

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_validatedCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_processedCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_ignoredCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_invalidCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_processingFailedCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_lastReceiveTime.DeserializeLengthAndBody(messageValidator));

		m_latency = 0;

		return STATUS_CODE_SUCCESS;
	}
	else
	{
		uint8_t flag;

		// Version 4. Latency and flag added
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint8_t), MessageHandler::ENDIAN_LITTLE, &flag));

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_receivedCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_validatedCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_processedCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_ignoredCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_invalidCount));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_processingFailedCount));
		
		if ((flag & FLAG_BITS_RECEIVE_TIME) == FLAG_BITS_RECEIVE_TIME)
			ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_lastReceiveTime.DeserializeLengthAndBody(messageValidator));
		else
			m_lastReceiveTime.Reset();

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint32_t), MessageHandler::ENDIAN_LITTLE, &m_latency));

		return STATUS_CODE_SUCCESS;
	}

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_MESSAGE_STATS_MESSAGE_H)
