/**
 *	@file	ActivFieldTypes.h
 *
 *	@brief	Global functions from ActivFieldTypes.
 *
 *	$Log: $
 */

#if !defined (ACTIV_FIELD_TYPES_H)
#define ACTIV_FIELD_TYPES_H

#include "ActivMiddleware/ActivFieldTypes/External.h"

#include <string>

/**
 *	@brief	The list of field types and their string representation.
 *
 *	SIZE1	size of inlined field length in a record message.
 *	SIZE2	size of inlined field length in an update message.
 *
 *	  TYPE						NAME				SIZE1	SIZE2
 */
#define ACTIV_FIELD_TYPE_LIST(d)									\
	d(FIELD_TYPE_DATE,			Date,				0,		0)		\
	d(FIELD_TYPE_TIME,			Time,				0,		0)		\
	d(FIELD_TYPE_DATE_TIME,		DateTime,			0,		0)		\
	d(FIELD_TYPE_SINT,			SInt,				0,		1)		\
	d(FIELD_TYPE_UINT,			UInt,				0,		1)		\
	d(FIELD_TYPE_RATIONAL,		Rational,			0,		0)		\
	d(FIELD_TYPE_TRATIONAL,		TRational,			0,		0)		\
	d(FIELD_TYPE_TEXT_STRING,	TextString,			1,		1)		\
	d(FIELD_TYPE_TEXT_ARRAY,	TextArray,			0,		1)		\
	d(FIELD_TYPE_BINARY_STRING,	BinaryString,		1,		1)		\
	d(FIELD_TYPE_BINARY_ARRAY,	BinaryArray,		0,		1)		\
	d(FIELD_TYPE_BLOB,			Blob,				4,		4)		\
	d(FIELD_TYPE_CRC_BLOB,		CrcBlob,			4,		4)

/**
 *	@brief	The field type enumeration declaration.
 */
#define ACTIV_DECLARE_FIELD_TYPE_ENUMERATIONS(id, name, size1, size2)	id,

namespace Activ
{

typedef uint8_t FieldType;			///< The field type type definition.

/**
 *	@brief	Enumeration of all field types.
 */
enum
{
	ACTIV_FIELD_TYPE_LIST(ACTIV_DECLARE_FIELD_TYPE_ENUMERATIONS)
	NUMBER_OF_FIELD_TYPES,
	FIELD_TYPE_UNKNOWN = static_cast<FieldType>(~0),
};

#undef ACTIV_DECLARE_FIELD_TYPE_ENUMERATIONS

/**
 *	@brief	Convert a field type to a string.
 *
 *	@param	fieldType the field type.
 *
 *	@return	the string corresponding to the field type.
 */
ACTIV_FIELD_TYPES_API std::string FieldTypeToString(const FieldType fieldType);

/**
 *	@brief	Convert a string to a field type.
 *
 *	@param	name the name of the field type
 *
 *	@return	the field type the corresponds to the name (or FIELD_TYPE_UNKNOWN).
 */
ACTIV_FIELD_TYPES_API FieldType StringToFieldType(const std::string &name);

/**
 *	@brief	Convert a field type to the field length size for a record message.
 *
 *	@param	fieldType the field type.
 *
 *	@return	the length corresponding to the field type.
 */
ACTIV_FIELD_TYPES_API size_t FieldTypeToRecordLengthSize(const FieldType fieldType);

/**
 *	@brief	Convert a field type to the field length size for an update message.
 *
 *	@param	fieldType the field type.
 *
 *	@return	the length corresponding to the field type.
 */
ACTIV_FIELD_TYPES_API size_t FieldTypeToUpdateLengthSize(const FieldType fieldType);

} // namespace Activ

#endif // !defined (ACTIV_FIELD_TYPES_H)
