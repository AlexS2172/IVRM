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
	 *	@brief	Constructor. Sets all the members.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_API SystemInfo();
	
	typedef std::vector<std::string> ProcessorInfoList;

	size_t				m_numberOfProcessors;					///< number of processors in the the machine
	ProcessorInfoList	m_processorInfoList;					///< list of processor info
	uint64_t			m_physicalMemory;						///< amount of physical RAM in the machine
	uint64_t			m_freeMemory;							///< amount of free RAM in the machine
	std::string			m_osName;								///< printable os name string
	std::string			m_computerName;							///< name of this computer
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_SYSTEM_INFO_H)
