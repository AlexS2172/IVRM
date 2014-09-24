/**
 *	@file	LoggerService.h
 *
 *	@brief	Header file for the logger service class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_LOGGER_SERVICE_H)
#define ACTIV_LOGGER_SERVICE_H

#include "ActivMiddleware/ActivBaseServices/External.h"
#include "ActivMiddleware/ActivBaseServices/LoggerDefinitions.h"
#include "ActivMiddleware/ActivBase/MessageBuilder.h"
#include "ActivMiddleware/ActivBase/File.h"

#include <deque>

// Default heap size
#define ACTIV_DEFAULT_LOGGER_SERVICE_BUCKET_INFO	{ { 100, 10000 }, { 500, 1000 }, { 1000, 500 } }

namespace Activ
{

class LogPort;

/**
 *	@brief	Logger service class.
 */
class LoggerService : private ManagedEndPoint, public Component
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

		std::string				m_defaultLogDirectory;			///< directory to put logs in if none specified, default current
		IHeap::BucketInfoVector	m_heapBucketInfoVector;			///< heap size for logger service, default above
		size_t					m_systemLogHistorySize;			///< number of system log entries to cache
	};

	/**
	 *	@brief	Logger service ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	loggerService the logger service to have the ui.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_BASE_SERVICES_API UiComponent(LoggerService &loggerService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		LoggerService	&m_loggerService;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pSystem the system the endpoint is running in.
	 *	@param	serviceName the name of the logger service.
	 *	@param	settings settings for the service.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_SERVICES_API LoggerService(Application &application, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_SERVICES_API ~LoggerService();

protected:
	ACTIV_BASE_SERVICES_API virtual StatusCode OnRequest(HeapMessage &request);
	ACTIV_BASE_SERVICES_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_BASE_SERVICES_API virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

private:
	static const size_t DATA_BUFFER_SIZE = 512;				///< Size of data buffer used for building/validating messages.

	/**
	 *	@brief	Make settings for this service.
	 *
	 *	@param	settings original settings.
	 *
	 *	@return	Updated settings.
	 */
	static Settings MakeSettings(const Settings &settings);

	/**
	 *	@brief	Make fully qualified path name for a log file.
	 *
	 *	@param	providedFileName as passed to the logger service.
	 *
	 *	@return	Fully qualified log file (applying the default log directory if necessary).
	 */
	std::string MakeFullyQualifiedLogFileName(const std::string &providedFileName) const;

	virtual bool CanThreadExit();

	/**
	 *	@brief	Process an open log file request.
	 */
	void ProcessOpenLogFileRequest(HeapMessage &request);

	/**
	 *	@brief	Process system log message.
	 *
	 *	@param	message.
	 *	@param	pSystemLog system log port, or 0 if none.
	 */
	void ProcessSystemLogMessage(HeapMessage &message, LogPort * const pSystemLog);

	/**
	 *	@brief	Process a close all logs opened by a port message.
	 */
	void ProcessCloseLogFileMessage(HeapMessage &message);

	/**
	 *	@brief	Process a endpoint set message.
	 */
	void ProcessSystemEndPointIdSetMessage(HeapMessage &message, const char * const type);

	/**
	 *	@brief	Add log port to service.
	 *
	 *	@param	pLogPort the port to add.
	 */
	void AddLogPort(LogPort * const pLogPort);

	/**
	 *	@brief	Remove log port from service.
	 *
	 *	@param	pLogPort the port to remove.
	 */
	void RemoveLogPort(LogPort * const pLogPort);

	/**
	 *	@brief	Display current logs.
	 */
	void UiDisplayLogs(const UiIo &uiIo);

	/**
	 *	@brief	Display system log cache.
	 *
	 *	@param	uiIo.
	 */
	void UiDisplaySystemLogCache(const UiIo &uiIo);

	typedef std::set<LogPort *> LogPortSet;
	typedef std::deque<std::string> SystemLogCache;

	const Settings			m_settings;							///< service settings

	LogPort *				m_pSystemLog;						///< one system log allowed per service
	SystemLogCache			m_systemLogCache;					///< last entries in system log
	Address					m_overnightResetTimerAddress;		///< address of overnight reset timer
	LogPortSet				m_logPortSet;						///< set of logs

	byte_t					m_logMessageBuffer[8096];			///< buffer for building published log messages
	MessageBuilder			m_logMessageBuilder;				///< builder for log messages

	friend class LogPort;
};

} // namespace Activ

#endif // !defined (ACTIV_LOGGER_SERVICE_H)
