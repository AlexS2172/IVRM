/**
 *  @file	UdpReceiver.h
 *
 *  @brief	Udp receiver component.
 *
 *  $Log: $
 */

#if !defined (ACTIV_UDP_RECEIVER_H)
#define ACTIV_UDP_RECEIVER_H

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
	#define ACTIV_SERVICES_UDP_RECEIVER_STATE_LIST(d)	\
		d(STATE_INITIALIZED)							\
		d(STATE_CLOSED)									\
		d(STATE_OPEN)									\
		d(STATE_SUBSCRIBED_LOCAL)

	enum State
	{
		ACTIV_SERVICES_UDP_RECEIVER_STATE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)
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
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SERVICES_API UdpReceiver(Application &application, ManagedEndPoint &managedEndPoint, const std::string &name = Port::anonymousName,
								   IHeap * const pHeap = 0);

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

protected:
	/**
	 *	@brief	Received a message.
	 *
	 *	@param	pData pointer to the message.
	 *	@param	dataLength the length of the message.
	 */
	ACTIV_SERVICES_API virtual void OnReceive(const void * const pData, const size_t dataLength);

	ACTIV_SERVICES_API virtual StatusCode OnMessage(HeapMessage &message);
	ACTIV_SERVICES_API virtual StatusCode OnTimer(const Address &timerMessage, const BaseTime &time);
	ACTIV_SERVICES_API virtual void OnSocketError(const Address &socketAddress);

private:
	/**
	 *	@brief	Create socket to receive from udp transmitter.
	 */
	StatusCode CreateUdpSocket();

	Address						m_lazyTimerAddress;
	State						m_state;
	std::string					m_interfaceAddress;
	std::string					m_multicastGroup;
	uint16_t					m_port;
	uint16_t					m_maximumTransmissionUnitSize;
	Address						m_socketAddress;
	boost::scoped_array<byte_t>	m_pRxBuffer;
	Subject						m_subject;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Address &UdpReceiver::GetSocketAddress() const
{
	return m_socketAddress;
}

} // namespace Activ

#endif // !defined (ACTIV_UDP_RECEIVER_H)
