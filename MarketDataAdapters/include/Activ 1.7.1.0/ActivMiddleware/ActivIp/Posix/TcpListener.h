/**
 *	@file	Posix/TcpListener.h
 *
 *	@brief	Header file for tcp listener class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_POSIX_TCP_LISTENER_H)
#define ACTIV_IP_POSIX_TCP_LISTENER_H

#include "ActivMiddleware/ActivIp/Posix/EventManagee.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivIp/IpManager.h"
#include "ActivMiddleware/ActivIp/SocketWrapper.h"

#include "ActivMiddleware/ActivBase/MutexHelper.h"

#include <list>

namespace Activ
{

class TcpSocket;

/**
 *	@brief	TCP listener class.
 */
class TcpListener : public EventManagee
{
public:
	/**
	 *	@brief	List of states.
	 */
	#define ACTIV_TCP_LISTENER_STATE_LIST(d)	\
		d(STATE_NOT_LISTENING)					\
		d(STATE_LISTENING)						\
		d(STATE_CLOSING)
	
	/**
	 *	@brief	Listening states.
	 */
	enum State
	{
		ACTIV_TCP_LISTENER_STATE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)
		NUM_STATES
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	ipManager the IpManager object owning the listening socket.
	 *
	 *	@throw	Exception.
	 */
	TcpListener(IpManager &ipManager);

	/**
	 *	@brief	Destructor.
	 */
	~TcpListener();

	/**
	 *	@brief	Start listening.
	 *
	 *	@param	localPort port to listen on.
	 *	@param	localAddress optional interface to bind to.
	 *	@param	backlog maximum length of the queue of pending connections.
	 *
	 *	@throw	Exception.
	 */
	void Start(const uint16_t localPort, const char *localAddress = 0, const int backlog = SOMAXCONN);

	/**
	 *	@brief	Start listening.
	 *
	 *	@param	localAddress address to listen on.
	 *	@param	backlog maximum length of the queue of pending connections.
	 *
	 *	@throw	Exception.
	 */
	void Start(const IpAddress &localAddress, const int backlog = SOMAXCONN);

	/**
	 *	@brief	Stop listening.
	 *
	 *	@return	true if the lsock was closed, else false if OnStop() will be invoked later when the lsock was closed.
	 *
	 *	@throw	Exception.
	 */
	bool Stop();

	/**
	 *	@brief	Get the local address of the listening socket.
	 *
	 *	@param	ipAddress a reference to an IpAddress object to receive the local socket address.
	 */
	void GetLocalAddress(IpAddress &ipAddress) const;
	
	/**
	 *	@brief	Get the listener state.
	 *
	 *	@return	The current state.
	 */
	State GetState() const;

	/**
	 *	@brief	Return state in string form.
	 *
	 *	@return	State in string form.
	 */
	static std::string StateToString(const State state);
	
protected:
	/**
	 *	@brief	A previous Stop() has completed and the lsock is closed.
	 */
	virtual void OnStop();

private:
	/**
	 *	@brief	Add a socket to the accept queue.
	 *
	 *	@param	socket the socket to add.
	 *
	 *	@retval	STATUS_CODE_SUCCESS added.
	 *	@retval	STATUS_CODE_WRONG_STATE not listening.
	 */
	StatusCode AddToQueue(TcpSocket &socket);
	
	/**
	 *	@brief	Remove a socket from the accept queue.
	 *
	 *	@param	socket the socket to remove.
	 */
	void RemoveFromQueue(TcpSocket &tcpSocket);

	// EventManagee overrides
	virtual void OnEvents(const Events events);
	
	/**
	 *	@brief	Process an accept completion.
	 */
	void ProcessAcceptEvent();
	
	/**
	 *	@brief	Process a Stop() completion.
	 */
	void ProcessCloseEvent();

	typedef std::list<TcpSocket *> AcceptQueue;
	
	IpManager &		m_ipManager;								///< This object is managed by an IpManager
	State			m_state;									///< State of the listening socket
	IpAddress		m_localAddress;								///< Local bind address

	AcceptQueue		m_acceptQueue;								///< A list of connections we can accept on
	
	friend class TcpSocket;
};

} // namespace Activ

#endif // !defined (ACTIV_IP_POSIX_TCP_LISTENER_H)
