/**
 *  @file	SerializableBuffer.h
 *
 *  @brief	Serializable buffer.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERIALIZABLE_BUFFER_H)
#define ACTIV_SERIALIZABLE_BUFFER_H

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
	 *	@param	messageValidater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidater &messageValidater);

	const void *m_pData;
	size_t		m_dataLength;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::SerializableBuffer() :
	m_pData(0),
	m_dataLength(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::SerializableBuffer(const SerializableBuffer &serializableBuffer) :
	m_pData(0),
	m_dataLength(0)
{
	*this = serializableBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::~SerializableBuffer()
{
	std::free(const_cast<void *>(m_pData));
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH> &SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::operator=(const SerializableBuffer &rhs)
{
	if (this != &rhs)
	{
		std::free(const_cast<void *>(m_pData));
		m_pData = 0;
		m_dataLength = 0;

		if (rhs.m_dataLength > 0)
		{
			void *pData = std::malloc(rhs.m_dataLength);

			if (0 != pData)
			{
				MemoryCopy(pData, rhs.m_pData, rhs.m_dataLength);

				m_dataLength = rhs.m_dataLength;
				m_pData = pData;
			}
		}
	}

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
StatusCode SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::Serialize(MessageBuilder &messageBuilder, const void * const pData, const size_t dataLength)
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
StatusCode SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::Serialize(MessageBuilder &messageBuilder) const
{
	return Serialize(messageBuilder, m_pData, m_dataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename LENGTH_TYPE, LENGTH_TYPE MIN_LENGTH, LENGTH_TYPE MAX_LENGTH>
StatusCode SerializableBuffer<LENGTH_TYPE, MIN_LENGTH, MAX_LENGTH>::Deserialize(MessageValidater &messageValidater)
{
	LENGTH_TYPE actualLength;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(actualLength), MIN_LENGTH, MAX_LENGTH, MessageValidater::ENDIAN_LITTLE, &actualLength));

	std::free(const_cast<void *>(m_pData));
	m_pData = 0;
	m_dataLength = 0;

	if (actualLength > 0)
	{	
		void *pData = std::malloc(actualLength);

		if (0 == pData)
			return STATUS_CODE_HEAP_EMPTY;

		m_pData = pData;
		m_dataLength = actualLength;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateBytesCopy(m_dataLength, pData));
	}

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // !defined (ACTIV_SERIALIZABLE_BUFFER_H)
