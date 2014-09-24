/**
 *  @file	UdpReceiver.h
 *
 *  @brief	Udp receiver component.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERVICES_UDP_RECEIVER_H)
#define ACTIV_SERVICES_UDP_RECEIVER_H

#include "ActivMiddleware/ActivServices/External.h"
#include "ActivMiddleware/ActivFramework/UdpComponent.h"

#include "boost/scoped_array.hpp"

namespace Activ
{

class OpenTransmitterResponse;

/**
 *	@brief	Udp receiver component.
 *
 *	NB there is no concurrency at all in this object, all methods are assumed to be called from the same thread.
 */
class UdpReceiver : public UdpComponent
{
public:
	/**
	 *	@brief	Receive statistics.
	 */
	class Stats
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SERVICES_API Stats();

		/**
		 *	@brief	Reset all stats to 0 or undefined.
		 */
		ACTIV_SERVICES_API void Reset();
	
		size_t	m_rxDatagrams;									///< number of datagrams received
		size_t	m_processingErrors;								///< number of datagrams failed to process
		size_t	m_discontinuities;								///< number of gaps in sequence number
		size_t	m_missedDatagrams;								///< number of datagrams missed in m_discontinuities gaps
		Time	m_lastRxTime;									///< time of receipt of last message excluding heartbeats
		Time	m_lastRxHeartbeatTime;							///< time of receipt of last heartbeat
		bool	m_isNoHeartbeatWarningActive;					///< in warning state because heartbeats aren't being received?
	};

	#define ACTIV_SERVICES_UDP_RECEIVER_STATE_LIST(d)	\
		d(STATE_INITIALIZED)							\
		d(STATE_CLOSED)									\
		d(STATE_OPEN)									\
		d(STATE_SUBSCRIBED_LOCAL)

	enum State
	{
		ACTIV_SERVICES_UDP_RECEIVER_STATE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)
		NUM_STATES
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this object to a managed endpoint at the next available port.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	name name of the port, local to the endpoint.
	 *	@param	pHeap heap to use for messaging, or 0 for the system heap.
	 *	@param	componentId the component id.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SERVICES_API UdpReceiver(Application &application, ManagedEndPoint &managedEndPoint, const std::string &name = Port::anonymousName,
								   IHeap * const pHeap = 0, const std::string &componentId = defaultComponentId);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_SERVICES_API ~UdpReceiver();

	/**
	 *	@brief	Open a transmitter.
	 *
	 *	@param	serviceId service id of the transmitter.
	 *	@param	interfaceAddress local interface address to bind to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_WRONG_STATE
	 *	@retval	...
	 */
	ACTIV_SERVICES_API StatusCode OpenTransmitter(const ServiceId &serviceId, const std::string &interfaceAddress);

	/**
	 *	@brief	Close a transmitter.
	 */
	ACTIV_SERVICES_API void CloseTransmitter();

	/**
	 *	@brief	Set the internal rx buffer size of the socket.
	 *
	 *	@param	size the rx buffer size.
	 *
	 *	@retval	...
	 */
    ACTIV_SERVICES_API StatusCode SetRxBufferSize(const int size) const;

	/**
	 *	@brief	Process a recieved datagram. Calls OnReceive() callback for each message.
	 *
	 *	@param	heapMessage a recieved message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_BUFFER_UNDERRUN.
	 *	@retval	...
	 */
	ACTIV_SERVICES_API StatusCode ProcessDatagram(HeapMessage &heapMessage);

	/**
	 *	@brief	Get the udp socket address.
	 *
	 *	@return	Address of the udp socket used by this object.
	 */
	ACTIV_SERVICES_API const Address &GetSocketAddress() const;

	/**
	 *	@brief	Get the current state of the receiver.
	 *
	 *	@return	State.
	 */
	ACTIV_SERVICES_API State GetState() const;

	/**
	 *	@brief	Get string representation of receiver state.
	 *
	 *	@param	state.
	 *
	 *	@return	State in string form.
	 */
	ACTIV_SERVICES_API static std::string StateToString(const State state);

	/**
	 *	@brief	Get stats.
	 *
	 *	@return	the current receiver statistics.
	 */
	ACTIV_SERVICES_API Stats GetStats() const;

	/**
	 *	@brief	Get time of receipt of last datagram.
	 *
	 *	NB datagrams that contain only a heartbeat do not alter this value.
	 *
	 *	@param	time to receive the time of last datagram.
	 */
	ACTIV_SERVICES_API void GetLastRxTime(Time &time) const;

	/**
	 *	@brief	Reset stats.
	 */
	ACTIV_SERVICES_API void ResetStats();	

protected:
	/**
	 *	@brief	Received a message.
	 *
	 *	@param	pData pointer to the message.
	 *	@param	dataLength the length of the message.
	 */
	ACTIV_SERVICES_API virtual void OnReceive(const void * const pData, const size_t dataLength);

	/**
	 *	@brief	Haven't received a heartbeat message for a while.
	 */
	ACTIV_SERVICES_API virtual void OnNoHeartbeatWarning();

	/**
	 *	@brief	Received a heartbeat after a previous OnNoHeartbeatWarning().
	 */
	ACTIV_SERVICES_API virtual void OnNoHeartbeatWarningClear();

	ACTIV_SERVICES_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_SERVICES_API virtual StatusCode OnTimer(const Address &timerMessage, const BaseTime &time);
	ACTIV_SERVICES_API virtual void OnSocketError(const Address &socketAddress);

private:
	class Impl;

	std::auto_ptr<Impl>	m_pImpl;								///< hidden implmentation details
};

} // namespace Activ

#endif // !defined (ACTIV_SERVICES_UDP_RECEIVER_H)
