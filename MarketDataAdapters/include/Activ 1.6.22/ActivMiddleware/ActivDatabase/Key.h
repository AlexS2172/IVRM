/**
 *  @file	Key.h
 *
 *	@brief	Header file for the key class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_KEY_H)
#define ACTIV_DATABASE_KEY_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"
#include "ActivMiddleware/ActivDatabase/ITable.h"
#include "ActivMiddleware/ActivDatabase/IIndex.h"
#include "ActivMiddleware/ActivDatabase/FieldListObject.h"

#include <string>
#include <memory>

namespace Activ
{

class ScatterBufferHelper;
class IFieldType;

namespace Database
{

class Record;
	
/**
 *	@brief	Key class.
 */
class Key : public FieldListObject
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	Key();

	/**
	 *	@brief	Constructor that initializes the key for the specified table and index.
	 *
	 *	@param	pTable table key will relate to.
	 *	@param	indexNo index number the key will relate to.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Key(ITable *pTable, const IndexSpecification::Number indexNo);

	/**
	 *	@brief	Constructor that initializes the key with data from a record for the specified index.
	 *
	 *	@param	indexNo index number the key will relate to.
	 *	@param	record the record to copy information from.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Key(const IndexSpecification::Number indexNo, const Record &record);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Key(const Key &rhs);

	/**
	 *	@brief	Destructor.
	 */
	~Key();

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Key &operator=(const Key &rhs);

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator==(const Key &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator!=(const Key &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator<(const Key &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator<=(const Key &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>(const Key &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	bool operator>=(const Key &rhs) const;

	/**
	 *	@brief	Initialize to an undefined table and index.
	 */
	void Initialize();

	/**
	 *	@brief	Initialize key object for specified table and index number.
	 *
	 *	@param	pTable table key will relate to.
	 *	@param	indexNo index number the key will relate to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_DATABASE_API StatusCode Initialize(ITable *pTable, const IndexSpecification::Number indexNo);

	/**
	 *	@brief	Initialize with information contained in another key.
	 *
	 *	@param	rhs key to copy information from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_DATABASE_API StatusCode Initialize(const Key &rhs);

	/**
	 *	@brief	Initialize with information contained in a record.
	 *
	 *	@param	indexNo index number the key will relate to.
	 *	@param	record the record to copy information from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_DATABASE_API StatusCode Initialize(const IndexSpecification::Number indexNo, const Record &record);

	/**
	 *	@brief	Put the key information into a message.
	 *
	 *	@param	messageBuilder the message builder in which key data will be placed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_DATABASE_API virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the key information from a message.
	 *
	 *	@param	pTable table key will relate to.
	 *	@param	messageValidater the message validater from which key data will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API virtual StatusCode Deserialize(ITable *pTable, MessageValidater &messageValidater);

	/**
	 *	@brief	Reset the contents of the key data buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API virtual StatusCode Reset();

	/**
	 *	@brief	Build a key using the supplied record data.
	 *
	 *	@param	record record to use to build the key.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API StatusCode BuildKey(const Record &record);

	/**
	 *	@brief	Set the index fields in the key, starting with the first field in the index.
	 *
	 *	@param	scatterBufferHelper a scatter buffer helper containing the fields to copy to the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode SetIndexFields(const ScatterBufferHelper &scatterBufferHelper);

	/**
	 *	@brief	Copy all fields from a supplied key.
	 *
	 *	The supplied key does not have to be for the same table.
     *
	 *	@param	sourceKey the key fields are copied from.
	 *	@param	copyAllFields whether all fields must be found in the source key.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API virtual StatusCode CopyFields(const Key &sourceKey, const bool copyAllFields = false);

	/**
	 *	@brief	Copy all fields from a supplied record.
	 *
	 *	The supplied record does not have to be for the same table.
     *
	 *	@param	sourceRecord the record fields are copied from.
	 *	@param	copyAllFields whether all fields must be found in the source record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API virtual StatusCode CopyFields(const Record &sourceRecord, const bool copyAllFields = false);

	/**
	 *	@brief	Get a pointer to the key's index.
	 *
	 *	@param	pIndex set equal to the index pointer of the key.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetIndexPointer(IIndex *&pIndex) const;

	/**
	 *	@brief	Get the index specification.
	 *
	 	@param	indexSpecification set to the index specification associated with the key.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetIndexSpecification(IndexSpecification &indexSpecification) const;

	/**
	 *	@brief	Set the contents of the key data buffer equal to the provided data buffer.
	 *
	 *	@param	pRhsData pointer to data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Set(const void *pRhsData);

	/**
	 *	@brief	Get data length.
	 *
	 *	@return	the length of the key data.
	 */
	size_t GetDataLength() const;

	/**
	 *	@brief	Get the index number of the key.
	 *
	 *	@param	indexNo set to the index number that the key is initialized for.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	StatusCode GetIndexNumber(IndexSpecification::Number &indexNo) const;

	/**
	 *	@brief	Check whether the key is valid with respect to the specified table.
	 *
	 *	@param	pTable pointer to table.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 */
	StatusCode Validate(const ITable *pTable) const;

	/**
	 *	@brief	Check whether the key is valid with respect to the specified table.
	 *
	 *	@param	pTable pointer to table.
	 *	@param	nIndexes number of indexes in the specified table.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 */
	StatusCode Validate(const ITable *pTable, const size_t nIndexes) const;

private:
	IIndex						*m_pIndex;				///< Index the key relates to.
	IndexSpecification::Number	m_indexNo;				///< Number of index.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Key::Key() :
	m_pIndex(0),
	m_indexNo(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Key::~Key()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Key::operator==(const Key &rhs) const
{
	bool lhsIsInitialized = IsInitialized(), rhsIsInitialized = rhs.IsInitialized();

	if (lhsIsInitialized != rhsIsInitialized)
		return false;

	if (!lhsIsInitialized && !rhsIsInitialized)
		return true;

	if ((m_pTable != rhs.m_pTable) || (m_pIndex != rhs.m_pIndex))
		return false;

	return (0 == m_pIndex->Compare(*this, rhs));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Key::operator!=(const Key &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Key::operator<(const Key &rhs) const
{
	bool lhsIsInitialized = IsInitialized(), rhsIsInitialized = rhs.IsInitialized();

	if (lhsIsInitialized != rhsIsInitialized)
		return !lhsIsInitialized;

	if (!lhsIsInitialized && !rhsIsInitialized)
		return false;

	if (m_pTable != rhs.m_pTable)
		return (m_pTable->GetName() < rhs.m_pTable->GetName());

	if (m_pIndex != rhs.m_pIndex)
		return (m_pIndex->GetName() < rhs.m_pIndex->GetName());

	return (m_pIndex->Compare(*this, rhs) < 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Key::operator<=(const Key &rhs) const
{
	bool lhsIsInitialized = IsInitialized(), rhsIsInitialized = rhs.IsInitialized();

	if (lhsIsInitialized != rhsIsInitialized)
		return !lhsIsInitialized;

	if (!lhsIsInitialized && !rhsIsInitialized)
		return true;

	if (m_pTable != rhs.m_pTable)
		return (m_pTable->GetName() < rhs.m_pTable->GetName());

	if (m_pIndex != rhs.m_pIndex)
		return (m_pIndex->GetName() < rhs.m_pIndex->GetName());

	return (m_pIndex->Compare(*this, rhs) <= 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Key::operator>(const Key &rhs) const
{
	return !(*this <= rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Key::operator>=(const Key &rhs) const
{
	bool lhsIsInitialized = IsInitialized(), rhsIsInitialized = rhs.IsInitialized();

	if (lhsIsInitialized != rhsIsInitialized)
		return lhsIsInitialized;

	if (!lhsIsInitialized && !rhsIsInitialized)
		return true;

	if (m_pTable != rhs.m_pTable)
		return (m_pTable->GetName() > rhs.m_pTable->GetName());

	if (m_pIndex != rhs.m_pIndex)
		return (m_pIndex->GetName() > rhs.m_pIndex->GetName());

	return (m_pIndex->Compare(*this, rhs) >= 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Key::Initialize()
{
	FieldListObject::Initialize();
	m_pIndex = 0;
	m_indexNo = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::Reset()
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	m_pIndex->ResetKey(*this);
	m_id = UNDEFINED_ID;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::SetIndexFields(const ScatterBufferHelper &scatterBufferHelper)
{
	return (IsInitialized()) ? m_pIndex->SetFields(*this, scatterBufferHelper) : STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::GetIndexPointer(IIndex *&pIndex) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	pIndex = m_pIndex;
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::GetIndexSpecification(IndexSpecification &indexSpecification) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	indexSpecification = m_pIndex->GetSpecification();
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::Set(const void *pRhsData)
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	if (0 == pRhsData)
		return STATUS_CODE_INVALID_PARAMETER;

	MemoryCopy(m_pData, pRhsData, GetDataLength());
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Key::GetDataLength() const
{
	return (IsInitialized() ? m_pIndex->GetKeyLength() : 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::GetIndexNumber(IndexSpecification::Number &indexNo) const
{
	if (IsInitialized())
	{
		indexNo = m_indexNo;
		return STATUS_CODE_SUCCESS;
	}
	else
		return STATUS_CODE_NOT_INITIALIZED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::Validate(const ITable *pTable) const
{
	return ((IsInitialized() && (pTable->GetIdentity() == m_tableIdentity) && (m_indexNo < pTable->GetNumberOfIndexes())) ? STATUS_CODE_SUCCESS : STATUS_CODE_INVALID_KEY);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Key::Validate(const ITable *pTable, const size_t nIndexes) const
{
	return ((IsInitialized() && (pTable->GetIdentity() == m_tableIdentity) && (m_indexNo < nIndexes)) ? STATUS_CODE_SUCCESS : STATUS_CODE_INVALID_KEY);
}

// ---------------------------------------------------------------------------------------------------------------------------------

}	// namespace Database

}	// namespace Activ

#endif	// !defined(ACTIV_DATABASE_KEY_H)
