/**
 *  @file	Record.h
 *
 *	@brief	Header file for the record class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_RECORD_H)
#define ACTIV_DATABASE_RECORD_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"
#include "ActivMiddleware/ActivDatabase/ITable.h"
#include "ActivMiddleware/ActivDatabase/FieldListObject.h"

#include <iostream>
#include <string>
#include <memory>

namespace Activ
{

class ScatterBufferHelper;
class IFieldType;

namespace Database
{

class Key;
	
/**
 *	@brief	Database record class.
 */
class Record : public FieldListObject
{
public:
	/**
	 *	@brief	Constructor.
	 */
	Record();

	/**
	 *	@brief	Constructor that initializes record for the specified table.
	 *
	 *	@param	pTable table the record will be stored in.
	 *	@param	pSourceData pointer to data to store in the record.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Record(ITable *pTable);

	/**
	 *	@brief	Constructor that initializes record with provided record data.
	 *
	 *	@param	pTable table the record will be stored in.
	 *	@param	pSourceData pointer to data to store in the record.
	 *	@param	id the identifier to store with the record.
	 *
	 *	@throw	DatabaseException
	 */
	Record(ITable *pTable, const void *pSourceData, const DataId id);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Record(const Record &rhs);

	/**
	 *	@brief	Constructor that initializes the record with data from a key.
	 *
	 *	@param	key the key to copy information from.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Record(const Key &key);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~Record();

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Record &operator=(const Record &rhs);

	/**
	 *	@brief	Equality operator.
	 *
	 *	@return	whether the two objects are equal.
	 */
	ACTIV_DATABASE_API bool operator==(const Record &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@return	whether the two objects are not equal.
	 */
	ACTIV_DATABASE_API bool operator!=(const Record &rhs) const;

	using FieldListObject::Initialize;

	/**
	 *	@brief	Initialize for a specified table.
	 *
	 *	@param	pTable table the record will be stored in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Initialize(ITable *pTable);

	/**
	 *	@brief	Initialize for a specified table.
	 *
	 *	@param	pTable table the record will be stored in.
	 *	@param	pSourceData pointer to data to store in the record.
	 *	@param	id the identifier to store with the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Initialize(ITable *pTable, const void *pSourceData, const DataId id);

	/**
	 *	@brief	Initialize with information contained in another record.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_DATABASE_API StatusCode Initialize(const Record &rhs);

	/**
	 *	@brief	Initialize with information contained in a key.
	 *
	 *	@param	key the key to copy information from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_DATABASE_API StatusCode Initialize(const Key &key);

	/**
	 *	@brief	Serialize the record information into a message.
	 *
	 *	@param	messageBuilder the message builder into which the record data will be placed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_DATABASE_API virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the record information from a message.
	 *
	 *	@param	pTable table the record will be stored in.
	 *	@param	messageValidater message validater containing the record data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API virtual StatusCode Deserialize(ITable *pTable, MessageValidater &messageValidater);

	/**
	 *	@brief	Reset the contents of the record data buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API virtual StatusCode Reset();

	/**
	 *	@brief	Set the index fields in the record, starting with the first field in the index.
	 *
	 *	@param	indexNo the number of the index which the fields will be set for.
	 *	@param	scatterBufferHelper a scatter buffer helper containing the fields to copy to the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetIndexFields(const IndexSpecification::Number indexNo, const ScatterBufferHelper &scatterBufferHelper);

	/**
	 *	@brief	Get a copy of the key created for the specified index.
	 *
	 *	@param	indexNo the index number to create the key on.
	 *
	 *	@return	a key created from the record for the specified index.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Key GetKey(const IndexSpecification::Number indexNo) const;

	/**
	 *	@brief	Get a copy of the primary key.
	 *
	 *	@return	a key created from the record for the primary index.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Key GetPrimaryKey() const;

	/**
	 *	@brief	Set the contents of the record data buffer equal to the provided data buffer.
	 *
	 *	@param	pSourceData pointer to data to store in the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const void *pSourceData);

	/**
	 *	@brief	Get the length of the record.
	 *
	 *	@return	the length of the record.
	 */
	size_t GetDataLength() const;

	/**
	 *	@brief	Check whether the key is valid with respect to the specified table.
	 *
	 *	This function does not check whether the supplied table pointer is valid.
	 *
	 *	@param	pTable pointer to table.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 */
	StatusCode Validate(const ITable *pTable) const;

private:
	static const size_t MAX_UNSERIALIZED_LENGTH = 2048;		///< The maximum length of the record before fields are serialized individually.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Record::Record()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Record::~Record()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Record::Initialize(ITable *pTable)
{
	if ((0 == pTable) || (0 == pTable->GetIdentity()))
		return STATUS_CODE_INVALID_PARAMETER;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, FieldListObject::Initialize(pTable, &pTable->GetFieldList(), pTable->GetRecordLength()));

	return m_pTable->ResetRecord(*this);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Record::Reset()
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	GetFieldList().ResetData(m_pData);
	m_id = UNDEFINED_ID;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Record::SetIndexFields(const IndexSpecification::Number indexNo, const ScatterBufferHelper &scatterBufferHelper)
{
	return (IsInitialized()) ? m_pTable->SetIndexFields(*this, indexNo, scatterBufferHelper) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Record::Set(const void *pSourceData)
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	if (0 == pSourceData)
		return STATUS_CODE_INVALID_PARAMETER;

	::memcpy(m_pData, pSourceData, GetDataLength());
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Record::GetDataLength() const
{
	return (IsInitialized()) ? m_pTable->GetRecordLength() : 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Record::Validate(const ITable *pTable) const
{
	return (pTable->GetIdentity() == m_tableIdentity) ? STATUS_CODE_SUCCESS : STATUS_CODE_INVALID_RECORD;
}

// ---------------------------------------------------------------------------------------------------------------------------------

}	// namespace Database

}	// namespace Activ

#endif	// !defined(ACTIV_DATABASE_RECORD_H)
