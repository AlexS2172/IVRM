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
#include "ActivMiddleware/ActivIp/IpManagedObject.h"
#include "ActivMiddleware/ActivIp/Windows/Overlapped.h"
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
		GET_BUFFERS_RESULT_CLOSE_SOCKET,			///< Major error, close socket
		GET_BUFFERS_RESULT_NO_MORE_BUFFERS,			///< No buffers available, but keep socket alive
	};

	/**
	 *	@brief	Return values from OnCompletion().
	 */
	enum CompletionResult
	{
		COMPLETION_RESULT_CLOSE_SOCKET,				///< Major error, close socket (eg out of protocol data)
		COMPLETION_RESULT_OK,						///< Completion data was OK
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

		IpAddress	m_remoteAddress;				///< Remote address information
		IpAddress	m_localAddress;					///< Local address information

		bool		m_rxPosted;						///< Read outstanding
		bool		m_txPosted;						///< Write outstanding

		bool		m_closePosted;					///< User has requested a close
		bool		m_closeOnTxCompletion;			///< Close gracefully when there is no more data to transmit
	};

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	manager the IpManager that owns the socket.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_IP_API IpSocket(IpManager &manager);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_IP_API virtual ~IpSocket();

	/**
	 *	@brief	Read some data.
	 *
	 *	Generally a read will always be outstanding on a socket, unless an rx handler returns 0 buffers
	 *	from OnGetRxBuffers(), in which case Read() here needs to be called at some point to start reading from the
	 *	socket again.
	 *
	 *	Note this function will obtain the socket mutex for the duration of the call.
	 */
	ACTIV_IP_API void LockedRead();

	/**
	 *	@brief	Read some data.
	 *
	 *	Generally a read will always be outstanding on a socket, unless an rx handler returns 0 buffers
	 *	from OnGetRxBuffers(), in which case Read() here needs to be called at some point to start reading from the
	 *	socket again.
	 *
	 *	There is potentially a small saving over using LockedRead() if you have a derived
	 *	socket function, e.g.
	 *				MySocket::MyRead()
	 *				{
	 *					MutexHelper mutexHelper(m_pMutex);
	 *					// something
	 *					IpSocket::NonLockedWrite(false);
	 *				}
	 *	In this case use NonLockedRead() as there is no need to obtain the mutex again, saving an interlocked operation.
	 */
	ACTIV_IP_API void NonLockedRead();

	/**
	 *	@brief	Write some data.
	 *
	 *	If a write is currently in progress, this will do nothing. Otherwise it will callback OnGetTxBuffers()
	 *	in order to retrieve some data to send.
	 *
	 *	Note this function will obtain the socket mutex for the duration of the call.
	 */
	ACTIV_IP_API void LockedWrite();

	/**
	 *	@brief	Write some data without obtaining the socket mutex.
	 *
	 *	If a write is currently in progress, this will do nothing. Otherwise it will callback OnGetTxBuffers()
	 *	in order to retrieve some data to send.
	 *
	 *	There is potentially a small saving over using LockedWrite() if you have a derived
	 *	socket function, e.g.
	 *				MySocket::MyWrite()
	 *				{
	 *					MutexHelper mutexHelper(m_pMutex);
	 *					// queue up some data
	 *					IpSocket::NonLockedWrite(false);
	 *				}
	 *	In this case use NonLockedWrite() as there is no need to obtain the mutex again, saving an interlocked operation.
	 */
	ACTIV_IP_API void NonLockedWrite();

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
	 *	@brief	Overlapped io has completed.
	 *
	 *	When overriding, always call the base class OnIoCompletion() first, and check the return value.
	 *	You only need to process the completion even if the base class doesn't know the overlapped object
	 *	(ie it returns false).
	 *
	 *	@param	pOverlapped pointer to the overlapped object that has completed.
	 *
	 *	@retval	true if the overlapped pointer was known.
	 *	@retval	false if the overlapped pointer was not known.
	 */
	ACTIV_IP_API virtual bool OnIoCompletion(Overlapped *pOverlapped);

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

	IpManagedObject	m_managedObject;							///< This object is managed by an IpManager

	IMutex * const	m_pMutex;									///< Mutex
	SocketWrapper	m_socket;									///< The underlying socket

	Overlapped		m_rxOverlapped;								///< Receive overlapped info
	WsaBufHelper	m_rxBuffers;								///< WSABUF array for receive
	DWORD			m_rxFlags;									///< Flags for WSARecv()

	Overlapped		m_txOverlapped;								///< Transmission overlapped info
	WsaBufHelper	m_txBuffers;								///< WSABUF array for transmission
	DWORD			m_txFlags;									///< Flags for WSASend()

	IpAddress		m_remoteAddress;							///< Remote address information (if socket is connected)
	IpAddress		m_localAddress;								///< Local address information

	IpAddress		m_toAddress;								///< Destination address for current write (connectionless)
	IpAddress		m_fromAddress;								///< Source address for current read (connectionless)

	bool			m_closePosted;								///< User has requested a close
	bool			m_closeOnTxCompletion;						///< Close gracefully when there is no more data to transmit

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

	friend class IpManager;										///< Manager needs access to the objects it is managing
};

} // namespace Activ

#endif // !defined (ACTIV_IP_WINDOWS_IP_SOCKET_H)
