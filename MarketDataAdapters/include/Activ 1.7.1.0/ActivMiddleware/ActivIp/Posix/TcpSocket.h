/**
 *	@file	Posix/TcpSocket.h
 *
 *	@brief	Header file for tcp socket class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_POSIX_TCP_SOCKET_H)
#define ACTIV_IP_POSIX_TCP_SOCKET_H

#include "ActivMiddleware/ActivIp/IpSocket.h"

namespace Activ
{

class TcpListener;

/**
 *	@brief	TCP socket class.
 */
class TcpSocket : public IpSocket
{
public:
	/**
	 *	@brief	List of tcp socket states.
	 */
	#define ACTIV_IP_SOCKET_STATE_LIST(d)	\
		d(STATE_INITIALIZED)				\
		d(STATE_W4_CONNECT)					\
		d(STATE_W4_ACCEPT)					\
		d(STATE_CONNECTED)					\
		d(STATE_W4_DISCONNECT)
	
	/**
	 *	@brief	Connection states.
	 */
	enum State
	{
		ACTIV_IP_SOCKET_STATE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)
		NUM_STATES
	};
		
	/**
	 *	@brief	Stats.
	 */
	class Stats : public IpSocket::Stats
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
		Stats(const TcpSocket &socket);

		State	m_state;										///< Connection state
		bool	m_connectPosted;								///< Connect posted
		bool	m_acceptPosted;									///< Accept posted
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	ipManager the IpManager that owns the socket.
	 *
	 *	@throw	Exception.
	 */
	TcpSocket(IpManager &ipManager);

	/**
	 *	@brief	Destructor.
	 */
	~TcpSocket();

	/**
	 *	@brief	Initiate connection.
	 *
	 *	@param	remoteAddress the remote address to connect to.
	 *	@param	remotePort the remote port to connect to (local endian).
	 *	@param	localAddress the optional local address to bind to.
	 *	@param	localPort the optional local port number to bind to (local endian).
	 *
	 *	@throw	Exception.
	 */
	void Connect(const char *remoteAddress, const uint16_t remotePort, const char *localAddress = 0, const uint16_t localPort = 0);

	/**
	 *	@brief	Initiate connection.
	 *
	 *	@param	remoteAddress the remote address to connect to.
	 *	@param	localAddress the local address to bind to.
	 *
	 *	@throw	Exception.
	 */
	void Connect(const IpAddress &remoteAddress, const IpAddress &localAddress);

	/**
	 *	@brief	Post a connect to an already set up address.
	 *
	 *	@throw	Exception.
	 */
	void PostConnect();

	/**
	 *	@brief	Post an accept.
	 *
	 *	@param	listener the server to accept from.
	 *
	 *	@throw	Exception.
	 */
	void Accept(TcpListener &listener);

	/**
	 *	@brief	Close a connection.
	 *
	 *	@param	gracefully true to only close the socket once any outstanding tx has completed,
	 *			false to force the socket closed immediately.
	 *
	 *	@retval	true the socket is fully closed.
	 *	@retval	false a close event will be fired when the socket is fully closed (OnDisconnect()).
	 *
	 *	@throw	Exception.
	 */
	bool Disconnect(const bool gracefully = false);

	/**
	 *	@brief	Forcably initiate a connection break. If the socket was connected, OnBreak() will be fired.
	 *
	 *	Note this function will obtain the socket mutex for the duration of the call.
	 */
	ACTIV_IP_API void LockedBreak();

	/**
	 *	@brief	Forcably initiate a connection break. If the socket was connected, OnBreak() will be fired.
	 *
	 *	As LockedBreak(), but do not obtain the socket mutex first (assumes the mutex is already locked).
	 */
	ACTIV_IP_API void PreLockedBreak();

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

protected:
	/**
	 *	@brief	Connection has closed at the user's request.
	 */
	virtual void OnDisconnect();

	/**
	 *	@brief	Connection has broken (the user didn't initiate).
	 */
	virtual void OnBreak();

	/**
	 *	@brief	Connect has succeeded.
	 */
	virtual void OnConnect();

	/**
	 *	@brief	Connect has been aborted (with a Disconnect()) before it completed either successfully or unsuccessfully.
	 */
	virtual void OnConnectAborted();

	/**
	 *	@brief	Connect has failed.
	 *
	 *	@param	errorCode os error code.
	 */
	virtual void OnConnectFailed(const uint32_t errorCode);

	/**
	 *	@brief	Accept has succeeded.
	 */
	virtual void OnAccept();

	/**
	 *	@brief	Accept has been aborted. Calling Disconnect() before an accept has completed will cause this event.
	 */
	virtual void OnAcceptAborted();

	/**
	 *	@brief	Accept has failed. Stopping the listener will cause this event on an accepting connection.
	 *
	 *	@param	errorCode os error code.
	 */
	virtual void OnAcceptFailed(const uint32_t errorCode);
	
	// IpSocket overrides
	virtual CompletionResult OnRxBuffers(const size_t dataLength);
	virtual CompletionResult OnTxBuffers(const size_t dataLength);

	State		m_state;										///< The state of the connection
	
private:
	/**
	 *	@brief	Post a connect to an already set up address.
	 *
	 *	NB mutex must already be locked.
	 *
	 *	@throw	Exception.
	 */
	void PreLockedPostConnect();

	/**
	 *	@brief	Initiate an accept on this socket.
	 *
	 *	@param	listener the TcpListener.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_WRONG_STATE
	 */
	StatusCode InitiateAccept(TcpListener &listener);
	
	/**
	 *	@brief	Initiate disconnect on socket with i/o outstanding.
	 *
	 *	@param	gracefully.
	 */
	void InitiateDisconnect(const bool gracefully);
	
	// IpSocket overrides
	virtual void HandleIoFailure(const int errorCode);

	// EventManagee overrides
	virtual void OnEvents(const Events events);

	/**
	 *	@brief	Process connect completion.
	 */
	void ProcessConnectCompletion();                      	

	/**
	 *	@brief	Process successful connect completion.
	 */
	void ProcessSuccessfulConnectCompletion();
	
	/**
	 *	@brief	Process disconnect completion.
	 */
	void ProcessDisconnectCompletion(); 

	/**
	 *	@brief	Process a server shutdown.
	 */
	void ProcessServerShutdown();
	
	/**
	 *	@brief	Process an accept.
	 *
	 *	@param	listeningSocket the listening socket.
	 *
	 *	@retval	STATUS_CODE_SUCCESS state was STATE_W4_ACCEPT and socket was processed
	 *	@retval	STATUS_CODE_WRONG_STATE a Disconnect() has been performed
	 *	@retval	STATUS_CODE_FAILURE failed to accept the connection
	 *	@retval	STATUS_CODE_PENDING accepting would block.
	 */
	StatusCode ProcessAccept(const SOCKET listeningSocket);
		
	// client side members
	IpAddress	m_providedLocalAddress;

	// server side members
	TcpListener	*m_pTcpListener;								///< The server that will accept

	friend class TcpListener;
};

} // namespace Activ

#endif // !defined (ACTIV_IP_POSIX_TCP_SOCKET_H)
