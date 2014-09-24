/**
 *	@file	Windows/TcpSocket.h
 *
 *	@brief	Header file for tcp socket class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_WINDOWS_TCP_SOCKET_H)
#define ACTIV_IP_WINDOWS_TCP_SOCKET_H

#include "ActivMiddleware/ActivIp/IpSocket.h"

#include <mswsock.h>

/**
 *	@brief	List of socket states.
 */
#define ACTIV_IP_SOCKET_STATE_LIST(d)	\
	d(STATE_INITIALIZED)				\
	d(STATE_W4_CONNECT)					\
	d(STATE_W4_ACCEPT)					\
	d(STATE_CONNECTED)					\
	d(STATE_W4_DISCONNECT)

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
		ACTIV_IP_API Stats(const TcpSocket &socket);

		State	m_state;										///< connection state
		bool	m_connectPosted;								///< connect posted
		bool	m_acceptPosted;									///< accept posted
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	manager the IpManager that owns the socket.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API TcpSocket(IpManager &manager);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_IP_API ~TcpSocket();

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
	ACTIV_IP_API void Connect(const char *remoteAddress, const uint16_t remotePort, const char *localAddress = 0, const uint16_t localPort = 0);

	/**
	 *	@brief	Initiate connection.
	 *
	 *	@param	remoteAddress the remote address to connect to.
	 *	@param	localAddress the local address to bind to.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API void Connect(const IpAddress &remoteAddress, const IpAddress &localAddress);

	/**
	 *	@brief	Post a connect to an already set up address.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API void PostConnect();

	/**
	 *	@brief	Post an accept.
	 *
	 *	@param	listener the server to accept from.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API void Accept(TcpListener &listener);

	/**
	 *	@brief	Close a connection.
	 *
	 *	@param	gracefully true to only close the socket once any outstanding tx has completed,
	 *			false to force the socket closed immediately.
	 *
	 *	@retval	true the socket is fully closed.
	 *	@retval	false a close event will be fired when the socket is fully closed (OnDisconnect()).
	 */
	ACTIV_IP_API bool Disconnect(const bool gracefully = false);

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
	ACTIV_IP_API static std::string StateToString(const State state);

protected:
	// IpSocket overrides
	ACTIV_IP_API virtual CompletionResult OnRxBuffers(const size_t dataLength);
	ACTIV_IP_API virtual CompletionResult OnTxBuffers(const size_t dataLength);

	/**
	 *	@brief	Connection has closed at the user's request.
	 */
	ACTIV_IP_API virtual void OnDisconnect();

	/**
	 *	@brief	Connection has broken (the user didn't initiate).
	 */
	ACTIV_IP_API virtual void OnBreak();

	/**
	 *	@brief	Connect has succeeded.
	 */
	ACTIV_IP_API virtual void OnConnect();

	/**
	 *	@brief	Connect has been aborted (with a Disconnect()) before it completed either successfully or unsuccessfully.
	 */
	ACTIV_IP_API virtual void OnConnectAborted();

	/**
	 *	@brief	Connect has failed.
	 *
	 *	@param	errorCode Win32 error code.
	 */
	ACTIV_IP_API virtual void OnConnectFailed(const uint32_t errorCode);

	/**
	 *	@brief	Accept has succeeded.
	 */
	ACTIV_IP_API virtual void OnAccept();

	/**
	 *	@brief	Accept has been aborted. Calling Disconnect() before an accept has completed will cause this event.
	 */
	ACTIV_IP_API virtual void OnAcceptAborted();

	/**
	 *	@brief	Accept has failed. Stopping the listener will cause this event on an accepting connection.
	 *
	 *	@param	errorCode Win32 error code.
	 */
	ACTIV_IP_API virtual void OnAcceptFailed(const uint32_t errorCode);

	State	m_state;											///< The state of the connection

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
	 *	@brief	Set up a client side socket.
	 *
	 *	@throw	Exception.
	 */
	void CreateClientSocket();

	/**
	 *	@brief	Set up an accepting socket.
	 *
	 *	@throw	Exception.
	 */
	void CreateAcceptSocket();

	/**
	 *	@brief	Complete set up of an accepting socket.
	 *
	 *	@throw	Exception.
	 */
	void CompleteCreateAcceptSocket();

	/**
	 *	@brief	Do final cleanup on the connection, including firing disconnection events.
	 */
	void Cleanup();

	ACTIV_IP_API virtual void HandleIoFailure(const bool gracefully);

	/**
	 *	@brief	Process connect completion.
	 */
	void ProcessConnectCompletion();

	/**
	 *	@brief	Process successful connect completion.
	 */
	void ProcessSuccessfulConnectCompletion();

	/**
	 *	@brief	Process accept completion.
	 */
	void ProcessAcceptCompletion();

	/**
	 *	@brief	Process successful accept completion.
	 */
	void ProcessSuccessfulAcceptCompletion();

	/**
	 *	@brief	Sync bsd thread info.
	 */
	class BsdThreadInfo
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	tcpSocket TcpSocket object initiating the operation.
		 *	@param	socket socket to operate on.
		 */
		BsdThreadInfo(TcpSocket &tcpSocket, const SOCKET socket);

		TcpSocket &	m_tcpSocket;
		SOCKET		m_socket;
	};

	/**
	 *	@brief	Sync bsd thread connect info.
	 */
	class ConnectThreadInfo : public BsdThreadInfo
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	tcpSocket TcpSocket object initiating the connect.
		 *	@param	socket socket to connect on.
		 *	@param	remoteAddress address to connect to.
		 */
		ConnectThreadInfo(TcpSocket &tcpSocket, const SOCKET socket, const IpAddress &remoteAddress);

		IpAddress	m_remoteAddress;
	};

	/**
	 *	@brief	Initiate a bsd sync connect.
	 *
	 *	@param	pConnectThreadInfo info for the connect.
	 */
	void BsdConnect(ConnectThreadInfo * const pConnectThreadInfo);

	/**
	 *	@brief	Thread function to invoke bsd connect.
	 */
	static unsigned WINAPI BsdConnectThread(void *p);

	/**
	 *	@brief	Initiate a bsd sync accept.
	 *
	 *	@param	pBsdThreadInfo info for the accept.
	 */
	void BsdAccept(BsdThreadInfo * const pBsdThreadInfo);

	/**
	 *	@brief	Thread function to invoke bsd accept.
	 */
	static unsigned WINAPI BsdAcceptThread(void *p);

	/**
	 *	@brief	Overlapped class for connects.
	 */
	class ConnectOverlapped : public IoCompletionManager::Overlapped
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	tcpSocket TcpSocket containing this object.
		 */
		ConnectOverlapped(TcpSocket &tcpSocket);

	private:
		virtual void OnIoCompletion();

		TcpSocket &m_tcpSocket;
	};

	/**
	 *	@brief	Overlapped class for accepts.
	 */
	class AcceptOverlapped : public IoCompletionManager::Overlapped
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	tcpSocket TcpSocket containing this object.
		 */
		AcceptOverlapped(TcpSocket &tcpSocket);

	private:
		virtual void OnIoCompletion();

		TcpSocket &m_tcpSocket;
	};

	// client side members
	ConnectOverlapped			m_connectOverlapped;					///< Overlapped info for async connect
	LPFN_CONNECTEX				m_pConnectEx;							///< Pointer to ConnectEx() function
	IpAddress					m_providedLocalAddress;					///< Provided local address

	// server side members
	static const size_t PADDED_ADDRESS_SIZE = sizeof(SOCKADDR_IN6) + 16;
	static const size_t ACCEPT_BUFFER_SIZE = 2 * PADDED_ADDRESS_SIZE;

	AcceptOverlapped			m_acceptOverlapped;						///< Overlapped info for async accept
	SOCKET						m_listeningSocket;						///< The server socket that will accept
	byte_t						m_acceptBuffer[ACCEPT_BUFFER_SIZE];		///< Buffer for addresses
	LPFN_ACCEPTEX				m_pAcceptEx;							///< Pointer to AcceptEx() function
	LPFN_GETACCEPTEXSOCKADDRS	m_pGetAcceptExSockAddrs;				///< Pointer to GetAcceptExSockAddrs() function
};

} // namespace Activ

#endif // !defined (ACTIV_IP_WINDOWS_TCP_SOCKET_H)
