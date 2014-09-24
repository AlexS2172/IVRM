/**
 *  @file	DiskMonitor.h
 *
 *	@brief	Header file for the disk monitor class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_DISK_MONITOR_H)
#define ACTIV_BASE_DISK_MONITOR_H

#include "ActivMiddleware/ActivBase/External.h"

#include <vector>
#include <string>

namespace Activ
{

/**
 *	@brief	Disk monitor class.
 */
class DiskMonitor
{
public:
	/**
	 *	@brief	Stats.
	 */
	class Stats
	{
	public:
		std::string	m_diskName;									///< Name of disk
		uint8_t		m_usagePercentage;							///< Usage % of disk
	};

	typedef std::vector<Stats> StatsList;

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	diskList list of disks to monitor. If empty, monitor the disk corresponding to the current working directory.
	 */
	ACTIV_BASE_API DiskMonitor(const std::vector<std::string> &diskList);

	/**
	 *	@brief	Get stats for monitored disks.
	 *
	 *	@return	List of stats.
	 */
	ACTIV_BASE_API const StatsList &GetDiskStatsList();

private:
	/**
	 *	@brief	Get stats for single disk.
	 *
	 *	@param	diskName name of disk.
	 */
	void GetDiskStats(const std::string &diskName);

	std::vector<std::string>	m_diskList;
	StatsList					m_statsList;
};

} // namespace Activ

#endif // defined (ACTIV_BASE_DISK_MONITOR_H)
