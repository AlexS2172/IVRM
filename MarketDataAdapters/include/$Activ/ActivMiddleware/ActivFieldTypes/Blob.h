/**
 *	@file	Blob.h
 *
 *	@brief	Header file for the blob class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BLOB_H)
#define ACTIV_BLOB_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"

namespace Activ
{

class MessageBuilder;
class MessageValidater;

/**
 *	@brief	Blob class.
 */
class ACTIV_FIELD_TYPES_API Blob : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_BLOB; ///< The field type.

	using IFieldType::FromString;

	/**
	 *	@brief	Default constructor.
	 */
	Blob();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@throw	ActivException
	 */
	Blob(const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	Blob(const Blob &rhs);

	/**
	 *	@brief	Destructor.
	 */
	~Blob();

	/**
	 *	@brief	Convertment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	Blob& operator=(const Blob &rhs);

	/**
	 *	@brief	Set the data and size.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Set(const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

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
	 *	@brief	Get the data and size.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(const uint8_t *&pData, size_t &size, size_t &decompressedSize) const;

	/**
	 *	@brief	Get the data.
	 *
	 *	@return	the pointer to the data.
	 *
	 *	@throw	ActivException
	 */
	const uint8_t *GetData() const;

	/**
	 *	@brief	Get the size.
	 *
	 *	@return	the size of the data.
	 *
	 *	@throw	ActivException
	 */
	size_t GetSize() const;

	/**
	 *	@brief	Get the decompressed size.
	 *
	 *	@return	the size of the decompressed data.
	 *
	 *	@throw	ActivException
	 */
	size_t GetDecompressedSize() const;

	/**
	 *	@brief	Reset the object.
	 */
	virtual void Reset();

	/**
	 *	@brief	Compare.
	 *
	 *	@param	rhs the object to compare with.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode Compare(const Blob &rhs, int &result) const;

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
	virtual StatusCode Compare(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result) const;

	/**
	 *	@brief	Convert another IFieldType object to this type.
	 *
	 *	@param	pIFieldType the value to convert from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Convert(const IFieldType * const pIFieldType);

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	virtual std::string ToString() const;

	/**
	 *	@brief	Convert the object from a string.
	 *
	 *	@param	messageValidater the message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	...
	 */
	virtual StatusCode FromString(MessageValidater &messageValidater);

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
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength) const;

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
	virtual StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength) const;

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	...
	 */
	virtual StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, const size_t maxSerializedBodyLength);

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
	virtual StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	virtual StatusCode DeserializeLengthAndBody(MessageValidater &messageValidater);

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
	virtual StatusCode IsDifferent(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, bool &isDifferent) const;

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
	static StatusCode Compare(const void * const pLhsSerializedBody, const size_t lhsSerializedBodyLength, const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer to store a serialized version of the object's body in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	serializedBodyLength the serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	static StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	static StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder, const uint8_t * const pData, const size_t size, const size_t decompressedSize = 0);

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidater &messageValidater, const size_t maxSerializedBodyLength, const uint8_t *&pData, size_t &size, size_t &decompressedSize);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	static StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength, const uint8_t *&pData, size_t &size, size_t &decompressedSize);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	decompressedSize indicates the size of the decompressed data (zero is uncompressed).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode DeserializeLengthAndBody(MessageValidater &messageValidater, const uint8_t *&pData, size_t &size, size_t &decompressedSize);

	/**
	 *	@brief	Validate the serialized length and body.
	 *
	 *	@param	messageValidater the message validater.
	 *	@param	serializedBodyOffset the serialized body offset.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode ValidateSerializedLengthAndBody(MessageValidater &messageValidater, size_t &serializedBodyOffset, size_t &serializedBodyLength);

	/**
	 *	@brief	Is valid serialized body length.
	 *
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@return	whether the serialized body length is valid.
	 */
	static bool IsValidSerializedBodyLength(const size_t serializedBodyLength);

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
	static StatusCode Compress(const uint8_t * const pSource, const size_t sourceLength, uint8_t * const pTarget, const size_t targetSize, size_t &targetLength);

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
	static StatusCode Decompress(const uint8_t * const pSource, const size_t sourceLength, uint8_t * const pTarget, const size_t targetSize, size_t &targetLength);

private:
	/**
	 *	@brief	Swap.
	 *
	 *	@param	rhs the object to swap with.
	 */
	void Swap(Blob &rhs);

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

	uint8_t		*m_pData;			///< Pointer to the data.
	size_t		m_size;				///< The size of the data.
	size_t		m_decompressedSize;	///< The size of the decompressed data.
};

} // namespace Activ

#endif // !defined (ACTIV_BLOB_H)
