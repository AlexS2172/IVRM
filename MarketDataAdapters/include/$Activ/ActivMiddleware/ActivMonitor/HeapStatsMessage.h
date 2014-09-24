/**
 *  @file	ActivMonitor/HeapStatsMessage.h
 *
 *	@brief	Header file for monitor heap stats message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_HEAP_STATS_MESSAGE_H)
#define ACTIV_MONITOR_HEAP_STATS_MESSAGE_H

#include "ActivMonitor.h"

#include "ActivMiddleware/ActivSystem/IHeap.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	Heap stats message.
 */
class HeapStatsMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_HEAP_STATS;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	heapStats IHeap::Stats reference.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const IHeap::Stats &heapStats);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	IHeap::Stats	m_heapStats;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HeapStatsMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const IHeap::Stats &heapStats)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, heapStats.Serialize(messageBuilder));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode HeapStatsMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_heapStats.Deserialize(messageValidater));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_HEAP_STATS_MESSAGE_H)
