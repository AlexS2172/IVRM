/**
 *	@file	ActivBase/SystemInfo.h
 *
 *	@brief	Header file for the system info class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_SYSTEM_INFO_H)
#define ACTIV_BASE_SYSTEM_INFO_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>
#include <vector>

namespace Activ
{

class MessageBuilder;
class MessageValidator;

/**
 *	@brief	System information.
 */
class SystemInfo
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	Sets no members.
	 */
	ACTIV_BASE_API SystemInfo();

	/**
	 *	@brief	Populate all members.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_API void Populate();

	/**
	 *	@brief	Serialize this SystemInfo to the latest version.
	 *
	 *	@param	messageBuilder to serialize into.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error code.
	 */
	ACTIV_BASE_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Serialize this SystemInfo to the version of the SystemInfo parameter.
	 *
	 *	@param	messageBuilder to serialize into.
	 *	@param	systemInfo containing the version to serialize to.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error code.
	 */
	ACTIV_BASE_API StatusCode Serialize(MessageBuilder &messageBuilder, const SystemInfo &systemInfo) const;

	/**
	 *	@brief	Deserialize system info.
	 *
	 *	@param	messageValidator to deserialize from.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error code.
	*/
	ACTIV_BASE_API StatusCode Deserialize(MessageValidator &messageValidator);
	
	typedef std::vector<std::string> ProcessorInfoList;
	
	// Machine info
	size_t				m_numberOfProcessorThreads;				///< number of logical processors in the machine (e.g. HT enabled)
	size_t				m_numberOfProcessorCores;				///< number of processor cores across all packages in the machine
	size_t				m_numberOfProcessorPackages;			///< number of actual physical processors in the machine
	ProcessorInfoList	m_processorInfoList;					///< list of processor info

	uint64_t			m_physicalMemory;						///< amount of physical RAM in the machine
	uint64_t			m_freeMemory;							///< amount of free RAM in the machine
	std::string			m_osName;								///< printable os name string
	std::string			m_schedulerInfo;						///< printable info about scheduler
	std::string			m_computerName;							///< name of this computer

	// Windows specific (not ifdef as non-Windows platform may receive SystemInfo from a Windows machine), if m_platformType == PLATFORM_TYPE_WINDOWS
	uint32_t			m_windowsDpcProcessorAffinityMask;		///< processor affinity for deferred procedure calls

	// ActivMiddleware info
	PlatformType		m_platformType;							///< ActivMiddleware platform
	uint32_t			m_middlewareVersionValue;				///< numeric ActivMiddleware version
	std::string			m_middlewareVersion;					///< ActivMiddleware version string
	std::string			m_middlewareDate;						///< date/time of compilation of ActivMiddleware
	std::string			m_boostVersion;							///< Boost version info

private:
	/**
	 *	@brief	Serialize this SystemInfo to a specific version.
	 *
	 *	@param	messageBuilder to serialize into.
	 *	@param	version containing the version to serialize to.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error code.
	 */
	StatusCode Serialize(MessageBuilder &messageBuilder, const byte_t version) const;

	static const byte_t	currentVersion = 2;						///< current serialized version

	byte_t				m_version;
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_SYSTEM_INFO_H)
