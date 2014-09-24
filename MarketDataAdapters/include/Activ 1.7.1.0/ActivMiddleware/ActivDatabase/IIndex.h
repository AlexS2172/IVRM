/**
 *  @file	IIndex.h
 *
 *	@brief	Header file for the index interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_IINDEX_H)
#define ACTIV_DATABASE_IINDEX_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"

#include <vector>
#include <memory>

namespace Activ
{

class ScatterBufferHelper;
class IFieldType;

namespace Database
{

class Record;
class Key;
class FieldList;
	
/**
 *	@brief	Abstract base index class.
 */
class IIndex
{
public:
	/**
	 *	@brief	Default destructor.
	 */
	virtual ~IIndex() = 0;

	/**
	 *	@brief	Reset key.
	 *
	 *	@param	key key to reset.
	 */
	virtual void ResetKey(Key &key) const = 0;

	/**
	 *	@brief	Build key using record pointer and record identifier.
	 *
	 *	@param	key built key is stored in this parameter.
	 *	@param	pRecordData a pointer to the record data from which the key fields will be copied.
	 *	@param	recordId record indentifier of matching key in the index.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	virtual StatusCode BuildKey(Key &key, const byte_t *pRecordData, const DataId recordId) const = 0;

	/**
	 *	@brief	Build a key using another key.
	 *
	 *	@param	targetKey built key is stored in this parameter.
	 *	@param	sourceKey key from which fields will be copied.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 */
	virtual StatusCode BuildKey(Key &targetKey, const Key &sourceKey) const = 0;

	/**
	 *	@brief	Set the index fields in the record, starting with the first field in the index.
	 *
	 *	@param	record the record in which the index fields will be stored.
	 *	@param	scatterBufferHelper a scatter buffer helper containing the fields to copy to the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	virtual StatusCode SetFields(Record &record, const ScatterBufferHelper &scatterBufferHelper) const = 0;

	/**
	 *	@brief	Set the index fields in the key, starting with the first field in the index.
	 *
	 *	@param	key the key in which the index fields will be stored.
	 *	@param	scatterBufferHelper a scatter buffer helper containing the fields to copy to the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	virtual StatusCode SetFields(Key &key, const ScatterBufferHelper &scatterBufferHelper) const = 0;

	/**
	 *	@brief	Compare keys.
	 *
	 *	@param	lhs left hand key.
	 *	@param	rhs right hand key.
	 *
	 *	@return	negative value if left hand data is less than right hand data, positive if left is greater than right and 0 otherwise.
	 */
	virtual int Compare(const Key &lhs, const Key &rhs) const = 0;

	/**
	 *	@brief	Get number of the index.
	 *
	 *	@return number of the index.
	 */
	virtual IndexSpecification::Number GetNumber() const = 0;

	/**
	 *	@brief	Get name.
	 *
	 *	@return name of index.
	 */
	virtual std::string GetName() const = 0;

	/**
	 *	@brief	Get key length.
	 *
	 *	@return length of key.
	 */
	virtual size_t GetKeyLength() const = 0;

	/**
	 *	@brief	Get a reference to the field list of the index.
	 *
	 *	@return	the field list of the index.
	 */
	virtual const FieldList &GetFieldList() const = 0;

	/**
	 *	@brief	Get the list of all field specifications.
	 *
	 *	@return	the field specification list.
	 */
	virtual FieldSpecificationList GetFieldSpecificationList() const = 0;

	/**
	 *	@brief	Get the list of all field identifiers.
	 *
	 *	@return	the field identifier list.
	 */
	virtual FieldIdList GetFieldIdList() const = 0;

	/**
	 *	@brief	Get the specification of the index.
	 *
	 *	@return	the specification of the index.
	 */
	virtual IndexSpecification GetSpecification() const = 0;

	/**
	 *	@brief	Get the number of records.
	 *
	 *	@return	the number of records in the index.
	 *
	 *	@throw	StatusCodeException
	 */
	virtual size_t GetCurrentRecords() const = 0;

	/**
	 *	@brief	Is this the primary unique index.
	 *
	 *	@return	true if this is the primary unqiue index, false otherwise.
	 */
	virtual bool IsPrimaryUnique() const = 0;

	/**
	 *	@brief	Is this a unique index.
	 *
	 *	@return	true if added keys have to be unique, false otherwise.
	 */
	virtual bool IsUnique() const = 0;

	/**
	 *	@brief	Is this a duplicate index.
	 *
	 *	@return	true if added keys have can be duplicated in the index, false otherwise.
	 */
	virtual bool IsDuplicate() const = 0;

	/**
	 *	@brief	Is this a tree index.
	 *
	 *	@return	true if the index is a tree and therefore supports in order walks, false otherwise.
	 */
	virtual bool IsTree() const = 0;

	/**
	 *	@brief	Check the integrity of the index.
	 *
	 *	@throw	DatabaseException.
	 */
	virtual void CheckIntegrity() const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IIndex::~IIndex()
{
}

} // namespace Database

} // namespace Activ

#endif // !defined (ACTIV_DATABASE_IINDEX_H)
