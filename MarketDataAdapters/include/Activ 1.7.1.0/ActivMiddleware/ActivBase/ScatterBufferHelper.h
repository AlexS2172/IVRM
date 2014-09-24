/**
 *	@file	ScatterBufferHelper.h
 *
 *	@brief	Header file for the scatter buffer helper class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_SCATTER_BUFFER_HELPER_H)
#define ACTIV_BASE_SCATTER_BUFFER_HELPER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/StatusCodeException.h"

namespace Activ
{

/**
 *	@brief	Scatter buffer helper class.
 */
class ScatterBufferHelper
{
public:
	/**
	 *	@brief	Scatter buffer class.
	 */
	class ScatterBuffer
	{
	public:
		void	*m_pData;			///< pointer to the data.
		size_t	m_dataLength;		///< the length of the data.
	};

	/**
	 *	@brief	Scatter buffer const iterator class.
	 */
	class ConstIterator
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ConstIterator();

		/**
		 *	@brief	Dereference operator.
		 *
		 *	@return	Reference to the current message.
		 */
		const ScatterBuffer &operator*() const;

		/**
		 *	@brief	Member access operator.
		 *
		 *	@return	Pointer to the current message.
		 */
		const ScatterBuffer *operator->() const ;

		/**
		 *	@brief	Prefix increment operator.
		 *
		 *	@return	Reference to the updated object.
		 */
		ConstIterator &operator++();

		/**
		 *	@brief	Postfix increment operator.
		 *
		 *	@return	Copy of the original object.
		 */
		const ConstIterator operator++(int);

		/**
		*	@brief	Equality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are equal.
		*/
		bool operator==(const ConstIterator &rhs) const;

		/**
		*	@brief	Inequality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are not equal.
		*/
		bool operator!=(const ConstIterator &rhs) const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	pScatterBufferHelper the underlying heap message object.
		 *	@param	pMessage the current message object.
		 */
		ConstIterator(const ScatterBufferHelper *pScatterBufferHelper, ScatterBuffer *pScatterBuffer);

		const ScatterBufferHelper	*m_pScatterBufferHelper;	///< The underlying scatter buffer helper object.
		ScatterBuffer				*m_pScatterBuffer;			///< The current scatter buffer object.

		friend class ScatterBufferHelper;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	nInitialScatterBuffers the number of scatter buffers to allocate initially.
	 *	@param	initialGetBufferSize the initial size of the get buffer.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_BASE_API ScatterBufferHelper(const size_t nInitialScatterBuffers = 5, const size_t initialGetBufferSize = 0);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_BASE_API ScatterBufferHelper(const ScatterBufferHelper &rhs);

	/**
	 *	@brief	Destructor.
	 */
	~ScatterBufferHelper();

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_BASE_API ScatterBufferHelper& operator=(const ScatterBufferHelper &rhs);

	/**
	 *	@brief	Reset the number of buffers in use.
	 *
	 *	@param	nInitialScatterBuffers the number of scatter buffers to allocate initially.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Reset(const size_t nInitialScatterBuffers);

	/**
	 *	@brief	Reset the number of buffers in use.
	 */
	void Reset();

	/**
	 *	@brief	Attach a new buffer, expand the scatter buffer list if required.
	 *
	 *	@brief	pData pointer to the data.
	 *	@brief	dataLength the length of the buffer.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode Attach(void *pData, const size_t dataLength);

	/**
	 *	@brief	Attach a new buffer(s), expand the scatter buffer list if required.
	 *
	 *	@brief	from the scatterBufferHelper to copy the data from.
	 *	@brief	fromOffset the offset to copy the data from.
	 *	@brief	dataLength the length to set.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode Attach(const ScatterBufferHelper &from, const size_t fromOffset, const size_t dataLength);

	/**
	 *	@brief	Copy data from the scatter buffer.
	 *
	 *	@brief	offset the offset to copy from.
	 *	@brief	pData pointer to copy data to.
	 *	@brief	dataLength the length to copy.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode Copy(const size_t offset, void *pData, const size_t dataLength) const;

	/**
	 *	@brief	Copy data.
	 *
	 *	@param	offset the offset at which to copy data from the message.
	 *	@param	pData the address to copy data to.
	 *	@param	dataSize the size of data.
	 *	@param	dataLength the length copied.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode Copy(void *pData, const size_t dataSize, size_t &dataLength) const;

	/**
	 *	@brief	Get data from the scatter buffer.
	 *
	 *	@brief	offset the offset to get from.
	 *	@brief	pData reference to the address of the data.
	 *	@brief	dataLength the length to get.
	 *	@param	pGetBuffer pointer to an optional buffer (used if data not contiguous).
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode Get(const size_t offset, const void *&pData, const size_t dataLength, void *pGetBuffer = 0) const;

	/**
	 *	@brief	Get data.
	 *
	 *	@param	pData reference to the address of the data.
	 *	@param	dataLength the length got.
	 *	@param	pGetBuffer buffer to be used if data not contiguous.
	 *	@param	getBufferSize the size of the get buffer.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode Get(const void *&pData, size_t &dataLength, void *pGetBuffer = 0, const size_t getBufferSize = 0) const;

	/**
	 *	@brief	Get data from the scatter buffer.
	 *
	 *	@brief	offset the offset to get from.
	 *	@brief	pData reference to the address of the data.
	 *	@brief	dataLength the length to get.
	 *	@brief	dataLengthRead the length read.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode Get(const size_t offset, const void *&pData, const size_t dataLength, size_t &dataLengthRead) const;

	/**
	 *	@brief	Set data in the scatter buffer.
	 *
	 *	@brief	offset the offset to set at.
	 *	@brief	pData the address of the data.
	 *	@brief	dataLength the length to set.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode Set(const size_t offset, const void *pData, const size_t dataLength);

	/**
	 *	@brief	Set data in the scatter buffer.
	 *
	 *	@brief	offset the offset to set at.
	 *	@brief	from the scatterBufferHelper to copy the data from.
	 *	@brief	fromOffset the offset to copy the data from.
	 *	@brief	dataLength the length to set.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode Set(const size_t offset, const ScatterBufferHelper &from, const size_t fromOffset, const size_t dataLength);

	/**
	 *	@brief	Get the total number of bytes in the scatter buffer list.
	 *
	 *	@return	The total number of bytes in the scatter buffer list.
	 */
	size_t GetTotalBytes() const;

	/**
	 *	@brief	Adjust the get buffer size.
	 *
	 *	@param	getBufferSize the size of the get buffer.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode AdjustGetBufferSize(const size_t getBufferSize) const;

	/**
	 *	@brief	Get the number of scatter buffers currently in use.
	 *
	 *	@return the number of scatter buffers in use.
	 */
	size_t GetNumberOfScatterBuffersInUse() const;

	/**
	 *	@brief	Get an iterator to the first scatter buffer in the container.
	 *
	 *	@return an iterator object.
	 */
	ConstIterator Begin() const;

	/**
	 *	@brief	Get an iterator that points to beyond the last scatter buffer in the container.
	 *
	 *	@return an iterator object.
	 */
	ConstIterator End() const;

private:
	/**
	 *	@brief	Adjust the scatter buffer list.
	 *
	 *	@param	nScatterBuffers the number of scatter buffers.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode AdjustScatterBufferList(const size_t nScatterBuffers);

	size_t			m_nInitialScatterBuffers;		///< Initial number of scatter buffers

	ScatterBuffer	*m_pScatterBufferList;			///< Pointer to scatter buffer list.
	size_t			m_nScatterBuffers;				///< The number of allocated scatter buffers.
	size_t			m_nScatterBuffersInUse;			///< The number of scatter buffers in use.
	size_t			m_totalBytes;					///< Total number of bytes in scatter buffer list

	mutable void	*m_pGetBuffer;					///< Pointer to get buffer.
	mutable size_t	m_getBufferSize;				///< Total size of the get buffer.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ScatterBufferHelper::~ScatterBufferHelper()
{
	if (NULL != m_pScatterBufferList)
		std::free(m_pScatterBufferList);

	if (NULL != m_pGetBuffer)
		std::free(m_pGetBuffer);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode ScatterBufferHelper::Reset(const size_t nInitialScatterBuffers)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AdjustScatterBufferList(nInitialScatterBuffers));

	m_nInitialScatterBuffers = nInitialScatterBuffers;
	m_nScatterBuffersInUse = 0;
	m_totalBytes = 0;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void ScatterBufferHelper::Reset()
{
	m_nScatterBuffersInUse = 0;
	m_totalBytes = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t ScatterBufferHelper::GetTotalBytes() const
{
	return m_totalBytes;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t ScatterBufferHelper::GetNumberOfScatterBuffersInUse() const
{
	return m_nScatterBuffersInUse;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ScatterBufferHelper::ConstIterator ScatterBufferHelper::Begin() const
{
	return ConstIterator(this, m_pScatterBufferList);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ScatterBufferHelper::ConstIterator ScatterBufferHelper::End() const
{
	return ConstIterator(this, ((0 != m_pScatterBufferList) ? m_pScatterBufferList + m_nScatterBuffersInUse : 0));
}


// ---------------------------------------------------------------------------------------------------------------------------------

inline ScatterBufferHelper::ConstIterator::ConstIterator() :
	m_pScatterBufferHelper(0),
	m_pScatterBuffer(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const ScatterBufferHelper::ScatterBuffer &ScatterBufferHelper::ConstIterator::operator*() const
{
	return *m_pScatterBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const ScatterBufferHelper::ScatterBuffer *ScatterBufferHelper::ConstIterator::operator->() const
{
	return m_pScatterBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ScatterBufferHelper::ConstIterator &ScatterBufferHelper::ConstIterator::operator++()
{
	++m_pScatterBuffer;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const ScatterBufferHelper::ConstIterator ScatterBufferHelper::ConstIterator::operator++(int)
{
	ConstIterator oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool ScatterBufferHelper::ConstIterator::operator==(const ScatterBufferHelper::ConstIterator &rhs) const
{
	return ((m_pScatterBufferHelper == rhs.m_pScatterBufferHelper) && (m_pScatterBuffer == rhs.m_pScatterBuffer));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool ScatterBufferHelper::ConstIterator::operator!=(const ScatterBufferHelper::ConstIterator &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ScatterBufferHelper::ConstIterator::ConstIterator(const ScatterBufferHelper *pScatterBufferHelper, ScatterBuffer *pScatterBuffer) :
	m_pScatterBufferHelper(pScatterBufferHelper),
	m_pScatterBuffer(pScatterBuffer)
{
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_SCATTER_BUFFER_HELPER_H)

