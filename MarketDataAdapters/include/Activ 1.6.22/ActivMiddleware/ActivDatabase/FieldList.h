/**
 *  @file	FieldList.h
 *
 *	@brief	Header file for the field list class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_FIELD_LIST_H)
#define ACTIV_DATABASE_FIELD_LIST_H

#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"
#include "ActivMiddleware/ActivFieldTypes/BinaryArray.h"
#include "ActivMiddleware/ActivFieldTypes/BinaryString.h"
#include "ActivMiddleware/ActivFieldTypes/Blob.h"
#include "ActivMiddleware/ActivFieldTypes/CrcBlob.h"
#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/DateTime.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"
#include "ActivMiddleware/ActivFieldTypes/SInt.h"
#include "ActivMiddleware/ActivFieldTypes/TextArray.h"
#include "ActivMiddleware/ActivFieldTypes/TextString.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/TRational.h"
#include "ActivMiddleware/ActivFieldTypes/UInt.h"

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"

#include "ActivMiddleware/Misc/StlHelper.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

namespace Activ
{

class MessageBuilder;
class MessageValidater;

namespace Database
{

class Field;

/**
 *	@brief	Declaration of the field list class.
 */
class FieldList
{
public:
	/**
	 *	@brief	Const iterator class.
	 */
	ACTIV_DECLARE_NESTED_CLASS(ConstIterator)
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ConstIterator();

		/**
		 *	@brief	Copy constructor.
		 *
		 *	@param	rhs the value to assign to this object.
		 *
		 *	@throw	DatabaseException
		 */
		ConstIterator(const ConstIterator &rhs);

		/**
		 *	@brief	Prefix increment operator.
		 *
		 *	@return	a reference to the updated object.
		 */
		ACTIV_DATABASE_API ConstIterator &operator++();

		/**
		 *	@brief	Postfix increment operator.
		 *
		 *	@return	a copy of the original object.
		 */
		const ConstIterator operator++(int);

		/**
		 *	@brief	Equality operator.
		 *
		 *	@param	rhs the object to test against.
		 *
		 *	@return	whether the two objects are equal.
		 */
		bool operator==(const ConstIterator &rhs) const;

		/**
		 *	@brief	Inequality operator.
		 *
		 *	@param	rhs the object to test against.
		 *
		 *	@return	whether the two objects are not equal.
		 */
		bool operator!=(const ConstIterator &rhs) const;

		/**
		 *	@brief	Get the id of the field.
		 *
		 *	@return	the id of the field.
		 */
		ACTIV_DATABASE_API FieldSpecification::Id GetId() const;

		/**
		 *	@brief	Get the name of the field.
		 *
		 *	@return	the name of the field.
		 */
		ACTIV_DATABASE_API std::string GetName() const;

		/**
		 *	@brief	Get the type of the field.
		 *
		 *	@return	the type of the field.
		 */
		ACTIV_DATABASE_API FieldSpecification::Type GetType() const;

		/**
		 *	@brief	Get the specification of the field.
		 *
		 *	@return	the specification of the field.
		 */
		ACTIV_DATABASE_API FieldSpecification GetSpecification() const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	pFieldContainer the field list to iterate through.
		 *	@param	fieldNo the number of the field.
		 *
		 *	@param	iterator the iterator.
		 */
		ConstIterator(const std::vector<Field> *pFieldContainer, const size_t fieldNo);

		const std::vector<Field>		*m_pFieldContainer;		///< The container holding the fields to iterate through.
		size_t							m_fieldNo;				///< The number of the field.

		friend class FieldList;
	};

	/**
	 *	@brief	Default constructor.
	 *
	 *	@throw	bad_alloc
	 */
	FieldList();

	/**
	 *	@brief	Destructor.
	 */
	virtual ~FieldList();

	/**
	 *	@brief	Create a lookup list for the field identifiers.
	 *
	 *	@param	fieldSpecificationList list of field specifications in the field list.
	 */
	void CreateFieldIdLookupList(const FieldSpecificationList &fieldSpecificationList);

	/**
	 *	@brief	Create a map of field identifiers to field numbers.
	 *
	 *	@param	fieldSpecificationList list of field specifications in the field list.
	 */
	void CreateFieldIdMap(const FieldSpecificationList &fieldSpecificationList);

	/**
	 *	@brief	Put the field list specification into a message.
	 *
	 *	@param	messageBuilder the message builder into which the field specification is put.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_DATABASE_API StatusCode SerializeSpecification(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Get the field list specification out of a message.
	 *
	 *	@param	messageValidater the message validater containing the field list specification.
	 *	@param	fieldSpecificationList field specification list.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API static StatusCode DeserializeSpecification(MessageValidater &messageValidater, FieldSpecificationList &fieldSpecificationList);

	/**
	 *	@brief	Serialize the field list data into a message.
	 *
	 *	@param	messageBuilder the message builder into which the field list data will be placed.
	 *	@param	pSourceData pointer to source.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_DATABASE_API virtual StatusCode Serialize(MessageBuilder &messageBuilder, const byte_t *pSourceData) const = 0;

	/**
	 *	@brief	Deserialize the field list data information from a message.
	 *
	 *	@param	messageValidater message validater containing the field list data.
	 *	@param	pTargetData pointer to target.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_DATABASE_API virtual StatusCode Deserialize(MessageValidater &messageValidater, byte_t *pTargetData) const = 0;

	/**
	 *	@brief	Reset data.
	 *
	 *	@param	pTargetData pointer to target data.
	 */
	ACTIV_DATABASE_API virtual void ResetData(byte_t *pTargetData) const = 0;

	/**
	 *	@brief	Set field by source field list and source field number.
	 *
	 *	@param	targetFieldNo the target field number.
	 *	@param	pTargetData a pointer to the target data where the field will be stored.
	 *	@param	sourceFieldNo the source field number.
	 *	@param	sourceFieldList source field list.
	 *	@param	pSourceData pointer to source.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	virtual StatusCode SetField(const size_t targetFieldNo, byte_t *pTargetData, const size_t sourceFieldNo, const FieldList &sourceFieldList, const byte_t *pSourceData) const = 0;

	/**
	 *	@brief	Set field by field number from a supplied pointer and length.
	 *
	 *	@param	fieldNo the number of the field to set.
	 *	@param	pTargetData pointer to target data where the field will be stored.
	 *	@param	pField pointer to the field to be set in the target data.
	 *	@param	fieldLength the length of the field to be set in the target data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	virtual StatusCode SetField(const size_t fieldNo, byte_t *pTargetData, const void *pField, const size_t fieldLength) const = 0;

	/**
	 *	@brief	Set field by field id from a supplied pointer and length.
	 *
	 *	@param	pTargetData pointer to target data where the field will be stored.
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	pField pointer to the field to be set in the target data.
	 *	@param	fieldLength the length of the field to be set in the target data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_DATABASE_API StatusCode SetField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength) const;

	/**
	 *	@brief	Set field by field id from a supplied pointer and length.
	 *
	 *	@param	pTargetData pointer to target data where the field will be stored.
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	pField pointer to the field to be set in the target data.
	 *	@param	fieldLength the length of the field to be set in the target data.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_DATABASE_API StatusCode SetField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength, bool &isDifferent) const;

	/**
	 *	@brief	Set a field.
	 *
	 *	This function fails if the identified field's type is not signed integral.
	 *
	 *	@param	pTargetData pointer to target data where the field will be stored.
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	value integral value to store in the field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	template <typename T> StatusCode SetField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const T &value) const;

	/**
	 *	@brief	Set a field and determine whether the field's value changed.
	 *
	 *	This function fails if the identified field's type is not signed integral.
	 *
	 *	@param	pTargetData pointer to target data where the field will be stored.
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	value integral value to store in the field.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	template <typename T> StatusCode SetField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const T &value, bool &isDifferent) const;

	/**
	 *	@brief	Set a field from a boolean value.
	 *
	 *	This function fails if the identified field's type is not boolean.
	 *
	 *	@param	pTargetData pointer to target data where the field will be stored.
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	value boolean value to store in the field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API virtual StatusCode SetField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const bool value) const = 0;

	/**
	 *	@brief	Get the matching field from the source data and copy its contents to the provided field buffer.
	 *
	 *	@param	pSourceData pointer to the source of the field.
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	pField the matching field's contents will be copied to here.
	 *	@param	fieldSize length of the buffer the field will be stored in.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetField(const byte_t *pSourceData, const FieldSpecification::Id fieldId, void *pField, const size_t fieldSize, size_t &fieldLength) const;

	/**
	 *	@brief	Get a pointer to and the length of a field within the source data.
	 *
	 *	@param	fieldNo the number of the field to retrieve.
	 *	@param	pSourceData pointer to the source of the field.
	 *	@param	pField pointer that will be set to the position of the field within the record.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode GetField(const size_t fieldNo, const byte_t *pSourceData, const void *&pField, size_t &fieldLength) const;

	/**
	 *	@brief	Get a const pointer to and the length of a field within the source data.
	 *
	 *	@param	pSourceData pointer to the source of the field.
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	pField pointer that will be set to the position of the field within the record.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetField(const byte_t *pSourceData, const FieldSpecification::Id fieldId, const void *&pField, size_t &fieldLength) const;

	/**
	 *	@brief	Get a pointer to and the length of a field within the source data.
	 *
	 *	@param	pSourceData pointer to the source of the field.
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	pField pointer that will be set to the position of the field within the record.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetField(byte_t *pSourceData, const FieldSpecification::Id fieldId, void *&pField, size_t &fieldLength) const;

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	pSourceData pointer to target.
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	value value to receive the field contents.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	template <typename T> StatusCode GetField(const byte_t *pSourceData, const FieldSpecification::Id fieldId, T &value) const;

	/**
	 *	@brief	Copy all matching fields from source field list data.
	 *
	 *	@param	pTargetData pointer to target data where the field will be copied to.
	 *	@param	sourceFieldList the source field list.
	 *	@param	pSourceData pointer to source data where field is copied from.
	 *	@param	copyAllFields whether all fields must be found in the source field list.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_DATABASE_API 	StatusCode CopyFields(byte_t *pTargetData, const FieldList &sourceFieldList, const byte_t *pSourceData, const bool copyAllFields = false) const;

	/**
	 *	@brief	Copy a field from source field list data.
	 *
	 *	@param	fieldId the id of the field to copy.
	 *	@param	pTargetData pointer to target data where the field will be copied to.
	 *	@param	sourceFieldList the source field list.
	 *	@param	pSourceData pointer to source data where field is copied from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode CopyField(const FieldSpecification::Id fieldId, byte_t *pTargetData, const FieldList &sourceFieldList, const byte_t *pSourceData) const;

	/**
	 *	@brief	Copy a field from source field list data.
	 *
	 *	@param	fieldId the id of the field to copy.
	 *	@param	pTargetData pointer to target data where the field will be copied to.
	 *	@param	sourceFieldList the source field list.
	 *	@param	pSourceData pointer to source data where field is copied from.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode CopyField(const FieldSpecification::Id fieldId, byte_t *pTargetData, const FieldList &sourceFieldList, const byte_t *pSourceData, bool &isDifferent) const;

	/**
	 *	@brief	Copy a field from source field list data.
	 *
	 *	@param	targetFieldId the identifier of the field to set.
	 *	@param	pTargetData pointer to target data where the field will be copied to.
	 *	@param	sourceFieldList the source field list.
	 *	@param	sourceFieldId the identifier of the field to copy.
	 *	@param	pSourceData pointer to source data where field is copied from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API 	StatusCode CopyField(const FieldSpecification::Id targetFieldId, byte_t *pTargetData, const FieldList &sourceFieldList, const FieldSpecification::Id sourceFieldId, const byte_t *pSourceData) const;

	/**
	 *	@brief	Copy a field from source field list data.
	 *
	 *	@param	targetFieldId the identifier of the field to set.
	 *	@param	pTargetData pointer to target data where the field will be copied to.
	 *	@param	sourceFieldList the source field list.
	 *	@param	sourceFieldId the identifier of the field to copy.
	 *	@param	pSourceData pointer to source data where field is copied from.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode CopyField(const FieldSpecification::Id targetFieldId, byte_t *pTargetData, const FieldList &sourceFieldList, const FieldSpecification::Id sourceFieldId, const byte_t *pSourceData, bool &isDifferent) const;

	/**
	 *	@brief	Compare a field in a field list with a supplied field.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	iFieldType the field to compare with the record.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API virtual StatusCode CompareField(const FieldSpecification::Id fieldId, const byte_t *pLhsData, const IFieldType &iFieldType, int &result) const = 0;

	/**
	 *	@brief	Compare a field in a field list with a supplied string.
	 *
	 *	Only works for string and array based fields.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	value the field to compare with the record.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API virtual StatusCode CompareField(const FieldSpecification::Id fieldId, const byte_t *pLhsData, const std::string &value, int &result) const = 0;

	/**
	 *	@brief	Compare two fields of the same type.
	 *
	 *	@param	lhsFieldId the id of the left hand field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	rhsFieldList the right hand field list.
	 *	@param	rhsFieldId the id of the right hand field to compare.
	 *	@param	pRhsData pointer to right hand data to compare.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API virtual StatusCode CompareField(const FieldSpecification::Id lhsFieldId, const byte_t *pLhsData, const FieldList &rhsFieldList, const FieldSpecification::Id rhsFieldId, const byte_t *pRhsData, int &result) const;

	/**
	 *	@brief	Check whether a field is different than a supplied field buffer and length.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pField pointer to field.
	 *	@param	fieldLength field length.
	 *	@param	result set equal to true if the field has been modified, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const byte_t *pLhsData, const void *pField, const size_t fieldLength, bool &result) const;

	/**
	 *	@brief	Check whether a field is different than a supplied field.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	iFieldType the field to compare with the record.
	 *	@param	result set equal to true if the field has been modified, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const byte_t *pLhsData, const IFieldType &iFieldType, bool &result) const;

	/**
	 *	@brief	Check whether a field is different than a supplied string.
	 *
	 *	Only works for string and array based fields.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	value the field to compare with the record.
	 *	@param	result set equal to true if the field has been modified, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const byte_t *pLhsData, const std::string &value, bool &result) const;

	/**
	 *	@brief	Check whether a field is different between two data buffers.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	rhsFieldList the right hand field list to compare with.
	 *	@param	pRhsData right hand data to compare.
	 *	@param	result set equal to true if the field has been modified, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const byte_t *pLhsData, const FieldList &rhsFieldList, const byte_t *pRhsData, bool &result) const;

	/**
	 *	@brief	Check whether a field is different between two data buffers.
	 *
	 *	@param	lhsFieldId the id of the left hand field to compare.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	rhsFieldList the right hand field list to compare with.
	 *	@param	rhsFieldId the id of the right hand field to compare.
	 *	@param	pRhsData right hand data to compare.
	 *	@param	result set equal to true if the field has been modified, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode IsFieldDifferent(const FieldSpecification::Id lhsFieldId, const byte_t *pLhsData, const FieldList &rhsFieldList, const FieldSpecification::Id rhsFieldId, const byte_t *pRhsData, bool &result) const;

	/**
	 *	@brief	Define a field.
	 *
	 *	@param	pTargetData pointer to target data where the field will be defined.
	 *	@param	fieldId the identifier of the field to define.
	 *	@param	fieldLength the length of the field being defined. The default sets the field length to the maximum.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API virtual StatusCode DefineField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const size_t fieldLength = ~0) const = 0;

	/**
	 *	@brief	Undefine a field.
	 *
	 *	@param	pTargetData pointer to target data where the field will be undefined.
	 *	@param	fieldId the identifier of the field to undefine.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_SUPPORTED
	 */
	ACTIV_DATABASE_API virtual StatusCode UndefineField(byte_t *pTargetData, const FieldSpecification::Id fieldId) const = 0;

	/**
	 *	@brief	Undefine a field.
	 *
	 *	@param	pTargetData pointer to target data where the field will be undefined.
	 *	@param	fieldId the identifier of the field to undefine.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_SUPPORTED
	 */
	ACTIV_DATABASE_API virtual StatusCode UndefineField(byte_t *pTargetData, const FieldSpecification::Id fieldId, bool &isDifferent) const = 0;

	/**
	 *	@brief	Check whether a field is defined.
	 *
	 *	@param	pSourceData pointer to source data where the field will be checked.
	 *	@param	fieldId the identifier of the field to define.
	 *	@param	result set equal to true if the field is defined, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API virtual StatusCode IsFieldDefined(const byte_t *pSourceData, const FieldSpecification::Id fieldId, bool &result) const = 0;

	/**
	 *	@brief	Define field.
	 *
	 *	@param	fieldNo the number of the field to define.
	 *	@param	pTargetData pointer to target data where the field will be defined.
	 */
	virtual void DefineField(const size_t fieldNo, byte_t *pTargetData) const = 0;

	/**
	 *	@brief	Undefine field.
	 *
	 *	@param	fieldNo the number of the field to undefine.
	 *	@param	pTargetData pointer to target data where the field will be defined.
	 */
	virtual void UndefineField(const size_t fieldNo, byte_t *pTargetData) const = 0;

	/**
	 *	@brief	Check whether a field is defined.
	 *
	 *	@param	fieldNo the number of the field to check.
	 *	@param	pSourceData pointer to source data where the field will be checked.
	 *
	 *	@return	true if the specified field is defined, false otherwise.
	 */
	virtual bool IsFieldDefined(const size_t fieldNo, const byte_t *pSourceData) const = 0;

	/**
	 *	@brief	Compare two data buffers of the same format.
	 *
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pRhsData right hand data to compare.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	ACTIV_DATABASE_API int Compare(const byte_t *pLhsData, const byte_t *pRhsData) const;

	/**
	 *	@brief	Partially compare two data buffers of the same format.
	 *
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pRhsData right hand data to compare.
	 *	@param	lastFieldNo the field number of the last field to be compared.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	ACTIV_DATABASE_API int Compare(const byte_t *pLhsData, const byte_t *pRhsData, const size_t lastFieldNo) const;

	/**
	 *	@brief	Partially compare two data buffers of the same format.
	 *
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pRhsData right hand data to compare.
	 *	@param	lastFieldNo the field number of the last field to be compared.
	 *	@param	lastCompareLength the amount of the last field that should be compared.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	ACTIV_DATABASE_API int Compare(const byte_t *pLhsData, const byte_t *pRhsData, const size_t lastFieldNo, const size_t lastCompareLength) const;

	/**
	 *	@brief	Compare the specified field in two data buffers of the same format.
	 *
	 *	@param	fieldNo the field number of the field to be compared.
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pRhsData right hand data to compare.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	ACTIV_DATABASE_API int Compare(const size_t fieldNo, const byte_t *pLhsData, const byte_t *pRhsData) const;

	/**
	 *	@brief	Compare two key data buffers of the same format.
	 *
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pRhsData right hand data to compare.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	ACTIV_DATABASE_API int CompareKey(const byte_t *pLhsData, const byte_t *pRhsData) const;

	/**
	 *	@brief	Partially compare two data buffers of the same format.
	 *
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pRhsData right hand data to compare.
	 *	@param	lastFieldNo the field number of the last field to be compared.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	ACTIV_DATABASE_API int CompareKey(const byte_t *pLhsData, const byte_t *pRhsData, const size_t lastFieldNo) const;

	/**
	 *	@brief	Partially compare two data buffers of the same format.
	 *
	 *	@param	pLhsData left hand data to compare.
	 *	@param	pRhsData right hand data to compare.
	 *	@param	lastFieldNo the field number of the last field to be compared.
	 *	@param	lastCompareLength the amount of the last field that should be compared.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	ACTIV_DATABASE_API int CompareKey(const byte_t *pLhsData, const byte_t *pRhsData, const size_t lastFieldNo, const size_t lastCompareLength) const;

	/**
	 *	@brief	Get the length of data stored using this field list.
	 *
	 *	@return	the length of the data.
	 */
	size_t GetDataLength() const;

	/**
	 *	@brief	Get the number of fields in the field list.
	 *
	 *	@return	the number of fields in the field list.
	 */
	size_t GetNumberOfFields() const;

	/**
	 *	@brief	Get the list of all field specifications.
	 *
	 *	@return	the field specification list.
	 */
	ACTIV_DATABASE_API FieldSpecificationList GetFieldSpecificationList() const;

	/**
	 *	@brief	Get the field specification of the first field in the field list.
	 *
	 *	@param	fieldSpecification variable to store field specification in.
	 */
	ACTIV_DATABASE_API void GetFirstFieldSpecification(FieldSpecification &fieldSpecification) const;

	/**
	 *	@brief	Get the field specification of the next field in the field list.
	 *
	 *	@param	fieldSpecification variable to store field specification in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetNextFieldSpecification(FieldSpecification &fieldSpecification) const;

	/**
	 *	@brief	Get the list of all field identifiers.
	 *
	 *	@return	the field identifier list.
	 */
	ACTIV_DATABASE_API FieldIdList GetFieldIdList() const;

	/**
	 *	@brief	Get the id of the first field in the field list object.
	 *
	 *	@param	fieldId set equal to the first field id in the field list object
	 */
	ACTIV_DATABASE_API void GetFirstFieldId(FieldSpecification::Id &fieldId) const;

	/**
	 *	@brief	Get the id of the next field in the field list object.
	 *
	 *	@param	fieldId set equal to the next field id in the field list object
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetNextFieldId(FieldSpecification::Id &fieldId) const;

	/**
	 *	@brief	Get the field number of the field with the matching field identifier.
	 *
	 *	@param	fieldId the field identifier to find in the field list.
	 * 	@param	fieldNo set equal to the field number of the matching field.
	 *
	 *	@return	true if the field identifier is found in the field list, false otherwise.
	 */
	bool GetFieldNumber(const FieldSpecification::Id fieldId, size_t &fieldNo) const;

	/**
	 *	@brief	Get the field number of the field with the matching field specification.
	 *
	 *	@param	fieldSpecification the field specification to find in the field list.
	 * 	@param	fieldNo set equal to the field number of the matching field.
	 *
	 *	@return	true if the field identifier is found in the field list, false otherwise.
	 */
	ACTIV_DATABASE_API bool GetFieldNumber(const FieldSpecification &fieldSpecification, size_t &fieldNo) const;

	/**
	 *	@brief	Get the specification of a field.
	 *
	 *	@param	fieldId the field identifier to find in the field list.
	 *	@param	fieldSpecification set equal to the field specification of the matching field.
	 *	
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetFieldSpecification(const FieldSpecification::Id fieldId, FieldSpecification &fieldSpecification) const;

	/**
	 *	@brief	Get the specification of a field.
	 *
	 *	@param	fieldId the field identifier to find in the field list.
	 * 	@param	fieldNo set equal to the field number of the matching field.
	 *	@param	pFieldSpecification set to point to the field specification of the matching field.
	 *	
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetFieldSpecification(const FieldSpecification::Id fieldId, size_t &fieldNo, const FieldSpecification *&pFieldSpecification) const;

	/**
	 *	@brief	Get the field type for the field id specified.
	 *
	 *	@param	fieldId the id of the field whose specification is to be retrieved.
	 *	@param	type set equal to the type of the field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode GetType(const FieldSpecification::Id fieldId, FieldSpecification::Type &type) const;

	/**
	 *	@brief	Get the definition of the field object for the specified field number.
	 *
	 * 	@param	fieldNo the field number.
	 *
	 *	@return	a copy of the matching field object.
	 */
	const Field &GetFieldDefinition(const size_t fieldNo) const;

	/**
	 *	@brief	Get field length.
	 *
	 *	@param	fieldNo the field number of the field whose length is to be returned.
	 *	@param	pSourceData pointer to source.
	 *	
	 *	@return	length of the field.
	 */
	ACTIV_DATABASE_API virtual size_t GetFieldLength(const size_t fieldNo, const byte_t *pSourceData) const = 0;

	/**
	 *	@brief	Convert a list of fields to a string.
	 *
	 *	@param	pSourceData field data to display.
	 *
	 *	@return the fields converted to a string.
	 */
	ACTIV_DATABASE_API std::string ToString(const byte_t *pSourceData) const;

	/**
	 *	@brief	Convert a list of fields to a log string.
	 *
	 *	@param	pSourceData field data to display.
	 *
	 *	@return the fields converted to a string.
	 */
	ACTIV_DATABASE_API std::string ToLogString(const byte_t *pSourceData) const;

	/**
	 *	@brief	Get an iterator to the first field in the container.
	 *
	 *	@return an iterator object.
	 */
	ConstIterator Begin() const;

	/**
	 *	@brief	Get an iterator that points to beyond the last field in the container.
	 *
	 *	@return an iterator object.
	 */
	ConstIterator End() const;

	/**
	 *	@brief	Check whether a field id is valid for this field list object.
	 *
	 *	@return	true if the field id can be found, false otherwise.
	 */
	bool IsFound(const FieldSpecification::Id fieldId) const;

#if defined (_DEBUG)
	/**
	 *	@brief	Display the field information for the field list.
	 *
	 *	@param	os the stream to send output to.
	 */
	ACTIV_DATABASE_API void DisplayFieldInformation(std::ostream &os = std::cout) const;
#endif

protected:
	typedef hash_map<FieldSpecification::Id, size_t> FieldIdMap;	///< Definition of a map of field identifiers to field numbers.
	static const size_t		UNDEFINED_FIELD_NUMBER = ~0;			///< Undefined field number.

	std::vector<Field>		&m_fieldList;							///< List of fields in the field list.
	std::vector<size_t>		m_fieldNoList;							///< Sparsely populated list of field numbers used for field id lookups.
	FieldIdMap				m_fieldIdMap;							///< Map of field identifiers to field numbers.
	
	size_t					m_nFields;								///< Number of fields in the field list.
	size_t					m_dataLength;							///< Total length of all fields.
	
private:
	/**
	 *	@brief	Copy constructor.
	 *
	 *	Made private without a definition so that users cannot copy construct the class.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	FieldList(const FieldList &rhs);

	/**
	 *	@brief	Assignment operator.
	 *
	 *	Made private without a definition so that users cannot assign the class.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	FieldList &operator=(const FieldList &rhs);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_DATABASE_API) StatusCode SetFieldT(byte_t *pTargetData, const FieldSpecification::Id fieldId, const T &value) const;
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<char>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const char &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int8_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int8_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int16_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int16_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int32_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int32_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int64_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int64_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint8_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint8_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint16_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint16_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint32_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint32_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint64_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint64_t &value) const;)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<size_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const size_t &value) const;)
#endif
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<std::string>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const std::string &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<IFieldType>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const IFieldType &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<BinaryArray>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const BinaryArray &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<BinaryString>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const BinaryString &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Blob>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Blob &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<CrcBlob>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const CrcBlob &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Date>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Date &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<DateTime>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const DateTime &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Rational>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Rational &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<SInt>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const SInt &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<TextArray>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const TextArray &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<TextString>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const TextString &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Time>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Time &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<TRational>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const TRational &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<UInt>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const UInt &value) const;)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_DATABASE_API) StatusCode SetFieldT(byte_t *pTargetData, const FieldSpecification::Id fieldId, const T &value, bool &isDifferent) const;
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<char>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const char &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int8_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int8_t &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int16_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int16_t &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int32_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int32_t &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<int64_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const int64_t &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint8_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint8_t &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint16_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint16_t &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint32_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint32_t &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<uint64_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const uint64_t &value, bool &isDifferent) const;)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<size_t>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const size_t &value, bool &isDifferent) const;)
#endif
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<std::string>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const std::string &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<IFieldType>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const IFieldType &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<BinaryArray>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const BinaryArray &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<BinaryString>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const BinaryString &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Blob>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Blob &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<CrcBlob>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const CrcBlob &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Date>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Date &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<DateTime>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const DateTime &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Rational>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Rational &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<SInt>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const SInt &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<TextArray>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const TextArray &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<TextString>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const TextString &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<Time>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const Time &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<TRational>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const TRational &value, bool &isDifferent) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode SetFieldT<UInt>(byte_t *pTargetData, const FieldSpecification::Id fieldId, const UInt &value, bool &isDifferent) const;)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_DATABASE_API) StatusCode GetFieldT(const byte_t *pSourceData, const FieldSpecification::Id fieldId, T &value) const;
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<char>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, char &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<int8_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, int8_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<int16_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, int16_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<int32_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, int32_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<int64_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, int64_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<uint8_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, uint8_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<uint16_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, uint16_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<uint32_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, uint32_t &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<uint64_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, uint64_t &value) const;)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<size_t>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, size_t &value) const;)
#endif
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<std::string>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, std::string &field) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<IFieldType>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, IFieldType &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<BinaryArray>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, BinaryArray &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<BinaryString>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, BinaryString &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<Blob>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, Blob &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<CrcBlob>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, CrcBlob &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<Date>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, Date &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<DateTime>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, DateTime &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<Rational>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, Rational &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<SInt>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, SInt &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<TextArray>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, TextArray &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<TextString>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, TextString &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<Time>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, Time &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<TRational>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, TRational &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT<UInt>(const byte_t *pSourceData, const FieldSpecification::Id fieldId, UInt &value) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldT< std::auto_ptr<IFieldType> >(const byte_t *pSourceData, const FieldSpecification::Id fieldId, std::auto_ptr<IFieldType> &pIFieldType) const;)

	/**
	 *	@brief	Get a pointer to and the length of a field within the source data.
	 *
	 *	Made private without a definition so that users cannot call the version of GetField()
	 *	that returns a pointer to a field with a non-const pointer.
	 *
	 *	@param	pSourceData pointer to the source of the field.
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	pField pointer that will be set to the position of the field within the record.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode GetField(const byte_t *pSourceData, const FieldSpecification::Id fieldId, void *&pField, size_t &fieldLength) const;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode FieldList::SetField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const T &value) const
{
	return SetFieldT<T>(pTargetData, fieldId, value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode FieldList::SetField(byte_t *pTargetData, const FieldSpecification::Id fieldId, const T &value, bool &isDifferent) const
{
	return SetFieldT<T>(pTargetData, fieldId, value, isDifferent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode FieldList::GetField(const byte_t *pSourceData, const FieldSpecification::Id fieldId, T &value) const
{
	return GetFieldT<T>(pSourceData, fieldId, value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldList::CopyField(const FieldSpecification::Id fieldId, byte_t *pTargetData, const FieldList &sourceFieldList, const byte_t *pSourceData, bool &isDifferent) const
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, IsFieldDifferent(fieldId, pTargetData, sourceFieldList, pSourceData, isDifferent));

	return CopyField(fieldId, pTargetData, sourceFieldList, pSourceData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldList::CopyField(const FieldSpecification::Id targetFieldId, byte_t *pTargetData, const FieldList &sourceFieldList, const FieldSpecification::Id sourceFieldId, const byte_t *pSourceData, bool &isDifferent) const
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, IsFieldDifferent(targetFieldId, pTargetData, sourceFieldList, sourceFieldId, pSourceData, isDifferent));

	return CopyField(targetFieldId, pTargetData, sourceFieldList, sourceFieldId, pSourceData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldList::GetFieldNumber(const FieldSpecification::Id fieldId, size_t &fieldNo) const
{
	if ((fieldId >= m_fieldNoList.size()) || (UNDEFINED_FIELD_NUMBER == m_fieldNoList[fieldId]))
		return false;

	fieldNo = m_fieldNoList[fieldId];

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t FieldList::GetDataLength() const
{
	return m_dataLength;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t FieldList::GetNumberOfFields() const
{
	return m_nFields;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldList::ConstIterator FieldList::Begin() const
{
	return ConstIterator(&m_fieldList, 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldList::ConstIterator FieldList::End() const
{
	return ConstIterator(0, UNDEFINED_FIELD_NUMBER);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldList::IsFound(const FieldSpecification::Id fieldId) const
{
	size_t fieldNo;

	return GetFieldNumber(fieldId, fieldNo);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldList::ConstIterator::ConstIterator() :
	m_pFieldContainer(0),
	m_fieldNo(UNDEFINED_FIELD_NUMBER)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldList::ConstIterator::ConstIterator(const FieldList::ConstIterator &rhs) :
	m_pFieldContainer(rhs.m_pFieldContainer),
	m_fieldNo(rhs.m_fieldNo)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const FieldList::ConstIterator FieldList::ConstIterator::operator++(int)
{
	ConstIterator oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldList::ConstIterator::operator==(const FieldList::ConstIterator &rhs) const
{
	return ((m_pFieldContainer == rhs.m_pFieldContainer) && (m_fieldNo == rhs.m_fieldNo));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldList::ConstIterator::operator!=(const FieldList::ConstIterator &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldList::ConstIterator::ConstIterator(const std::vector<Field> *pFieldContainer, size_t fieldNo) :
	m_pFieldContainer(pFieldContainer),
	m_fieldNo(fieldNo)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

}	// namespace Database

}	// namespace Activ

#endif	// !defined(ACTIV_DATABASE_FIELD_LIST_H)
