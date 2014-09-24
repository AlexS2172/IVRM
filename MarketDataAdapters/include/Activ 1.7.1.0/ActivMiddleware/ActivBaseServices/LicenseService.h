/**
 *	@file	LicenseService.h
 *
 *	@brief	Header file for the license service class.
 *
 *	$Log: $
 */

#if !defined(ACTIV_LICENSE_SERVICE_H)
#define ACTIV_LICENSE_SERVICE_H

#include "ActivMiddleware/ActivBaseServices/External.h"

#include <set>

// Default heap size
#define ACTIV_DEFAULT_LICENSE_SERVICE_BUCKET_INFO		{ { 20, 1000 } }

namespace Activ
{

/**
 *	@brief	License service class.
 */
class LicenseService : private ManagedEndPoint, public Component
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

		typedef std::set<std::string>	SystemNameSet;					///< system name set type
		typedef std::set<std::string>	MacAddressSet;					///< mac address set type
		typedef std::set<std::string>	ComponentLicenseIdFilterSet;	///< component license id filter set type

		ACTIV_BASE_SERVICES_API static const std::string defaultLicenseFileName;

		std::string						m_licenseFileName;				///< license file name, if exists it is read and overrides all license settings
		IHeap::BucketInfoVector			m_heapBucketInfoVector;			///< bucket info for service

		// License details, these will not be read from an ini file. They will be overridden if a license file is found.
		std::string						m_licenseeName;					///< licensee name
		SystemNameSet					m_systemNameSet;				///< system name set
		MacAddressSet					m_macAddressSet;				///< mac address set
		std::string						m_ipAddress;					///< ip address
		std::string						m_hostName;						///< host name
		std::string						m_validFrom;					///< valid from date
		std::string						m_validTo;						///< valid to date
		std::string						m_maximumUpTime;				///< maximum up time
		std::string						m_maximumUpTimeDays;			///< maximum up time days
		ComponentLicenseIdFilterSet		m_componentLicenseIdFilterSet;	///< component license id filter set
		std::string						m_signature;					///< digital signature
	};

	/**
	 *	@brief	Timer service ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	licenseService the license service to have the ui.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_BASE_SERVICES_API UiComponent(LicenseService &licenseService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		LicenseService &	m_licenseService;					///< license service
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	The constructor creates an endpoint and heap in the system and runs a single thread.
	 *
	 *	@param	application the application the service is to run in.
	 *	@param	serviceName the name of the license service.
	 *	@param	settings settings for the license service.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_SERVICES_API LicenseService(Application &application, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_SERVICES_API ~LicenseService();

	class Impl;													///< license service internals

private:
	ACTIV_BASE_SERVICES_API virtual StatusCode OnRequest(HeapMessage &request);
	ACTIV_BASE_SERVICES_API virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

	boost::scoped_ptr<Impl>	m_pImpl;
};

} // namespace Activ

#endif // !defined(ACTIV_LICENSE_SERVICE_H)
