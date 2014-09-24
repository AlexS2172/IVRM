/**
 *  @file	FieldListObject.h
 *
 *	@brief	Header file for the field list object class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_FIELD_LIST_OBJECT_H)
#define ACTIV_DATABASE_FIELD_LIST_OBJECT_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"
#include "ActivMiddleware/ActivDatabase/ITable.h"
#include "ActivMiddleware/ActivDatabase/FieldList.h"

#include <iostream>
#include <string>
#include <memory>

namespace Activ
{

class MessageBuilder;
class MessageValidater;

namespace Database
{

/**
 *	@brief	Field list object class.
 */
class FieldListObject
{
public:
	typedef FieldList::ConstIterator ConstIterator;			///< Definition of an iterator through the field list obejct.

	/**
	 *	@brief	Constructor.
	 */
	FieldListObject();

	/**
	 *	@brief	Destructor.
	 */
	virtual ~FieldListObject() = 0;

	/**
	 *	@brief	Initialize.
	 */
	void Initialize();

	/**
	 *	@brief	Initialize.
	 *
	 *	@param	pTable the table to initialize the field list object for.
	 *	@param	pFieldList the field list.
	 *	@param	requiredBufferSize the required buffer size.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Initialize(ITable *pTable, const FieldList *pFieldList, const size_t requiredBufferSize);

	/**
	 *	@brief	Serialize the field list object information into a message.
	 *
	 *	@param	messageBuilder the message builder into which the field list object data will be placed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder) const = 0;

	/**
	 *	@brief	Deserialize the field list object information from a message.
	 *
	 *	@param	pTable table the object will be stored in.
	 *	@param	messageValidater message validater containing the field list object data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	virtual StatusCode Deserialize(ITable *pTable, MessageValidater &messageValidater) = 0;

	/**
	 *	@brief	Reset the contents of the field list object data buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	virtual StatusCode Reset() = 0;

	/**
	 *	@brief	Set a field from a supplied pointer and length.
	 *
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	pField pointer to the field to be set in the record.
	 *	@param	fieldLength the length of the field to be set in the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode SetField(const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength);

	/**
	 *	@brief	Set a field from a supplied pointer and length.
	 *
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	pField pointer to the field to be set in the record.
	 *	@param	fieldLength the length of the field to be set in the record.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode SetField(const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength, bool &isDifferent);

	/**
	 *	@brief	Set a field.
	 *
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	value the value to store in the field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	template <typename T> StatusCode SetField(const FieldSpecification::Id fieldId, const T &value);

	/**
	 *	@brief	Set a field.
	 *
	 *	@param	fieldId the identifier of the field to set.
	 *	@param	value the value to store in the field.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	template <typename T> StatusCode SetField(const FieldSpecification::Id fieldId, const T &value, bool &isDifferent);

	/**
	 *	@brief	Get the matching field from the record and copy its contents to the provided field buffer.
	 *
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	pField the matching field's contents will be copied to here.
	 *	@param	fieldSize size of the buffer the field will be stored in.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	StatusCode GetField(const FieldSpecification::Id fieldId, void *pField, const size_t fieldSize, size_t &fieldLength) const;

	/**
	 *	@brief	Get a pointer to and the length of a field within the record.
	 *
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	pField pointer that will be set to the position of the field within the record.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	StatusCode GetField(const FieldSpecification::Id fieldId, const void *&pField, size_t &fieldLength) const;

	/**
	 *	@brief	Get a pointer to and the length of a field within the record.
	 *
	 *	The field length is set to zero if the field is undefined.
	 *
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	pField pointer that will be set to the position of the field within the record.
	 *	@param	fieldLength the retrieved field's length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	StatusCode GetField(const FieldSpecification::Id fieldId, void *&pField, size_t &fieldLength);

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	fieldId the identifier of the field to retrieve.
	 *	@param	value extracted field is stored in this variable.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	template <typename T> StatusCode GetField(const FieldSpecification::Id fieldId, T &value) const;

	/**
	 *	@brief	Copy all matching fields from a supplied field list object.
	 *
	 *	@param	sourceFieldListObject the field list object where fields are copied from.
	 *	@param	copyAllFields whether all fields must be found in the source record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode CopyFields(const FieldListObject &sourceFieldListObject, const bool copyAllFields = false);

	/**
	 *	@brief	Copy a field.
	 *
	 *	@param	fieldId the id of the field to copy.
	 *	@param	sourceFieldListObject the field list object the field is copied from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CopyField(const FieldSpecification::Id fieldId, const FieldListObject &sourceFieldListObject);

	/**
	 *	@brief	Copy a field.
	 *
	 *	@param	fieldId the id of the field to copy.
	 *	@param	sourceFieldListObject the field list object the field is copied from.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CopyField(const FieldSpecification::Id fieldId, const FieldListObject &sourceFieldListObject, bool &isDifferent);

	/**
	 *	@brief	Copy a field from a supplied field list object.
	 *
	 *	@param	targetFieldId the identifier of the field to set.
	 *	@param	sourceFieldListObject the field list object the field is copied from.
	 *	@param	sourceFieldId the identifier of the field to copy.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CopyField(const FieldSpecification::Id targetFieldId, const FieldListObject &sourceFieldListObject, const FieldSpecification::Id sourceFieldId);

	/**
	 *	@brief	Copy a field from a supplied field list object.
	 *
	 *	@param	targetFieldId the identifier of the field to set.
	 *	@param	sourceFieldListObject the field list object the field is copied from.
	 *	@param	sourceFieldId the identifier of the field to copy.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CopyField(const FieldSpecification::Id targetFieldId, const FieldListObject &sourceFieldListObject, const FieldSpecification::Id sourceFieldId, bool &isDifferent);

	/**
	 *	@brief	Compare a field in a field list object with a supplied field.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	iFieldType the field to compare with the record.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CompareField(const FieldSpecification::Id fieldId, const IFieldType &iFieldType, int &result) const;

	/**
	 *	@brief	Compare a field in a field list object with a supplied string.
	 *
	 *	Only works for string and array based fields.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	value the string to compare with the record.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CompareField(const FieldSpecification::Id fieldId, const std::string &value, int &result) const;

	/**
	 *	@brief	Compare a field in two field list objects.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	rhsFieldListObject the right hand field list object to compare against.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CompareField(const FieldSpecification::Id fieldId, const FieldListObject &rhsFieldListObject, int &result) const;

	/**
	 *	@brief	Compare two fields of the same type in two field list objects.
	 *
	 *	@param	lhsFieldId the id of the left hand field to compare.
	 *	@param	rhsFieldListObject the right hand field list object to compare against.
	 *	@param	rhsFieldId the id of the right hand field to compare.
	 *	@param	result the result of the comparison.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode CompareField(const FieldSpecification::Id lhsFieldId, const FieldListObject &rhsFieldListObject, const FieldSpecification::Id rhsFieldId, int &result) const;

	/**
	 *	@brief	Check whether a field is different than a supplied field buffer and length.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	pField pointer to field.
	 *	@param	fieldLength field length.
	 *	@param	result set equal to true if the field is different, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength, bool &result) const;

	/**
	 *	@brief	Check whether a field is different than a supplied field.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	iFieldType the field to compare with the record.
	 *	@param	result set equal to true if the field is different, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const IFieldType &iFieldType, bool &result) const;

	/**
	 *	@brief	Check whether a field is different than a supplied string.
	 *
	 *	Only works for string and array based fields.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	value the string to compare with the record.
	 *	@param	result set equal to true if the field is different, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const std::string &value, bool &result) const;

	/**
	 *	@brief	Check whether a field is different between two field list objects.
	 *
	 *	@param	fieldId the id of the field to compare.
	 *	@param	rhsFieldListObject the right hand field list object to compare against.
	 *	@param	result set equal to true if the field is different, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode IsFieldDifferent(const FieldSpecification::Id fieldId, const FieldListObject &rhsFieldListObject, bool &result) const;

	/**
	 *	@brief	Check whether two fields of the same type are different between two field list objects.
	 *
	 *	@param	lhsFieldId the id of the left hand field to compare.
	 *	@param	rhsFieldListObject the right hand field list object to compare against.
	 *	@param	rhsFieldId the id of the right hand field to compare.
	 *	@param	result set equal to true if the field is different, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode IsFieldDifferent(const FieldSpecification::Id lhsFieldId, const FieldListObject &rhsFieldListObject, const FieldSpecification::Id rhsFieldId, bool &result) const;

	/**
	 *	@brief	Define a field.
	 *
	 *	@param	fieldId the identifier of the field to define.
	 *	@param	fieldLength the length of the field being defined. The default sets the field length to the maximum.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode DefineField(const FieldSpecification::Id fieldId, const size_t fieldLength = ~0);

	/**
	 *	@brief	Undefine a field.
	 *
	 *	@param	fieldId the identifier of the field to undefine.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode UndefineField(const FieldSpecification::Id fieldId);

	/**
	 *	@brief	Undefine a field.
	 *
	 *	@param	fieldId the identifier of the field to undefine.
	 *	@param	isDifferent set to true if the field changed as a result of the call.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode UndefineField(const FieldSpecification::Id fieldId, bool &isDifferent);

	/**
	 *	@brief	Check whether a field is defined.
	 *
	 *	@param	fieldId the identifier of the field to define.
	 *	@param	result set equal to true if the field is defined, false otherwise.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode IsFieldDefined(const FieldSpecification::Id fieldId, bool &result) const;

	/**
	 *	@brief	Get the list of field specifications.
	 *
	 *	@param	fieldSpecificationList field specification list.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetFieldSpecificationList(FieldSpecificationList &fieldSpecificationList) const;

	/**
	 *	@brief	Get the field specification for the field id specified.
	 *
	 *	@param	fieldId the id of the field whose specification is to be retrieved.
	 *	@param	fieldSpecification the variable that will hold the retrieved field specification.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetFieldSpecification(const FieldSpecification::Id fieldId, FieldSpecification &fieldSpecification) const;

	/**
	 *	@brief	Get the list of field identifiers.
	 *
	 *	@param	fieldIdList set equal to the field identifiers used for this field list object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetFieldIdList(FieldIdList &fieldIdList) const;

	/**
	 *	@brief	Get the id of the first field in the field list object.
	 *
	 *	@param	fieldId set equal to the first field id in the field list object
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetFirstFieldId(FieldSpecification::Id &fieldId) const;

	/**
	 *	@brief	Get the id of the next field in the field list object.
	 *
	 *	@param	fieldId set equal to the next field id in the field list object
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetNextFieldId(FieldSpecification::Id &fieldId) const;

	/**
	 *	@brief	Get the matching fields in two records by name.
	 *
	 *	@param	lhsFieldIdList the left hand field id list.
	 *	@param	rhsFieldListObject the right hand field list object to compare against.
	 *	@param	rhsFieldIdlist the right hand field id list.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API StatusCode GetMatchingFieldsByName(FieldIdList &lhsFieldIdList, const FieldListObject &rhsFieldListObject, FieldIdList &rhsFieldIdList);

	/**
	 *	@brief	Get the matching fields in two records by id.
	 *
	 *	@param	fieldIdList the list of field ids that match.
	 *	@param	rhsFieldListObject the right hand field list object to compare against.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API StatusCode GetMatchingFieldsById(FieldIdList &fieldIdList, const FieldListObject &rhsFieldListObject);

	/**
	 *	@brief	Get the field type for the field id specified.
	 *
	 *	@param	fieldId the id of the field whose specification is to be retrieved.
	 *	@param	type set equal to the type of the field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetType(const FieldSpecification::Id fieldId, FieldSpecification::Type &type) const;

	/**
	 *	@brief	Get a pointer to the table.
	 *
	 *	@param	pTable set equal to the table pointer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetTablePointer(ITable *&pTable) const;

	/**
	 *	@brief	Get a pointer to the beginning of the field list object data buffer.
	 *
	 *	@return	a pointer to the beginning of the field list object data buffer.
	 */
	byte_t *Get();

	/**
	 *	@brief	Get a pointer to the beginning of the field list object data buffer.
	 *
	 *	@return	a pointer to the beginning of the field list object data buffer.
	 */
	const byte_t *Get() const;

	/**
	 *	@brief	Set the record id in this field list object.
	 *
	 *	@param	id the identifier to store with the field list object.
	 */
	void SetId(const DataId id) const;

	/**
	 *	@brief	Get the current identifier stored in this field list object.
	 *
	 *	@return	the identifier of the field list object.
	 */
	DataId GetId() const;

	/**
	 *	@brief	Convert the field list object to a string.
	 *
	 *	@return	the field list object as a string.
	 */
	std::string ToString() const;

	/**
	 *	@brief	Convert the field list object to a log string.
	 *
	 *	@return	the field list object as a log string.
	 */
	std::string ToLogString() const;

	/**
	 *	@brief	Get an iterator to the first field in the list.
	 *
	 *	@return	an iterator object.
	 */
	ConstIterator Begin() const;

	/**
	 *	@brief	Get an iterator that points to beyond the last field in the list.
	 *
	 *	@return	an iterator object.
	 */
	ConstIterator End() const;

	/**
	 *	@brief	Is the field list object initialized.
	 *
	 *	@return	true if the field list object has been initialized, false otherwise.
	 */
	bool IsInitialized() const;

	/**
	 *	@brief	Check whether a field id is valid for this field list object.
	 *
	 *	@return	true if the field id can be found, false otherwise.
	 */
	bool IsFound(const FieldSpecification::Id fieldId) const;

protected:
	/**
	 *	@brief	Get the field list associated with the object.
	 *
	 *	@return	a reference to the field list of the object.
	 */
	const FieldList &GetFieldList() const;

	/**
	 *	@brief	Allocate the buffer for storing the field list object data.
	 *
	 *	@param	requiredBufferSize the required size of the buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode AllocateBuffer(const size_t requiredBufferSize);

	static const size_t		DEFAULT_BUFFER_SIZE = 1024;

	bool					m_isInitialized;						///< Flag indicating whether the field list object is initialized.
	ITable					*m_pTable;								///< Table the record relates to.
	ITable::Identity		m_tableIdentity;						///< Identity of the table. Cached for efficiency.
	const FieldList			*m_pFieldList;							///< Pointer to the field list.
	byte_t					m_defaultBuffer[DEFAULT_BUFFER_SIZE];	///< Default buffer used to store the record data if record length is less than the buffer size.
	byte_t					*m_pData;								///< Pointer to record data. Either points to the data buffer or to dynamically allocated memory.
	size_t					m_bufferSize;							///< Current size of the record buffer.
	mutable DataId			m_id;									///< Identifier of the record in the data store.

	friend class FieldList::ConstIterator;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListObject::FieldListObject() :
	m_isInitialized(false),
	m_pTable(0),
	m_tableIdentity(0),
	m_pFieldList(0),
	m_pData(m_defaultBuffer),
	m_bufferSize(sizeof(m_defaultBuffer)),
	m_id(UNDEFINED_ID)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListObject::~FieldListObject()
{
	if (m_defaultBuffer != m_pData)
		delete [] m_pData;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FieldListObject::Initialize()
{
	if (m_defaultBuffer != m_pData)
		delete [] m_pData;

	m_isInitialized = false;
	m_pTable = m_tableIdentity = 0;
	m_pFieldList = 0;
	m_pData = m_defaultBuffer;
	m_bufferSize = sizeof(m_defaultBuffer);
	m_id = UNDEFINED_ID;
	::memset(m_pData, 0, m_bufferSize);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::Initialize(ITable *pTable, const FieldList *pFieldList, const size_t requiredBufferSize)
{
	if (m_bufferSize < requiredBufferSize)
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AllocateBuffer(requiredBufferSize));

	m_isInitialized = true;
	m_pTable = pTable;
	m_tableIdentity = pTable->GetIdentity();
	m_pFieldList = pFieldList;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::AllocateBuffer(const size_t requiredBufferSize)
{
	byte_t *pData = new (std::nothrow) byte_t[requiredBufferSize];
	if (0 == pData)
		return STATUS_CODE_HEAP_EMPTY;

	if (m_defaultBuffer != m_pData)
		delete [] m_pData;

	m_pData = pData;
	m_bufferSize = requiredBufferSize;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::SetField(const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength)
{
	return (IsInitialized()) ? GetFieldList().SetField(Get(), fieldId, pField, fieldLength) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::SetField(const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength, bool &isDifferent)
{
	return (IsInitialized()) ? GetFieldList().SetField(Get(), fieldId, pField, fieldLength, isDifferent) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode FieldListObject::SetField(const FieldSpecification::Id fieldId, const T &value)
{
	return (IsInitialized()) ? GetFieldList().SetField(Get(), fieldId, value) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode FieldListObject::SetField(const FieldSpecification::Id fieldId, const T &value, bool &isDifferent)
{
	return (IsInitialized()) ? GetFieldList().SetField(Get(), fieldId, value, isDifferent) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::GetField(const FieldSpecification::Id fieldId, void *pField, const size_t fieldSize, size_t &fieldLength) const
{
	return (IsInitialized()) ? GetFieldList().GetField(Get(), fieldId, pField, fieldSize, fieldLength) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::GetField(const FieldSpecification::Id fieldId, const void *&pField, size_t &fieldLength) const
{
	return (IsInitialized()) ? GetFieldList().GetField(Get(), fieldId, pField, fieldLength) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::GetField(const FieldSpecification::Id fieldId, void *&pField, size_t &fieldLength)
{
	return (IsInitialized()) ? GetFieldList().GetField(Get(), fieldId, pField, fieldLength) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode FieldListObject::GetField(const FieldSpecification::Id fieldId, T &value) const
{
	return (IsInitialized()) ? GetFieldList().GetField(Get(), fieldId, value) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CopyFields(const FieldListObject &sourceFieldListObject, const bool copyAllFields)
{
	if ((!IsInitialized()) || (!sourceFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CopyFields(Get(), sourceFieldListObject.GetFieldList(), sourceFieldListObject.Get(), copyAllFields);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CopyField(const FieldSpecification::Id fieldId, const FieldListObject &sourceFieldListObject)
{
	if ((!IsInitialized()) || (!sourceFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CopyField(fieldId, Get(), sourceFieldListObject.GetFieldList(), sourceFieldListObject.Get());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CopyField(const FieldSpecification::Id fieldId, const FieldListObject &sourceFieldListObject, bool &isDifferent)
{
	if ((!IsInitialized()) || (!sourceFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CopyField(fieldId, Get(), sourceFieldListObject.GetFieldList(), sourceFieldListObject.Get(), isDifferent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CopyField(const FieldSpecification::Id targetFieldId, const FieldListObject &sourceFieldListObject, const FieldSpecification::Id sourceFieldId)
{
	if ((!IsInitialized()) || (!sourceFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CopyField(targetFieldId, Get(), sourceFieldListObject.GetFieldList(), sourceFieldId, sourceFieldListObject.Get());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CopyField(const FieldSpecification::Id targetFieldId, const FieldListObject &sourceFieldListObject, const FieldSpecification::Id sourceFieldId, bool &isDifferent)
{
	if ((!IsInitialized()) || (!sourceFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CopyField(targetFieldId, Get(), sourceFieldListObject.GetFieldList(), sourceFieldId, sourceFieldListObject.Get(), isDifferent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CompareField(const FieldSpecification::Id fieldId, const IFieldType &iFieldType, int &result) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CompareField(fieldId, Get(), iFieldType, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CompareField(const FieldSpecification::Id fieldId, const std::string &value, int &result) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CompareField(fieldId, Get(), value, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CompareField(const FieldSpecification::Id fieldId, const FieldListObject &rhsFieldListObject, int &result) const
{
	if ((!IsInitialized()) || (!rhsFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CompareField(fieldId, Get(), rhsFieldListObject.GetFieldList(), fieldId, rhsFieldListObject.Get(), result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::CompareField(const FieldSpecification::Id lhsFieldId, const FieldListObject &rhsFieldListObject, const FieldSpecification::Id rhsFieldId, int &result) const
{
	if ((!IsInitialized()) || (!rhsFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().CompareField(lhsFieldId, Get(), rhsFieldListObject.GetFieldList(), rhsFieldId, rhsFieldListObject.Get(), result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::IsFieldDifferent(const FieldSpecification::Id fieldId, const void *pField, const size_t fieldLength, bool &result) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().IsFieldDifferent(fieldId, Get(), pField, fieldLength, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::IsFieldDifferent(const FieldSpecification::Id fieldId, const IFieldType &iFieldType, bool &result) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().IsFieldDifferent(fieldId, Get(), iFieldType, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::IsFieldDifferent(const FieldSpecification::Id fieldId, const std::string &value, bool &result) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().IsFieldDifferent(fieldId, Get(), value, result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::IsFieldDifferent(const FieldSpecification::Id fieldId, const FieldListObject &rhsFieldListObject, bool &result) const
{
	if ((!IsInitialized()) || (!rhsFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().IsFieldDifferent(fieldId, Get(), rhsFieldListObject.GetFieldList(), rhsFieldListObject.Get(), result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::IsFieldDifferent(const FieldSpecification::Id lhsFieldId, const FieldListObject &rhsFieldListObject, const FieldSpecification::Id rhsFieldId, bool &result) const
{
	if ((!IsInitialized()) || (!rhsFieldListObject.IsInitialized()))
		return STATUS_CODE_NOT_INITIALIZED;

	return GetFieldList().IsFieldDifferent(lhsFieldId, Get(), rhsFieldListObject.GetFieldList(), rhsFieldId, rhsFieldListObject.Get(), result);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::DefineField(const FieldSpecification::Id fieldId, const size_t fieldLength)
{
	return (IsInitialized()) ? GetFieldList().DefineField(Get(), fieldId, fieldLength) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::UndefineField(const FieldSpecification::Id fieldId)
{
	return (IsInitialized()) ? GetFieldList().UndefineField(Get(), fieldId) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::UndefineField(const FieldSpecification::Id fieldId, bool &isDifferent)
{
	return (IsInitialized()) ? GetFieldList().UndefineField(Get(), fieldId, isDifferent) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::IsFieldDefined(const FieldSpecification::Id fieldId, bool &result) const
{
	return (IsInitialized()) ? GetFieldList().IsFieldDefined(Get(), fieldId, result) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline  StatusCode FieldListObject::GetFieldSpecificationList(FieldSpecificationList &fieldSpecificationList) const
{
	if (IsInitialized())
	{
		fieldSpecificationList = GetFieldList().GetFieldSpecificationList();
		return STATUS_CODE_SUCCESS;
	}
	else
		return STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline  StatusCode FieldListObject::GetFieldSpecification(const FieldSpecification::Id fieldId, FieldSpecification &fieldSpecification) const
{
	return (IsInitialized()) ? GetFieldList().GetFieldSpecification(fieldId, fieldSpecification) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline  StatusCode FieldListObject::GetFieldIdList(FieldIdList &fieldIdList) const
{
	if (IsInitialized())
	{
		fieldIdList = GetFieldList().GetFieldIdList();
		return STATUS_CODE_SUCCESS;
	}
	else
		return STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::GetFirstFieldId(FieldSpecification::Id &fieldId) const
{
	if (IsInitialized())
	{
		GetFieldList().GetFirstFieldId(fieldId);
		return STATUS_CODE_SUCCESS;
	}
	else
		return STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::GetNextFieldId(FieldSpecification::Id &fieldId) const
{
	return (IsInitialized()) ? GetFieldList().GetNextFieldId(fieldId) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::GetType(const FieldSpecification::Id fieldId, FieldSpecification::Type &type) const
{
	return (IsInitialized()) ? GetFieldList().GetType(fieldId, type) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListObject::GetTablePointer(ITable *&pTable) const
{
	if (IsInitialized())
	{
		pTable = m_pTable;
		return STATUS_CODE_SUCCESS;
	}
	else
		return STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline byte_t *FieldListObject::Get()
{
	return m_pData;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const byte_t *FieldListObject::Get() const
{
	return m_pData;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FieldListObject::SetId(const DataId id) const
{
	m_id = id;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DataId FieldListObject::GetId() const
{
	return m_id;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string FieldListObject::ToString() const
{
	return (IsInitialized()) ? GetFieldList().ToString(Get()) : std::string("Not initialized\n");
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string FieldListObject::ToLogString() const
{
	return (IsInitialized()) ? GetFieldList().ToLogString(Get()) : std::string("Not initialized\n");
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListObject::ConstIterator FieldListObject::Begin() const
{
	return (IsInitialized()) ? GetFieldList().Begin() : ConstIterator();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListObject::ConstIterator FieldListObject::End() const
{
	return (IsInitialized()) ? GetFieldList().End() : ConstIterator();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListObject::IsInitialized() const
{
	return (m_isInitialized && (m_tableIdentity == m_pTable->GetIdentity()));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListObject::IsFound(const FieldSpecification::Id fieldId) const
{
	return IsInitialized() ? GetFieldList().IsFound(fieldId) : false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const FieldList &FieldListObject::GetFieldList() const
{
	return *m_pFieldList;
}

// ---------------------------------------------------------------------------------------------------------------------------------

}	// namespace Database

}	// namespace Activ

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Global operator<< to dump a record to an ostream.
 *
 *	@param	os the ostream to write the index specification to.
 *	@param	rhs the field list object to write.
 *
 *	@return	the ostream \a os.
 */
ACTIV_DATABASE_API inline std::ostream &operator<<(std::ostream &os, const Activ::Database::FieldListObject &rhs)
{
	os << rhs.ToString();
	return os;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#endif	// !defined(ACTIV_DATABASE_FIELD_LIST_OBJECT_H)
