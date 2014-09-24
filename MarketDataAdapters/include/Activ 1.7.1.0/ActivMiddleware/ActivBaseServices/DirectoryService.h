/**
 *	@file	DirectoryService.h
 *
 *	@brief	Header file for directory service class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_DIRECTORY_SERVICE_H)
#define ACTIV_DIRECTORY_SERVICE_H

#include "ActivMiddleware/ActivBaseServices/External.h"

// Default heap size
#define ACTIV_DEFAULT_DIRECTORY_SERVICE_BUCKET_INFO	{ { 200, 500 } }

namespace Activ
{

// available directory providers
class IniDirectoryComponent;
class DynamicDirectoryComponent;

/**
 *	@brief	DirectoryService.
 */
class DirectoryService : private ManagedEndPoint, public Component
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

		IHeap::BucketInfoVector	m_heapBucketInfoVector;			///< Heap size for directory service, default above
	};

	/**
	 *	@brief	Ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	directoryService object to show menu for.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_BASE_SERVICES_API UiComponent(DirectoryService &directoryService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		DirectoryService	&m_directoryService;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	serviceName name of the service.
	 *	@param	settings settings for directory service.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_SERVICES_API DirectoryService(Application &application, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_SERVICES_API ~DirectoryService();

	/**
	 *	@brief	Initialize default directory providers.
	 *
	 *	@param	serviceLocationIniFile filename of ini file containing static services.
	 */
	ACTIV_BASE_SERVICES_API void Initialize(const std::string &serviceLocationIniFile);

private:
	std::auto_ptr<IniDirectoryComponent>		m_pIniDirectoryComponent;
	std::auto_ptr<DynamicDirectoryComponent>	m_pDynamicDirectoryComponent;
};

} // namespace Activ

#endif // !defined (ACTIV_DIRECTORY_SERVICE_H)
