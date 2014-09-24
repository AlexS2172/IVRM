/**
 *	@file	UpdateService.h
 *
 *	@brief	Header file for the update service class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UPDATE_SERVICE_H)
#define ACTIV_UPDATE_SERVICE_H

#include "ActivMiddleware/ActivUpdate/External.h"
#include "ActivMiddleware/ActivUpdate/UpdateTarget.h"
#include "ActivMiddleware/ActivUpdate/UpdateDescription.h"
#include "ActivMiddleware/ActivUpdate/UpdateDownloader.h"

#include <map>

namespace Activ
{

/**
 *	@brief	Update service class.
 */
class UpdateService : public Component
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
		ACTIV_UPDATE_API Settings();

		/**
		 *	@brief	Copy constructor.
		 */
		ACTIV_UPDATE_API Settings(const Settings &settings);

		/**
		 *	@brief	Destructor.
		 */
		ACTIV_UPDATE_API ~Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_UPDATE_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		std::string										m_updateDirectory;				///< Directory to put updates in if none specified, default './Updates'.
		bool											m_enableAutomaticUpdates;		///< Enable automatic updates?
		std::string										m_updateCommand;				///< The update command.
		std::string										m_systemStartCommand;			///< The system start command.
		boost::scoped_ptr<UpdateDownloader::Settings>	m_pUpdateDownloaderSettings;	///< Pointer to the update downloader settings.
	};

	/**
	 *	@brief	Update service ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	updateService the update service to have the ui.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_UPDATE_API UiComponent(UpdateService &updateService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		UpdateService &	m_updateService;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application the application the service will attach to.
	 *	@param	managedEndPoint the managed end point the service will attach to.
	 *	@param	serviceName the name of the update service.
	 *	@param	settings settings for the service.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_UPDATE_API UpdateService(Application &application, ManagedEndPoint &managedEndPoint, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_UPDATE_API ~UpdateService();

	/**
	 *	@brief	Load update.
	 *
	 *	@param	updateDescriptionFileName update description file name.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_UPDATE_API StatusCode LoadUpdate(const std::string &updateDescriptionFileName);

	/**
	 *	@brief	Get settings.
	 *
	 *	@return	the object's settings.
	 */
	const Settings &GetSettings() const;

protected:
	// Component interface
	ACTIV_UPDATE_API StatusCode OnMessage(HeapMessage &message);
	ACTIV_UPDATE_API StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

private:
	/**
	 *	@brief	Process update available message.
	 *
	 *	@param	heapMessage the heap message.
	 */
	void ProcessUpdateAvailableMessage(HeapMessage &heapMessage);

	/**
	 *	@brief	On overnight reset timer.
	 */
	void OnOvernightResetTimer();

	/**
	 *	@brief	On update timer.
	 */
	void OnUpdateTimer();

	/**
	 *	@brief	Load updates from update directory.
	 */
	void LoadUpdatesFromUpdateDirectory();

	/**
	 *	@brief	Shutdown the system.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode ShutdownSystem();

	/**
	 *	@brief	Restart the system.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode RestartSystem();

	/**
	 *	@brief	Add process ids to run update command.
	 *
	 *	@param	comamnd the command.
	 */
	void AddProcessIdsToRunUpdateCommand(std::string &command);

	/**
	 *	@brief	Launch run update command.
	 *
	 *	@param	arguments the command arguments.
	 *	@param	shouldShutdown whether should shutdown the system after executing the command.
	 */
	StatusCode LaunchRunUpdateCommand(const std::string &arguments, const bool shouldShutdown);

	/**
	 *	@brief	Perform update.
	 *
	 *	@param	fileName file name of the update.
	 *	@param	updateDescription description of the update to perform.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode PerformUpdate(const std::string &fileName, const UpdateDescription &updateDescription);

	/**
	 *	@brief	Display statistics.
	 *
	 *	@param	uiIo ui i/o to use.
	 */
	void UiDisplayStatistics(const UiIo &uiIo);

	/**
	 *	@brief	Load update.
	 *
	 *	@param	uiIo ui i/o to use.
	 */
	void UiLoadUpdate(const UiIo &uiIo);

	/**
	 *	@brief	Perform update.
	 *
	 *	@param	uiIo ui i/o to use.
	 */
	void UiPerformUpdate(const UiIo &uiIo);

	/**
	 *	@brief	Remove update.
	 *
	 *	@param	uiIo ui i/o to use.
	 */
	void UiRemoveUpdate(const UiIo &uiIo);

	typedef std::map<std::string, UpdateDescription> UpdateDescriptionMap;	///< The update desctiption map.

	Settings							m_settings;						///< The object's settings.
	const std::string					m_systemName;					///< The system name.
	const std::string					m_platformName;					///< The platform name.
	const std::string					m_hostName;						///< The host name.
	Address								m_overnightResetTimerAddress;	///< The overnight reset timer address.	
	Address								m_updateTimerAddress;			///< The update timer adrdess.
	UpdateDescriptionMap				m_scheduledUpdateMap;			///< The sechedulaed update map.
	boost::scoped_ptr<UpdateDownloader>	m_pUpdateDownloader;			///< The update downloader.

	Port::Enabler						m_portEnabler;					///< The port enabler.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline const UpdateService::Settings &UpdateService::GetSettings() const
{
	return m_settings;
}

} // namespace Activ

#endif // !defined (ACTIV_UPDATE_SERVICE_H)
