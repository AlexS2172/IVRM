/**
 *	@file	ThinApplication.h
 *
 *	@brief	Header file for thin application class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_THIN_APPLICATION_H)
#define ACTIV_THIN_APPLICATION_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/Application.h"

namespace Activ
{

/**
 *	@brief	Thin application class.
 *
 *	The ThinApplication class should be used when linking to an existing shared memory system. It provides
 *	access to always on agent services.
 */
class ThinApplication : public Application
{
public:
	/**
	 *	@brief	Settings.
	 */
	class Settings : public Application::Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_FRAMEWORK_API Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	iniFileParser ini file.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API StatusCode Read(const IniFileParser &iniFileParser);

		std::string	m_processName;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	systemName the name of the system to open.
	 *	@param	processName the name of this process, empty string gives a process name based on the os process id.
	 *	@param	maxNumberOfThreads the max number of threads to allow on the default endpoint.
	 *	@param	timeout how long to wait for the system to exist, in ms.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_FRAMEWORK_API ThinApplication(const Settings &settings, Timeout &timeout = Timeout::GetInfinite());

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API virtual ~ThinApplication();

protected:
	ACTIV_FRAMEWORK_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_FRAMEWORK_API virtual void OnShutdown();

private:
	class Private;

	boost::scoped_ptr<Private>	m_pPrivate;						///< Hidden implementation details
};

} // namespace Activ

#endif // !defined (ACTIV_THIN_APPLICATION_H)
