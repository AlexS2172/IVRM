/**
 *  @file	ActivMonitor/FeedStatusMessage.h
 *
 *	@brief	Header file for monitor feed status message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_FEED_STATUS_MESSAGE_H)
#define ACTIV_MONITOR_FEED_STATUS_MESSAGE_H

#include "ActivMonitor.h"

#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/UInt.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	Feed status message.
 */
class FeedStatusMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_FEED_STATUS;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	cgName.
	 *	@param	feedName.
	 *	@param	feedLatency.
	 *	@param	systemLatency.
	 *	@param	systemTimeStamp.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder,
								const std::string &systemName,
								const std::string &cgName,
								const std::string &feedName,
								const Time &systemTimeStamp,
								const UInt &systemLatency,
								const UInt &feedLatency);

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

	std::string	m_cgName;
	std::string	m_feedName;
	Time		m_systemTimeStamp;
	UInt		m_systemLatency;
	UInt		m_feedLatency;

private:
	static const uint8_t FLAG_BITS_SYSTEM_TIME_STAMP = 0x01;
	static const uint8_t FLAG_BITS_SYSTEM_LATENCY = 0x02;
	static const uint8_t FLAG_BITS_FEED_LATENCY = 0x04;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FeedStatusMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const std::string &cgName, const std::string &feedName, const Time &systemTimeStamp, const UInt &systemLatency, const UInt &feedLatency)
{
	uint8_t flag = 0;

	if (systemTimeStamp.IsInitialized())
		flag |= FLAG_BITS_SYSTEM_TIME_STAMP;

	if (systemLatency.IsInitialized())
		flag |= FLAG_BITS_SYSTEM_LATENCY;

	if (feedLatency.IsInitialized())
		flag |= FLAG_BITS_FEED_LATENCY;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(cgName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(feedName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(flag));

	if ((flag & FLAG_BITS_SYSTEM_TIME_STAMP) == FLAG_BITS_SYSTEM_TIME_STAMP)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, systemTimeStamp.SerializeLengthAndBody(messageBuilder));

	if ((flag & FLAG_BITS_SYSTEM_LATENCY) == FLAG_BITS_SYSTEM_LATENCY)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, systemLatency.SerializeLengthAndBody(messageBuilder));

	if ((flag & FLAG_BITS_FEED_LATENCY) == FLAG_BITS_FEED_LATENCY)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, feedLatency.SerializeLengthAndBody(messageBuilder));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FeedStatusMessage::Serialize(MessageBuilder &messageBuilder) const
{
	return Serialize(messageBuilder, m_systemName, m_cgName, m_feedName, m_systemTimeStamp, m_systemLatency, m_feedLatency);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FeedStatusMessage::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&m_cgName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&m_feedName));

	uint8_t flag;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint8_t), MessageHandler::ENDIAN_LITTLE, &flag));

	if ((flag & FLAG_BITS_SYSTEM_TIME_STAMP) == FLAG_BITS_SYSTEM_TIME_STAMP)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_systemTimeStamp.DeserializeLengthAndBody(messageValidator));
	else
		m_systemTimeStamp.Reset();

	if ((flag & FLAG_BITS_SYSTEM_LATENCY) == FLAG_BITS_SYSTEM_LATENCY)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_systemLatency.DeserializeLengthAndBody(messageValidator));
	else
		m_systemLatency.Reset();

	if ((flag & FLAG_BITS_FEED_LATENCY) == FLAG_BITS_FEED_LATENCY)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_feedLatency.DeserializeLengthAndBody(messageValidator));
	else
		m_feedLatency.Reset();

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_FEED_STATUS_MESSAGE_H)
