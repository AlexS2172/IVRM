/**
 *  @file	FieldSpecification.h
 *
 *	@brief	Header file for the field specification class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_FIELD_SPECIFICATION_H)
#define ACTIV_DATABASE_FIELD_SPECIFICATION_H

#include "ActivMiddleware/ActivFieldTypes/ActivFieldTypes.h"

#include "ActivMiddleware/ActivDatabase/External.h"

#include "ActivMiddleware/ActivDatabase/ActivDatabase.h"

#include <string>

namespace Activ
{

class MessageBuilder;
class MessageValidater;

namespace Database
{

// ---------------------------------------------------------------------------------------------------------------------------------

#define ACTIV_DATABASE_FIELD_TYPE_ENUM(id, name, isVariableLength, isString, validate, compare, toString)				id,																///< Enumeration of allowed field types.
#define ACTIV_DATABASE_FIELD_TYPE_SPECIFICATION(id, name, isVariableLength, isString, validate, compare, toString)	{ name, isVariableLength, isString, validate, compare, toString },	///< Initializer for field type specification structures.

/**
 *	@brief	The list of field type information.
 */
#define ACTIV_DATABASE_FIELD_TYPE_LIST(d)																																\
	d(TYPE_NONE,				"None",					false,	false,	ValidateNoneSpecification,				CompareUnsignedMemory,		BinaryToString)				\
	d(TYPE_BINARY_ARRAY,		"Binary Array",			false,	true,	ValidateSpecification,					CompareUnsignedMemory,		BinaryToString)				\
	d(TYPE_BINARY_STRING,		"Binary String",		true,	true,	ValidateSpecification,					CompareUnsignedMemory,		BinaryToString)				\
	d(TYPE_BITMAP,				"Bitmap",				false,	false,	ValidateSpecification,					CompareUnsignedMemory,		BinaryToString)				\
	d(TYPE_BLOB,				"BLOB",					true,	false,	ValidateBlobSpecification,				CompareBlob,				BlobToString)				\
	d(TYPE_DATE,				"Date",					false,	false,	ValidateDateSpecification,				CompareDate,				DateToString)				\
	d(TYPE_DATE_TIME,			"Date Time",			false,	false,	ValidateDateTimeSpecification,			CompareDateTime,			DateTimeToString)			\
	d(TYPE_POINTER,				"Pointer",				false,	false,	ValidateSpecification,					CompareUnsignedMemory,		BinaryToString)				\
	d(TYPE_RATIONAL,			"Rational",				true,	false,	ValidateRationalSpecification,			CompareRational,			RationalToString)			\
	d(TYPE_SIGNED_INTEGRAL,		"Signed Integral",		false,	false,	ValidateSignedIntegralSpecification,	CompareSignedIntegral,		SignedIntegralToString)		\
	d(TYPE_TEXT_ARRAY,			"Text Array",			false,	true,	ValidateSpecification,					CompareUnsignedMemory,		TextToString)				\
	d(TYPE_TEXT_STRING,			"Text String",			true,	true,	ValidateSpecification,					CompareUnsignedMemory,		TextToString)				\
	d(TYPE_TIME,				"Time",					false,	false,	ValidateTimeSpecification,				CompareTime,				TimeToString)				\
	d(TYPE_TRATIONAL,			"Trended Rational",		true,	false,	ValidateTRationalSpecification,			CompareTRational,			TRationalToString)			\
	d(TYPE_UNSIGNED_INTEGRAL,	"Unsigned Integral",	false,	false,	ValidateUnsignedIntegralSpecification,	CompareUnsignedIntegral,	UnsignedIntegralToString)	\
	d(TYPE_BOOLEAN,				"Boolean",				false,	false,	ValidateBooleanSpecification,			CompareUnsignedIntegral,	BooleanToString)			\
	d(TYPE_CRC_BLOB,			"Crc BLOB",				true,	false,	ValidateCrcBlobSpecification,			CompareCrcBlob,				CrcBlobToString)

// ---------------------------------------------------------------------------------------------------------------------------------

#define ACTIV_DATABASE_FIELD_ENUM(id, name, type, maxLength, offset, properties)			id,																																								///< Enumerations of field ids.
#define ACTIV_DATABASE_FIELD_SPECIFICATION(id, name, type, maxLength, offset, properties)	Activ::Database::FieldSpecification(id, name, Activ::Database::FieldSpecification::type, maxLength, offset, Activ::Database::FieldSpecification::properties),	///< Initializer for field specification structures.

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Field specification structure.
 */
class FieldSpecification
{
public:
	/**
	 *	@brief	Unique identifier for fields.
	 */
	typedef uint16_t Id;

	/**
	 *	@brief	Types.
	 */
	enum Type
	{
		ACTIV_DATABASE_FIELD_TYPE_LIST(ACTIV_DATABASE_FIELD_TYPE_ENUM)
		NUMBER_OF_TYPES,
	};

	/**
	 *	@brief	Field properties.
	 *
	 */
	enum
	{
		PROPERTY_NONE = 0x0000000,
		//PROPERTY_NOT_NULL = 0x00000001,
		//PROPERTY_REMOTE_MODIFY_DISABLED = 0x00000002,
//		PROPERTY_RESERVED = 0x80000000,		// This is a place holder for the descending property of index fields and should not be removed.
	};
	typedef uint32_t Properties;		///< Definition of the field properties.

	/**
	 *	@brief	Constructor.
	 */
	ACTIV_DATABASE_API FieldSpecification();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	id unique identifier for the field.
	 *	@param	name name of the field.
	 *	@param	type type of the field.
	 *	@param	maxLength maximum length of the field.
	 *	@param	offset offset of the field.
	 *	@param	properties properties of the field.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API FieldSpecification(const Id id, const std::string &name, const Type type, const size_t maxLength, const size_t offset = 0, const Properties properties = PROPERTY_NONE);

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_DATABASE_API bool operator==(const FieldSpecification &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_DATABASE_API bool operator!=(const FieldSpecification &rhs) const;

	/**
	 *	@brief	Validate a field specification.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API void Validate() const;

	/**
	 *	@brief	Serialize the field specification into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_DATABASE_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize a field specification from a message.
	 *
	 *	@param	messageValidater the message validater from which the field specification is extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief	Get a database field type from an Activ field type.
	 *
	 *	@param	activFieldType the Activ field type to convert to a database field type.
	 *	@param	databaseFieldType set equal to the matching database field type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 */
	ACTIV_DATABASE_API static StatusCode GetDatabaseFieldType(const FieldType activFieldType, FieldSpecification::Type &databaseFieldType);

	/**
	 *	@brief	Get an Activ field type from a database field type.
	 *
	 *	@param	databaseFieldType the database field type to convert to an Activ field type.
	 *	@param	activFieldType set equal to the matching Activ field type.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 */
	ACTIV_DATABASE_API static StatusCode GetActivFieldType(const FieldSpecification::Type databaseFieldType, FieldType &activFieldType);

	/**
	 *	@brief	Is a match.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	true if this field specification matches the criteria specified in the supplied field specification.
	 */
	ACTIV_DATABASE_API bool IsMatch(const FieldSpecification &rhs) const;

	/**
	 *	@brief	Convert the specification to a C-style string.
	 *
	 *	@return	the specification as a string.
	 */
	ACTIV_DATABASE_API std::string ToString() const;

	static const Id	UNDEFINED_FIELD_ID	= ~0;						///< Undefined field id.

	Id				m_id;											///< Unique field identifier.
	std::string		m_name;											///< Name.
	Type			m_type;											///< Type.
	size_t			m_maxLength;									///< Maximum allowed length.
	size_t			m_offset;										///< Offset within record buffer. Automatically calculated for managed tables.
	uint32_t		m_properties;									///< Properties.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldSpecification::GetDatabaseFieldType(const FieldType activFieldType, Type &databaseFieldType)
{
	switch (activFieldType)
	{
		case FIELD_TYPE_BINARY_ARRAY:
			databaseFieldType = FieldSpecification::TYPE_BINARY_ARRAY;
			break;

		case FIELD_TYPE_BINARY_STRING:
			databaseFieldType = FieldSpecification::TYPE_BINARY_STRING;
			break;

		case FIELD_TYPE_BLOB:
			databaseFieldType = FieldSpecification::TYPE_BLOB;
			break;

		case FIELD_TYPE_CRC_BLOB:
			databaseFieldType = FieldSpecification::TYPE_CRC_BLOB;
			break;

		case FIELD_TYPE_DATE:
			databaseFieldType = FieldSpecification::TYPE_DATE;
			break;

		case FIELD_TYPE_DATE_TIME:
			databaseFieldType = FieldSpecification::TYPE_DATE_TIME;
			break;

		case FIELD_TYPE_RATIONAL:
			databaseFieldType = FieldSpecification::TYPE_RATIONAL;
			break;

		case FIELD_TYPE_SINT:
			databaseFieldType = FieldSpecification::TYPE_SIGNED_INTEGRAL;
			break;

		case FIELD_TYPE_TEXT_ARRAY:
			databaseFieldType = FieldSpecification::TYPE_TEXT_ARRAY;
			break;

		case FIELD_TYPE_TEXT_STRING:
			databaseFieldType = FieldSpecification::TYPE_TEXT_STRING;
			break;

		case FIELD_TYPE_TIME:
			databaseFieldType = FieldSpecification::TYPE_TIME;
			break;

		case FIELD_TYPE_TRATIONAL:
			databaseFieldType = FieldSpecification::TYPE_TRATIONAL;
			break;

		case FIELD_TYPE_UINT:
			databaseFieldType = FieldSpecification::TYPE_UNSIGNED_INTEGRAL;
			break;

		default:
			return STATUS_CODE_INVALID_FIELD_TYPE;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldSpecification::GetActivFieldType(const Type databaseFieldType, FieldType &activFieldType)
{
	switch (databaseFieldType)
	{
		case FieldSpecification::TYPE_BINARY_ARRAY:
			activFieldType = FIELD_TYPE_BINARY_ARRAY;
			break;

		case FieldSpecification::TYPE_BINARY_STRING:
			activFieldType = FIELD_TYPE_BINARY_STRING;
			break;

		case FieldSpecification::TYPE_BLOB:
			activFieldType = FIELD_TYPE_BLOB;
			break;

		case FieldSpecification::TYPE_CRC_BLOB:
			activFieldType = FIELD_TYPE_CRC_BLOB;
			break;

		case FieldSpecification::TYPE_DATE:
			activFieldType = FIELD_TYPE_DATE;
			break;

		case FieldSpecification::TYPE_DATE_TIME:
			activFieldType = FIELD_TYPE_DATE_TIME;
			break;

		case FieldSpecification::TYPE_RATIONAL:
			activFieldType = FIELD_TYPE_RATIONAL;
			break;

		case FieldSpecification::TYPE_SIGNED_INTEGRAL:
			activFieldType = FIELD_TYPE_SINT;
			break;

		case FieldSpecification::TYPE_TEXT_ARRAY:
			activFieldType = FIELD_TYPE_TEXT_ARRAY;
			break;

		case FieldSpecification::TYPE_TEXT_STRING:
			activFieldType = FIELD_TYPE_TEXT_STRING;
			break;

		case FieldSpecification::TYPE_TIME:
			activFieldType = FIELD_TYPE_TIME;
			break;

		case FieldSpecification::TYPE_TRATIONAL:
			activFieldType = FIELD_TYPE_TRATIONAL;
			break;

		case FieldSpecification::TYPE_UNSIGNED_INTEGRAL:
			activFieldType = FIELD_TYPE_UINT;
			break;

		default:
			return STATUS_CODE_INVALID_FIELD_TYPE;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

}	// namespace Database

}	// namespace Activ

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Global operator<< to dump a field specification to an ostream.
 *
 *	@param	os the ostream to write the index specification to.
 *	@param	rhs the field specification to write.
 *
 *	@return	the ostream \a os.
 */
ACTIV_DATABASE_API std::ostream &operator<<(std::ostream &os, const Activ::Database::FieldSpecification &rhs);

// ---------------------------------------------------------------------------------------------------------------------------------

#endif	// !defined(ACTIV_DATABASE_FIELD_SPECIFICATION_H)
