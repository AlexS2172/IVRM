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
#include "ActivMiddleware/ActivBase/DiskMonitor.h"
#include "ActivMiddleware/ActivMessageHandler/MessageBuilder.h"

#include "boost/scoped_ptr.hpp"

// Default pubsub heap size as set in default Settings object
#define ACTIV_MONITOR_SERVICE_DEFAULT_BUCKET_INFO	{ { 200, 1000 }, { 500, 1000 }, { 1000, 1000 } }

namespace Activ
{

class CpuMonitor;

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
		 *	@brief	Default constructor.
		 */
		ACTIV_BASE_SERVICES_API Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_BASE_SERVICES_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		IHeap::BucketInfoVector		m_heapBucketInfoVector;
		std::vector<std::string>	m_diskMonitorList;
		bool						m_enableCpuMonitor;			///< Default false
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

private:
	ACTIV_DECLARE_NESTED_CLASS(ProcessMonitor);

	virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

	/**
	 *	@brief	Process lazy timer.
	 */
	void ProcessLazyTimer();

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
	 *	@brief	Publish heap stats.
	 */
	void PublishHeapStats();

	/**
	 *	@brief	Publish process stats.
	 */
	void PublishProcessStats();

	/**
	 *	@brief	Display cpu usage.
	 */
	void UiDisplayCpuUsage(const UiIo &uiIo);

	/**
	 *	@brief	Display disk usage.
	 */
	void UiDisplayDiskUsage(const UiIo &uiIo);

	Address								m_lazyTimerAddress;
	boost::scoped_ptr<CpuMonitor>		m_pCpuMonitor;
	DiskMonitor							m_diskMonitor;
	boost::scoped_ptr<ProcessMonitor>	m_pProcessMonitor;

	byte_t								m_txBuffer[4096];
	MessageBuilder						m_messageBuilder;
};

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_SERVICE_H)
