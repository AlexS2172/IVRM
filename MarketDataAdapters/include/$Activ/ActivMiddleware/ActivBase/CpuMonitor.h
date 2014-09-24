/**
 *  @file	CpuMonitor.h
 *
 *	@brief	Header file for the cpu monitor class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_CPU_MONITOR_H)
#define ACTIV_BASE_CPU_MONITOR_H

#include "ActivMiddleware/ActivBase/External.h"

#include <vector>

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
 *	@brief	Cpu monitor class.
 */
class CpuMonitor
{
public:
	typedef std::vector<uint8_t> CpuUsageList;					///< List of cpu usage percentages

	/**
	 *	@brief	Constructor.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_API CpuMonitor();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API ~CpuMonitor();

	/**
	 *	@brief	Build the current cpu usage list snapsot.
	 *
	 *	This should be called on a timer with a regular interval. GetCpuUsageList() can then be called
	 *	at will, and will return the state at the point CollateCpuUsageList() was last called.
	 */
	ACTIV_BASE_API void CollateCpuUsageList();

	/**
	 *	@brief	Get the list of current cpu percentages.
	 *
	 *	@return	List of cpu usages percentages.
	 */
	ACTIV_BASE_API const CpuUsageList &GetCpuUsageList();

private:
#if (defined ACTIV_PLATFORM_WINDOWS)

#if (!defined ACTIV_COMPILER_VISUAL_C)
// for now, no pdh for non-visualc builds
#else

	typedef std::vector<PDH_HCOUNTER> CpuCounterList;
	typedef PDH_STATUS (WINAPI PdhOpenQueryF)(LPCTSTR, DWORD_PTR, PDH_HQUERY *);
	typedef PDH_STATUS (WINAPI PdhAddCounterF)(PDH_HQUERY, LPCTSTR, DWORD_PTR dwUserData, PDH_HCOUNTER *);
    typedef PDH_STATUS (WINAPI PdhRemoveCounterF)(PDH_HCOUNTER);
	typedef PDH_STATUS (WINAPI PdhCloseQueryF)(PDH_HQUERY);
	typedef PDH_STATUS (WINAPI PdhCollectQueryDataF)(PDH_HQUERY);
	typedef PDH_STATUS (WINAPI PdhGetFormattedCounterValueF)(PDH_HCOUNTER, DWORD, LPDWORD, PPDH_FMT_COUNTERVALUE);

	HMODULE							m_pdhHandle;
	PdhOpenQueryF *					m_pPdhOpenQuery;
	PdhAddCounterF *				m_pPdhAddCounter;
	PdhRemoveCounterF *				m_pPdhRemoveCounter;
	PdhCloseQueryF *				m_pPdhCloseQuery;
	PdhCollectQueryDataF *			m_pPdhCollectQueryData;
	PdhGetFormattedCounterValueF *	m_pPdhGetFormattedCounterValue;

	PDH_HQUERY						m_cpuQuery;					///< Query for cpu counters
	CpuCounterList					m_cpuCounterList;			///< List of cpu counters

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
		uint64_t	m_lastIdleJiffies;							///< Jiffies are 100ths of a second
		uint64_t	m_lastTotalJiffies;
		byte_t		m_usagePercent;								///< Current usage percent of processor
	};
	
	/**
	 *	@brief	Get cpu stats.
	 */
	void GetStats();
	
	typedef std::vector<CpuInfo> CpuInfoList;
	static const uint32_t maxCpuNumber = 32;
	
	CpuInfoList		m_cpuInfoList;

#else
#error Write me
#endif

#if (defined ACTIV_PLATFORM_SOLARIS)

	void * const	m_pKStatControl;							///< Have to use void * as including kstat.h causes namespace problems

#endif // (defined ACTIV_PLATFORM_SOLARIS)

	CpuUsageList	m_cpuUsageList;								///< Resolved cpu usage percentages
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_CPU_MONITOR_H)
