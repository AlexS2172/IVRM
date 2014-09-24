/**
 *	@file	SocketWrapper.h
 *
 *	@brief	Header file for socket wrapper class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_SOCKET_WRAPPER_H)
#define ACTIV_IP_SOCKET_WRAPPER_H

#include "ActivMiddleware/ActivIp/External.h"

namespace Activ
{

/**
 *	@brief	Socket wrapper class.
 *
 *	Allows cleaning up of a socket if an exception is thrown.
 */
class SocketWrapper
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	SocketWrapper();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	socket SOCKET handle to wrap.
	 */
	SocketWrapper(const SOCKET socket);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	If rhs has an attached socket, ownership is transferred to this.
	 *
	 *	@param	rhs SocketWrapper to copy from.
	 */
	SocketWrapper(SocketWrapper &rhs);

	/**
	 *	@brief	Destructor.
	 *
	 *	The destructor will close the underlying socket if it is still attached to this.
	 */
	~SocketWrapper();

	/**
	 *	@brief	Assignment operator. Any current attached socket is closed.
	 *
	 *	@param	socket SOCKET handle to wrap.
	 */
	SocketWrapper &operator=(const SOCKET socket);

	/**
	 *	@brief	Assignment operator. Any current attached socket is closed.
	 *
	 *	If rhs has an attached socket, ownership is transferred to this.
	 *
	 *	@param	rhs SocketWrapper to copy from.
	 */
	SocketWrapper &operator=(SocketWrapper &rhs);

	/**
	 *	@brief	Cast operator to SOCKET.
	 *
	 *	@return	Underlying socket.
	 */
	operator SOCKET() const;

#if (defined ACTIV_PLATFORM_WINDOWS)

	/**
	 *	@brief	Cast operator to HANDLE.
	 *
	 *	@return	Underlying socket.
	 */
	operator HANDLE() const;
	
#endif

	/**
	 *	@brief	Test if socket attached is valid.
	 *
	 *	@return	true if attached to a valid socket handle.
	 */
	bool IsValid() const;

private:
	/**
	 *	@brief	Attach a socket.
	 */
	void Attach(const SOCKET socket);

	SOCKET	m_socket;			///< socket handle.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline SocketWrapper::SocketWrapper() :
	m_socket(INVALID_SOCKET)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SocketWrapper::SocketWrapper(const SOCKET socket) :
	m_socket(socket)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SocketWrapper::SocketWrapper(SocketWrapper &rhs) :
	m_socket(INVALID_SOCKET)
{
	*this = rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SocketWrapper::~SocketWrapper()
{
	Attach(INVALID_SOCKET);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SocketWrapper &SocketWrapper::operator=(SocketWrapper &rhs)
{
	if (this == &rhs)
		return *this;

	*this = rhs.m_socket;
	rhs.m_socket = INVALID_SOCKET;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SocketWrapper &SocketWrapper::operator=(const SOCKET socket)
{
	Attach(socket);

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SocketWrapper::operator SOCKET() const
{
	return m_socket;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#if (defined ACTIV_PLATFORM_WINDOWS)

inline SocketWrapper::operator HANDLE() const
{
	return reinterpret_cast<HANDLE>(m_socket);
}

#endif

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool SocketWrapper::IsValid() const
{
	return (INVALID_SOCKET != m_socket);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void SocketWrapper::Attach(const SOCKET socket)
{
	if (IsValid())
		(void) ::closesocket(m_socket);

	m_socket = socket;
}

} // namespace Activ

#endif // !defined (ACTIV_IP_SOCKET_WRAPPER_H)
