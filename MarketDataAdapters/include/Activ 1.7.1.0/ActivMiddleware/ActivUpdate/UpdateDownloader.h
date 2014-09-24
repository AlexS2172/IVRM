/**
 *	@file	UpdateDownloader.h
 *
 *	@brief	Header file for the update downloader class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UPDATE_DOWNLOADER_H)
#define ACTIV_UPDATE_DOWNLOADER_H

#include "ActivMiddleware/ActivUpdate/External.h"
#include "ActivMiddleware/ActivUpdate/UpdateDescription.h"
#include "ActivMiddleware/ActivUpdate/UpdateTarget.h"

#include "ActivMiddleware/Misc/FtpHelper.h"

#include <map>

namespace Activ
{

// Forward declarations
class Application;
class IMutex;

// Default heap size
#define ACTIV_DEFAULT_UPDATE_DOWNLOADER_BUCKET_INFO	{ { 100, 10 } }

/**
 *	@brief	Update downloader class.
 */
class UpdateDownloader : public ManagedEndPoint, public Component
{
public:
	/**
	 *	@brief	Settings class.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_UPDATE_API Settings();

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

		std::string				m_server;						///< The server.
		std::string				m_username;						///< The username.
		std::string				m_password;						///< The password.
		std::string				m_remoteDirectory;				///< The remote directory.
		UpdateTarget			m_targetFilter;					///< Target filter for updates.
		bool					m_isTargetFilterInitialized;	///< Is the target filter initialized?
		std::string				m_updateDirectory;				///< The update directory.
		Time					m_pollDirectoryTime;			///< The time to poll the directory.
		IHeap::BucketInfoVector	m_heapBucketInfoVector;			///< Heap size for update service, default above.
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application the application.
	 *	@param	name the name of the component.
	 *	@param	settings the object's settings.
	 */
	UpdateDownloader(Application &application, const std::string &name, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~UpdateDownloader();

	/**
	 *	@brief	Get settings.
	 *
	 *	@return	the object's settings.
	 */
	const Settings &GetSettings() const;

	/**
	 *	@brief	Pricess update description file.
	 *
	 *	@param	fileName the file name.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode ProcessUpdateDescriptionFile(const std::string &fileName);

	/**
	 *	@brief	Display update downloads.
	 *
	 *	@param	uiIo ui i/o to use.
	 */
	void UiDisplayUpdateDownloads(const UiIo &uiIo);

	/**
	 *	@brief	Clear update downloads.
	 *
	 *	@param	uiIo ui i/o to use.
	 */
	void UiClearUpdateDownloads(const UiIo &uiIo);

private:
	/**
	 *	@brief	Update download class.
	 */
	class UpdateDownload
	{
	public:
		std::string			m_updateDescriptionFileName;		///< The update description file name.
		UpdateDescription	m_updateDescription;				///< The update description.
		uint32_t			m_attemptCount;						///< The number of attempts.
	};

	// Component interface
	StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

	/**
	 *	@brief	On directory poll timer callback.
	 *
	 *	@param	time the time.
	 */
	void OnPollDirectoryTimer(const BaseTime &time);

	/**
	 *	@brief	On download file timer callback.
	 *
	 *	@param	timerAddress the address of the timer.
	 *	@param	time the time.
	 */
	void OnDownloadFileTimer(const Address &timerAddress, const BaseTime &time);

	/**
	 *	@brief	Load updates from update directory.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode LoadUpdatesFromUpdateDirectory();

	/**
	 *	@brief	Clear update downloads.
	 */
	void ClearUpdateDownloads();

	typedef std::map<Address, UpdateDownload>	TimerAddressUpdateDownloadMap;	///< Timer address update download map type.

	const Settings &				m_settings;							///< The object's settings.
	Address							m_pollDirectoryTimerAddress;		///< The poll directory timer address.
	FtpHelper						m_ftpHelper;						///< The ftp helper.
	MessageBuilder					m_messageBuilder;					///< The message builder.
	TimerAddressUpdateDownloadMap	m_timerAddressUpdateDownloadMap;	///< The timer address update download map.
	boost::scoped_ptr<IMutex>		m_pMutex;							///< The mutex.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline const UpdateDownloader::Settings &UpdateDownloader::GetSettings() const
{
	return m_settings;
}

} // namespace Activ

#endif // !defined (ACTIV_UPDATE_DOWNLOADER_H)
