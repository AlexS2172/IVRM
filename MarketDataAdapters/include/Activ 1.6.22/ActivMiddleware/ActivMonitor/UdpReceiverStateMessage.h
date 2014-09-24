/**
 *  @file	ActivMonitor/UdpReceiverStateMessage.h
 *
 *	@brief	Header file for monitor udp receiver state message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_UDP_RECEIVER_STATE_MESSAGE_H)
#define ACTIV_MONITOR_UDP_RECEIVER_STATE_MESSAGE_H

#include "ActivMiddleware/ActivMonitor/ActivMonitor.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	Udp receiver state message.
 */
class UdpReceiverStateMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_UDP_RECEIVER_STATE;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	feedName.
	 *	@param	multicastGroup.
	 *	@param	port.
	 *	@param	lastRxHeartbeatTime.
	 *	@param	isLastRxHeartbeatTimeOverTimeout.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const std::string &feedName, const std::string &multicastGroup, const uint16_t port, const Time &lastRxHeartbeatTime, const bool isLastRxHeartbeatTimeOverTimeout);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	std::string	m_feedName;
	std::string	m_multicastGroup;
	uint16_t	m_port;
	Time		m_lastRxHeartbeatTime;
	bool		m_isLastRxHeartbeatTimeOverTimeout;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UdpReceiverStateMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const std::string &feedName, const std::string &multicastGroup, const uint16_t port, const Time &lastRxHeartbeatTime, const bool isLastRxHeartbeatTimeOverTimeout)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(feedName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(multicastGroup));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(port, MessageHandler::ENDIAN_LITTLE));

	if (!lastRxHeartbeatTime.IsInitialized())
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<uint8_t>(0), MessageHandler::ENDIAN_LITTLE));
	}
	else
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<uint8_t>(1), MessageHandler::ENDIAN_LITTLE));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, lastRxHeartbeatTime.SerializeLengthAndBody(messageBuilder));
	}

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<uint8_t>(isLastRxHeartbeatTimeOverTimeout), MessageHandler::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UdpReceiverStateMessage::Deserialize(MessageValidater &messageValidater)
{
	uint8_t temp8;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_feedName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_multicastGroup));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(uint16_t), MessageHandler::ENDIAN_LITTLE, &m_port));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(temp8), MessageHandler::ENDIAN_LITTLE, &temp8));

	if (0 != temp8)
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_lastRxHeartbeatTime.DeserializeLengthAndBody(messageValidater));
	}

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(temp8), MessageHandler::ENDIAN_LITTLE, &temp8));
	m_isLastRxHeartbeatTimeOverTimeout = (temp8 != 0);

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_UDP_RECEIVER_STATE_MESSAGE_H)
