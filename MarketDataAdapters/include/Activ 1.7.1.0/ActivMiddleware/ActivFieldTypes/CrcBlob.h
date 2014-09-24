/**
 *	@file	CrcBlob.h
 *
 *	@brief	Header file for the crc blob class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CRC_BLOB_H)
#define ACTIV_CRC_BLOB_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"
 
#include "ActivMiddleware/Misc/MemoryHelper.h"

#include "ActivMiddleware/ActivBase/ActivBase.h"
#include "ActivMiddleware/ActivBase/StatusCodeException.h"

namespace Activ
{

class MessageBuilder;
class MessageValidator;

/**
 *	@brief	Crc blob class.
 */
class CrcBlob : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_CRC_BLOB; ///< The field type.

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
	CrcBlob();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@throw	StatusCodeException
	 */
	CrcBlob(const uint32_t crc, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	CrcBlob(const CrcBlob &rhs);

	/**
	 *	@brief	Destructor.
	 */
	~CrcBlob();

	/**
	 *	@brief	Convertment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	CrcBlob& operator=(const CrcBlob &rhs);

	/**
	 *	@brief	Set the data and size.
	 *
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Set(const uint32_t crc, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Get the data and size.
	 *
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(uint32_t &crc, const uint8_t *&pData, size_t &size) const;

	/**
	 *	@brief	Get the data and size.
	 *
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(uint32_t &crc, const uint8_t *&pData, size_t &size, size_t &decompressedSize) const;

	/**
	 *	@brief	Get the crc.
	 *
	 *	@return	the crc.
	 *
	 *	@throw	StatusCodeException
	 */
	uint32_t GetCrc() const;

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
	ACTIV_FIELD_TYPES_API StatusCode Compare(const CrcBlob &rhs, int &result) const;

	/**
	 *	@brief	Compare.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
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
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
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
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API virtual StatusCode IsDifferent(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, bool &isDifferent) const;

	/**
	 *	@brief	Is the data compressed.
	 *
	 *	@return	whether the data is compressed.
	 */
	bool IsCompressed() const;

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
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength, const uint32_t crc, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer to store a serialized version of the object's body in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	serializedBodyLength the serialized body length.
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength, const uint32_t crc, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength, const uint32_t crc, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Deserialize the object from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Deserialize(MessageValidator &messageValidator, const size_t maxSerializedBodyLength, uint32_t &crc, const uint8_t *&pData, size_t &size, size_t &decompressedSize);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength, uint32_t &crc, const uint8_t *&pData, size_t &size, size_t &decompressedSize);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validator.
	 *
	 *	@param	messageValidator the message validator from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	crc the crc.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode DeserializeLengthAndBody(MessageValidator &messageValidator, const size_t maxSerializedBodyLength, uint32_t &crc, const uint8_t *&pData, size_t &size, size_t &decompressedSize);

	/**
	 *	@brief	Validate the serialized length and body.
	 *
	 *	@param	messageValidator the message validator.
	 *	@param	serializedBodyOffset the serialized body offset.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
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

	/**
	 *	@brief	Calculate the crc.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	intitialCrc the initial crc value to be used.
	 *
	 *	@return	the crc.
	 */
	ACTIV_FIELD_TYPES_API static uint32_t CalculateCrc(const uint8_t * const pData, const size_t size, const uint32_t intitialCrc = 0);

	/**
	 *	@brief	Compress a buffer.
	 *
	 *	@param	pSource the source buffer.
	 *	@param	sourceLength the source buffer length.
	 *	@param	pTarget the target buffer.
	 *	@param	targetSize the size of the target buffer.
	 *	@param	targetLength the target buffer length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Compress(const uint8_t * const pSource, const size_t sourceLength, uint8_t * const pTarget, const size_t targetSize, size_t &targetLength);

	/**
	 *	@brief	Decompress a buffer.
	 *
	 *	@param	pSource the source buffer.
	 *	@param	sourceLength the source buffer length.
	 *	@param	pTarget the target buffer.
	 *	@param	targetSize the size of the target buffer.
	 *	@param	targetLength the target buffer length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode Decompress(const uint8_t * const pSource, const size_t sourceLength, uint8_t * const pTarget, const size_t targetSize, size_t &targetLength);

private:
	size_t					m_allocatedSize;	///< The size of the allocated memory.

	uint32_t				m_crc;				///< The crc.
	uint8_t					*m_pData;			///< Pointer to the data.
	size_t					m_size;				///< The size of the data.
	size_t					m_decompressedSize;	///< The size of the decompressed data.

	static const uint32_t	m_crcTable[256];	///< The crc table.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline CrcBlob::CrcBlob() :
	IFieldType(FIELD_TYPE),
	m_allocatedSize(0),
	m_size(0)
{
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CrcBlob::CrcBlob(const uint32_t crc, const uint8_t * const pData, const size_t size, const size_t decompressedSize) :
	IFieldType(FIELD_TYPE),
	m_allocatedSize(0),
	m_size(0)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(crc, pData, size, decompressedSize));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CrcBlob::CrcBlob(const CrcBlob &rhs) :
	IFieldType(FIELD_TYPE),
	m_allocatedSize(0),
	m_size(0)
{
	if (rhs.IsInitialized())
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(rhs.m_crc, rhs.m_pData, rhs.m_size, rhs.m_decompressedSize));
	else
		m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CrcBlob::~CrcBlob()
{
	if (0 != m_allocatedSize)
		std::free(m_pData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CrcBlob& CrcBlob::operator=(const CrcBlob &rhs)
{
	if (this != &rhs)
	{
		if (rhs.IsInitialized())
		{
			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Set(rhs.m_crc, rhs.m_pData, rhs.m_size, rhs.m_decompressedSize));
		}
		else
		{
			Reset();
		}
	}

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode CrcBlob::Set(const uint32_t crc, const uint8_t * const pData, const size_t size, const size_t decompressedSize)
{
	if (0 == size)
		return STATUS_CODE_INVALID_PARAMETER;

	if (size > m_allocatedSize)
	{
		const size_t allocatedSize = (4 * (((std::max<size_t>(static_cast<const size_t>(m_allocatedSize * 1.5), size) - 1) / 4) + 1));
		uint8_t * const pAllocated = static_cast<uint8_t *>(((0 != m_allocatedSize) ? std::realloc(m_pData, allocatedSize) : std::malloc(allocatedSize)));

		if (0 == pAllocated)
			return STATUS_CODE_HEAP_EMPTY;

		m_allocatedSize = allocatedSize;
		m_pData = pAllocated;
	}

	m_crc = crc;

	MemoryCopy(m_pData, pData, size);

	m_size = size;
	m_decompressedSize = decompressedSize;
	m_isInitialized = true;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode CrcBlob::Get(uint32_t &crc, const uint8_t *&pData, size_t &size) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	crc = m_crc;
	pData = m_pData;
	size = m_size;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode CrcBlob::Get(uint32_t &crc, const uint8_t *&pData, size_t &size, size_t &decompressedSize) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	crc = m_crc;
	pData = m_pData;
	size = m_size;
	decompressedSize = m_decompressedSize;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t CrcBlob::GetCrc() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return m_crc;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const uint8_t *CrcBlob::GetData() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return m_pData;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CrcBlob::GetSize() const
{
	if (!IsInitialized())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_NOT_INITIALIZED);

	return m_size;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CrcBlob::Reset()
{
	m_size = 0;
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CrcBlob::Clear()
{
	if (0 != m_allocatedSize)
		std::free(m_pData);

	m_allocatedSize = 0;
	m_size = 0;
	m_isInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CrcBlob::GetSerializedLength() const
{
	return (IsInitialized() ? GetMaxSerializedLength(m_size) : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CrcBlob::GetSerializedBodyLength() const
{
	return (IsInitialized() ? GetMaxSerializedBodyLength(m_size) : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool CrcBlob::IsCompressed() const
{
	return (IsInitialized() ? (0 != m_decompressedSize) : false);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CrcBlob::GetMaxSerializedLength(const size_t size)
{
	return (sizeof(uint32_t) + GetMaxSerializedBodyLength(size));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CrcBlob::GetMaxSerializedBodyLength(const size_t size)
{
	return (sizeof(uint32_t) + sizeof(uint32_t) + size);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool CrcBlob::IsValidSerializedBodyLength(const size_t serializedBodyLength)
{
	return ((serializedBodyLength > GetMaxSerializedBodyLength(0)) && (serializedBodyLength <= UINT32_MAX));
}

} // namespace Activ

#endif // !defined (ACTIV_CRC_BLOB_H)
