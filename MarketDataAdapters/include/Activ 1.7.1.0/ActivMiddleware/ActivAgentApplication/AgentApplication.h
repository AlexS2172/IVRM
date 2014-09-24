/**
 *	@file	AgentApplication.h
 *
 *	@brief	Header file for agent application class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_AGENT_APPLICATION_H)
#define ACTIV_AGENT_APPLICATION_H

#include "ActivMiddleware/ActivAgentApplication/External.h"
#include "ActivMiddleware/ActivBaseServices/TimerService.h"
#include "ActivMiddleware/ActivBaseServices/LoggerService.h"
#include "ActivMiddleware/ActivBaseServices/PubSubService.h"
#include "ActivMiddleware/ActivBaseServices/MonitorService.h"
#include "ActivMiddleware/ActivBaseServices/DirectoryService.h"
#include "ActivMiddleware/ActivBaseServices/UiService.h"
#include "ActivMiddleware/ActivBaseServices/LicenseService.h"
#include "ActivMiddleware/ActivRouter/RouterService.h"

#include "boost/scoped_ptr.hpp"

/**
 *	@brief	Default system heap size.
 */
#define ACTIV_DEFAULT_SYSTEM_HEAP_BUCKET_INFO		{ { 100, 1000 }, { 500, 1000 } }

namespace Activ
{

/**
 *	@brief	Agent application class.
 *
 *	The AgentApplication class should be used when creating a shared memory system or stand alone process.
 *	It provides always on timer, logger, directory, router and pub-sub services.
 */
class AgentApplication : public Application
{
public:
	/**
	 *	@brief	Agent settings.
	 */
	class Settings : public Application::Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_AGENT_APPLICATION_API Settings();

		/**
		 *	@brief	Default destructor.
		 */
		ACTIV_AGENT_APPLICATION_API ~Settings();

		/**
		 *	@brief	Read settings from an ini file.
		 *
		 *	@param	iniFileParser already parsed ini file to get settings from.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_AGENT_APPLICATION_API StatusCode Read(const IniFileParser &iniFileParser);

		IHeap::BucketInfoVector		m_systemHeapBucketInfo;		///< size of the system heap
		std::string					m_serviceLocationIniFile;	///< service location ini file
		std::string					m_processStartupIniFile;	///< ini file for processes to launch
		bool						m_createSystemLog;			///< whether to create system log
		LoggerDefinitions::FileType	m_systemLogType;			///< type of system log (rolling, daily etc)
		uint64_t					m_systemLogMaxSize;			///< max size if rolling log
		size_t						m_systemLogNumberOfBackups;	///< number of backups to keep for rolling log
		bool						m_systemLogUniqueName;		///< unique name for log
		bool						m_enableUiService;			///< enable ui service (default == true)
		bool						m_extendedUiShutdown;		///< extended ui shutdown validation (default == false)

		// base service settings
		TimerService::Settings		m_timerServiceSettings;		///< settings for timer service
		LoggerService::Settings		m_loggerServiceSettings;	///< settings for logger service
		RouterService::Settings		m_routerServiceSettings;	///< settings for router service
		PubSubService::Settings		m_pubSubServiceSettings;	///< pubsub service settings
		MonitorService::Settings	m_monitorServiceSettings;	///< monitor service settings
		DirectoryService::Settings	m_directoryServiceSettings;	///< settings for directory service
		UiService::Settings			m_uiServiceSettings;		///< settings for ui service
		LicenseService::Settings	m_licenseServiceSettings;	///< settings for license service
	};

	/**
	 *	@brief	Create a system.
	 *
	 *	@param	settings agent settings.
	 *	@param	componentId the component id.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_AGENT_APPLICATION_API AgentApplication(const Settings &settings = Settings(), const std::string &componentId = defaultComponentId);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_AGENT_APPLICATION_API virtual ~AgentApplication();

	/**
	 *	@brief	Initiate a system restart.
	 *
	 *	@return	STATUS_CODE_SUCCESS or error status if unable to initiate restart.
	 */
	ACTIV_AGENT_APPLICATION_API StatusCode RestartSystem();

	/**
	 *	@brief	Is the system restartable?
	 *
	 *	@return	true if system can be restarted.
	 */
	ACTIV_AGENT_APPLICATION_API bool IsSystemRestartable();

	class Private;

protected:
	ACTIV_AGENT_APPLICATION_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_AGENT_APPLICATION_API virtual void OnCtrlEvent(const CtrlHandler::EventType eventType);
	ACTIV_AGENT_APPLICATION_API virtual void OnShutdown();
	ACTIV_AGENT_APPLICATION_API virtual bool CanThreadExit();

private:
	class UiFunctionality;

	boost::scoped_ptr<Private>	m_pPrivate;						///< Hidden implementation details
};

} // namespace Activ

#endif // !defined (ACTIV_AGENT_APPLICATION_H)
