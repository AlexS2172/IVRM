/**
 *	@file	UiService.h
 *
 *	@brief	Header file for the ui service class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_SERVICES_UI_SERVICE_H)
#define ACTIV_BASE_SERVICES_UI_SERVICE_H

#include "ActivMiddleware/ActivBaseServices/External.h"

#include "boost/scoped_ptr.hpp"

#include <map>

// Default heap sizes
#define ACTIV_DEFAULT_UI_SERVICE_BUCKET_INFO		{ { 80, 10000 }, { 500, 1000 }, { 1000, 500 }, { 4000, 500 } }

namespace Activ
{

/**
 *	@brief	Ui service class.
 *
 *	A ui service requires the address of the ui component that is to act as the main menu of the service,
 *	which exists on a separate endpoint. A heap with the same name as this service must have been created
 *	before construction of this object.
 */
class UiService : private ManagedEndPoint, public Service
{
public:
	typedef std::string UserId;									///< type of user id
	typedef std::string UserPassword;							///< type of user password

	/**
	 *	@brief	User info.
	 */
	class UserInfo
	{
	public:
		UserId			m_id;									///< id of user
		UserPassword	m_password;								///< password of user
		UiPermissions	m_uiPermissions;						///< ui permissions of user
	};

	typedef std::map<UserId, UserInfo> UserInfoMap;				///< type of user info map

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
		ACTIV_BASE_SERVICES_API StatusCode Read(IniFileParser::ConstSectionIterator &sectionIterator);

		bool					m_enableConsoleClient;			///< enable console client? Default == true
		bool					m_enableLogOutput;				///< enable log output? Default == true
		bool					m_enableAnonymousAccess;		///< enable view-only anonymous user? Default == true
		UiPermissions			m_anonymousAccessPermissions;	///< anonymous access permissions
		ServiceAccessPointList	m_serviceAccessPointList;		///< access points. Default is port 9023
		IHeap::BucketInfoVector	m_heapBucketInfoVector;			///< heap size. Default, see above
		UserInfoMap				m_userInfoMap;					///< info about users
	};

	/**
	 *	@brief	Ui service ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	uiService the ui service to have the ui.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_BASE_SERVICES_API UiComponent(UiService &uiService);

	private:
		virtual void Run(UiUser &uiUser);

		UiService	&m_uiService;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	serviceName name of the service.
	 *	@param	settings settings for the service.
	 *	@param	mainUiComponentAddress address of a ui component to act as the main menu.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_BASE_SERVICES_API UiService(Application &application, const std::string &serviceName, const Settings &settings, const Address &mainUiComponentAddress);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_SERVICES_API ~UiService();

	/**
	 *	@brief	Process a ctrl event.
	 *
	 *	@param	eventType the type of ctrl event that has occured.
	 */
	ACTIV_BASE_SERVICES_API void ProcessCtrlEvent(const CtrlHandler::EventType eventType);

	class Impl;													///< private implementation class forward reference

protected:
	ACTIV_BASE_SERVICES_API virtual void Process();
	ACTIV_BASE_SERVICES_API virtual bool CanThreadExit();
	ACTIV_BASE_SERVICES_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_BASE_SERVICES_API virtual StatusCode OnAccept(HeapMessage &acceptMessage);

private:
	static const ServiceId	m_serviceId;						///< id of the ui service

	boost::scoped_ptr<Impl>	m_pImpl;							///< implementation
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_SERVICES_UI_SERVICE_H)
