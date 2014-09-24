/**
 *  @file	ActivMonitor/HeapStatsMessage.h
 *
 *	@brief	Header file for monitor heap stats message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_HEAP_STATS_MESSAGE_H)
#define ACTIV_MONITOR_HEAP_STATS_MESSAGE_H

#include "ActivMiddleware/ActivMonitor/ActivMonitor.h"
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
	static const MessageType MESSAGE_TYPE_EOD = MONITOR_MESSAGE_HEAP_STATS_EOD;

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
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

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

inline StatusCode HeapStatsMessage::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_heapStats.Deserialize(messageValidator));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_HEAP_STATS_MESSAGE_H)
