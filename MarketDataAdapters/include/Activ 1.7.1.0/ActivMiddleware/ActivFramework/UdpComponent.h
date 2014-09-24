/**
 *	@file	UdpComponent.h
 *
 *	@brief	Header file for udp component class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_UDP_COMPONENT_H)
#define ACTIV_UDP_COMPONENT_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/Component.h"

namespace Activ
{

/**
 *	@brief	Udp ingest component
 */
class UdpComponent : public Component
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this object to a managed endpoint at the next available slot.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	name name of the port, local to the endpoint.
	 *	@param	pHeap heap to use for messaging, or 0 for the system heap.
	 *	@param	componentId the component id.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_FRAMEWORK_API UdpComponent(Application &application, ManagedEndPoint &managedEndPoint,
									 const std::string &name = Port::anonymousName, IHeap * const pHeap = 0,
									 const std::string &componentId = defaultComponentId);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API ~UdpComponent();

	/**
	 *	@brief	Create a socket and bind it to a local address.
	 *
	 *	@param	localAddress local address to bind to.
	 *	@param	localPort local port to bind to.
	 *	@param	maxRxSize maximum size of a datagram that can be received.
	 *	@param	socketAddress reference to an Address object to receive the address of the socket.
	 *	@param	pHeap the heap to use for received messages. Default is this port's heap.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_TIMEOUT.
	 *	@retval	STATUS_CODE_FAILURE ...
	 */
	ACTIV_FRAMEWORK_API StatusCode CreateSocket(const std::string &localAddress,
												const uint16_t localPort, const uint16_t maxRxSize,
												Address &socketAddress, IHeap * const pHeap = 0) const;

	/**
	 *	@brief	Initiate a read on a socket.
	 *
	 *	@param	socketAddress address of the socket to connect.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Read(const Address &socketAddress) const;

	/**
	 *	@brief	Connect a socket to a remote address (filter all traffic from other addresses).
	 *
	 *	@param	socketAddress address of the socket to connect.
	 *	@param	remoteAddress the remote address to connect to in string form.
	 *	@param	remotePort the remote port to connect to (local endian).
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Connect(const Address &socketAddress,
										   const std::string &remoteAddressString,
										   const uint16_t remotePort) const;

	/**
	 *	@brief	Disconnect a socket from a remote address (allow all traffic).
	 *
	 *	@param	socketAddress address of the socket to disconnect.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Disconnect(const Address &socketAddress) const;

	/**
	 *	@brief	Close a socket.
	 *
	 *	@param	socketAddress address of the socket to shutdown.
	 *	@param	gracefully true to only close the socket once any outstanding tx has completed,
	 *			false to force the socket closed immediately.
	 *
	 *	@retval	STATUS_CODE_SUCCESS the socket is fully closed.
	 *	@retval	STATUS_CODE_PENDING the socket is closing and OnSocketClose() will be called.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Close(const Address &socketAddress, const bool gracefully = false) const;

	/**
	 *	@brief	Join a multicast group.
	 *
	 *	@param	socketAddress address of the socket to join the multicast group on.
	 *	@param	multicastGroupString an ipv4 or ipv6 multicast address string.
	 *	@param	interfaceAddressString an ipv4 or ipv6 interface address string.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode JoinMulticastGroup(const Address &socketAddress,
													  const std::string &multicastGroupString,
													  const std::string &interfaceAddressString) const;

	/**
	 *	@brief	Leave a multicast group.
	 *
	 *	@param	socketAddress address of the socket to leave the multicast group.
	 *	@param	multicastGroupString an ipv4 or ipv6 multicast address string.
	 *	@param	interfaceAddressString an ipv4 or ipv6 interface address string.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode LeaveMulticastGroup(const Address &socketAddress,
													   const std::string &multicastGroupString,
													   const std::string &interfaceAddressString) const;

	/**
	 *	@brief	Set multicast interface for transmission.
	 *
	 *	@param	socketAddress address of the socket to set multicast interface on.
	 *	@param	interfaceAddressString an ipv4 or ipv6 address string.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode SetMulticastTxInterface(const Address &socketAddress,
														   const std::string &interfaceAddressString) const;

	/**
	 *	@brief	Set TTL to be used for multicasting.
	 *
	 *	@param	socketAddress address of the socket to set the ttl on.
	 *	@param	ttl the TTL.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode SetMulticastTtl(const Address &socketAddress, const int ttl) const;

	/**
	 *	@brief	Set TTL to be used for unicasting.
	 *
	 *	@param	socketAddress address of the socket to set unicast ttl on.
	 *	@param	ttl the TTL.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode SetUnicastTtl(const Address &socketAddress, const int ttl) const;

	/**
	 *	@brief	Set the internal rx buffer size of a socket.
	 *
	 *	@param	socketAddress address of the socket to set receive buffer size.
	 *	@param	size the rx buffer size.
	 *
	 *	@retval	...
	 */
    ACTIV_FRAMEWORK_API StatusCode SetRxBufferSize(const Address &socketAddress, const int size) const;

	/**
	 *	@brief	Set the internal tx buffer size of a socket.
	 *
	 *	@param	socketAddress address of the socket to set transmit buffer size on.
	 *	@param	size the tx buffer size.
	 *
	 *	@retval	...
	 */
    ACTIV_FRAMEWORK_API StatusCode SetTxBufferSize(const Address &socketAddress, const int size) const;

	/**
	 *	@brief	Set the receive multicast loopback option of a socket.
	 *
	 *	@param	socketAddress address of the socket to set transmit buffer size on.
	 *	@param	enable true to enable receipt of multicast loopback data, else false.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode SetMulticastLoopback(const Address &socketAddress, const bool enable) const;

	/**
	 *	@brief	Post some data.
	 *
	 *	@param	socketAddress the socket to send the data on.
	 *	@param	heapMessage the message to send.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode PostData(const Address &socketAddress, HeapMessage &heapMessage) const;

	/**
	 *	@brief	Get the underying socket handle.
	 *
	 *	@param	socketAddress the socket to get the handle of.
	 *	@param	socket reference to a SOCKET variable to receive the handle.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode GetSocketHandle(const Address &socketAddress, SOCKET &socket) const;

	/**
	 *	@brief	Post some data.
	 *
	 *	@param	socketAddress the socket to send the data on.
	 *	@param	pData pointer to data to send.
	 *	@param	dataLength length of data.
	 *
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode PostData(const Address &socketAddress, const void *pData, const size_t dataLength) const;

protected:
	ACTIV_FRAMEWORK_API virtual StatusCode OnMessage(HeapMessage &message);

	/**
	 *	@brief	Socket has closed due to an error.
	 */
	ACTIV_FRAMEWORK_API virtual void OnSocketError(const Address &socketAddress);

	/**
	 *	@brief	Socket has closed due to being shutdown.
	 */
	ACTIV_FRAMEWORK_API virtual void OnSocketClose(const Address &socketAddress);
};

} // namespace Activ

#endif // !defined (ACTIV_UDP_COMPONENT_H)
