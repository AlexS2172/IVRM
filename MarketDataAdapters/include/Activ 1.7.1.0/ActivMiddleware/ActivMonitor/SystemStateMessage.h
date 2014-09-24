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
	 *	@param	diskInfoList.
	 *	@param	freeMemoryPercent.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const SystemMonitor::Stats::CpuUsageList &cpuUsageList, const SystemMonitor::Stats::DiskInfoList &diskInfoList, const uint8_t freeMemoryPercent);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	SystemMonitor::Stats::CpuUsageList	m_cpuUsageList;
	SystemMonitor::Stats::DiskInfoList	m_diskInfoList;
	std::string							m_middlewareVersion;
	uint8_t								m_freeMemoryPercent;

private:
	typedef SerializableObjectContainerHelper<SystemMonitor::Stats::CpuUsageList, uint8_t> CpuUsageListSerializer;
	typedef SerializableObjectContainerHelper<SystemMonitor::Stats::DiskInfoList, uint8_t> DiskInfoListSerializer;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SystemStateMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const SystemMonitor::Stats::CpuUsageList &cpuUsageList, const SystemMonitor::Stats::DiskInfoList &diskInfoList, const uint8_t freeMemoryPercent)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CpuUsageListSerializer::Serialize(messageBuilder, cpuUsageList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DiskInfoListSerializer::Serialize(messageBuilder, diskInfoList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(std::string(ACTIV_MIDDLEWARE_VERSION_STRING)));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(freeMemoryPercent, MessageHandler::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SystemStateMessage::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CpuUsageListSerializer::Deserialize(messageValidator, m_cpuUsageList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DiskInfoListSerializer::Deserialize(messageValidator, m_diskInfoList));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&m_middlewareVersion));

	if (messageValidator.GetMessageVersion() >= 4)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint8_t), MessageHandler::ENDIAN_LITTLE, &m_freeMemoryPercent));
	else
		m_freeMemoryPercent = 100;

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Specialization of serialize helper for SystemMonitor::Stats::DiskInfo (as it doesn't provide a Serialize() function).
 */
inline StatusCode SerializeObject(MessageBuilder &messageBuilder, const SystemMonitor::Stats::DiskInfo &diskInfo)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(diskInfo.m_diskName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(diskInfo.m_usagePercentage, MessageHandler::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Specialization of deserialize helper for DiskMonitor::Stats (as it doesn't provide a Deserialize() function).
 */
inline StatusCode DeserializeObject(MessageValidator &messageValidator, SystemMonitor::Stats::DiskInfo &diskInfo)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&diskInfo.m_diskName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(uint8_t), MessageHandler::ENDIAN_LITTLE, &diskInfo.m_usagePercentage));

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_SYSTEM_STATE_MESSAGE_H)
