/**
 *	@file	IoVecHelper.h
 *
 *	@brief	Header file for struct iovec helper class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_POSIX_IO_VEC_HELPER_H)
#define ACTIV_IP_POSIX_IO_VEC_HELPER_H

#include "ActivMiddleware/ActivIp/External.h"

#include <cassert>
#include <vector>
#include <sys/uio.h>
#include <sys/socket.h>

namespace Activ
{

/**
 *	@brief	iovec helper class.
 */
class ACTIV_IP_API IoVecHelper
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	initialSize the number of struct iovec to allocate initially.
	 *
	 *	@throw	Exception.
	 */
	IoVecHelper(const size_t initialSize = 10);

	/**
	 *	@brief	Destructor.
	 */
	~IoVecHelper();

	/**
	 *	@brief	Reset the number of buffers in use.
	 */
	void Reset();

	/**
	 *	@brief	Append a new buffer, expand the iovec array if required.
	 *
	 *	@brief	pData pointer to the data.
	 *	@brief	dataLength the length of the buffer.
	 *
	 *	@throw	Exception if the vector couldn't be resized.
	 */
	void Append(void *pData, const size_t dataLength);

	/**
	 *	@brief	Get a pointer to the first iovec.
	 *
	 *	@return	Pointer to the first iovec.
	 */
	iovec *GetFirstBuffer() const;
	
	/**
	 *	@brief	Get buffer at an offset.
	 *
	 *	@param	offset.
	 *	@param	startOffset offset at which buffer \a index begins.
	 *	@param	index index of the returned buffer.
	 *
	 *	@return	iovec pointer or 0.
	 */
	iovec *GetBuffer(const size_t offset, size_t &startOffset, size_t &index);

	/**
	 *	@brief	Get the number of buffers in use.
	 *
	 *	@return	The number of buffers in use.
	 */
	size_t GetNumberOfBuffersInUse() const;

	/**
	 *	@brief	Get the total number of bytes in the iovec arary.
	 *
	 *	@return	The total number of bytes in the iovec array.
	 */
	size_t GetTotalBytes() const;

private:
	/**
	 *	@brief	Extend the iovec array.
	 *
	 *	@throw	Exception if the iovec couldn't be resized.
	 */
	void Extend();

	typedef std::vector<iovec> IoVecArray;

	IoVecArray		m_ioVecArray;
	const size_t	m_initialSize;				///< Initial size of the iovec array
	size_t			m_numberOfBuffers;			///< The number of allocated iovecs
	size_t			m_numberInUse;				///< The number of iovecs in use
	size_t			m_totalBytes;				///< Total number of bytes in iovec array
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IoVecHelper::IoVecHelper(const size_t initialSize) :
	m_ioVecArray(initialSize),
	m_initialSize(initialSize),
	m_numberInUse(0),
	m_totalBytes(0)
{
	assert(0 != initialSize);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IoVecHelper::~IoVecHelper()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IoVecHelper::Reset()
{
	m_numberInUse = 0;
	m_totalBytes = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IoVecHelper::Append(void *pData, const size_t dataLength)
{
	// do we need to extend the array?
	if (m_numberInUse == m_ioVecArray.size())
		Extend();

	m_ioVecArray[m_numberInUse].iov_base = reinterpret_cast<char *>(pData);
	m_ioVecArray[m_numberInUse].iov_len = dataLength;

	m_totalBytes += dataLength;
	++m_numberInUse;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline iovec *IoVecHelper::GetFirstBuffer() const
{
	return (iovec *)&m_ioVecArray[0];
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline iovec *IoVecHelper::GetBuffer(const size_t offset, size_t &startOffset, size_t &index)
{
	size_t currentPosition = 0, nextPosition = 0;
	
	for (size_t i = 0; i < m_numberInUse; ++i)
	{
		nextPosition += m_ioVecArray[i].iov_len;
		
		if (offset < nextPosition)
		{
			index = i;
			startOffset = currentPosition;
			
			return &m_ioVecArray[i];
		}
		
		currentPosition = nextPosition;
	}
	
	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t IoVecHelper::GetNumberOfBuffersInUse() const
{
	return m_numberInUse;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t IoVecHelper::GetTotalBytes() const
{
	return m_totalBytes;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IoVecHelper::Extend()
{
	size_t newSize = m_ioVecArray.size() + m_initialSize;

	// TODO exceptions, catch and rethrow as Exception?
    m_ioVecArray.resize(newSize);
}

} // namespace Activ

#endif // !defined (ACTIV_IP_POSIX_IO_VEC_HELPER_H)
