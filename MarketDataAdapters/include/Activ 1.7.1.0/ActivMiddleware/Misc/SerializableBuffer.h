/**
 *  @file	SerializableBuffer.h
 *
 *  @brief	Serializable buffer.
 *
 *  $Log: $
 */

#if (!defined ACTIV_MISC_SERIALIZABLE_BUFFER_H)
#define ACTIV_MISC_SERIALIZABLE_BUFFER_H

#include "ActivMiddleware/Misc/MemoryHelper.h"

#include "boost/integer_traits.hpp"

namespace Activ
{

/**
 *	@brief	Serializable buffer template.
 *
 *	@param	LENGTH_TYPE type of length to serialize.
 *	@param	MIN_LENGTH min length of serializable buffer.
 *	@param	MAX_LENGTH max length of serializable buffer.
 */
template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH = 0, LENGTH_TYPE MAX_LENGTH = boost::integer_traits<LENGTH_TYPE>::const_max>
class SerializableBuffer
{
public:
	typedef LENGTH_TYPE Length;

	/**
	 *	@brief	Default constructor.
	 */
	SerializableBuffer();

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	serializableBuffer to copy from.
	 */
	SerializableBuffer(const SerializableBuffer &serializableBuffer);

	/**
	 *	@brief	Destructor.
	 */
	~SerializableBuffer();

	/**
	 *	@brief	Assignment operator.
	 */
	SerializableBuffer &operator=(const SerializableBuffer &rhs);

	/**
	 *	@brief	Set data pointer and length
	 *
	 *	@param	pData	Pointer to data
	 *	@param	length	Length of data
	 *	@param	copy	If true then data is copied into the object
						If false it just sets the pointer value and length
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	StatusCode Set(const void *pData, size_t length, bool copy);

	/**
	 *	@brief	Get pointer to data.
	 *
	 *	@return	Pointer to data.
	 */
	const void *GetData() const;

	/**
	 *	@brief	Get length of data.
	 *
	 *	@return	Length of data.
	 */
	size_t GetDataLength() const;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	pData data to serialize.
	 *	@param	dataLength length of data to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const void * const pData, const size_t dataLength);

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

private:
	const void *m_pData;
	void *		m_pBuffer;
	size_t		m_dataLength;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::SerializableBuffer() :
	m_pData(0),
	m_pBuffer(0),
	m_dataLength(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::SerializableBuffer(const SerializableBuffer &serializableBuffer) :
	m_pData(0),
	m_pBuffer(0),
	m_dataLength(0)
{
	*this = serializableBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::~SerializableBuffer()
{
	if (0 != m_pBuffer)
		std::free(m_pBuffer);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH> &SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::operator=(const SerializableBuffer &rhs)
{
	if (this != &rhs)
	{
		// If RHS has an allocated buffer then copy bytes, otherwise
		Set(rhs.GetData(), rhs.GetDataLength(), 0 != rhs.m_pBuffer);
	}

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline StatusCode SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::Set(const void *pData, size_t length, bool copy)
{
	if (0 != m_pBuffer)
	{
		std::free(m_pBuffer);
		m_pBuffer = 0;
	}

	m_pData = 0;
	m_dataLength = 0;

	if (copy)
	{
		if ((pData != 0) && (length != 0))
		{
			void *pNewBuffer = std::malloc(length);

			if (0 == pNewBuffer)
				return STATUS_CODE_HEAP_EMPTY;

			MemoryCopy(pNewBuffer, pData, length);

			m_pData = m_pBuffer = pNewBuffer;
			m_dataLength = length;
		}
	}
	else
	{
		m_pData = pData;
		m_dataLength = length;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline const void *SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::GetData() const
{
	return m_pData;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline size_t SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::GetDataLength() const
{
	return m_dataLength;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline StatusCode SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::Serialize(MessageBuilder &messageBuilder, const void * const pData, const size_t dataLength)
{
	if ((dataLength < MIN_LENGTH) || (dataLength > MAX_LENGTH))
		return STATUS_CODE_INVALID_LENGTH;

	LENGTH_TYPE actualLength = static_cast<LENGTH_TYPE>(dataLength);

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(actualLength, MessageBuilder::ENDIAN_LITTLE));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(pData, dataLength));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline StatusCode SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::Serialize(MessageBuilder &messageBuilder) const
{
	return Serialize(messageBuilder, m_pData, m_dataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
inline StatusCode SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::Deserialize(MessageValidator &messageValidator)
{
	LENGTH_TYPE actualLength;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(actualLength), MIN_LENGTH, MAX_LENGTH, MessageValidator::ENDIAN_LITTLE, &actualLength));

	if (0 != m_pBuffer)
	{
		std::free(m_pBuffer);
		m_pBuffer = 0;
	}

	m_pData = 0;
	m_dataLength = 0;

	if (actualLength > 0)
	{
		void *pNewBuffer = std::malloc(actualLength);

		if (0 == pNewBuffer)
			return STATUS_CODE_HEAP_EMPTY;

		m_pData = m_pBuffer = pNewBuffer;
		m_dataLength = actualLength;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateBytesCopy(m_dataLength, m_pBuffer));
	}

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_SERIALIZABLE_BUFFER_H)
