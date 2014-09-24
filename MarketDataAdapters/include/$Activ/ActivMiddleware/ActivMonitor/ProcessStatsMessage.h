/**
 *  @file	ActivMonitor/ProcessStatsMessage.h
 *
 *	@brief	Header file for monitor process stats message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_PROCESS_STATS_MESSAGE_H)
#define ACTIV_MONITOR_PROCESS_STATS_MESSAGE_H

#include "ActivMonitor.h"

#include "ActivMiddleware/ActivSystem/IProcess.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	Process stats message.
 */
class ProcessStatsMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_PROCESS_STATS;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	processStats IProcess::Stats reference.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const IProcess::Stats &processStats);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	IProcess::Stats	m_processStats;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode ProcessStatsMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const IProcess::Stats &processStats)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, processStats.Serialize(messageBuilder));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode ProcessStatsMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_processStats.Deserialize(messageValidater));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_PROCESS_STATS_MESSAGE_H)
