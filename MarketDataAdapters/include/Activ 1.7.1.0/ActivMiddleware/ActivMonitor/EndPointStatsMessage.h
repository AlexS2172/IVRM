/**
 *  @file	ActivMonitor/EndPointStatsMessage.h
 *
 *	@brief	Header file for monitor endpoint stats message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_ENDPOINT_STATS_MESSAGE_H)
#define ACTIV_MONITOR_ENDPOINT_STATS_MESSAGE_H

#include "ActivMiddleware/ActivMonitor/ActivMonitor.h"
#include "ActivMiddleware/ActivSystem/IEndPoint.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	EndPoint stats message.
 */
class EndPointStatsMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_ENDPOINT_STATS;
	static const MessageType MESSAGE_TYPE_EOD = MONITOR_MESSAGE_ENDPOINT_STATS_EOD;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	endPointStats ISyncEndPoint::Stats reference.
	 *	@param	pingTime in seconds.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const ISyncEndPoint::Stats &endPointStats, const double pingTime);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	ISyncEndPoint::Stats	m_endPointStats;
	double					m_pingTime;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode EndPointStatsMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const ISyncEndPoint::Stats &endPointStats, const double pingTime)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, endPointStats.Serialize(messageBuilder));

	if (CURRENT_VERSION >= 3)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<uint32_t>(pingTime * 1000.0), MessageBuilder::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode EndPointStatsMessage::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_endPointStats.Deserialize(messageValidator));

	if (messageValidator.GetMessageVersion() >= 3)
	{
		uint32_t pingTime;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(pingTime), MessageValidator::ENDIAN_LITTLE, &pingTime));

		m_pingTime = static_cast<double>(pingTime) / 1000.0;
	}
	else
	{
		m_pingTime = 0.0;
	}

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_ENDPOINT_STATS_MESSAGE_H)
