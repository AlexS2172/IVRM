/**
 *  @file	UdpTransmitter.h
 *
 *  @brief	Udp transmitter service.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERVICES_UDP_TRANSMITTER_H)
#define ACTIV_SERVICES_UDP_TRANSMITTER_H

#include "ActivMiddleware/ActivServices/External.h"

namespace Activ
{

/**
 *	@brief	Udp transmitter service.
 */
class UdpTransmitter : public Service
{
public:
	/**
	 *	@brief	Settings.
	 *
	 *	Default m_flushThresholdSize is 0, which sets the flush threshold to the mtu.
	 */
	class Settings
	{
	public:
		enum PublishMode
		{
			PUBLISH_MODE_OFF,										///< don't publish
			PUBLISH_MODE_DIRECT,									///< send direct to subscribers
			PUBLISH_MODE_SERVICE									///< publish with pub-sub service
		};

		std::string					m_name;							///< name
		std::string					m_serviceId;					///< service id
		ServiceAccessPoint			m_serviceAccessPoint;			///< service access point
		bool						m_isNewProtocol;				///< output in new protocol (default false)
		bool						m_isOutputEnabled;				///< indicates whether all output is enabled initially
		bool						m_shouldSendHeartbeats;			///< indicates whether heartbeats should be sent by the transmitter

		// multicast output settings
		bool						m_isMulticastEnabled;			///< multicast output defined? (default true)
		std::string					m_bindAddress;					///< bind address
		uint32_t					m_txBufferSize;					///< underlying *socket* tx buffer size
		uint16_t					m_flushThresholdSize;			///< flush tx buffer when >= this & no queued data
		TimeoutPeriod				m_blockingTimeout;				///< the blocking timeout
		IHeap::BucketInfoVector		m_bucketInfoVector;				///< the size of the heap
		bool						m_useBlockingThread;			///< whether to use a separate thread to block up (default true)
		size_t						m_ttl;							///< ttl for socket, default 8

		// published output settings
		PublishMode					m_publishMode;
		Subject						m_subject;
		IHeap::BucketInfoVector		m_publishHeap;					///< heap to use for published messages

		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SERVICES_API Settings();
		
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

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SERVICES_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this service to a managed endpoint at the next available slot.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	settings the settings.
	 *	@param	pHeap heap to use for messaging, or 0 for the system heap.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SERVICES_API UdpTransmitter(Application &application, ManagedEndPoint &managedEndPoint, const Settings &settings, IHeap * const pHeap = 0);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_SERVICES_API ~UdpTransmitter();

	/**
	 *	@brief	Enable output.
	 */
	ACTIV_SERVICES_API void EnableOutput();

	/**
	 *	@brief	Disable output.
	 */
	ACTIV_SERVICES_API void DisableOutput();

	/**
	 *	@brief	Is output enabled?
	 *
	 *	@return	true if output is enabled.
	 */
	ACTIV_SERVICES_API bool IsOutputEnabled() const;

	/**
	 *	@brief	Get the settings.
	 *
	 *	@param	settings UDP transmitter settings.
	 */
	ACTIV_SERVICES_API void GetSettings(Settings &settings) const;

	/**
	 *	@brief	Write a message.
	 *
	 *	@param	heapMessage the message to write.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_DISABLED
	 */
	ACTIV_SERVICES_API StatusCode Write(HeapMessage &heapMessage);

	/**
	 *	@brief	Write a message.
	 *
	 *	@param	pData the message to write.
	 *	@param	dataLength.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_DISABLED
	 */
	ACTIV_SERVICES_API StatusCode Write(const void *pData, const size_t dataLength);

	/**
	 *	@brief	Flush any queued data now.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_SERVICES_API StatusCode Flush();

protected:
	ACTIV_SERVICES_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_SERVICES_API virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

private:
	ACTIV_DECLARE_NESTED_CLASS(Impl);

	std::auto_ptr<Impl>	m_pImpl;								///< hidden implmentation details
};

} // namespace Activ

#endif // !defined (ACTIV_SERVICES_UDP_TRANSMITTER_H)
