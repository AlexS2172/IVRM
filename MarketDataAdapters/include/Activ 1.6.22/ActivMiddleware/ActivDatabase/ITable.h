/**
 *  @file	ITable.h
 *
 *	@brief	Header file for the table interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_ITABLE_H)
#define ACTIV_DATABASE_ITABLE_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"
#include "ActivMiddleware/ActivDatabase/DatabaseException.h"

#include <iostream>
#include <vector>
#include <memory>

namespace Activ
{

class ScatterBufferHelper;
class MessageBuilder;
class MessageValidater;
class IFieldType;

namespace Database
{

class IIndex;
class Record;
class Key;
class FieldList;

typedef std::vector<Record> RecordList;		///< Record list type definition.
typedef std::vector<Key> KeyList;			///< Key list type definition.

/**
 *	@brief	Abstract base table class.
 */
class ITable
{
public:
	typedef ITable *Identity;	///< Identity of a table. Allows different tables to behave as if they were the same.

	/**
	 *	@brief	Default destructor.
	 */
	ITable();

	/**
	 *	@brief	Default destructor.
	 */
	virtual ~ITable() = 0;

	/**
	 *	@brief	Create the table record file.
	 *
	 *	@throw	DatabaseException
	 */
	virtual void CreateRecordFile() = 0;

	/**
	 *	@brief	Open the table record file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode OpenRecordFile() = 0;

	/**
	 *	@brief	Close the table record file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	virtual StatusCode CloseRecordFile() = 0;

	/**
	 *	@brief	Convert a table's data according to the new specification.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	f knows
	 */
	virtual StatusCode Convert() = 0;

	/**
	 *	@brief	Save the table data to a given location.
	 *
	 *	@param	directory the directory to store the table data in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_OPEN
	 */
	virtual StatusCode Save(const std::string &directory) = 0;

	/**
	 *	@brief	Copy the files for this table.
	 *
	 *	@param	sourceDirectory the source directory.
	 *	@param	targetDirectory the target directory.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_OPEN
	 */
	virtual StatusCode CopyFiles(const std::string &sourceDirectory, const std::string &targetDirectory) const = 0;

	/**
	 *	@brief	Create all permanent indexes on the data.
	 *
	 *	@param	indexSpecificationList list of index specifications.
	 *	@param	nIndexSpecifications the number of index specifications.
	 *
	 *	@throw	DatabaseException
	 */
	void CreateIndexes(IndexSpecification indexSpecificationList[], const size_t nIndexSpecifications);

	/**
	 *	@brief	Create all permanent indexes on the data.
	 *
	 *	@param	indexSpecificationList list of index specifications.
	 *
	 *	@throw	DatabaseException
	 */
	virtual void CreateIndexes(IndexSpecificationList &indexSpecificationList) = 0;

	/**
	 *	@brief	Create a temporary index on the data stored in this table.
	 *
	 *	@param	indexSpecification the specification of the index.
	 *
	 *	@throw	ActivException
	 *	@throw	DatabaseException
	 *	@throw	OsException
	 */
	virtual void CreateTemporaryIndex(IndexSpecification &indexSpecification) = 0;

	/**
	 *	@brief	Clone a table.
	 *
	 *	The table record file will always be created when a table is cloned.
	 *
	 *	@param	directory name of directory in which to store persistent table files, or empty to create a temporary version of the table.
	 *	@param	createMutex flag indicating whether a mutex should be created for the table.
	 *	@param	copyRecords flag indicating whether any existing records should be copied to the new table.
	 *
	 *	@return	a pointer to the cloned table.
	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	DatabaseException
	 *	@throw	OsException
	 */
	virtual ITable *Clone(const std::string &directory = std::string(), const bool createMutex = false, const bool copyRecords = true) const = 0;

	/**
	 *	@brief	Clear the table of all records contained in it.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 */
	virtual StatusCode Clear() = 0;

	/**
	 *	@brief	Copy all records from one table to another.
	 *
	 *	@param	pSourceTable table to copy records from.
	 *	@param	clearTargetTable flag indicating whether the target table should be emptied before copying records.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NO_PRIMARY_KEY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode Copy(const ITable *pSourceTable, const bool clearTargetTable = true) = 0;

	/**
	 *	@brief	Serialize the specification into a message.
	 *
	 *	@param	messageBuilder the message builder which the specification will be put in.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode SerializeSpecification(MessageBuilder &messageBuilder) const = 0;

	/**
	 *	@brief	Serialize key and record information into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	key key data to store in the message.
	 *	@param	record record data to store in the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const Key &key, const Record &record) const = 0;

	/**
	 *	@brief	Serialize a list of keys into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	keyList the list of key information to be stored in the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const KeyList &keyList) const = 0;

	/**
	 *	@brief	Serialize a list of records into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	recordList a list of records returned from table lookups.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const RecordList &recordList) const = 0;

	/**
	 *	@brief	Serialize a key and a list of records into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	key key data to store in the message.
	 *	@param	recordList a list of records returned from table lookups.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const Key &key, const RecordList &recordList) const = 0;

	/**
	 *	@brief	Serialize search information for returning multiple records into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	key key to use for the search of the table.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const Key &key, const size_t maxResponseRecords) const = 0;

	/**
	 *	@brief	Serialize partial search information into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	key key to use for the search of the table.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const Key &key, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength) const = 0;

	/**
	 *	@brief	Serialize partial search information for returning multiple records into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	key key to use for the search of the table.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const Key &key, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength, const size_t maxResponseRecords) const = 0;

	/**
	 *	@brief	Serialize a list of status codes and records into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing a message.
	 *	@param	statusCodeList a list of status codes returned from table lookups.
	 *	@param	recordList a list of records returned from table lookups.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder, const std::vector<StatusCode> &statusCodeList, const RecordList &recordList) const = 0;

	/**
	 *	@brief	Deserialize key and record data from a message.
	 *
	 *	@param	messageValidater the message validater from which the key and record data is extracted.
	 *	@param	key key data that is extracted from the message.
	 *	@param	record record data that is extracted from the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, Key &key, Record &record) const = 0;

	/**
	 *	@brief	Deserialize a list of keys from a message.
	 *
	 *	@param	messageValidater the message validater from which key data is extracted.
	 *	@param	keyList the list of key information extracted from the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, KeyList &keyList) const = 0;

	/**
	 *	@brief	Deserialize a list of records from a message.
	 *
	 *	@param	messageValidater the message validater from which the key and record data is extracted.
	 *	@param	recordList a list of records returned from table lookups.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, RecordList &recordList) const = 0;

	/**
	 *	@brief	Deserialize a key and a list of records from a message.
	 *
	 *	@param	messageValidater the message validater from which the key and record data is extracted.
	 *	@param	key key data that is extracted from the message.
	 *	@param	recordList a list of records returned from table lookups.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, Key &key, RecordList &recordList) const = 0;

	/**
	 *	@brief	Deserialize key data required to return multiple records from a message.
	 *
	 *	@param	messageValidater the message validater from which key and partial search data is extracted.
	 *	@param	key key data that is extracted from the message.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, Key &key, size_t &maxResponseRecords) const = 0;

	/**
	 *	@brief	Deserialize record id and maximum response records used to return multiple records.
	 *
	 *	@param	messageValidater the message validater from which key and partial search data is extracted.
	 *	@param	recordId the id of the record to use in the search of the table.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, DataId &recordId, size_t &maxResponseRecords) const = 0;

	/**
	 *	@brief	Deserialize key and partial search data from a message.
	 *
	 *	@param	messageValidater the message validater from which key and partial search data is extracted.
	 *	@param	key key data that is extracted from the message.
	 *	@param	lastKeyFieldId last key field field identifier extracted from the message.
	 *	@param	lastKeyCompareLength comparison length of the last key field that is extracted from the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, Key &key, FieldSpecification::Id &lastKeyFieldId, size_t &lastKeyCompareLength) const = 0;

	/**
	 *	@brief	Deserialize key and partial search data required to return multiple records from a message.
	 *
	 *	@param	messageValidater the message validater from which key and partial search data is extracted.
	 *	@param	key key data that is extracted from the message.
	 *	@param	lastKeyFieldId last key field field identifier extracted from the message.
	 *	@param	lastKeyCompareLength comparison length of the last key field that is extracted from the message.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, Key &key, FieldSpecification::Id &lastKeyFieldId, size_t &lastKeyCompareLength, size_t &maxResponseRecords) const = 0;

	/**
	 *	@brief	Deserialize a list of status codes and records from a message.
	 *
	 *	@param	messageValidater the message validater from which the key and record data is extracted.
	 *	@param	statusCodeList a list of status codes returned from table lookups.
	 *	@param	recordList a list of records returned from table lookups.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater, std::vector<StatusCode> &statusCodeList, RecordList &recordList) const = 0;

	/**
	 *	@brief	Reset record.
	 *
	 *	@param	record record to reset.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 */
	virtual StatusCode ResetRecord(Record &record) const = 0;

	/**
	 *	@brief	Add record to table.
	 *
	 *	@param	record record to add to table.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NO_PRIMARY_KEY
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode Add(Record &record) = 0;

	/**
	 *	@brief	Add record to table.
	 *
	 *	@param	record record to add to table.
	 *	@param	originalRecord the original version of the record. Not initialized if no record is found.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NO_PRIMARY_KEY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode Add(Record &record, Record &originalRecord) = 0;

	/**
	 *	@brief	Delete record from table.
	 *
	 *	@param	key key of record to be deleted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode Delete(const Key &key) = 0;

	/**
	 *	@brief	Delete record with specified key from this table and retrieve the deleted record.
	 *
	 *	@param	key key of record to be deleted.
	 *	@param	record the contents of the record prior to deletion is stored in this parameter.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode Delete(const Key &key, Record &record) = 0;

	/**
	 *	@brief	Get record from specified index matching supplied key information.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetEqual(const Key &key, Record &record) const = 0;

	/**
	 *	@brief	Get multiple records from specified indexes matching supplied key information.
	 *
	 *	@param	keyList list of key information that will be searched for.
	 *	@param	statusCodeList the status code associated with each look up is stored in this parameter.
	 *	@param	recordList the record matching each supplied key information is stored in this parameter, or a blank record if no match is found.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetEqual(const KeyList &keyList, std::vector<StatusCode> &statusCodeList, RecordList &recordList) const = 0;

	/**
	 *	@brief	Get the first record in the table on the specified index.
	 *
	 *	@param	key set equal to the first key.
	 *	@param	record matching record information will be stored in this parameter.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetFirst(Key &key, Record &record) const = 0;

	/**
	 *	@brief	Get multiple records from the first.
	 *
	 *	@param	key set equal to the first key.
	 *	@param	recordList all located records are stored in this parameter.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetFirst(Key &key, RecordList &recordList, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Get the last record in the table for the specified index.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetLast(Key &key, Record &record) const = 0;

	/**
	 *	@brief	Get multiple records from the last.
	 *
	 *	@param	key set equal to the last key.
	 *	@param	recordList all located records are stored in this parameter.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetLast(Key &key, RecordList &recordList, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Get the next record in the table for the specified index.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetNext(Key &key, Record &record) const = 0;

	/**
	 *	@brief	Get multiple records from the next one to the specified key.
	 *
	 *	@param	key set equal to the first key.
	 *	@param	recordList all located records are stored in this parameter.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetNext(Key &key, RecordList &recordList, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Get the previous record in the table.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetPrevious(Key &key, Record &record) const = 0;

	/**
	 *	@brief	Get multiple records from the previous one to the specified key.
	 *
	 *	@param	key set equal to the first key.
	 *	@param	recordList all located records are stored in this parameter.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetPrevious(Key &key, RecordList &recordList, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Get the first record that matches the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetFirstPartial(Key &key, Record &record, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE) const = 0;

	/**
	 *	@brief	Get multiple records from the first that matches the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	recordList all records matching the supplied key information are stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetFirstPartial(Key &key, RecordList &recordList, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Get the last record that matches the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetLastPartial(Key &key, Record &record, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE) const = 0;

	/**
	 *	@brief	Get multiple records from the last that matches the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	recordList all records matching the supplied key information are stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetLastPartial(Key &key, RecordList &recordList, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Get the next record that matches the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *
	 *	@retval	ACTVI_STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetNextPartial(Key &key, Record &record, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE) const = 0;

	/**
	 *	@brief	Get multiple next records that match the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	recordList all records matching the supplied key information are stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetNextPartial(Key &key, RecordList &recordList, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Get the previous record that matches the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	record matching record information will be stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *
	 *	@retval	ACTVI_STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetPreviousPartial(Key &key, Record &record, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE) const = 0;

	/**
	 *	@brief	Get multiple previous records that match the partially specified key.
	 *
	 *	@param	key key information which will be searched for.
	 *	@param	recordList all records matching the supplied key information are stored in this parameter.
	 *	@param	lastKeyFieldId the field identifier of the last key field to be used for the partial search.
	 *	@param	lastKeyCompareLength the amount of the last key field that should be used in the partial key comparisons.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	virtual StatusCode GetPreviousPartial(Key &key, RecordList &recordList, const FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength = FIELD_COMPARE_LENGTH_WHOLE, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Step to the first record in the table.
	 *
	 *	@param	recordId set equal to the id of the first record retrieved from the table.
	 *	@param	record set equal to the first record information retrieved from the table.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_OPEN
	 */
	virtual StatusCode StepFirst(DataId &recordId, Record &record) const = 0;

	/**
	 *	@brief	Step to the first record in the table and get multiple records after it.
	 *
	 *	@param	recordId the id of the last record retrieved from the table.
	 *	@param	recordList all located records are stored in this parameter.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_OPEN
	 */
	virtual StatusCode StepFirst(DataId &recordId, RecordList &recordList, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Step to the next record in the table.
	 *
	 *	@param	recordId set equal to the id of the next record retrieved from the table.
	 *	@param	record matching record information will be stored in this parameter.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_OPEN
	 */
	virtual StatusCode StepNext(DataId &recordId, Record &record) const = 0;

	/**
	 *	@brief	Step to the next multiple records from the next one.
	 *
	 *	@param	recordId the id of the last record retrieved from the table.
	 *	@param	recordList all located records are stored in this parameter.
	 *	@param	maxResponseRecords the maximum number of records to include in the response. Default of zero implies all matching records will be returned.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_INVALID_RECORD
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_OPEN
	 */
	virtual StatusCode StepNext(DataId &recordId, RecordList &recordList, const size_t maxResponseRecords = 0) const = 0;

	/**
	 *	@brief	Set the index fields in the record, starting with the first field in the index.
	 *
	 *	@param	record the record in which the index fields will be stored.
	 *	@param	indexNo the number of the index which the fields will be set for.
	 *	@param	scatterBufferHelper a scatter buffer helper containing the fields to copy to the record.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 *	@retval	STATUS_CODE_INVALID_LENGTH
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 */
	virtual StatusCode SetIndexFields(Record &record, const IndexSpecification::Number indexNo, const ScatterBufferHelper &scatterBufferHelper) const = 0;

	/**
	 *	@brief	Check the integrity of the table.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual void CheckIntegrity() const = 0;

	/**
	 *	@brief	Get a reference to the field list of the table.
	 *
	 *	@return	the field list of the table.
	 */
	virtual const FieldList &GetFieldList() const = 0;

	/**
	 *	@brief	Get the table specification.
	 *
	 *	@return	the table specification.
	 */
	virtual TableSpecification GetTableSpecification() const = 0;

	/**
	 *	@brief	Get the list of all field specifications.
	 *
	 *	@return	the field specification list.
	 */
	virtual FieldSpecificationList GetFieldSpecificationList() const = 0;

	/**
	 *	@brief	Get the list of field specifications that match the criteria specified in the field specification parameter.
	 *
	 *	@param	fieldSpecification the criteria used to match specifications.
	 *	@param	matchingFieldSpecificationList set equal to the matched field specification list.
	 *
	 *	@retval	true if at least one matching field specification is found.
	 */
	virtual bool GetFieldSpecificationList(const FieldSpecification &fieldSpecification, FieldSpecificationList &matchingFieldSpecificationList) const = 0;

	/**
	 *	@brief	Get the field specification for the field id specified.
	 *
	 *	@param	fieldId the id of the field whose specification is to be retrieved.
	 *	@param	fieldSpecification the variable that will hold the retrieved field specification.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetFieldSpecification(const FieldSpecification::Id fieldId, FieldSpecification &fieldSpecification) const = 0;

	/**
	 *	@brief	Get the field specification of the first field in the record.
	 *
	 *	@param	fieldSpecification the variable that will hold the retrieved field specification.
	 */
	virtual void GetFirstFieldSpecification(FieldSpecification &fieldSpecification) const = 0;

	/**
	 *	@brief	Get the field specification of the next field in the record.
	 *
	 *	@param	fieldSpecification the variable that will hold the retrieved field specification.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetNextFieldSpecification(FieldSpecification &fieldSpecification) const = 0;

	/**
	 *	@brief	Get the list of all index specifications.
	 *
	 *	@return	the index specification list.
	 */
	virtual IndexSpecificationList GetIndexSpecificationList() const = 0;

	/**
	 *	@brief	Get the list of index specifications that match the criteria specified in the index specification parameter.
	 *
	 *	@param	indexSpecification the criteria used to match specifications.
	 *	@param	matchingIndexSpecificationList set equal to the matched index specification list.
	 *
	 *	@retval	true if at least one matching index specification is found.
	 */
	virtual bool GetIndexSpecificationList(const IndexSpecification &indexSpecification, IndexSpecificationList &matchingIndexSpecificationList) const = 0;

	/**
	 *	@brief	Get the index specification for the specified index number.
	 *
	 *	@param	indexNo the number of the index whose specification is to be retrieved.
	 *	@param	indexSpecification set equal to the retrieved index specification.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetIndexSpecification(const IndexSpecification::Number indexNo, IndexSpecification &indexSpecification) const = 0;

	/**
	 *	@brief	Get the list of all field identifiers.
	 *
	 *	@return	the field identifier list.
	 */
	virtual FieldIdList GetFieldIdList() const = 0;

	/**
	 *	@brief	Get the id of the first field in the field list object.
	 *
	 *	@param	fieldId set equal to the first field id in the field list object
	 */
	virtual void GetFirstFieldId(FieldSpecification::Id &fieldId) const = 0;

	/**
	 *	@brief	Get the id of the next field in the field list object.
	 *
	 *	@param	fieldId set equal to the next field id in the field list object
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetNextFieldId(FieldSpecification::Id &fieldId) const = 0;

	/**
	 *	@brief	Get the field type for the field id specified.
	 *
	 *	@param	fieldId the id of the field whose specification is to be retrieved.
	 *	@param	type set equal to the type of the field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_KEY
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetType(const FieldSpecification::Id fieldId, FieldSpecification::Type &type) const = 0;

	/**
	 *	@brief	Get name.
	 *
	 *	@return	the name of the table.
	 */
	virtual std::string GetName() const = 0;

	/**
	 *	@brief	Get the name of the directory used to persist information.
	 *
	 *	@return	the name of the directory.
	 */
	virtual std::string GetDirectory() const = 0;

	/**
	 *	@brief	Get the basic file name used for storing table record data.
	 *
	 *	@return	the basic file name.
	 */
	virtual std::string GetBasicFileName() const = 0;

	/**
	 *	@brief	Get the maximum number of records.
	 *
	 *	@param	maxRecords set equal to the maximum number of records.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	virtual StatusCode GetMaxRecords(size_t &maxRecords) const = 0;

	/**
	 *	@brief	Get the current number of records.
	 *
	 *	@param	nRecords set equal to the current number of records.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 */
	virtual StatusCode GetCurrentRecords(size_t &nRecords) const = 0;

	/**
	 *	@brief	Get Record Length.
	 */
	virtual size_t GetRecordLength() const = 0;

	/**
	 *	@brief	Get number of indexes.
	 *
	 *	@return number of indexes.
	 */
	virtual size_t GetNumberOfIndexes() const = 0;

	/**
	 *	@brief	Get the number of the primary index.
	 *
	 *	@return the number of the primary index.
	 */
	virtual IndexSpecification::Number GetPrimaryIndexNumber() const = 0;

	/**
	 *	@brief	Get an index pointer.
	 *
	 *	@param	indexNo the number of the index.
	 *	@param	pIndex set equal to the index pointer of the requested index.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_INDEX
	 */
	virtual StatusCode GetIndexPointer(const IndexSpecification::Number indexNo, IIndex *&pIndex) const = 0;

	/**
	 *	@brief	Get the identity of the table.
	 *
	 *	@return	the identity of the table.
	 */
	Identity GetIdentity() const;

	/**
	 *	@brief	Set mode.
	 *
	 *	@param	mode the new mode of the table.
	 */
	void SetMode(const Mode mode);

	/**
	 *	@brief	Get mode.
	 *
	 *	@return	the mode of the table.
	 */
	Mode GetMode() const;

	/**
	 *	@brief	Is the record file open.
	 *
	 *	@return	true if the record file is open, false otherwise.
	 */
	virtual bool IsRecordFileOpen() const = 0;

	/**
	 *	@brief	Is this a temporary table.
	 *
	 *	@return true if this is a temporary table, false otherwise.
	 */
	virtual bool IsTemporary() const = 0;

	/**
	 *	@brief	Is add.
	 *
	 *	Returns whether an update to a table resulted in a new record being added.
	 *
	 *	@param	record the new version of the record.
	 *	@param	originalRecord the original version of the record.
	 *
	 *	@return	whether this is an add.
	 */
	virtual bool IsAdd(const Record &record, const Record &originalRecord) const = 0;

	/**
	 *	@brief	Is update.
	 *
	 *	Returns whether an update to a table resulted in a record being updated.
	 *
	 *	@param	record the new version of the record.
	 *	@param	originalRecord the original version of the record.
	 *
	 *	@return	whether this is an update.
	 */
	virtual bool IsUpdate(const Record &record, const Record &originalRecord) const = 0;

	/**
	 *	@brief	Check whether a field id is valid for this table.
	 *
	 *	@return	true if the field id can be found, false otherwise.
	 */
	virtual bool IsFound(const FieldSpecification::Id fieldId) const = 0;

	/**
	 *	@brief	Is this a managed table.
	 *
	 *	@return true if this is a managed table, false otherwise.
	 */
	virtual bool IsManaged() const = 0;

#if defined (_DEBUG)
	/**
	 *	@brief	Display field information.
	 *
	 *	@param	os stream to send output to.
	 */
	virtual void DisplayFieldInformation(std::ostream &os = std::cout) const = 0;

	/**
	 *	@brief	Display key field information.
	 *
	 *	@param	indexNo index number.
	 *	@param	os stream to send output to.
	 */
	virtual void DisplayKeyFieldInformation(const IndexSpecification::Number indexNo, std::ostream &os = std::cout) const = 0;
#endif

protected:
	/**
	 *	@brief	Set the identity of the table.
	 *
	 *	@param	identity the new identity of the table.
	 */
	void SetIdentity(const Identity identity = 0);

	Identity		m_identity;		///< The identity of the table.
	Mode			m_mode;			///< The mode of the table.

private:
	/**
	 *	@brief	Copy constructor.
	 *
	 *	Made private without a definition so that users cannot copy construct the class.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	ITable(const ITable &rhs);

	/**
	 *	@brief	Assignment operator.
	 *
	 *	Made private without a definition so that users cannot assign the class.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	ITable &operator=(const ITable &rhs);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ITable::ITable() : m_identity(this)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ITable::~ITable()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void ITable::CreateIndexes(IndexSpecification indexSpecificationList[], const size_t nIndexSpecifications)
{
	IndexSpecificationList indexSpecificationVector(indexSpecificationList, indexSpecificationList + nIndexSpecifications);

	CreateIndexes(indexSpecificationVector);

	const IndexSpecificationList::const_iterator iteratorEnd = indexSpecificationVector.end();
	IndexSpecificationList::const_iterator iterator = indexSpecificationVector.begin();

	for (size_t i = 0; iteratorEnd != iterator; ++i, ++iterator)
		indexSpecificationList[i].m_number = iterator->m_number;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void ITable::SetIdentity(const Identity identity)
{
	m_identity = identity;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ITable::Identity ITable::GetIdentity() const
{
	return m_identity;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void ITable::SetMode(const Mode mode)
{
	m_mode = mode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Mode ITable::GetMode() const
{
	return m_mode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

} // namespace Database

} // namespace Activ

#endif // !defined (ACTIV_DATABASE_ITABLE_H)
