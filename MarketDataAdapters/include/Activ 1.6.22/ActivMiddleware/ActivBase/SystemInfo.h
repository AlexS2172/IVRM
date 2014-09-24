/**
 *	@file	ActivBase/SystemInfo.h
 *
 *	@brief	Header file for the system info class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_SYSTEM_INFO_H)
#define ACTIV_BASE_SYSTEM_INFO_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>
#include <vector>

namespace Activ
{

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
	
	typedef std::vector<std::string> ProcessorInfoList;
	
	static const byte_t	currentVersion = 1;						///< current serialized version

	// Machine info
	size_t				m_numberOfLogicalProcessors;			///< number of logical processors in the machine (e.g. HT enabled), always > 0
	size_t				m_numberOfPhysicalProcessors;			///< number of actual processors in the machine, 0 if unknown
	ProcessorInfoList	m_processorInfoList;					///< list of processor info
	uint64_t			m_physicalMemory;						///< amount of physical RAM in the machine
	uint64_t			m_freeMemory;							///< amount of free RAM in the machine
	std::string			m_osName;								///< printable os name string
	std::string			m_schedulerInfo;						///< printable info about scheduler
	std::string			m_computerName;							///< name of this computer
	
	// ActivMiddleware info
	std::string			m_middlewareVersion;					///< ActivMiddleware version info
	std::string			m_middlewareDate;						///< date/time of compilation of ActivMiddleware
	std::string			m_boostVersion;							///< Boost version info
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_SYSTEM_INFO_H)
