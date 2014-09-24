/**
 *	@file	UInt.h
 *
 *	@brief	Header file for the unsigned integer class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UINT_H)
#define ACTIV_UINT_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"
#include "ActivMiddleware/ActivFieldTypes/SInt.h"

namespace Activ
{

class MessageBuilder;
class MessageValidater;

/**
 *	@brief	Unsigned integer class.
 */
class ACTIV_FIELD_TYPES_API UInt : public IFieldType
{
public:
	static const FieldType FIELD_TYPE = FIELD_TYPE_UINT; ///< The field type.

	using IFieldType::FromString;

	/**
	 *	@brief	Default constructor.
	 */
	UInt();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	value the value.
	 */
	UInt(const uint64_t value);

	/**
	 *	@brief	Destructor.
	 */
	~UInt();

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator==(const UInt &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator!=(const UInt &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<(const UInt &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<=(const UInt &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>(const UInt &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator>=(const UInt &rhs) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	UInt& operator+=(const UInt &rhs);

	/**
	 *	@brief	Subtraction assignment operator.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	UInt& operator-=(const UInt &rhs);

	/**
	 *	@brief	Multiplication assignment operator.
	 *
	 *	@param	rhs the value to multiply this object by.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	UInt& operator*=(const UInt &rhs);

	/**
	 *	@brief	Division assignment operator.
	 *
	 *	@param	rhs the value to divide this object by.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	ActivException
	 */
	UInt& operator/=(const UInt &rhs);

	/**
	 *	@brief	Rational conversion operator.
	 *
	 *	@throw	ActivException
	 */
	operator const Rational() const;

	/**
	 *	@brief	SInt conversion operator.
	 *
	 *	@throw	ActivException
	 */
	operator const SInt() const;

	/**
	 *	@brief	Set a new value.
	 *
	 *	@param	value the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode Set(const uint64_t value);

	/**
	 *	@brief	Get the value.
	 *
	 *	@param	value the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode Get(uint64_t &value) const;

	/**
	 *	@brief	Get the value.
	 *
	 *	@return	the value.
	 *
	 *	@throw	ActivException
	 */
	uint64_t Get() const;

	/**
	 *	@brief	Add.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode Add(const UInt &rhs);

	/**
	 *	@brief	Subtract.
	 *
	 *	@param	rhs the value to subtract from this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode Subtract(const UInt &rhs);

	/**
	 *	@brief	Multiply.
	 *
	 *	@param	rhs the value to multiply this object by.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode Multiply(const UInt &rhs);

	/**
	 *	@brief	Divide.
	 *
	 *	@param	rhs the value to divide this object by.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode Divide(const UInt &rhs);

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
	StatusCode Compare(const UInt &rhs, int &result) const;

	/**
	 *	@brief	Compare.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
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
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
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
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	virtual StatusCode IsDifferent(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, bool &isDifferent) const;

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
	 *	@brief	Validate the serialized length and body.
	 *
	 *	@param	messageValidater the message validater.
	 *	@param	serializedBodyOffset the serialized body offset.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
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

private:
	/**
	 *	@brief	Get the compressed value size.
	 *
	 *	@return	the compressed value size.
	 */
	size_t GetCompressedValueSize() const;

	/**
	 *	@brief	Get an unsigned 64 bit value.
	 *
	 *	@param	pData pointer to the data.
	 *	@param	size the size of the data.
	 *	@param	value the value to be copied to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	static StatusCode GetUnsignedInt64(const void * const pData, const size_t size, uint64_t &value);

	uint64_t	m_value;	///< The value.
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Addition operator for the unsigned integer object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const UInt operator+(const UInt &lhs, const UInt &rhs)
{
	return UInt(lhs) += rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Subtraction operator for the unsigned integer object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const UInt operator-(const UInt &lhs, const UInt &rhs)
{
	return UInt(lhs) -= rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Multiplication operator for the unsigned integer object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const UInt operator*(const UInt &lhs, const UInt &rhs)
{
	return UInt(lhs) *= rhs;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Division operator for the unsigned integer object.
 *
 *	@param	lhs the left hand side value.
 *	@param	rhs the right hand side value.
 *
 *	@return	an object containing the result of the operation.
 *
 *	@throw	ActivException
 */
inline const UInt operator/(const UInt &lhs, const UInt &rhs)
{
	return UInt(lhs) /= rhs;
}

} // namespace Activ

#endif // !defined (ACTIV_UINT_H)
