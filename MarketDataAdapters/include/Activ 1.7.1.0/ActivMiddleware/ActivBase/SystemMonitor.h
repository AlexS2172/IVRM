/**
 *  @file	SystemMonitor.h
 *
 *	@brief	Header file for the system monitor class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_SYSTEM_MONITOR_H)
#define ACTIV_BASE_SYSTEM_MONITOR_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/Process.h"

#include <vector>
#include <map>

#if (defined ACTIV_PLATFORM_WINDOWS)
#if (defined ACTIV_COMPILER_VISUAL_C)
#include <pdh.h>
#endif
#elif (!defined ACTIV_PLATFORM_POSIX)
#error Write me
#endif

namespace Activ
{

/**
 *	@brief	System monitor class.
 *
 *	Monitors cpu usage, memory, disk usage, context switching and paging.
 *
 *	Note this class is not thread aware - it is not safe to call the methods from different threads
 *	without some user-provided locking.
 */
class SystemMonitor
{
public:
	/**
	 *	@brief	Collected stats.
	 */
	class Stats
	{
	public:
		/**
		 *	@brief	Per-disk info.
		 */
		class DiskInfo
		{
		public:
			std::string	m_diskName;								///< name of disk
			uint8_t		m_usagePercentage;						///< usage % of disk
		};

		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_BASE_API Stats();

		typedef std::vector<uint8_t> CpuUsageList;
		typedef std::map<Process::Id, uint8_t> ProcessCpuUsageMap;
		typedef std::vector<DiskInfo> DiskInfoList;

		CpuUsageList		m_cpuUsageList;						///< list of total cpu usage percentages per cpu
		uint8_t				m_averageCpuUsage;					///< average cpu usage across all cpus
		ProcessCpuUsageMap	m_processCpuUsageMap;				///< map of cpu usage per process, see SystemMonitor::AddProcess()
		DiskInfoList		m_diskInfoList;						///< list of disk usages

		uint32_t			m_contextSwitchRate;				///< context switches / sec
		uint32_t			m_pagesInOutRate;					///< pages in/out / sec

		uint8_t				m_freeMemoryPercent;				///< % of memory free
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	diskNameList list of disks to watch. If empty, watch the disk containing the current working directory.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_API SystemMonitor(const std::vector<std::string> &diskNameList = std::vector<std::string>());

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API ~SystemMonitor();

	/**
	 *	@brief	Add an individual process to the watch.
	 *
	 *	@param	processId os-pid of the process to monitor.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error code.
	 */
	ACTIV_BASE_API StatusCode AddProcess(const Process::Id processId);

	/**
	 *	@brief	Remove an individual process from the watch.
	 *
	 *	@param	processId os-pid of the process to monitor.
	 */
	ACTIV_BASE_API void RemoveProcess(const Process::Id processId);

	/**
	 *	@brief	Collate stats.
	 *
	 *	This should be called on a timer with a regular interval. GetStats() can then be called
	 *	at will, and will return the state at the point CollateStats() was last called.
	 */
	ACTIV_BASE_API void CollateStats();

	/**
	 *	@brief	Get the stats at the point CollateStats() was last called.
	 *
	 *	@return	Stats reference.
	 */
	ACTIV_BASE_API const Stats &GetStats();

private:
	/**
	 *	@brief	Platform specific collate stats.
	 */
	void PlatformCollateStats();

	/**
	 *	@brief	Get stats for single disk.
	 *
	 *	@param	diskName name of disk.
	 */
	void GetDiskStats(const std::string &diskName);

	std::vector<std::string>		m_diskNameList;

#if (defined ACTIV_PLATFORM_WINDOWS)

#if (!defined ACTIV_COMPILER_VISUAL_C)
	// for now, no pdh for non-visualc builds
#else

	typedef std::vector<PDH_HCOUNTER> TotalCpuCounterList;
	typedef std::map<Process::Id, PDH_HCOUNTER> ProcessCpuCounterMap;
	typedef PDH_STATUS (WINAPI PdhOpenQueryF)(LPCTSTR, DWORD_PTR, PDH_HQUERY *);
	typedef PDH_STATUS (WINAPI PdhAddCounterF)(PDH_HQUERY, LPCTSTR, DWORD_PTR dwUserData, PDH_HCOUNTER *);
    typedef PDH_STATUS (WINAPI PdhRemoveCounterF)(PDH_HCOUNTER);
	typedef PDH_STATUS (WINAPI PdhCloseQueryF)(PDH_HQUERY);
	typedef PDH_STATUS (WINAPI PdhCollectQueryDataF)(PDH_HQUERY);
	typedef PDH_STATUS (WINAPI PdhGetFormattedCounterValueF)(PDH_HCOUNTER, DWORD, LPDWORD, PPDH_FMT_COUNTERVALUE);
	typedef DWORD (WINAPI GetProcessImageFileNameF)(HANDLE, LPTSTR, DWORD);

	HMODULE							m_pdhHandle;
	PdhOpenQueryF *					m_pPdhOpenQuery;
	PdhAddCounterF *				m_pPdhAddCounter;
	PdhRemoveCounterF *				m_pPdhRemoveCounter;
	PdhCloseQueryF *				m_pPdhCloseQuery;
	PdhCollectQueryDataF *			m_pPdhCollectQueryData;
	PdhGetFormattedCounterValueF *	m_pPdhGetFormattedCounterValue;

	HMODULE							m_psApiHandle;
	GetProcessImageFileNameF *		m_pGetProcessImageFileName;

	PDH_HQUERY						m_cpuQuery;					///< query for cpu counters
	TotalCpuCounterList				m_totalCpuCounterList;		///< list of total cpu usage counters
	ProcessCpuCounterMap			m_processCpuCounterMap;		///< per-process cpu usage counters
	PDH_HCOUNTER					m_contextSwitchHandle;		///< counter for context switches
	PDH_HCOUNTER					m_pagesInOutHandle;			///< counter for pages/sec

#endif // (!defined ACTIV_COMPILER_VISUAL_C)

#elif (defined ACTIV_PLATFORM_POSIX)

	/**
	 *	@brief	Cpu usage info.
	 */
	class CpuInfo
	{
	public:
		/**
		 *	@brief	Constructor.
		 */
		CpuInfo();
		
		/**
		 *	@brief	Update.
		 *
		 *	@param	idleJiffies current idle jiffies for this processor.
		 *	@param	totalJiffies current total jiffies for this processor.
		 */
		void Update(const uint64_t idleJiffies, const uint64_t totalJiffies);
		
		bool		m_isInitialized;
		uint64_t	m_lastIdleJiffies;							///< jiffies are 100ths of a second
		uint64_t	m_lastTotalJiffies;
		uint64_t	m_lastTotalDiffJiffies;
		byte_t		m_usagePercent;								///< current usage percent of processor
	};

	/**
	 *	@brief	Initialize disk watches.
	 */
	void InitializeDiskNameList();
			
	/**
	 *	@brief	Posix platform specific stats collation.
	 */
	void PosixPlatformCollateStats();
	
	typedef std::vector<CpuInfo> CpuInfoList;					///< jiffy info per cpu
	typedef std::pair<uint64_t, uint64_t> KernelInfoHistory;	///< (previous, current) value from the kernel
	typedef std::map<Process::Id, KernelInfoHistory> ProcessCpuUsageMap;
	
	static const uint32_t maxCpuNumber = 32;
	
	uint32_t			m_lastTickCount;						///< time of last CollateStats()
	CpuInfoList			m_cpuInfoList;
	ProcessCpuUsageMap	m_processCpuUsageMap;
	KernelInfoHistory	m_contextSwitchCounts;					///< (previous, current) context switch counts
	KernelInfoHistory	m_pagingCounts;							///< (previous, current) pages in+out counts

#else
#error Write me
#endif

#if (defined ACTIV_PLATFORM_SOLARIS)

	void * const	m_pKStatControl;							///< have to use void * as including kstat.h causes namespace problems

#endif // (defined ACTIV_PLATFORM_SOLARIS)

	Stats	m_stats;											///< stats generated from last CollateStats() call
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_SYSTEM_MONITOR_H)
