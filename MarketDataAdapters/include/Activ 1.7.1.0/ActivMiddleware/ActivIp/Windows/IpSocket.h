/**
 *	@file	Windows/IpSocket.h
 *
 *	@brief	Header file for ip socket class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_WINDOWS_IP_SOCKET_H)
#define ACTIV_IP_WINDOWS_IP_SOCKET_H

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivIp/IpManager.h"
#include "ActivMiddleware/ActivIp/Windows/WsaBufHelper.h"
#include "ActivMiddleware/ActivIp/SocketWrapper.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivBase/MutexHelper.h"

namespace Activ
{

/**
 *	@brief	Base ip socket class.
 */
class IpSocket
{
public:
	/**
	 *	@brief	Return values from OnGet(Rx/Tx)Buffers().
	 */
	enum GetBuffersResult
	{
		GET_BUFFERS_RESULT_CLOSE_SOCKET,						///< major error, close socket
		GET_BUFFERS_RESULT_NO_MORE_BUFFERS						///< no buffers available, but keep socket alive
	};

	/**
	 *	@brief	Return values from OnCompletion().
	 */
	enum CompletionResult
	{
		COMPLETION_RESULT_CLOSE_SOCKET,							///< major error, close socket (eg out of protocol data)
		COMPLETION_RESULT_OK									///< completion data was OK
	};

	/**
	 *	Stats.
	 */
	class Stats
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	Note if constructing a Stats object directly, the socket's mutex must be locked (this isn't
		 *	done by the Stats constructor). Alternatively, use the helper function IpSocket::Stats::Get() which will
		 *	lock the mutex for you before gathering the stats.
		 *
		 *	@param	socket to get stats from.
		 */
		ACTIV_IP_API Stats(const IpSocket &socket);

		/**
		 *	@brief	Helper function to get stats for a socket.
		 *
		 *	This is templatized by a derived socket type and locks the socket before gathering the stats.
		 *
		 *	@param	socket socket to get stats for.
		 *
		 *	@return	Stats for socket.
		 */
		template <typename SOCKET_TYPE>
		static typename SOCKET_TYPE::Stats Get(const SOCKET_TYPE &socket);

		IpAddress	m_remoteAddress;							///< remote address information
		IpAddress	m_localAddress;								///< local address information

		bool		m_rxPosted;									///< read outstanding
		bool		m_txPosted;									///< write outstanding

		bool		m_closePosted;								///< user has requested a close
		bool		m_closeOnTxCompletion;						///< close gracefully when there is no more data to transmit

		uint64_t	m_rxBytes;									///< total bytes received
		uint64_t	m_txBytes;									///< total bytes transmitted

		size_t		m_mutexContentionCount;						///< approx. number of contentions on the mutex
	};

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	ipManager the IpManager that owns the socket.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API IpSocket(IpManager &ipManager);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_IP_API virtual ~IpSocket();

	/**
	 *	@brief	Read some data.
	 *
	 *	Generally a read will always be outstanding on a socket, unless an rx handler returns 0 buffers
	 *	from OnGetRxBuffers(), in which case LockedRead() here needs to be called at some point to start reading from the
	 *	socket again.
	 *
	 *	Note this function will obtain the socket mutex for the duration of the call.
	 */
	void LockedRead();

	/**
	 *	@brief	Read some data without obtaining the socket mutex.
	 *
	 *	As LockedRead(), but do not obtain the socket mutex first (assumes the mutex is already locked).
	 */
	void PreLockedRead();

	/**
	 *	@brief	Write some data.
	 *
	 *	If no write event is set for the socket, this will set one. At some point the event thread will callback OnGetTxBuffers().
	 *
	 *	Note this function will obtain the socket mutex for the duration of the call.
	 */
	void LockedWrite();

	/**
	 *	@brief	Write some data without obtaining the socket mutex.
	 *
	 *	As LockedWrite(), but do not obtain the socket mutex first (assumes the mutex is already locked).
	 */
	void PreLockedWrite();

	/**
	 *	@brief	Append an rx buffer.
	 *
	 *	@param	pData pointer to the buffer to read into.
	 *	@param	dataLength the length the buffer.
	 *
	 *	@throw	Exception if the buffer wasn't appended to the rx array.
	 */
	void AppendRxBuffer(void *pData, const size_t dataLength);

	/**
	 *	@brief	Append a tx buffer.
	 *
	 *	@param	pData pointer to the buffer to transmit.
	 *	@param	dataLength the length the buffer.
	 *
	 *	@throw	Exception if the buffer wasn't appended to the tx array.
	 */
	void AppendTxBuffer(const void *pData, const size_t dataLength);

	/**
	 *	@brief	Get the local address of the socket.
	 *
	 *	@param	ipAddress a reference to an IpAddress object to receive the local socket address.
	 */
	ACTIV_IP_API void GetLocalAddress(IpAddress &ipAddress) const;

	/**
	 *	@brief	Get the remote address of the socket.
	 *
	 *	@param	ipAddress a reference to an IpAddress object to receive the remote socket address.
	 */
	ACTIV_IP_API void GetRemoteAddress(IpAddress &ipAddress) const;

	/**
	 *	@brief	Set the internal rx buffer size.
	 *
	 *	This would most likely be used in an overriden OnSetSocketOptions(). Note once setting the buffer size,
	 *	this call then retrieves the buffer size and checks that it was actually set to the value asked for. If not,
	 *	an exception is thrown.
	 *
	 *	@param	size the rx buffer size.
	 *
	 *	@throw	Exception.
	 */
    ACTIV_IP_API void SetRxBufferSize(const int size);

	/**
	 *	@brief	Set the internal tx buffer size.
	 *
	 *	This would most likely be used in an overriden OnSetSocketOptions(). Note once setting the buffer size,
	 *	this call then retrieves the buffer size and checks that it was actually set to the value asked for. If not,
	 *	an exception is thrown.
	 *
	 *	@param	size the tx buffer size.
	 *
	 *	@throw	Exception.
	 */
    ACTIV_IP_API void SetTxBufferSize(const int size);

	/**
	 *	@brief	Set a socket option.
	 *
	 *	@param	optionName Socket option for which the value is to be set.
	 *	@param	pOptionValue Pointer to the buffer in which the value for the requested option is supplied.
	 *	@param	optionLength Size of the \a pOptionValue buffer.
	 *	@param	level Level at which the option is defined.
	 *
	 *	@throw	Exception if the socket option is not set successfully.
	 */
	ACTIV_IP_API void SetSocketOption(const int optionName, const char *pOptionValue, const socklen_t optionLength, const int level = SOL_SOCKET);

	/**
	 *	@brief	Get a socket option.
	 *
	 *	@param	optionName Socket option for which the value is to be retrieved.
	 *	@param	pOptionValue Pointer to the buffer in which the value for the requested option is to be returned.
	 *	@param	pOptionLength Pointer to the size of the \a pOptionValue buffer.
	 *	@param	level Level at which the option is defined.
	 *
	 *	@throw	Exception if the socket option is not retrieved successfully.
	 */
	ACTIV_IP_API void GetSocketOption(const int optionName, char *pOptionValue, socklen_t *pOptionLength, const int level = SOL_SOCKET);

	/**
	 *	@brief	Get the socket handle.
	 *
	 *	@return	The socket handle.
	 */
	ACTIV_IP_API SOCKET GetSocketHandle() const;

protected:
	/**
	 *	@brief	Overridable socket options.
	 *
	 *	Use SetSocketOption() in an overridden OnSetSocketOptions().
	 *
	 *	@throw	Exception if the socket option is not set correctly.
	 */
	ACTIV_IP_API virtual void OnSetSocketOptions();

	/**
	 *	@brief	Callback to initialize rx / tx state.
	 *
	 *	@retval	true if successful.
	 *	@retval	false if unsuccessful and the socket will be closed.
	 */
	ACTIV_IP_API virtual bool OnInitializeIo();

	/**
	 *	@brief	Callback to cleanup rx / tx state.
	 *
	 *	For example, this will be called when a tcp connection is closed.
	 */
	ACTIV_IP_API virtual void OnCleanupIo();

	/**
	 *	@brief	Callback to get rx buffers.
	 */
	ACTIV_IP_API virtual GetBuffersResult OnGetRxBuffers();

	/**
	 *	@brief	Callback on rx completion.
	 *
	 *	When overriding this function, you MUST call the base class OnRxBuffers() first, and return
	 *	the result if it isn't COMPLETION_RESULT_SUCCESS.
	 *
	 *	@param	dataLength the number of bytes transferred.
	 */
	ACTIV_IP_API virtual CompletionResult OnRxBuffers(const size_t dataLength);

	/**
	 *	@brief	Callback to get tx buffers.
	 */
	ACTIV_IP_API virtual GetBuffersResult OnGetTxBuffers();

	/**
	 *	@brief	Callback on tx completion.
	 *
	 *	When overriding this function, you MUST call the base class OnTxBuffers() first, and return
	 *	the result if it isn't COMPLETION_RESULT_SUCCESS.
	 *
	 *	@param	dataLength the number of bytes transferred.
	 */
	ACTIV_IP_API virtual CompletionResult OnTxBuffers(const size_t dataLength);

	/**
	 *	@brief	Post a read on a socket.
	 *
	 *	@return	false if a read was not posted on the socket, in which case the socket should be closed.
	 */
	ACTIV_IP_API bool PostRead();

	/**
	 *	@brief	Post a write on a socket.
	 *
	 *	@return	false if a write was not posted on the socket, in which case the socket should be closed.
	 */
	ACTIV_IP_API bool PostWrite();

	/**
	 *	@brief	Invoke the OnCleanupIo() callback if no rx or tx is outstanding.
	 *
	 *	@return	true if all io is complete and rx and tx handlers are fully cleaned up.
	 */
	ACTIV_IP_API bool CleanupIo();

	/**
	 *	@brief	Abortive close on a socket if it hasn't already been closed.
	 */
	ACTIV_IP_API void CloseSocket();

	/**
	 *	@brief	Shutdown a socket.
	 *
	 *	@return	true if the shutdown was successful.
	 */
	ACTIV_IP_API bool ShutdownSocket();

	/**
	 *	@brief	Handle an io failure.
	 *
	 *	@param	gracefully whether to do a graceful shutdown on the socket, or an abortive close.
	 */
	virtual void HandleIoFailure(const bool gracefully) = 0;

	/**
	 *	@brief	Overlapped class for writes.
	 */
	class TxOverlapped : public IoCompletionManager::Overlapped
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	ipSocket IpSocket containing this object.
		 */
		TxOverlapped(IpSocket &ipSocket);

	private:
		virtual void OnIoCompletion();

		IpSocket &m_ipSocket;
	};

	/**
	 *	@brief	Overlapped class for reads.
	 */
	class RxOverlapped : public IoCompletionManager::Overlapped
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	ipSocket IpSocket containing this object.
		 */
		RxOverlapped(IpSocket &ipSocket);

	private:
		virtual void OnIoCompletion();

		IpSocket &m_ipSocket;
	};

	IpManager &		m_ipManager;								///< this object is managed by an IpManager

	IMutex * const	m_pMutex;									///< mutex
	SocketWrapper	m_socket;									///< the underlying socket

	RxOverlapped	m_rxOverlapped;								///< receive overlapped info
	WsaBufHelper	m_rxBuffers;								///< WSABUF array for receive
	DWORD			m_rxFlags;									///< flags for WSARecv()
	uint64_t		m_rxBytes;									///< total rx bytes

	TxOverlapped	m_txOverlapped;								///< transmission overlapped info
	WsaBufHelper	m_txBuffers;								///< WSABUF array for transmission
	DWORD			m_txFlags;									///< flags for WSASend()
	uint64_t		m_txBytes;									///< total tx bytes

	IpAddress		m_remoteAddress;							///< remote address information (if socket is connected)
	IpAddress		m_localAddress;								///< local address information

	IpAddress		m_toAddress;								///< sestination address for current write (connectionless)
	IpAddress		m_fromAddress;								///< source address for current read (connectionless)

	bool			m_closePosted;								///< user has requested a close
	bool			m_closeOnTxCompletion;						///< close gracefully when there is no more data to transmit

private:
	/**
	 *	@brief	Initiate a write operation.
	 *
	 *	@return	true ok, false socket will be closed.
	 */
	bool InitiateWrite();

	/**
	 *	@brief	Initiate a read operation.
	 *
	 *	@return	true ok, false socket will be closed.
	 */
	bool InitiateRead();

	/**
	 *	@brief	Process an overlapped write completion.
	 */
	void ProcessTxCompletion();

	/**
	 *	@brief	Process an overlapped read completion.
	 */
	void ProcessRxCompletion();

	friend class IpManager;										///< manager needs access to the objects it is managing
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpSocket::LockedRead()
{
	MutexHelper mutexHelper(m_pMutex);

	PreLockedRead();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpSocket::PreLockedRead()
{
	// if a read is currently still in progress, we can't do anything here. Once the read completes, PostRead()
	// will be called internally
	if (!m_rxOverlapped.IsPending())
	{
		if (!PostRead())
			HandleIoFailure(false);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpSocket::LockedWrite()
{
	MutexHelper mutexHelper(m_pMutex);

	PreLockedWrite();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IpSocket::PreLockedWrite()
{
	// if a write is currently still in progress, we can't do anything here. Once the write completes, PostWrite()
	// will be called and the data will be taken off the tx handler's queue and written to the socket in the context
	// of an io thread
	if (!m_txOverlapped.IsPending())
	{
		if (!PostWrite())
			HandleIoFailure(false);
	}
}

} // namespace Activ

#endif // !defined (ACTIV_IP_WINDOWS_IP_SOCKET_H)
