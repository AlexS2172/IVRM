/**
 *	@file	RouterService.h
 *
 *	@brief	Header file for router service class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_ROUTER_SERVICE_H)
#define ACTIV_ROUTER_SERVICE_H

#include "ActivMiddleware/ActivRouter/External.h"
#include "ActivMiddleware/ActivIp/IpManager.h"

// Default heap size for router service
#define ACTIV_DEFAULT_ROUTER_SERVICE_BUCKET_INFO				{ { 5, 5000 }, { 50, 1000 } }
#define ACTIV_DEFAULT_ROUTER_SERVICE_NOTIFICATION_BUCKET_INFO	{ { 5, 5000 } }

namespace Activ
{

/**
 *	@brief	Router service class.
 */
class RouterService : private ManagedEndPoint, public Component
{
public:
	/**
	 *	@brief	Settings.
	 */
	class Settings : public IpManager::Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_ROUTER_API Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_ROUTER_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		ManagedEndPoint::Settings	m_managedEndPointSettings;			///< settings for the endpoint to run on
		IHeap::BucketInfoVector		m_serviceHeapBucketInfoVector;		///< bucket info for main service
		IHeap::BucketInfoVector		m_notificationHeapBucketInfoVector;	///< bucket info for notification heaps
		bool						m_useHardwareZlib;					///< use hardware zlib card if available? (default: true)
	};

	/**
	 *	@brief	Router service ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	routerService the router service to have the ui.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_ROUTER_API UiComponent(RouterService &routerService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		RouterService	&m_routerService;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application the application to run in.
	 *	@param	serviceName the name of the router service.
	 *	@param	settings settings for service.
	 *
	 *	@throw	Exception
	 */
	ACTIV_ROUTER_API RouterService(Application &application, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_ROUTER_API ~RouterService();

	class Impl;													///< router service internals

private:
	virtual void Process();
	virtual StatusCode OnMessage(HeapMessage &message);
	virtual StatusCode OnRequest(HeapMessage &request);
	virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);
	virtual bool CanThreadExit();
	
	const boost::scoped_ptr<Impl>	m_pImpl;
};

} // namespace Activ

#endif // !defined (ACTIV_ROUTER_SERVICE_H)
