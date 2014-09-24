/**
 *  @file	TableSpecification.h
 *
 *	@brief	Header file for the table specification class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_TABLE_SPECIFICATION_H)
#define ACTIV_DATABASE_TABLE_SPECIFICATION_H

#include "ActivMiddleware/ActivDatabase/External.h"

#include <string>

namespace Activ
{

class MessageBuilder;
class MessageValidater;

namespace Database
{

class ITable;
typedef std::vector<FieldSpecification> FieldSpecificationList;					///< Field specification list definition. Defined here as well as Definitions.h because of a cyclic dependency.

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Field specification structure.
 */
class TableSpecification
{
public:
	static const uint8_t VERSION = 1;						///< Version of the specification.

	static const size_t GROW_RECORDS_DEFAULT = ~0;			///< Table will grow by the default number of records when it becomes full.
	static const size_t CACHE_COMPLETE = ~0;				///< Cache the complete contents of the table.

	/**
	 *	@brief	Properties.
	 */
	enum
	{
		PROPERTY_NONE = 0x00000000,
		PROPERTY_CREATE_RECORD_FILE = 0x00000001,			///< If not specified hands control of creating the record data store to the user so that errors can be reported.
		PROPERTY_CREATE_MUTEX = 0x00000002,					///< Create a mutex to control access to the table. Locking occurs over the whole table.
		PROPERTY_CONVERT = 0x00000004,						///< Persisted table data is converted if the specification for a table changes.
	};
	typedef uint32_t Properties;							///< Definition of the properties of the table.

	/**
	 *	@brief	Constructor.
	 */
	ACTIV_DATABASE_API TableSpecification();

	/**
	 *	@brief	Constructor for managed tables.
	 *
	 *	@param	name set equal to the name of the table.
	 *	@param	properties set equal to the table properties.
	 *	@param	fieldSpecificationList set equal to the field specification list.
	 *	@param	initialMaxRecords set equal to the initial maxmimum number of records.
	 *	@param	nGrowRecords the number of records to grow the table by.
	 *	@param	cacheSize set equal to the cache size.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API TableSpecification(const std::string &name, const Properties properties, const FieldSpecificationList &fieldSpecificationList, const size_t initialMaxRecords, const size_t nGrowRecords = GROW_RECORDS_DEFAULT, const size_t cacheSize = CACHE_COMPLETE);

	/**
	 *	@brief	Constructor for unmanaged tables.
	 *
	 *	@param	name set equal to the name of the table.
	 *	@param	properties set equal to the table properties.
	 *	@param	fieldSpecificationList set equal to the field specification list.
	 *	@param	initialMaxRecords set equal to the initial maxmimum number of records.
	 *	@param	recordLength the length of a record.
	 *	@param	nGrowRecords the number of records to grow the table by.
	 *	@param	cacheSize set equal to the cache size.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API TableSpecification(const std::string &name, const Properties properties, const FieldSpecificationList &fieldSpecificationList, const size_t initialMaxRecords, const size_t recordLength, const size_t nGrowRecords = GROW_RECORDS_DEFAULT, const size_t cacheSize = CACHE_COMPLETE);

	/**
	 *	@brief	Constructor for unmanaged tables.
	 *
	 *	@param	name the name of the table.
	 *	@param	isManaged indicates whether the table is managed.
	 *	@param	properties set equal to the table properties.
	 *	@param	fieldSpecificationList set equal to the field specification list.
	 *	@param	initialMaxRecords set equal to the initial maxmimum number of records.
	 *	@param	recordLength the length of a record.
	 *	@param	nGrowRecords the number of records to grow the table by.
	 *	@param	cacheSize set equal to the cache size.
	 *
	 *	@throw	DatabaseException
	 */
	TableSpecification(const std::string &name, const bool isManaged, const Properties properties, const FieldSpecificationList &fieldSpecificationList, const size_t initialMaxRecords, const size_t recordLength, const size_t nGrowRecords, const size_t cacheSize);

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_DATABASE_API bool operator==(const TableSpecification &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_DATABASE_API bool operator!=(const TableSpecification &rhs) const;

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
	ACTIV_DATABASE_API static StatusCode Serialize(MessageBuilder &messageBuilder, ITable *pTable);

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
	 *	@brief	Create a table from a table specification.
	 *
	 *	@param	directory name of directory in which to store persistent table files, or empty to create a temporary version of the table.
	 *
	 *	@return	a pointer to the created table.
	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	DatabaseException
	 *	@throw	OsException
	 */
	ACTIV_DATABASE_API ITable *CreateTable(const std::string &directory = std::string()) const;

	/**
	 *	@brief	Should create record file.
	 *
	 *	@param	properties the properties to check.
	 *
	 *	@return	true if the record file should be created.
	 */
	ACTIV_DATABASE_API static bool ShouldCreateRecordFile(const Properties properties);

	/**
	 *	@brief	Should create mutex.
	 *
	 *	@param	properties the properties to check.
	 *
	 *	@return	true if the mutex should be created.
	 */
	ACTIV_DATABASE_API static bool ShouldCreateMutex(const Properties properties);

	/**
	 *	@brief	Should convert.
	 *
	 *	@param	properties the properties to check.
	 *
	 *	@return	true if the table data should be converted.
	 */
	ACTIV_DATABASE_API static bool ShouldConvert(const Properties properties);

	/**
	 *	@brief	Convert the specification to a C-style string.
	 *
	 *	@return	the specification as a string.
	 */
	ACTIV_DATABASE_API std::string ToString() const;

	std::string					m_name;						///< Name.
	bool						m_isManaged;				///< Indicates whether the table is managed.
	Properties					m_properties;				///< Properties.
	FieldSpecificationList		m_fieldSpecificationList;	///< Field specification list.
	size_t						m_initialMaxRecords;		///< Initial maximum number of records.
	size_t						m_recordLength;				///< Record length.
	size_t						m_nGrowRecords;				///< Number of records to grow the table by.
	size_t						m_cacheSize;				///< The size, in kb, of the record cache.
};

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
ACTIV_DATABASE_API std::ostream &operator<<(std::ostream &os, const Activ::Database::TableSpecification &rhs);

// ---------------------------------------------------------------------------------------------------------------------------------

#endif	// !defined(ACTIV_DATABASE_TABLE_SPECIFICATION_H)
