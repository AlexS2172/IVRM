/**
 *  @file	ActivMonitor/SystemStateMessage.h
 *
 *	@brief	Header file for monitor system state message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_SYSTEM_STATE_MESSAGE_H)
#define ACTIV_MONITOR_SYSTEM_STATE_MESSAGE_H

#include "ActivMiddleware/ActivMonitor/ActivMonitor.h"
#include "ActivMiddleware/ActivBase/SystemMonitor.h"
#include "ActivMiddleware/ActivBase/DiskMonitor.h"
#include "ActivMiddleware/Misc/SerializableContainerHelper.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	System state message.
 */
class SystemStateMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_SYSTEM_STATE;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	cpuUsageList.
	 *	@param	diskStatsList.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const SystemMonitor::Stats::CpuUsageList &cpuUsageList, const DiskMonitor::StatsList &diskStatsList);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	SystemMonitor::Stats::CpuUsageList	m_cpuUsageList;
	DiskMonitor::StatsList				m_diskStatsList;
	std::string							m_middlewareVersion;

private:
	typedef SerializableObjectContainerHelper<SystemMonitor::Stats::CpuUsageList, uint8_t> CpuUsageListSerializer;
	typedef SerializableObjectContainerHelper<DiskMonitor::StatsList, uint8_t> DiskStatsListSerializer;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SystemStateMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const SystemMonitor::Stats::CpuUsageList &cpuUsageList, const DiskMonitor::StatsList &diskStatsList)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CpuUsageListSerializer::Serialize(messageBuilder, cpuUsageList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DiskStatsListSerializer::Serialize(messageBuilder, diskStatsList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(std::string(ACTIV_MIDDLEWARE_VERSION)));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SystemStateMessage::Deserialize(MessageValidater &messageValidater)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidater));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CpuUsageListSerializer::Deserialize(messageValidater, m_cpuUsageList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DiskStatsListSerializer::Deserialize(messageValidater, m_diskStatsList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&m_middlewareVersion));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Specialization of serialize helper for DiskMonitor::Stats (as it doesn't provide a Serialize() function).
 */
inline StatusCode SerializeObject(MessageBuilder &messageBuilder, const DiskMonitor::Stats &stats)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(stats.m_diskName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(stats.m_usagePercentage, MessageHandler::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Specialization of deserialize helper for DiskMonitor::Stats (as it doesn't provide a Deserialize() function).
 */
inline StatusCode DeserializeObject(MessageValidater &messageValidater, DiskMonitor::Stats &stats)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateString(&stats.m_diskName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(stats.m_usagePercentage), MessageHandler::ENDIAN_LITTLE, &stats.m_usagePercentage));

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_SYSTEM_STATE_MESSAGE_H)
