/**
 *	@file	WsaBufHelper.h
 *
 *	@brief	Header file for WSABUF helper class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_WINDOWS_WSA_BUF_HELPER_H)
#define ACTIV_IP_WINDOWS_WSA_BUF_HELPER_H

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivBase/CrtException.h"

#include <cassert>

namespace Activ
{

/**
 *	@brief	WSABUF helper class.
 */
class ACTIV_IP_API WsaBufHelper
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	initialSize the number of WSABUFs to allocate initially.
	 *
	 *	@throw	Exception.
	 */
	WsaBufHelper(const size_t initialSize = 10);

	/**
	 *	@brief	Destructor.
	 */
	~WsaBufHelper();

	/**
	 *	@brief	Reset the number of buffers in use.
	 */
	void Reset();

	/**
	 *	@brief	Append a new buffer, expand the WSABUF array if required.
	 *
	 *	@brief	pData pointer to the data.
	 *	@brief	dataLength the length of the buffer.
	 *
	 *	@throw	Exception if the WSABUF couldn't be resized.
	 */
	void Append(const void *pData, const size_t dataLength);

	/**
	 *	@brief	Get a pointer to the first WSABUF.
	 *
	 *	@return	Pointer to the first WSABUF.
	 */
	WSABUF *GetFirstBuffer() const;

	/**
	 *	@brief	Get the number of buffers in use.
	 *
	 *	@return	The number of buffers in use.
	 */
	size_t GetNumberOfBuffersInUse() const;

	/**
	 *	@brief	Get the total number of bytes in the WSABUF arary.
	 *
	 *	@return	The total number of bytes in the WSABUF array.
	 */
	size_t GetTotalBytes() const;

private:
	/**
	 *	@brief	Extend the WSABUF array.
	 *
	 *	@throw	Exception if the WSABUF couldn't be resized.
	 */
	void Extend();

	WSABUF			*m_pWsaBuf;					///< Pointer to WSABUF array
	const size_t	m_initialSize;				///< Initial size of the WSABUF array
	size_t			m_numberOfBuffers;			///< The number of allocated WSABUFs
	size_t			m_numberInUse;				///< The number of WSABUFs in use
	size_t			m_totalBytes;				///< Total number of bytes in WSABUF array
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline WsaBufHelper::WsaBufHelper(const size_t initialSize) :
	m_pWsaBuf(NULL),
	m_initialSize(initialSize),
	m_numberOfBuffers(0),
	m_numberInUse(0),
	m_totalBytes(0)
{
	assert(0 != initialSize);
	Extend();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline WsaBufHelper::~WsaBufHelper()
{
	::free(m_pWsaBuf);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void WsaBufHelper::Reset()
{
	m_numberInUse = 0;
	m_totalBytes = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void WsaBufHelper::Append(const void *pData, const size_t dataLength)
{
	// do we need to extend the WSABUF array?
	if (m_numberInUse == m_numberOfBuffers)
		Extend();

	m_pWsaBuf[m_numberInUse].buf = (char *)pData;
	m_pWsaBuf[m_numberInUse].len = static_cast<DWORD>(dataLength);

	m_totalBytes += dataLength;
	++m_numberInUse;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline WSABUF *WsaBufHelper::GetFirstBuffer() const
{
	return m_pWsaBuf;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t WsaBufHelper::GetNumberOfBuffersInUse() const
{
	return m_numberInUse;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t WsaBufHelper::GetTotalBytes() const
{
	return m_totalBytes;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void WsaBufHelper::Extend()
{
	size_t newSize = m_numberOfBuffers + m_initialSize;
	WSABUF *pWsaBuf = reinterpret_cast<WSABUF *>(::realloc(m_pWsaBuf, newSize * sizeof(WSABUF)));

	if (NULL == pWsaBuf)
		ACTIV_THROW(CrtException, errno);

	m_pWsaBuf = pWsaBuf;
	m_numberOfBuffers = newSize;
}

} // namespace Activ

#endif // !defined (ACTIV_IP_WINDOWS_WSA_BUF_HELPER_H)
