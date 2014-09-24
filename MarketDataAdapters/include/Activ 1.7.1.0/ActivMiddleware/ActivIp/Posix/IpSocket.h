/**
 *	@file	Posix/IpSocket.h
 *
 *	@brief	Header file for ip socket class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_IP_POSIX_IP_SOCKET_H)
#define ACTIV_IP_POSIX_IP_SOCKET_H

#include "ActivMiddleware/ActivIp/Posix/EventManagee.h"
#include "ActivMiddleware/ActivIp/Posix/IoVecHelper.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivIp/IpManager.h"

#include "ActivMiddleware/ActivBase/MutexHelper.h"
#include "ActivMiddleware/ActivBase/ActivBase.h"

namespace Activ
{

/**
 *	@brief	Base ip socket class.
 */
class IpSocket : public EventManagee
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
	 *	@brief	Return values from On(Rx/Tx)Buffers().
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
		 *	Note if constructing a Stats object directly, you the socket's mutex must be locked (this isn't
		 *	done by the Stats constructor). Alternatively, use the helper function IpSocket::Stats::Get() which will
		 *	lock the mutex for you before gathering the stats.
		 *
		 *	@param	socket to get stats from.
		 */
		Stats(const IpSocket &socket);

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
	 *	@param	socketType socket type.
	 *
	 *	@throw	Exception.
	 */
	IpSocket(IpManager &ipManager, const int socketType);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~IpSocket();

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
	 *	@param	address a reference to an IpAddress object to receive the local socket address.
	 */
	void GetLocalAddress(IpAddress &ipAddress) const;

	/**
	 *	@brief	Get the remote address of the socket.
	 *
	 *	@param	address a reference to an IpAddress object to receive the remote socket address.
	 */
	void GetRemoteAddress(IpAddress &ipAddress) const;

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
    void SetRxBufferSize(const int size);

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
    void SetTxBufferSize(const int size);

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
	void SetSocketOption(const int optionName, const char *pOptionValue, const socklen_t optionLength, const int level = SOL_SOCKET);

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
	void GetSocketOption(const int optionName, char *pOptionValue, socklen_t *pOptionLength, const int level = SOL_SOCKET);

	/**
	 *	@brief	Get socket stats.
	 *
	 *	@param	stats reference to a Stats object to receive current stats.
	 */
	void GetStats(Stats &stats) const;
	
protected:
	/**
	 * 	@brief	Read/write return values.
	 */
	enum IoResult
	{
		IO_RESULT_SUCCESS,
		IO_RESULT_WOULD_BLOCK,
		IO_RESULT_CLEAR_EVENT,
		IO_RESULT_CLOSE_SOCKET
	};
	
	/**
	 *	@brief	Overridable socket options.
	 *
	 *	Use SetSocketOption() in an overridden OnSetSocketOptions().
	 *
	 *	@throw	Exception if the socket option is not set correctly.
	 */
	virtual void OnSetSocketOptions();

	/**
	 *	@brief	Callback to initialize rx / tx state.
	 *
	 *	For example, this will be called when a tcp connection is established.
	 *
	 *	@retval	true if successful.
	 *	@retval	false if unsuccessful and the socket will be closed.
	 */
	virtual bool OnInitializeIo();

	/**
	 *	@brief	Callback to cleanup rx / tx state.
	 *
	 *	For example, this will be called when a tcp connection is closed.
	 */
	virtual void OnCleanupIo();

	/**
	 *	@brief	Callback to get rx buffers.
	 *
	 *	In an override of this, call AppendRxBuffer() to attach a buffer to read into.
	 *
	 *	@retval	GET_BUFFERS_RESULT_CLOSE_SOCKET failed to get buffers to read into, close socket
	 *	@retval	GET_BUFFERS_RESULT_NO_MORE_DATA all rx buffers have been appended
	 */
	virtual GetBuffersResult OnGetRxBuffers();

	/**
	 *	@brief	Callback on rx completion.
	 *
	 *	An override of this should call the base class first and check the return value.
	 *
	 *	@param	dataLength the number of bytes transferred.
	 *
	 *	@retval	COMPLETION_RESULT_CLOSE_SOCKET socket should be closed
	 *	@retval	COMPLETION_RESULT_OK
	 */
	virtual CompletionResult OnRxBuffers(const size_t dataLength);

	/**
	 *	@brief	Callback to get tx buffers.
	 *
	 *	In an override of this, call AppendTxBuffer() to attach a buffer to write to the socket.
	 *
	 *	@retval	GET_BUFFERS_RESULT_CLOSE_SOCKET failed to get buffers to write from, close socket
	 *	@retval	GET_BUFFERS_RESULT_NO_MORE_DATA all tx buffers have been appended
	 */
	virtual GetBuffersResult OnGetTxBuffers();

	/**
	 *	@brief	Callback on tx completion.
	 *
	 *	An override of this should call the base class first and check the return value.
	 *
	 *	@param	dataLength the number of bytes transferred.
	 *
	 *	@retval	COMPLETION_RESULT_CLOSE_SOCKET socket should be closed
	 *	@retval	COMPLETION_RESULT_OK
	 */
	virtual CompletionResult OnTxBuffers(const size_t dataLength);

	/**
	 *	@brief	Post a read on a socket.
	 */
	void PostRead();
	
	/**
	 *	@brief	Post a write on a socket.
	 */
	void PostWrite();

	/**
	 *	@brief	Abortive close on a socket if it hasn't already been closed.
	 *
	 *	@return	true if the user had requested the close (m_closePosted was true).
	 */
	bool CloseSocket();

	/**
	 *	@brief	Handle an io failure.
	 *
	 *	@param	errorCode errno causing the close.
	 */
	virtual void HandleIoFailure(const int errorCode) = 0;

	IpManager &		m_ipManager;								///< this object is managed by an IpManager

	IoVecHelper		m_rxBuffers;								///< iovec array for receive
	uint64_t		m_rxBytes;									///< total rx bytes

	IoVecHelper		m_txBuffers;								///< iovec array for transmit
	size_t			m_lastTxBytes;								///< number of bytes of total in m_txBuffers written
	uint64_t		m_txBytes;									///< total tx bytes

	IpAddress		m_remoteAddress;							///< remote address information
	IpAddress		m_localAddress;								///< local address information
	
	IpAddress		m_toAddress;								///< optional to address for connectionless sockets
	IpAddress		m_fromAddress;								///< optional from address for connectionless sockets

	bool			m_closePosted;								///< user has requested a close
	bool			m_closeOnTxCompletion;						///< and only close when there's no data to write

private:
	/**
	 *	@brief	Get buffers and do the actual read on the socket.
	 *
	 *	@retval	IO_RESULT_SUCCESS data read ok
	 *	@retval	IO_RESULT_WOULD_BLOCK reading would block, so wait for next rx event
	 *	@retval	IO_RESULT_CLEAR_EVENT user doesn't want to read more data
	 *	@retval	IO_RESULT_CLOSE_SOCKET failed to read, socket must be closed
	 */
	IoResult InitiateRead();
	
	/**
	 *	@brief	Get buffers and do the actual write to socket.
	 *
	 *	@retval	IO_RESULT_SUCCESS data written ok
	 *	@retval	IO_RESULT_WOULD_BLOCK sending would block, so wait for next tx event
	 *	@retval	IO_RESULT_CLEAR_EVENT no data to transmit, clear tx event and wait for user to add more data to tx queues
	 *	@retval	IO_RESULT_CLOSE_SOCKET failed to send, socket must be closed
	 */
	IoResult InitiateWrite();

	/**
	 *	@brief	Get buffers to read into.
	 *
	 *	@retval	IO_RESULT_SUCCESS
	 *	@retval	IO_RESULT_CLEAR_EVENT user doesn't want to read more data
	 *	@retval	IO_RESULT_CLOSE_SOCKET
	 */
	IoResult GetRxBuffers();

	/**
	 *	@brief	Get buffers to write from.
	 *
	 *	@retval	IO_RESULT_SUCCESS
	 *	@retval	IO_RESULT_CLEAR_EVENT no data to transmit, clear tx event and wait for user to add more data to tx queues
	 *	@retval	IO_RESULT_CLOSE_SOCKET
	 */
	IoResult GetTxBuffers();
	
	const int		m_socketType;								///< type of socket in m_socket
	const size_t	m_maxIoVecs;								///< max iovecs to pass to send/recvmsg()

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
	if (STATUS_CODE_SUCCESS != AddEvents(EVENT_READ))
		LogMessage(LOG_TYPE_DEBUG, boost::format("Failed to set rx event, %s") % strerror(errno));
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
	if (STATUS_CODE_SUCCESS != AddEvents(EVENT_WRITE))
		LogMessage(LOG_TYPE_DEBUG, boost::format("Failed to set tx event, %s") % strerror(errno));
}

} // namespace Activ

#endif // (!defined ACTIV_IP_POSIX_IP_SOCKET_H)
