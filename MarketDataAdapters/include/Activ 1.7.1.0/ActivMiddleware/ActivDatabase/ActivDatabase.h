/**
 *  @file	ActivDatabase.h
 *
 *	@brief	Header file for the Activ database.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_ACTIV_DATABASE_H)
#define ACTIV_DATABASE_ACTIV_DATABASE_H

#include "ActivMiddleware/ActivDatabase/External.h"

#include "ActivMiddleware/ActivFieldTypes/Blob.h"
#include "ActivMiddleware/ActivFieldTypes/CrcBlob.h"
#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/DateTime.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/TRational.h"

#include <iostream>

namespace Activ
{

namespace Database
{

class ITable;
class FieldSpecification;
	
/**
 *	@brief	Open a table file.
 *
 *	@param	tableFileName the name of the table file.
 *	@param	createRecordFile indicated whether the record file should also be created.
 *	@param	createIndexes indicates whether the indexes related to the table should also be created.
 *
 *	@throw	StatusCodeException
 *	@throw	DatabaseException
 *	@throw	OsException
 */
ACTIV_DATABASE_API ITable *OpenTable(const std::string &tableFileName, const bool createRecordFile = true, const bool createIndexes = true);

/**
 *	@brief	Check whether a field specification is valid.
 *
 *	@param	fieldSpecification the field specification to validate.
 */
void ValidateSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type boolean.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateBooleanSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type blob.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateBlobSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type crc blob.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateCrcBlobSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type date.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateDateSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type date time.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateDateTimeSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type none.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateNoneSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type rational.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateRationalSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type signed integral.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateSignedIntegralSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type time.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateTimeSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type trended rational.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateTRationalSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Check whether a field specification is valid for field type unsigned integral.
 *
 *	@param	fieldSpecification the field specification to validate.
 *
 *	@throw	DatabaseException.
 */
void ValidateUnsignedIntegralSpecification(const FieldSpecification &fieldSpecification);

/**
 *	@brief	Comparison of two blob objects.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
inline int CompareBlob(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength)
{
	int result;
	(void) Blob::Compare(pLhsData, lhsLength, pRhsData, rhsLength, result);
	return result;
}

/**
 *	@brief	Comparison of two crc blob objects.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
inline int CompareCrcBlob(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength)
{
	int result;
	(void) CrcBlob::Compare(pLhsData, lhsLength, pRhsData, rhsLength, result);
	return result;
}

/**
 *	@brief	Comparison of two date objects.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
inline int CompareDate(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength)
{
	int result;
	(void) Date::Compare(pLhsData, lhsLength, pRhsData, rhsLength, result);
	return result;
}

/**
 *	@brief	Comparison of two date time objects.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
inline int CompareDateTime(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength)
{
	int result;
	(void) DateTime::Compare(pLhsData, lhsLength, pRhsData, rhsLength, result);
	return result;
}

/**
 *	@brief	Comparison of two rational objects.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
inline int CompareRational(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength)
{
	// TODO: Need to work out what the appropriate behaviour is if Compare fails
	int result;
	(void) Rational::Compare(pLhsData, lhsLength, pRhsData, rhsLength, result);
	return result;
}

/**
 *	@brief	Comparison of two time objects.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
inline int CompareTime(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength)
{
	int result;
	(void) Time::Compare(pLhsData, lhsLength, pRhsData, rhsLength, result);
	return result;
}

/**
 *	@brief	Comparison of two trational objects.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
inline int CompareTRational(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength)
{
	// TODO: Need to work out what the appropriate behaviour is if Compare fails
	int result;
	(void) TRational::Compare(pLhsData, lhsLength, pRhsData, rhsLength, result);
	return result;
}

/**
 *	@brief	Unsigned comparison of memory locations.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
int CompareUnsignedMemory(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength);

/**
 *	@brief	Unsigned memory comparison.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	length length to compare.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
int Umemcmp(const byte_t *pLhsData, const byte_t *pRhsData, size_t length);

/**
 *	@brief	Comparison of signed integral types.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
int CompareSignedIntegral(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength);

/**
 *	@brief	Comparison of unsigned integral types.
 *
 *	@param	pLhsData left hand data buffer.
 *	@param	lhsLength length of left hand data buffer.
 *	@param	pRhsData right hand data buffer.
 *	@param	rhsLength length of right hand data buffer.
 *
 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
 */
int CompareUnsignedIntegral(const byte_t *pLhsData, const size_t lhsLength, const byte_t *pRhsData, const size_t rhsLength);

/**
 *	@brief	Get a signed integer 64 number from a length of binary data.
 *
 *	@param	pData Data buffer to convert.
 *	@param	length length of data buffer.
 *
 *	@return	Signed integer 64 value calculated from the binary data, or 0 if an invalid length was supplied.
 */
int64_t GetSignedInt64(const void *pData, const size_t length);

/**
 *	@brief	Get an unsigned integer 64 number from a length of binary data.
 *
 *	@param	pData Data buffer to convert.
 *	@param	length length of data buffer.
 *
 *	@return	Unsigned integer 64 value calculated from the binary data, or 0 if an invalid length was supplied.
 */
uint64_t GetUnsignedInt64(const void *pData, const size_t length);

/**
 *	@brief	Convert a binary buffer to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
*/
std::string BinaryToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a boolean to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of the date object to output.
 *
 *	@return the data converted to a string.
 */
std::string BooleanToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a blob to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of the date object to output.
 *
 *	@return the data converted to a string.
 */
std::string BlobToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a crc blob to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of the date object to output.
 *
 *	@return the data converted to a string.
 */
std::string CrcBlobToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a Date object to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of the date object to output.
 *
 *	@return the data converted to a string.
 */
std::string DateToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a DateTime object to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
 */
std::string DateTimeToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a Rational object to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
 */
std::string RationalToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a signed integral value to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
 */
std::string SignedIntegralToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a text buffer to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
 */
std::string TextToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a Time object to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
 */
std::string TimeToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert a TRational object to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
 */
std::string TRationalToString(const byte_t *pSourceData, const size_t length);

/**
 *	@brief	Convert an unsigned integral to a string.
 *
 *	@param	pSourceData text to output.
 *	@param	length length of text to output.
 *
 *	@return the data converted to a string.
 */
std::string UnsignedIntegralToString(const byte_t *pSourceData, const size_t length);

}	// namespace Database

}	// namespace Activ

#endif	// !defined(ACTIV_DATABASE_ACTIV_DATABASE_H)
