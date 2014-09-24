/**
 *  @file	UdpArbitrator.h
 *
 *  @brief	Udp arbitrator component.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERVICES_UDP_ARBITRATOR_H)
#define ACTIV_SERVICES_UDP_ARBITRATOR_H

#include "ActivMiddleware/ActivServices/External.h"

#include "ActivMiddleware/ActivFramework/UdpComponent.h"

#include "boost/scoped_array.hpp"

namespace Activ
{

/**
 *	@brief	Udp arbitrator class.
 *
 *	NB there is no concurrency at all in this object, all methods are assumed to be called from the same thread.
 */
class UdpArbitrator
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
		ACTIV_SERVICES_API Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SERVICES_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		/**
		 *	@brief	Get settings from an ini file.
		 *
		 *	@param	iniFileName the file name of the int file.
		 *	@param	sectionName the section name within the ini file.
		 *
		 *	@return	a copy of a settings object.
		 *
		 *	@throw	Exception
		 */
		ACTIV_SERVICES_API static Settings GetFromIniFile(const std::string &iniFileName, const std::string &sectionName);

		uint32_t				m_datagramTimeout;				///< the timeout to wait for a datagram.
		uint32_t				m_feedTimeout;					///< the timeout before a jump backwards in the feed is allowed.
		uint32_t				m_feedSwitchTimeout;			///< the timeout without data before a switch to another feed.
		IHeap::BucketInfoVector	m_bucketInfoVector;				///< the size of the heap.
	};

	/**
	 *	@brief	Statistics class.
	 */
	class Statistics
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SERVICES_API Statistics();

		/**
		 *	@brief	Reset all statistics to 0.
		 */
		ACTIV_SERVICES_API void Reset();
	
		size_t	m_receivedDatagrams;							///< The number of datagrams received
		size_t	m_processingErrors;								///< The number of datagrams failed to process
		size_t	m_discontinuities;								///< The number of gaps in sequence number
		size_t	m_missedDatagrams;								///< The number of datagrams missed in m_discontinuities gaps
		Time	m_lastRxHeartbeatTime;							///< The time of the last heartbeat received.
		Time	m_lastRxTime;									///< The last rx time.
	};

	/**
	 *	@brief	Receiver class.
	 */
	class Receiver
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
			ACTIV_SERVICES_API Settings();

			/**
			 *	@brief	Read settings from an ini file section.
			 *
			 *	@param	sectionIterator iterator to an ini file section.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SERVICES_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

			/**
			 *	@brief	Get settings from an ini file.
			 *
			 *	@param	iniFileName the file name of the int file.
			 *	@param	sectionName the section name within the ini file.
			 *
			 *	@return	a copy of a settings object.
			 *
			 *	@throw	Exception
			 */
			ACTIV_SERVICES_API static Settings GetFromIniFile(const std::string &iniFileName, const std::string &sectionName);

			std::string				m_name;						///< The name.
			std::string				m_serviceId;				///< the service id.
			std::string				m_interfaceAddress;			///< The interface address.
			uint32_t				m_rxBufferSize;				///< The rx buffer size.
			IHeap::BucketInfoVector	m_bucketInfoVector;			///< The size of the heap.
		};

		/**
		 *	@brief	Receiver statistics.
		 */
		class Statistics
		{
		public:
			/**
			 *	@brief	Default constructor.
			 */
			ACTIV_SERVICES_API Statistics();

			/**
			*	@brief	Reset all statistics to 0.
			*/
			ACTIV_SERVICES_API void Reset();
		
			size_t	m_receivedDatagrams;						///< The number of datagrams received
			size_t	m_processingErrors;							///< The number of datagrams failed to process
			size_t	m_discontinuities;							///< The number of gaps in sequence number
			size_t	m_missedDatagrams;							///< The number of datagrams missed in m_discontinuities gaps
			size_t	m_duplicateDatagrams;						///< The number of datagrams received with previous sequence number
			Time	m_lastRxTime;								///< The last rx time.
			bool	m_isNoDataWarningActive;					///< Whether the no data warning is active.
		};

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	udpArbitrator the udp arbitrator this receiver will send data to.
		 *	@param	settings the object's settings.
		 */
		ACTIV_SERVICES_API Receiver(UdpArbitrator &udpArbitrator, const Settings &settings);

		/**
		 *	@brief	Destructor.
		 */
		ACTIV_SERVICES_API virtual ~Receiver();

		/**
		 *	@brief	Get statistics.
		 *
		 *	@return	a reference to the statistics.
		 */
		ACTIV_SERVICES_API const Statistics &GetStatistics() const;

		/**
		 *	@brief	Reset statistics.
		 */
		ACTIV_SERVICES_API void ResetStatistics();

	private:
		class Impl;

		std::auto_ptr<Impl>	m_pImpl;							///< hidden implmentation details
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application the application.
	 *	@paran	managedEndPoint the managed end point.
	 *	@param	portName the port name.
	 *	@param	settings the object's settings.
	 *	@param	componentId the component id.
	 */
	ACTIV_SERVICES_API UdpArbitrator(Application &application, ManagedEndPoint &managedEndPoint, const std::string &portName, const Settings &settings, const std::string &componentId = Component::defaultComponentId);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_SERVICES_API virtual ~UdpArbitrator();

	/**
	 *	@brief	Received a message.
	 *
	 *	@param	pData pointer to the message.
	 *	@param	dataLength the length of the message.
	 */
	ACTIV_SERVICES_API virtual void OnReceive(const void * const pData, const size_t dataLength) = 0;

	/**
	 *	@brief	Get statistics.
	 *
	 *	@return	a reference to the statistcs.
	 */
	ACTIV_SERVICES_API const Statistics &GetStatistics() const;

	/**
	 *	@brief	Reset statistics.
	 */
	ACTIV_SERVICES_API void ResetStatistics();

private:
	class Impl;

	std::auto_ptr<Impl>	m_pImpl;								///< hidden implmentation details
};

} // namespace Activ

#endif // !defined (ACTIV_SERVICES_UDP_ARBITRATOR_H)
