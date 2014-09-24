/**
 *	@file	TextArray.h
 *
 *	@brief	Header file for the text array class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_TEXT_ARRAY_H)
#define ACTIV_TEXT_ARRAY_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"

#include "ActivMiddleware/Misc/MemoryHelper.h"
 
#include "ActivMiddleware/ActivBase/StatusCodeException.h"

namespace Activ
{

class MessageBuilder;
class MessageValidator;

/**
 *	@brief	Text array class.
 */
class TextArray : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_TEXT_ARRAY; ///< The field type.

	using IFieldType::FromString;
	using IFieldType::Serialize;
	using IFieldType::SerializeBody;
	using IFieldType::SerializeLengthAndBody;
	using IFieldType::Deserialize;
	using IFieldType::DeserializeBody;
	using IFieldType::DeserializeLengthAndBody;

	/**
	 *	@brief	Default constructor.
	 */
	TextArray();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@throw	StatusCodeException
	 */
	TextArray(const uint8_t * const pData, const size_t size);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	TextArray(const TextArray &rhs);

	/**
	 *	@brief	Destructor.
	 */
	~TextArray();

	/**
	 *	@brief	Convertment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	TextArray& operator=(const TextArray &rhs);

	/**
	 *	@brief	Set the data and size.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Set(const uint8_t * const pData, const size_t size);

	/**
	 *	@brief	Get the data and size.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(const uint8_t *&pData, size_t &size) const;

	/**
	 *	@brief	Get the data.
	 *
	 *	@return	the pointer to the data.
	 *
	 *	@throw	StatusCodeException
	 */
	const uint8_t *GetData() const;

	/**
	 *	@brief	Get the size.
	 *
	 *	@return	the size of the data.
	 *
	 *	@throw	StatusCodeException
	 */
	size_t GetSize() const;

	/**
	 *	@brief	Reset the object.
	 */
	virtual void Reset();

	/**
	 *	@brief	Clear the object.
	 */
	virtual void Clear();

	/**
	 *	@brief	Compare.
	 *
	 *	@param	rhs the object to compare with.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	ACTIV_FIELD_TYPES_API StatusCode Compare(const TextArray &rhs, int &result) const;

	/**
	 *	@brief	Compare.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Compare(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result) const;

	/**
	 *	@brief	Convert another IFieldType object to this type.
	 *
	 *	@param	pIFieldType the value to convert from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Convert(const IFieldType * const pIFieldType);

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	ACTIV_FIELD_TYPES_API virtual std::string ToString() const;

	/**
	 *	@brief	Convert the object from a string.
	 *
	 *	@param	messageValidator the message validator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode FromString(MessageValidator &messageValidator);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength) const;

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer to store a serialized version of the object's body in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength) const;

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode Deserialize(MessageValidator &messageValidator, const size_t maxSerializedBodyLength);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode DeserializeLengthAndBody(MessageValidator &messageValidator, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED);

	/**
	 *	@brief	Get the serialized length of the object.
	 *
	 *	@return	the serialized length of the object.
	 */
	virtual size_t GetSerializedLength() const;

	/**
	 *	@brief	Get the serialized length of the object's body.
	 *
	 *	@return	the serialized length of the object's body.
	 */
	virtual size_t GetSerializedBodyLength() const;

	/**
	 *	@brief	Is different.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	isDifferent indicates whether the two objects are different.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode IsDifferent(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, bool &isDifferent) const;

	/**
	 *	@brief	Get the maximum serialized length of the object.
	 *
	 *	@param	size the size of the data.
	 *
	 *	@return	the maximum serialized length of the object.
	 */
	static size_t GetMaxSerializedLength(const size_t size);

	/**
	 *	@brief	Get the maximum serialized length of the object's body.
	 *
	 *	@param	size the size of the data.
	 *
	 *	@return	the maximum serialized length of the object's body.
	 */
	static size_t GetMaxSerializedBodyLength(const size_t size);

	/**
	 *	@brief	Compare two objects serialized bodies.
	 *
	 *	@param	pLhsSerializedBody the buffer containing the lhs serialized version of the object's body in.
	 *	@param	lhsSerializedBodyLength the lhs serialized body length.
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Compare(const void * const pLhsSerializedBody, const size_t lhsSerializedBodyLength, const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength, const uint8_t * const pData, const size_t size);

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer to store a serialized version of the object's body in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	serializedBodyLength the serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength, const uint8_t * const pData, const size_t size);

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength, const uint8_t * const pData, const size_t size);

	/**
	 *	@brief	Deserialize the object from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Deserialize(MessageValidator &messageValidator, const size_t maxSerializedBodyLength, const uint8_t *&pData, size_t &size);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_FIELD_TYPES_API static StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength, const uint8_t *&pData, size_t &size);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode DeserializeLengthAndBody(MessageValidator &messageValidator, const size_t maxSerializedBodyLength, const uint8_t *&pData, size_t &size);

	/**
	 *	@brief	Validate the serialized length and body.
	 *
	 *	@param	messageValidator the message validator.
	 *	@param	serializedBodyOffset the serialized body offset.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode ValidateSerializedLengthAndBody(MessageValidator &messageValidator, size_t &serializedBodyOffset, size_t &serializedBodyLength);

	/**
	 *	@brief	Is valid serialized body length.
	 *
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@return	whether the serialized body length is valid.
	 */
	static bool IsValidSerializedBodyLength(const size_t serializedBodyLength);

private:
	static const size_t RESERVED_BUFFER_SIZE = 32;	///< The reserved buffer size.

	/**
	 *	@brief	Compare two buffers.
	 *
	 *	@param	pLhsBuffer the lhs buffer.
	 *	@param	lhsBufferLength the lhs buffer length.
	 *	@param	pRhsBuffer the rhs buffer.
	 *	@param	rhsBufferLength the rhs buffer length.
	 *
	 *	@return	result the result of the comparison.
	 */
	static int Compare(const void * const pLhsBuffer, const size_t lhsBufferLength, const void * const pRhsBuffer, const size_t rhsBufferLength);

	uint8_t	m_reservedBuffer[RESERVED_BUFFER_SIZE];	///< The reserved buffer.
	size_t	m_allocatedSize;						///< The size of the allocated memory.

	uint8_t	*m_pData;								///< Pointer to the data.
	size_t	m_size;									///< The size of the data.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline TextArray::TextArray() :
	IFieldType(FIELD_TYPE),
	m_allocatedSize(0),
	m_pData(m_reservedBuffer),
	m_size(0)
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TextArray::TextArray(const uint8_t * const pData, const size_t size) :
	IFieldType(FIELD_TYPE),
	m_allocatedSize(0),
	m_pData(m_reservedBuffer),
	m_size(0)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(pData, size));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TextArray::TextArray(const TextArray &rhs) :
	IFieldType(FIELD_TYPE),
	m_allocatedSize(0),
	m_pData(m_reservedBuffer),
	m_size(0)
{
	if (rhs.IsInitialized())
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(rhs.m_pData, rhs.m_size));
	else
		m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TextArray::~TextArray()
{
	if (0 != m_allocatedSize)
		std::free(m_pData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline TextArray& TextArray::operator=(const TextArray &rhs)
{
	if (this != &rhs)
	{
		if (rhs.IsInitialized())
		{
			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(rhs.m_pData, rhs.m_size));
		}
		else
		{
			Reset();
		}
	}

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode TextArray::Set(const uint8_t * const pData, const size_t size)
{
	// TODO: range check?

	if (0 == size)
		return STATUS_CODE_INVALID_PARAMETER;

	if (size > std::max<size_t>(m_allocatedSize, size_t(RESERVED_BUFFER_SIZE)))
	{
		const size_t allocatedSize = (4 * (((std::max<size_t>(m_allocatedSize + (m_allocatedSize / 2), size) - 1) / 4) + 1));
		uint8_t * const pAllocated = static_cast<uint8_t *>(((0 != m_allocatedSize) ? std::realloc(m_pData, allocatedSize) : std::malloc(allocatedSize)));

		if (0 == pAllocated)
			return STATUS_CODE_HEAP_EMPTY;

		m_allocatedSize = allocatedSize;
		m_pData = pAllocated;
	}

	MemoryCopy(m_pData, pData, size);

	m_size = size;
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode TextArray::Get(const uint8_t *&pData, size_t &size) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	pData = m_pData;
	size = m_size;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const uint8_t *TextArray::GetData() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return m_pData;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t TextArray::GetSize() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return m_size;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void TextArray::Reset()
{
	m_size = 0;
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void TextArray::Clear()
{
	if (0 != m_allocatedSize)
		std::free(m_pData);

	m_pData = m_reservedBuffer;
	m_allocatedSize = 0;
	m_size = 0;
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t TextArray::GetSerializedLength() const
{
	return (IsInitialized() ? GetMaxSerializedLength(m_size) : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t TextArray::GetSerializedBodyLength() const
{
	return (IsInitialized() ? GetMaxSerializedBodyLength(m_size) : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t TextArray::GetMaxSerializedLength(const size_t size)
{
	return GetMaxSerializedBodyLength(size);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t TextArray::GetMaxSerializedBodyLength(const size_t size)
{
	return size;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool TextArray::IsValidSerializedBodyLength(const size_t serializedBodyLength)
{
	return (0 != serializedBodyLength);
}

} // namespace Activ

#endif // !defined (ACTIV_TEXT_ARRAY_H)
