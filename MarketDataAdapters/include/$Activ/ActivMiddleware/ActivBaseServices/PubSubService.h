/**
 *	@file	PubSubService.h
 *
 *	@brief	Header file for pubsub service.
 *
 *  $Log: $
 */

#if !defined (ACTIV_BASE_SERVICES_PUB_SUB_SERVICE_H)
#define ACTIV_BASE_SERVICES_PUB_SUB_SERVICE_H

#include "ActivMiddleware/ActivBaseServices/External.h"

#include <map>

// Default heap size as set in default Settings object
#define PUB_SUB_SERVICE_DEFAULT_BUCKET_INFO	{ { 50, 1000 }, { 100, 1000 }, { 1500, 1000 } }

namespace Activ
{

/**
 *	@brief	Publish-subscribe service class.
 */ 
class PubSubService : private ManagedEndPoint, public Component
{
public:
	/**
	 *	@brief	Settings.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Multicast settings.
		 *
		 *	This describes an instances of a multicast output.
		 */
		class Multicast
		{
		public:
			/**
			 *	@brief	Default constructor.
			 */
			ACTIV_BASE_SERVICES_API Multicast();

			std::string		m_url;
			std::string		m_interfaceAddress;
			size_t			m_txBufferSize;
			size_t			m_rxBufferSize;
			TimeoutPeriod	m_blockingTimeout;					///< default 50ms
			bool			m_listen;							///< receive on this group aswell. default false
			size_t			m_ttl;								///< ttl for transmission. default is 1
		};

		/**
		 *	@brief	Republish settings.
		 *
		 *	This describes a mapping from subject -> set of multicast outputs for that subject.
		 */
		class Republish
		{
		public:
			typedef std::vector<std::string> MulticastNameList;

			std::string			m_subject;						///< (Partial) subject to match on
			MulticastNameList	m_multicastNameList;			///< List of multicast groups to output on
		};

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

		typedef std::map<std::string, Multicast> MulticastMap;
		typedef std::vector<Republish> RepublishList;

		IHeap::BucketInfoVector	m_bucketInfoVector;				///< size of the service heap
		MulticastMap			m_multicastMap;					///< map of multicast outputs
		RepublishList			m_republishList;				///< list of subjects to write to multicast outputs

#if defined (SEQ_NUM_BUILD)
		uint32_t				m_cacheCheckTimeout;			///< how often to check source's cache for timed out cached msgs
		uint32_t				m_cacheTimeout;					///< how long after receipt that a cached record is forwarded if the gaps aren't received
#endif

	private:
		/**
		 *	@brief	Process multicast section in the ini file.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		StatusCode ProcessMulticastSection(const IniFileParser::ConstSectionIterator &sectionIterator);

		/**
		 *	@brief	Process republish section in the ini file.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		StatusCode ProcessRepublishSection(const IniFileParser::ConstSectionIterator &sectionIterator);
	};

	/**
	 *	@brief	Message broker service ui component.
	 */
	ACTIV_DECLARE_NESTED_CLASS(UiComponent) : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	pubSubService service to show menu for.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_BASE_SERVICES_API UiComponent(PubSubService &pubSubService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		PubSubService	&m_pubSubService;
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
	ACTIV_BASE_SERVICES_API PubSubService(Application &application, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_SERVICES_API ~PubSubService();

	class Impl;

protected:
	virtual bool CanThreadExit();
	virtual StatusCode OnMessage(HeapMessage &message);

#if defined (SEQ_NUM_BUILD)
	virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);
#endif

private:
	const boost::scoped_ptr<Impl>	m_pImpl;					///< implementation
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_SERVICES_PUB_SUB_SERVICE_H)
