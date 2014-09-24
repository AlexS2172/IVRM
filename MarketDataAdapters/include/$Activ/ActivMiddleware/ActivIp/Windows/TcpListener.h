/**
 *	@file	Windows/TcpListener.h
 *
 *	@brief	Header file for tcp listener class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_WINDOWS_TCP_LISTENER_H)
#define ACTIV_IP_WINDOWS_TCP_LISTENER_H

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivIp/IpManagedObject.h"
#include "ActivMiddleware/ActivIp/SocketWrapper.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivBase/MutexHelper.h"

/**
 *	@brief	List of states.
 */
#define ACTIV_TCP_LISTENER_STATE_LIST(d)	\
	d(STATE_NOT_LISTENING)					\
	d(STATE_LISTENING)

namespace Activ
{

/**
 *	@brief	TCP listener class.
 */
class TcpListener
{
public:
	/**
	 *	@brief	Listening states.
	 */
	enum State
	{
		ACTIV_TCP_LISTENER_STATE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	manager the IpManager object owning the listening socket.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API TcpListener(IpManager &manager);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_IP_API ~TcpListener();

	/**
	 *	@brief	Start listening.
	 *
	 *	@param	localPort port to listen on.
	 *	@param	localAddress optional interface to bind to.
	 *	@param	backlog maximum length of the queue of pending connections.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API void Start(const uint16_t localPort, const char *localAddress = 0, const int backlog = SOMAXCONN);

	/**
	 *	@brief	Start listening.
	 *
	 *	@param	localAddress address to listen on.
	 *	@param	backlog maximum length of the queue of pending connections.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API void Start(const IpAddress &localAddress, const int backlog = SOMAXCONN);

	/**
	 *	@brief	Stop listening.
	 *
	 *	@return	Always returns true.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API bool Stop();

	/**
	 *	@brief	Get the local address of the listening socket.
	 *
	 *	@param	ipAddress a reference to an IpAddress object to receive the local socket address.
	 */
	ACTIV_IP_API void GetLocalAddress(IpAddress &ipAddress) const;

	/**
	 *	@brief	Get the listener state.
	 *
	 *	@return	The current state.
	 */
	ACTIV_IP_API State GetState() const;

	/**
	 *	@brief	Return state in string form.
	 *
	 *	@return	State in string form.
	 */
	ACTIV_IP_API static std::string StateToString(const State state);

private:
	/**
	 *	@brief	Get the underlying listening socket.
	 *
	 *	@throw	Exception if the socket is not listening.
	 */
	SOCKET GetSocket() const;

	IpManagedObject	m_managedObject;							///< this object is managed by an IpManager
	IMutex * const	m_pMutex;									///< mutex
	State			m_state;									///< state of the listening socket
	SocketWrapper	m_socket;									///< the underlying listening socket
	IpAddress		m_localAddress;								///< local bind address

	friend class TcpSocket;
};

} // namespace Activ

#endif // !defined (ACTIV_IP_WINDOWS_TCP_LISTENER_H)
