/**
 *	@file	RouterServiceApi.h
 *
 *	@brief	Header file for router api.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_ROUTER_SERVICE_API_H)
#define ACTIV_FRAMEWORK_ROUTER_SERVICE_API_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivMessageHandler/ISerializableMessage.h"

namespace Activ
{

class Component;

/**
 *	@brief	Api to the agent's router service.
 */
class RouterServiceApi
{
public:
	/**
	 *	@brief	Tcp related apis.
	 */
	class Tcp
	{
	public:
		// CLIENT SIDE

		/**
		 *	@brief	Create a circuit and initiate a synchronous connect.
		 *
		 *	@param	component component to initiate the connection from.
		 *	@param	connectionAddress destination of the connect request.
		 *	@param	url of the form protocol://host:port/location?unique.
		 *	@param	connectResponse reference to a HeapMessage object to receive connect response. Source address of this
		 *			message for a successful connect will be the circuit address.
		 *	@param	pHeap the heap to use for received messages. Default is this component's heap.
		 *	@param	timeout How long to wait for the circuit to establish, in ms.
		 *	@param	pUserData optional userdata.
		 *
		 *	@retval	STATUS_CODE_SUCCESS circuit is up.
		 *	@retval	STATUS_CODE_TIMEOUT circuit did not establish in the timeout period.
		 *	@retval	STATUS_CODE_HOST_NOT_FOUND unknown destination machine.
		 *	@retval	STATUS_CODE_INVALID_ENDPOINT_PORT unknown endpoint-port on target system.
		 *	@retval	STATUS_CODE_FAILURE ...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Connect(const Component &component, const std::string &url, HeapMessage &connectResponse,
													  IHeap * const pHeap, const TimeoutPeriod timeout, const ISerializableMessage * const pUserData = 0);

		/**
		 *	@brief	Create a circuit and initiate an asynchronous connect.
		 *
		 *	@param	component component to initiate the connection from.
		 *	@param	connectionAddress destination of the connect request.
		 *	@param	url of the form protocol://host:port/location?unique.
		 *	@param	circuitAddress reference to an Address object to receive the address of the circuit.
		 *	@param	pHeap the heap to use for received messages. Default is this component's heap.
		 *	@param	pUserData optional userdata.
		 *
		 *	@retval	STATUS_CODE_SUCCESS OnConnect() will be invoked when the circuit is up.
		 *	@retval	STATUS_CODE_HOST_NOT_FOUND unknown destination machine.
		 *	@retval	STATUS_CODE_INVALID_ENDPOINT_PORT unknown endpoint-port on target system.
		 *	@retval	STATUS_CODE_FAILURE ...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Connect(const Component &component, const std::string &url, Address &circuitAddress,
													  IHeap * const pHeap = 0, const ISerializableMessage * const pUserData = 0);

		// SERVER SIDE

		/**
		 *	@brief	Allow an address to be accessable through a server.
		 *
		 *	@param	component component to allow access to.
		 *	@param	url of the form protocol://host:port?unique,accepts=n
		 *	@param	serverAddress reference to an Address object to receive the address of the server.
		 *	@param	accessUrlList to receive the urls that a client can use to access the server.
		 *	@param	pHeap the heap to use for received messages. Default is this component's heap.
		 *
		 *	@retval	STATUS_CODE_SUCCESS.
		 *	@retval	STATUS_CODE_NOT_SHAREABLE unique not specified for an non-multiplexable protocol.
		 *	@retval	STATUS_CODE_INVALID_PROTOCOL server already exists on the given address and port with a different protocol.
		 *	@retval	STATUS_CODE_SHUTTING_DOWN can't add new user as server is shutting down.
		 *	@retval	STATUS_CODE_ALREADY_LISTENING user already added.
		 *	@retval	STATUS_CODE_FAILURE.
		 *	@retval	STATUS_CODE_TIMEOUT.
		 */
		ACTIV_FRAMEWORK_API static StatusCode Listen(const Component &component, const std::string &url, Address &serverAddress, std::vector<std::string> &accessUrlList, IHeap * const pHeap = 0);

		/**
		 *	@brief	Allow an address to be accessable through a server.
		 *
		 *	@param	component component to allow access to.
		 *	@param	url of the form protocol://host:port?unique,accepts=n
		 *	@param	serverAddress reference to an Address object to receive the address of the server.
		 *	@param	pHeap the heap to use for received messages. Default is this component's heap.
		 *
		 *	@retval	STATUS_CODE_SUCCESS.
		 *	@retval	STATUS_CODE_NOT_SHAREABLE unique not specified for an non-multiplexable protocol.
		 *	@retval	STATUS_CODE_INVALID_PROTOCOL server already exists on the given address and port with a different protocol.
		 *	@retval	STATUS_CODE_SHUTTING_DOWN can't add new user as server is shutting down.
		 *	@retval	STATUS_CODE_ALREADY_LISTENING user already added.
		 *	@retval	STATUS_CODE_FAILURE.
		 *	@retval	STATUS_CODE_TIMEOUT.
		 */
		ACTIV_FRAMEWORK_API static StatusCode Listen(const Component &component, const std::string &url, Address &serverAddress, IHeap * const pHeap = 0);

		/**
		 *	@brief	Remove access to an address through a server.
		 *
		 *	@param	component component to stop listening.
		 *	@param	serverAddress address of the server to remove access from.
		 *
		 *	@retval	STATUS_CODE_SUCCESS.
		 *	@retval	STATUS_CODE_FAILURE.
		 */
		ACTIV_FRAMEWORK_API static StatusCode Stop(const Component &component, const Address &serverAddress);

		/**
		 *	@brief	Break all accepted connections by a server.
		 *
		 *	@param	component component to send message from.
		 *	@param	serverAddress address of the server to close the accepted connections of.
		 *
		 *	@retval	STATUS_CODE_SUCCESS.
		 *	@retval	STATUS_CODE_FAILURE.
		 */
		ACTIV_FRAMEWORK_API static StatusCode BreakAll(const Component &component, const Address &serverAddress);

		// CLIENT AND SERVER SIDE

		/**
		 *	@brief	Break a connection.
		 *
		 *	@param	component component to initiate the break from.
		 *	@param	connectionAddress the address of a circuit to break. If the connection is shared,
		 *			all circuits over that connection will break.
		 *
		 *	@retval	STATUS_CODE_SUCCESS the connection has been broken.
		 *	@retval	STATUS_CODE_FAILURE ...?
		 */
		ACTIV_FRAMEWORK_API static StatusCode Break(const Component &component, const Address &connectionAddress);

		/**
		 *	@brief	Disconnect a circuit.
		 *
		 *	@param	component component to initiate the disconnect from.
		 *	@param	circuitAddress the address of the circuit to initiate a disconnect on.
		 *	@param	timeout if 0, initiate an asynchronous disconnect. Otherwise, how long to wait for the circuit to
		 *			close, in ms.
		 *
		 *	@retval	STATUS_CODE_SUCCESS for an asynchronous disconnect, OnDisconnect() will be invoked when the circuit is down.
		 *	@retval	STATUS_CODE_SUCCESS for a synchronous disconnect, circuit is down.
		 *			Note data from this circuit may still be queued up on the destination endpoint, it is up to the
		 *			application to discard this data if required.
		 *	@retval	STATUS_CODE_TIMEOUT for a synchronous disconnect - circuit did not close in the timeout period.
		 *	@retval	STATUS_CODE_FAILURE ...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Disconnect(const Component &component, const Address &circuitAddress, const TimeoutPeriod timeout = 0);

		/**
		 *	@brief	Set heap characteristics for a connection.
		 *
		 *	@param	component component to send the message from.
		 *	@param	address the address of a circuit.
		 *	@param	pHeap heap to use for received data on this connection.
		 *	@param	maxTxQueueBytes max bytes to allow on tx queue before breaking connection (0 is no limit).
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode SetHeap(const Component &component, const Address &address, const IHeap * const pHeap, const size_t maxTxQueueBytes);
	}; // Tcp

	/**
	 *	@brief	Udp related apis.
	 */
	class Udp
	{
	public:
		/**
		 *	@brief	Create a socket and bind it to a local address.
		 *
		 *	@param	component component to send the message from.
		 *	@param	localAddress local address to bind to.
		 *	@param	localPort local port to bind to.
		 *	@param	maxRxSize maximum size of a datagram that can be received.
		 *	@param	socketAddress reference to an Address object to receive the address of the socket.
		 *	@param	pHeap the heap to use for received messages. Default is this component's heap.
		 *
		 *	@retval	STATUS_CODE_SUCCESS.
		 *	@retval	STATUS_CODE_TIMEOUT.
		 *	@retval	STATUS_CODE_FAILURE ...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Create(const Component &component, const std::string &localAddress,
													 const uint16_t localPort, const uint16_t maxRxSize,
													 Address &socketAddress, IHeap * const pHeap = 0);

		/**
		 *	@brief	Initiate a read on a udp socket.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to connect.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Read(const Component &component, const Address &socketAddress);

		/**
		 *	@brief	Connect a udp socket to a remote address (filter all traffic from other addresses).
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to connect.
		 *	@param	remoteAddress the remote address to connect to in string form.
		 *	@param	remotePort the remote port to connect to (local endian).
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Connect(const Component &component, const Address &socketAddress,
													  const std::string &remoteAddressString,
													  const uint16_t remotePort);

		/**
		 *	@brief	Disconnect a udp socket from a remote address (allow all traffic).
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to disconnect.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Disconnect(const Component &component, const Address &socketAddress);

		/**
		 *	@brief	Close a udp socket.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to shutdown.
		 *	@param	gracefully true to only close the socket once any outstanding tx has completed,
		 *			false to force the socket closed immediately.
		 *
		 *	@retval	STATUS_CODE_SUCCESS the socket is fully closed.
		 *	@retval	STATUS_CODE_PENDING the socket is closing and OnSocketClose() will be called.
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode Close(const Component &component, const Address &socketAddress, const bool gracefully = false);

		/**
		 *	@brief	Join a multicast group.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to join the multicast group on.
		 *	@param	multicastGroupString an ipv4 or ipv6 multicast address string.
		 *	@param	interfaceAddressString an ipv4 or ipv6 interface address string.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode JoinMulticastGroup(const Component &component,
																 const Address &socketAddress,
																 const std::string &multicastGroupString,
																 const std::string &interfaceAddressString);

		/**
		 *	@brief	Leave a multicast group.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to leave the multicast group.
		 *	@param	multicastGroupString an ipv4 or ipv6 multicast address string.
		 *	@param	interfaceAddressString an ipv4 or ipv6 interface address string.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode LeaveMulticastGroup(const Component &component,
																  const Address &socketAddress,
																  const std::string &multicastGroupString,
																  const std::string &interfaceAddressString);

		/**
		 *	@brief	Set multicast interface for transmission.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to set multicast interface on.
		 *	@param	interfaceAddressString an ipv4 or ipv6 address string.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode SetMulticastTxInterface(const Component &component, const Address &socketAddress,
																	  const std::string &interfaceAddressString);

		/**
		 *	@brief	Set TTL to be used for multicasting.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to set the ttl on.
		 *	@param	ttl the TTL.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode SetMulticastTtl(const Component &component, const Address &socketAddress, const uint32_t ttl);

		/**
		 *	@brief	Set TTL to be used for unicasting.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to set unicast ttl on.
		 *	@param	ttl the TTL.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode SetUnicastTtl(const Component &component, const Address &socketAddress, const uint32_t ttl);

		/**
		 *	@brief	Set the internal rx buffer size of a socket.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to set receive buffer size.
		 *	@param	size the rx buffer size.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode SetRxBufferSize(const Component &component, const Address &socketAddress, const uint32_t size);

		/**
		 *	@brief	Set the internal tx buffer size of a socket.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to set transmit buffer size on.
		 *	@param	size the tx buffer size.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode SetTxBufferSize(const Component &component, const Address &socketAddress, const uint32_t size);

		/**
		 *	@brief	Set the receive multicast loopback option of a socket.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress address of the socket to set transmit buffer size on.
		 *	@param	enable true to enable receipt of multicast loopback data, else false.
		 *
		 *	@retval	...
	 	 */
		ACTIV_FRAMEWORK_API static StatusCode SetMulticastLoopback(const Component &component, const Address &socketAddress, const bool enable);

		/**
		 *	@brief	Get the underying socket handle.
		 *
		 *	Currently only supports udp sockets created by the router.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress the socket to get the handle of.
		 *	@param	socket reference to a SOCKET variable to receive the handle.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode GetSocketHandle(const Component &component, const Address &socketAddress, SOCKET &socket);

		/**
		 *	@brief	Do a blocking send of some data to a specified ip address.
		 *
		 *	@param	socket socket handle obtained with GetSocketHandle().
		 *	@param	pData pointer to data to send.
		 *	@param	dataLength length of data to send.
		 * 	@param	toAddress destination ip address.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_FAILURE
		 */
		ACTIV_FRAMEWORK_API static StatusCode SendTo(const SOCKET socket, const void *pData, size_t dataLength, const IpAddress &toAddress);

		/**
		 *	@brief	Do a blocking send of some data.
		 *
		 *	@param	socket socket handle obtained with GetSocketHandle().
		 *	@param	pData pointer to data to send.
		 *	@param	dataLength length of data to send.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_FAILURE
		 */
		ACTIV_FRAMEWORK_API static StatusCode Send(const SOCKET socket, const void *pData, size_t dataLength);

		/**
		 *	@brief	Post some data. This call does not block, the transmission is handled by the router service.
		 *
		 *	@param	component component to send the message from.
		 *	@param	socketAddress the socket to send the data on.
		 *	@param	heapMessage the message to send.
		 *
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API static StatusCode PostData(const Component &component, const Address &socketAddress, HeapMessage &heapMessage);
	}; // Udp
};

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_ROUTER_SERVICE_API_H)
