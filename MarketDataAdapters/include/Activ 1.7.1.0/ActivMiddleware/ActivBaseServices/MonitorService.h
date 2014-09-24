/**
 *	@file	MonitorService.h
 *
 *	@brief	Header file for the monitor service class.
 *
 *	$Log: $
 */

#if !defined(ACTIV_MONITOR_SERVICE_H)
#define ACTIV_MONITOR_SERVICE_H

#include "ActivMiddleware/ActivBaseServices/External.h"
#include "ActivMiddleware/ActivBase/SystemMonitor.h"
#include "ActivMiddleware/ActivBase/IntervalTimer.h"
#include "ActivMiddleware/ActivBase/MessageBuilder.h"
#include "ActivMiddleware/Misc/IntrusivePtrHelper.h"

#include "boost/scoped_ptr.hpp"

#include <vector>

// Default pubsub heap size as set in default Settings object
#define ACTIV_MONITOR_SERVICE_DEFAULT_BUCKET_INFO	{ { 200, 1000 }, { 500, 1000 }, { 1000, 1000 } }

namespace Activ
{

/**
 *	@brief	Monitor service class.
 */
class MonitorService : private ManagedEndPoint, public Component
{
public:
	/**
	 *	@brief	Settings.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	System monitor modes.
		 */
		enum SystemMonitorMode
		{
			SYSTEM_MONITOR_MODE_OFF,							///< disable system monitor
			SYSTEM_MONITOR_MODE_ON,								///< enable system monitor
			SYSTEM_MONITOR_MODE_ON_WITH_LOG						///< enable system monitor & generate log file
		};

		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_BASE_SERVICES_API Settings();

		/**
		 *	@brief	Default destructor.
		 */
		ACTIV_BASE_SERVICES_API ~Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_BASE_SERVICES_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		IHeap::BucketInfoVector		m_heapBucketInfoVector;		///< size of heap to create for monitor service
		std::vector<std::string>	m_diskMonitorList;			///< list of disks to monitor (default is disk containing working directory)
		SystemMonitorMode			m_systemMonitorMode;		///< default SYSTEM_MONITOR_MODE_OFF
		bool						m_enableHeapStatsLog;		///< default false
		bool						m_enableEndPointStatsLog;	///< default false
		size_t						m_numberOfLogBackups;		///< previous days logs to keep, default 7
		byte_t						m_heapFreePublishPercent;	///< % free messages at which to publish heap stats, default 25
	};

	/**
	 *	@brief	Monitor service ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	monitorService the monitor service to have the ui.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_BASE_SERVICES_API UiComponent(MonitorService &monitorService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		MonitorService	&m_monitorService;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application the application the service will be linked to.
	 *	@param	serviceName name of the service.
	 *	@param	settings settings for the service.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_SERVICES_API MonitorService(Application &application, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_SERVICES_API ~MonitorService();

	/**
	 *	@brief	Process process startup message.
	 *
	 *	@param	message.
	 */
	ACTIV_BASE_SERVICES_API void ProcessSystemProcessStartupMessage(HeapMessage &message);

	/**
	 *	@brief	Process process shutdown message.
	 *
	 *	@param	message.
	 */
	ACTIV_BASE_SERVICES_API void ProcessSystemProcessShutdownMessage(HeapMessage &message);

	/**
	 *	@brief	Process endpoint shutdown message.
	 *
	 *	@param	message.
	 */
	ACTIV_BASE_SERVICES_API void ProcessSystemEndPointShutdownMessage(HeapMessage &message);

	/**
	 *	@brief	Get the number of processes being monitored.
	 *
	 *	@return	The number of processes.
	 */
	ACTIV_BASE_SERVICES_API size_t GetNumberOfProcesses() const;

protected:
	ACTIV_BASE_SERVICES_API virtual StatusCode OnResponse(HeapMessage &response);
	ACTIV_BASE_SERVICES_API virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

private:
	class ProcessMonitor;

	/**
	 *	@brief	Process async endpoint ping response.
	 *
	 *	@param	response.
	 */
	void ProcessEndPointPingResponse(HeapMessage &response);

	/**
	 *	@brief	Process ping timer.
	 */
	void ProcessPingTimer();

	/**
	 *	@brief	Process lazy timer.
	 */
	void ProcessLazyTimer();

	/**
	 *	@brief	Process overnight reset timer.
	 */
	void ProcessResetTimer();

	/**
	 *	@brief	Process a process die.
	 *
	 *	@param	pProcess the process that has died.
	 */
	void ProcessProcessDie(IProcess * const pProcess);

	/**
	 *	@brief	Publish endpoint die messages.
	 *
	 *	@param	processId id of process that has died.
	 *	@param	type message type to publish.
	 */
	void PublishEndPointShutdownMessage(const ProcessId processId, const MessageType type);

	/**
	 *	@brief	Publish endpoint stats.
	 *
	 *	@param	isEndOfDay overnight reset?
	 */
	void PublishEndPointStats(const bool isEndOfDay);

	/**
	 *	@brief	Publish heap stats.
	 *
	 *	@param	isEndOfDay overnight reset?
	 */
	void PublishHeapStats(const bool isEndOfDay);

	/**
	 *	@brief	Publish process stats.
	 *
	 *	@param	systemMonitorStats stats for the last time slice.
	 */
	void PublishProcessStats(const SystemMonitor::Stats &systemMonitorStats);

	/**
	 *	@brief	Check whether heap stats should be published.
	 *
	 *	@param	stats reference to heap stats.
	 *
	 *	@return	true if there are fails or were and a reset has happened.
	 */
	bool ShouldPublishHeapStats(const IHeap::Stats &stats);
	
	/**
	 *	@brief	Log stats from SystemMonitor.
	 *
	 *	@param	systemMonitorStats stats for the last time slice.
	 */
	void LogSystemMonitorStats(const SystemMonitor::Stats &systemMonitorStats);

	/**
	 *	@brief	Log heap stats.
	 *
	 *	@param	stats stats for a heap.
	 */
	void LogHeapStats(const IHeap::Stats &stats);

	/**
	 *	@brief	Log endpoint stats.
	 *
	 *	@param	stats stats for an endpoint.
	 *	@param	pingTime.
	 */
	void LogEndPointStats(const ISyncEndPoint::Stats &stats, const double pingTime);

	/**
	 *	@brief	Watch stats from SystemMonitor class.
	 *
	 *	@param	uiIo.
	 */
	void UiWatchSystemMonitorStats(const UiIo &uiIo);

	/**
	 *	@brief	Watch cpu usage for a process.
	 *
	 *	@param	uiIo.
	 */
	void UiWatchProcessCpuUsage(const UiIo &uiIo);

	/**
	 *	@brief	Display disk usage.
	 *
	 *	@param	uiIo.
	 */
	void UiDisplayDiskUsage(const UiIo &uiIo);

	class OldHeapStats
	{
	public:
		OldHeapStats() :
			m_totalFails(0),
			m_lowPercent(100)
		{
		}

		size_t	m_totalFails;
		byte_t	m_lowPercent;
	};

	class EndPointPing
	{
	public:
		EndPointPing() :
			m_isOutstanding(false),
			m_lastPingTime(0.0)
		{
		}

		bool					m_isOutstanding;
		HeapMessage				m_heapMessage;
		uint64_t				m_startTimeStamp;
		double					m_lastPingTime;

		ACTIV_INTRUSIVE_PTR_HELPER_ADD_REFERENCE_COUNTER;
	};

	typedef boost::intrusive_ptr<EndPointPing> EndPointPingPtr;
	
	IntervalTimer						m_intervalTimer;
	Address								m_lazyTimerAddress;
	Address								m_pingTimerAddress;
	Address								m_resetTimerAddress;
	Address								m_systemMonitorLogAddress;
	Address								m_heapStatsLogAddress;
	Address								m_endPointStatsLogAddress;
	boost::scoped_ptr<SystemMonitor>	m_pSystemMonitor;
	boost::scoped_ptr<ProcessMonitor>	m_pProcessMonitor;
	const byte_t						m_heapFreePublishPercent;

	LocalTime							m_lazyTimerTime;
	std::vector<OldHeapStats>			m_lastHeapStatsList;
	std::vector<EndPointPingPtr>		m_endPointPingList;

	MessageBuilder						m_messageBuilder;
	byte_t								m_txBuffer[4096];
};

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_SERVICE_H)
