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
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const ISyncEndPoint::Stats &endPointStats);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	ISyncEndPoint::Stats	m_endPointStats;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode EndPointStatsMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const ISyncEndPoint::Stats &endPointStats)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, endPointStats.Serialize(messageBuilder));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode EndPointStatsMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_endPointStats.Deserialize(messageValidater));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_ENDPOINT_STATS_MESSAGE_H)
