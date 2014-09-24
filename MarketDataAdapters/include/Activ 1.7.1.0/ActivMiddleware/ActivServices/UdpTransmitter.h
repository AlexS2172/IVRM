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
	 */
	class Settings
	{
	public:
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

		typedef uint32_t MulticastMode;

		/**
		 *	@brief	Multicast disabled.
		 *
		 *	Ini file key:
		 *		MulticastMode = off
		 */
		static const MulticastMode MULTICAST_MODE_OFF = 0;

		/**
		 *	@brief	Multicast enabled. Buffer data for writing to the socket from the Write()
		 *			thread context, don't launch a thread.
		 *
		 *	Ini file key:
		 *		MulticastMode = no-thread
		 */
		static const MulticastMode MULTICAST_MODE_NO_THREAD = 1;

		/**
		 *	@brief	Multicast enabled. Data passed to Write() calls will be immediately passed to
		 *			the internal thread where it will be buffered for writing to the socket.
		 *
		 *	Ini file key:
		 *		MulticastMode = unbuffered-write
		 */
		static const MulticastMode MULTICAST_MODE_UNBUFFERED_WRITE = 2;

		/**
		 *	@brief	Multicast enabled. Data passed to Write() calls will be buffered in the context
		 *			of the calling thread until a Flush(), which will push data to the internal
		 *			thread for writing to the socket.
		 *
		 *	Assumes only 1 thread will be calling Write() and therefore has a slight performance
		 *	advantage over MULTICAST_MODE_MULTI_THREADED_BUFFERED_WRITE. Must call Flush() at some
		 *	point.
		 *
		 *	Ini file key:
		 *		MulticastMode = single-threaded-buffered-write
		 */
		static const MulticastMode MULTICAST_MODE_SINGLE_THREADED_BUFFERED_WRITE = 3;

		/**
		 *	@brief	Multicast enabled. Data passed to Write() calls will be buffered in the context
		 *			of the calling thread until a Flush(), which will push data to the internal
		 *			thread for writing to the socket.
		 *
		 *	Any thread can call Write(), but each thread must call Flush().
		 *
		 *	Ini file key:
		 *		MulticastMode = multi-threaded-buffered-write
		 */
		static const MulticastMode MULTICAST_MODE_MULTI_THREADED_BUFFERED_WRITE = 4;

		/**
		 *	@brief	Controls whether Flush() or Write(IEndPoint *) ensures all data is immediately
		 *			written to the socket.
		 *
		 *	If this flag is NOT set and using MULTICAST_MODE_NO_THREAD, calling Flush() will only
		 *	result in any data queued to send on the socket being sent if the last write was over
		 *	m_flushTimeout milliseconds ago.
		 *
		 *	If this flag IS set and using MULTICAST_MODE_NO_THREAD, calling Flush() results in
		 *	any queued data being immediately written to the socket.
		 *
		 *	If this flag is NOT set and using MULTICAST_MODE_SINGLE_THREADED_BUFFERED_WRITE or
		 *	MULTICAST_MODE_MULTI_THREADED_BUFFERED_WRITE, calling Flush() will only push any
		 *	queued data to the internal thread and not force writing of all data (some data might
		 *	not be written if it doesn't trip the m_flushThreshold). Calling Write(IEndPoint *)
		 *	is similar: the data is passed to the internal thread but a flush of all data is not
		 *	performed.
		 *
		 *	If this flag IS set and using MULTICAST_MODE_SINGLE_THREADED_BUFFERED_WRITE or
		 *	MULTICAST_MODE_MULTI_THREADED_BUFFERED_WRITE, calling Flush() will push any
		 *	queued data to the internal thread and all data will be written to the socket.
		 *	Similarly for Write(IEndPoint *).
		 *
		 *	This flag does not affect the behaviour of MULTICAST_MODE_UNBUFFERED_WRITE.
		 *
		 *	Ini file key:
		 *		MulticastMode = aggressive-flush
		 */
		static const MulticastMode MULTICAST_MODE_AGGRESSIVE_FLUSH = 0x00010000;

		/**
		 *	@brief	Default multicast mode.
		 */
		static const MulticastMode MULTICAST_MODE_DEFAULT = MULTICAST_MODE_UNBUFFERED_WRITE | MULTICAST_MODE_AGGRESSIVE_FLUSH;

		/**
		 *	@brief	Mask off flags.
		 */
		static const MulticastMode MULTICAST_MODE_TYPE_MASK = 0x0000ffff;

		enum PublishMode
		{
			PUBLISH_MODE_OFF,										///< don't publish
			PUBLISH_MODE_DIRECT,									///< send direct to subscribers
			PUBLISH_MODE_SERVICE									///< publish with pub-sub service
		};

		std::string					m_name;							///< name
		std::string					m_serviceId;					///< service id
		bool						m_isOutputEnabled;				///< indicates whether all output is enabled initially, default true

		// multicast output settings
		MulticastMode				m_multicastMode;				///< multicast output mode, default MULTICAST_MODE_DEFAULT defined above
		ServiceAccessPoint			m_serviceAccessPoint;			///< service access point
		bool						m_isNewProtocol;				///< output in new protocol, default true
		bool						m_shouldSendHeartbeats;			///< indicates whether heartbeats should be sent by the transmitter, default true
		std::string					m_bindAddress;					///< bind address
		size_t						m_ttl;							///< ttl for socket, default 1
		uint32_t					m_txBufferSize;					///< underlying *socket* tx buffer size, default 0 (OS default)
		uint16_t					m_flushByteThreshold;			///< flush tx buffer when >= this & no queued data, default to the MTU
		TimeoutPeriod				m_flushTimeout;					///< the flush timeout, default 10
		IHeap::BucketInfoVector		m_bucketInfoVector;				///< the size of the heap. No default
#if defined (ACTIV_PLATFORM_WINDOWS)
		uint32_t					m_threadPriority;				///< priority of the thread, default THREAD_PRIORITY_NORMAL
#endif

		// published output settings
		PublishMode					m_publishMode;					///< default PUBLISH_MODE_OFF
		Subject						m_subject;						///< no default
		IHeap::BucketInfoVector		m_publishHeap;					///< heap to use for published messages, no default
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
	 *	@param	componentId the component id.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SERVICES_API UdpTransmitter(Application &application, ManagedEndPoint &managedEndPoint, const Settings &settings, IHeap * const pHeap = 0, const std::string &componentId = defaultComponentId);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_SERVICES_API ~UdpTransmitter();

	/**
	 *	@brief	Enable all output.
	 */
	ACTIV_SERVICES_API void EnableOutput();

	/**
	 *	@brief	Disable all output.
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
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_DISABLED
	 */
	ACTIV_SERVICES_API StatusCode Write(const void *pData, const size_t dataLength);

	/**
	 *	@brief	Write multiple messages.
	 *
	 *	NB all messages on @a pEndPoint must have type UDP_MESSAGE_DATA.
	 *
	 *	@param	pEndPoint endpoint containing the messages to write.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_DISABLED
	 */
	ACTIV_SERVICES_API StatusCode Write(IEndPoint * const pEndPoint);

	/**
	 *	@brief	Flush any queued data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_SERVICES_API StatusCode Flush();

protected:
	ACTIV_SERVICES_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_SERVICES_API virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

private:
	class Impl;

	std::auto_ptr<Impl>	m_pImpl;								///< hidden implmentation details
};

} // namespace Activ

#endif // !defined (ACTIV_SERVICES_UDP_TRANSMITTER_H)
