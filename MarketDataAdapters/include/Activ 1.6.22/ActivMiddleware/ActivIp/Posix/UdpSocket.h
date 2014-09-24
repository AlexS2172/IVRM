/**
 *	@file	UdpSocket.h
 *
 *	@brief	Header file for udp socket class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_POSIX_UDP_SOCKET_H)
#define ACTIV_IP_POSIX_UDP_SOCKET_H

#include "ActivMiddleware/ActivIp/IpSocket.h"

namespace Activ
{

/**
 *	@brief	UDP socket class.
 */
class UdpSocket : public IpSocket
{
public:
	/**
	 *	@brief	States.
	 */
	#define ACTIV_UDP_SOCKET_STATE_LIST(d)	\
		d(STATE_INITIALIZED)				\
		d(STATE_CREATED)					\
		d(STATE_CONNECTED)					\
		d(STATE_W4_CLOSE)
	
	/**
	 *	@brief	Creation flags.
	 */
	typedef uint32_t Flags;
	static const Flags FLAG_NONE = 0x00000000;
	static const Flags FLAG_BLOCKED_WRITE = 0x00000001;

	/**
	 *	@brief	States.
	 */
	enum State
	{
		ACTIV_UDP_SOCKET_STATE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)
		NUM_STATES
	};

	/**
	 *	@brief	Stats.
	 */
	ACTIV_DECLARE_NESTED_CLASS(Stats) : public IpSocket::Stats
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	Note if constructing a Stats object directly, you the socket's mutex must be locked (this isn't
		 *	done by the Stats constructor). Alternatively, use the helper function IpSocket::Stats::Get() which will
		 *	lock the mutex for you before gathering the stats.
		 *
		 *	@param	socket to get stats from.
		 */
		Stats(const UdpSocket &socket);

		State	m_state;										///< Udp socket state
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	ipManager the IpManager that owns the socket.
	 *	@param	flags creation flags.
	 *
	 *	@throw	Exception.
	 */
	UdpSocket(IpManager &ipManager, const Flags flags = FLAG_NONE);

	/**
	 *	@brief	Destructor.
	 *
	 *	Note a socket is only closed in the destructor.
	 */
	~UdpSocket();

	/**
	 *	@brief	Create and bind a socket.
	 *
	 *	@param	localAddressString the local address to bind to in string form.
	 *	@param	localPort the local port number to bind to (local endian).
	 *
	 *	@throw	Exception.
	 */
	void Initialize(const std::string &localAddressString, const uint16_t localPort);

	/**
	 *	@brief	Create and bind a socket.
	 *
	 *	@param	localAddress the local address to bind to.
	 *
	 *	@throw	Exception.
	 */
	void Initialize(const IpAddress &localAddress);

	/**
	 *	@brief	Connect to a remote address (filter all traffic from other addresses).
	 *
	 *	@param	remoteAddressString the remote address to connect to in string form.
	 *	@param	remotePort the remote port to connect to (local endian).
	 *
	 *	@throw	Exception.
	 */
	void Connect(const std::string &remoteAddressString, const uint16_t remotePort);

	/**
	 *	@brief	Connect to a remote address (filter all traffic from other addresses).
	 *
	 *	@param	remoteAddress the remote address to connect to.
	 *
	 *	@throw	Exception.
	 */
	void Connect(const IpAddress &remoteAddress);

	/**
	 *	@brief	Disconnect from a remote address (allow all traffic).
	 *
	 *	@throw	Exception.
	 */
	void Disconnect();

	/**
	 *	@brief	Shutdown the socket.
	 *
	 *	@param	gracefully true to only close the socket once any outstanding tx has completed,
	 *			false to force the socket closed immediately.
	 *
	 *	@retval	true the socket is fully closed.
	 *	@retval	false a close event will be fired when the socket is fully closed (OnShutdown()).
	 */
	bool Shutdown(const bool gracefully = false);
	
	/**
	 *	@brief	Initiate an abortive close on the socket.
	 *
	 *	This will result in an OnError() if the socket
	 *	was open and not shutting down "cleanly" as the result of a Shutdown() call.
	 */
	void AbortiveClose();

	/**
	 *	@brief	Join a multicast group.
	 *
	 *	@param	multicastGroupString an ipv4 or ipv6 multicast address string.
	 *	@param	interfaceAddressString an ipv4 or ipv6 interface address string.
	 *
	 *	@throw	Exception.
	 */
	void JoinMulticastGroup(const std::string &multicastGroupString, const std::string &interfaceAddressString);

	/**
	 *	@brief	Leave a multicast group.
	 *
	 *	@param	multicastGroupString an ipv4 or ipv6 multicast address string.
	 *	@param	interfaceAddressString an ipv4 or ipv6 interface address string.
	 *
	 *	@throw	Exception.
	 */
	void LeaveMulticastGroup(const std::string &multicastGroupString, const std::string &interfaceAddressString);

	/**
	 *	@brief	Set multicast interface for transmission.
	 *
	 *	@param	interfaceAddressString an ipv4 or ipv6 address string.
	 *
	 *	@throw	Exception.
	 */
	void SetMulticastTxInterface(const std::string &interfaceAddressString);

	/**
	 *	@brief	Set TTL to be used for multicasting.
	 *
	 *	@param	ttl the TTL.
	 *
	 *	@throw	Exception.
	 */
	void SetMulticastTtl(const int ttl);

	/**
	 *	@brief	Set TTL to be used for unicasting.
	 *
	 *	@param	ttl the TTL.
	 *
	 *	@throw	Exception.
	 */
	void SetUnicastTtl(const int ttl);

	/**
	 *	@brief	Set multicast loopback.
	 *
	 *	@param	isEnabled true to enable, false to disable.
	 *
	 *	@throw	Exception.
	 */
	void SetMulticastLoopback(const bool isEnabled);

	/**
	 *	@brief	Return state as string.
	 *
	 *	@return	Socket state in string form.
	 */
	static std::string StateToString(const State state);

	/**
	 *	@brief	Get socket stats.
	 *
	 *	@param	stats reference to a Stats object to receive current stats.
	 */
	void GetStats(Stats &stats) const;
	
	/**
	 *	@brief	Get socket handle.
	 *
	 *	@return Socket handle.
	 */
	SOCKET GetSocketHandle() const;

protected:
	/**
	 *	@brief	Socket has closed due to an error.
	 */
	virtual void OnError();

	/**
	 *	@brief	Socket has closed after a Shutdown() call.
	 */
	virtual void OnShutdown();

	State		m_state;										///< The state of the socket

private:
	/**
	 *	@brief	Set up a socket.
	 *
	 *	@param	socket reference to a SocketWrapper object to receive the socket.
	 *
	 *	@throw	Exception.
	 */
	void CreateSocket();
	
	/**
	 *	@brief	Initiate an close on the socket.
	 *
	 *	@retval	true the socket is fully closed.
	 *	@retval	false a close event will be fired when the socket is fully closed (OnShutdown()).
	 */
	bool InternalClose();

	// IpSocket overrides
	virtual void HandleIoFailure(const int errorCode);
	virtual void OnEvents(const Events events);

	/**
	 *	@brief	Process close completion.
	 */
	void ProcessCloseCompletion();

	/**
	 *	@brief	Join or leave an ipv4 or ipv6 multicast group.
	 *
	 *	@param	multicastGroupString an ipv4 or ipv6 multicast address string.
	 *	@param	interfaceAddressString an ipv4 or ipv6 interface address string.
	 *	@param	join true to join the group, false to leave.
	 *
	 *	@throw	Exception.
	 */
	void MulticastGroupAction(const std::string &multicastGroupString, const std::string &interfaceAddressString, const bool join);

	/**
	 *	@brief	Join or leave an ipv4 multicast group.
	 *
	 *	@param	multicastAddress the multicast group.
	 *	@param	interfaceAddress the interface address.
	 *	@param	join true to join the group, false to leave.
	 *
	 *	@throw	Exception.
	 */
	void Ipv4MulticastGroupAction(const IpAddress &multicastAddress, const IpAddress &interfaceAddress, const bool join);

	/**
	 *	@brief	Join or leave an ipv6 multicast group.
	 *
	 *	@param	multicastAddress the multicast group.
	 *	@param	interfaceAddress the interface address.
	 *	@param	join true to join the group, false to leave.
	 *
	 *	@throw	Exception.
	 */
	void Ipv6MulticastGroupAction(const IpAddress &multicastAddress, const IpAddress &interfaceAddress, const bool join);

	/**
	 *	@brief	Set the multicast tx interface for an ipv4 address.
	 *
	 *	@param	interfaceAddress the local interface address.
	 *
	 *	@throw	Exception.
	 */
	void SetIpv4MulticastTxInterface(const IpAddress &interfaceAddress);

	/**
	 *	@brief	Set the multicast tx interface for an ipv6 address.
	 *
	 *	@param	interfaceAddress the local interface address.
	 *
	 *	@throw	Exception.
	 */
	void SetIpv6MulticastTxInterface(const IpAddress &interfaceAddress);

	const Flags	m_flags;										///< Flags
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline SOCKET UdpSocket::GetSocketHandle() const
{
	return m_socket;
}

} // namespace Activ

#endif // !defined (ACTIV_IP_POSIX_UDP_SOCKET_H)
