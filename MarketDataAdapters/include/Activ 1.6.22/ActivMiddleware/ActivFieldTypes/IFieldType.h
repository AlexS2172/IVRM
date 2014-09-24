/**
 *	@file	IFieldType.h
 *
 *	@brief	Header file for the field type interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IFIELD_TYPE_H)
#define ACTIV_IFIELD_TYPE_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/ActivFieldTypes.h"

#include "ActivMiddleware/ActivMessageHandler/MessageValidater.h"

#include <string>

namespace Activ
{

class MessageBuilder;

/**
 *	@brief	Field type interface class.
 */
class IFieldType
{
public:
	static const size_t MAX_SERIALIZED_BODY_LENGTH_UNDEFINED = static_cast<const size_t>(~0); ///< The undefined max serialized body length.

	/**
	 *	@brief	Destructor.
	 */
	virtual ~IFieldType() = 0;

	/**
	 *	@brief	Reset the object.
	 */
	virtual void Reset() = 0;

	/**
	 *	@brief	Clear the object.
	 */
	virtual void Clear() = 0;

	/**
	 *	@brief	Compare.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Compare(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, int &result) const = 0;

	/**
	 *	@brief	Assign the value of this from another IFieldType object.
	 *
	 *	@param	pIFieldType the value to assign from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_MISMATCH
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API StatusCode Assign(const IFieldType * const pIFieldType);

	/**
	 *	@brief	Convert another IFieldType object to this type.
	 *
	 *	@param	pIFieldType the value to assign from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_MISMATCH
	 *	@retval	...
	 */
	virtual StatusCode Convert(const IFieldType * const pIFieldType) = 0;

	/**
	 *	@brief	Get the field type.
	 *
	 *	@return	the field type.
	 */
	FieldType GetType() const;

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	virtual std::string ToString() const = 0;

	/**
	 *	@brief	Convert the object from a string.
	 *
	 *	@param	messageValidater the message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode FromString(MessageValidater &messageValidater) = 0;

	/**
	 *	@brief	Convert the object from a string.
	 *
	 *	@param	field the field as a string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode FromString(const std::string &field);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	fieldType the field type.
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API StatusCode Serialize(const FieldType fieldType, MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength) const;

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength) const = 0;

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	@param	fieldType the field type.
	 *	@param	pSerializedBody the buffer to store a serialized version of the object's body in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API StatusCode SerializeBody(const FieldType fieldType, void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength) const;

	/**
	 *	@brief	Serialize the object's body into the supplied data buffer.
	 *
	 *	@param	fieldType the field type.
	 *	@param	pSerializedBody the buffer to store a serialized version of the object's body in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode SerializeBody(void *pSerializedBody, const size_t maxSerializedBodyLength, size_t &serializedBodyLength) const = 0;

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API StatusCode SerializeLengthAndBody(const FieldType fieldType, MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED) const;

	/**
	 *	@brief	Serialize the object's length and body into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode SerializeLengthAndBody(MessageBuilder &messageBuilder, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED) const = 0;

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	fieldType the field type.
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API StatusCode Deserialize(const FieldType fieldType, MessageValidater &messageValidater, const size_t maxSerializedBodyLength);

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, const size_t maxSerializedBodyLength) = 0;

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	fieldType the field type.
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API StatusCode DeserializeBody(const FieldType fieldType, const void * const pSerializedBody, const size_t serializedBodyLength);

	/**
	 *	@brief	Deserialize the object's body from the supplied data buffer.
	 *
	 *	@param	pSerializedBody the buffer from which the object's body will be extracted.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode DeserializeBody(const void * const pSerializedBody, const size_t serializedBodyLength) = 0;

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validater.
	 *
	 *	@param	fieldType the field type.
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API StatusCode DeserializeLengthAndBody(const FieldType fieldType, MessageValidater &messageValidater, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED);

	/**
	 *	@brief	Deserialize the object's length and body from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode DeserializeLengthAndBody(MessageValidater &messageValidater, const size_t maxSerializedBodyLength = MAX_SERIALIZED_BODY_LENGTH_UNDEFINED) = 0;

	/**
	 *	@brief	Get the serialized length of the object.
	 *
	 *	@return	the serialized length of the object.
	 */
	virtual size_t GetSerializedLength() const = 0;

	/**
	 *	@brief	Get the serialized length of the object's body.
	 *
	 *	@return	the serialized length of the object's body.
	 */
	virtual size_t GetSerializedBodyLength() const = 0;

	/**
	 *	@brief	Is the object initialized.
	 *
	 *	@return	whether the object has been initialized.
	 */
	bool IsInitialized() const;

	/**
	 *	@brief	Is different.
	 *
	 *	@param	pRhsSerializedBody the buffer containing the rhs serialized version of the object's body in.
	 *	@param	rhsSerializedBodyLength the rhs serialized body length.
	 *	@param	isDifferent indicates whether the two objects are different.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode IsDifferent(const void * const pRhsSerializedBody, const size_t rhsSerializedBodyLength, bool &isDifferent) const = 0;

	/**
	 *	@brief	Validate the serialized length and body.
	 *
	 *	@param	fieldType the field type.
	 *	@param	messageValidater the message validater.
	 *	@param	serializedBodyOffset the serialized body offset.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_FIELD_TYPES_API static StatusCode ValidateSerializedLengthAndBody(const FieldType fieldType, MessageValidater &messageValidater, size_t &serializedBodyOffset, size_t &serializedBodyLength);

	/**
	 *	@brief	Is valid serialized body length.
	 *
	 *	@param	fieldType the field type.
	 *	@param	serializedBodyLength the serialized body length.
	 *
	 *	@return	whether the serialized body length is valid.
	 */
	ACTIV_FIELD_TYPES_API static bool IsValidSerializedBodyLength(const FieldType fieldType, const size_t serializedBodyLength);

protected:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	fieldType the field type.
	 */
	IFieldType(const FieldType fieldType);

	bool		m_isInitialized;	///< Indicates whether the object is initialized.

private:
	FieldType	m_fieldType;		///< The field type.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IFieldType::~IFieldType()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldType IFieldType::GetType() const
{
	return m_fieldType;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode IFieldType::FromString(const std::string &field)
{
	MessageValidater messageValidater(field.c_str(), field.length());

	return FromString(messageValidater);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IFieldType::IsInitialized() const
{
	return m_isInitialized;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IFieldType::IFieldType(const FieldType fieldType) :
	m_fieldType(fieldType)
{
}

} // namespace Activ

#endif // !defined (ACTIV_IFIELD_TYPE_H)
