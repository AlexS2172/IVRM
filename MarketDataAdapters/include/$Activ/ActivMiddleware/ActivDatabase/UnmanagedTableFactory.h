/**
 *	@file	UnmanagedTableFactory.h
 *
 *	@brief	Header file for the unmanaged table factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_UNMANAGED_TABLE_FACTORY_H)
#define ACTIV_DATABASE_UNMANAGED_TABLE_FACTORY_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"

#include <string>
#include <vector>

namespace Activ
{

namespace Database
{

class ITable;

/**
 *	@brief	Unmanaged table factory class.
 */
class ACTIV_DATABASE_API UnmanagedTableFactory
{
public:
	/**
	 *	@brief	Create a temporary table using supplied field specification structures.
	 *
	 *	@param	name name of table.
	 *	@param	properties table properties.
	 *	@param	fieldSpecificationList specification list for fields.
	 *	@param	nFields number of table fields.
	 *	@param	initialMaxRecords initial maximum allowed number of records.
	 *	@param	recordLength length of record.
	 *	@param	nGrowRecords the number of records to grow the table by. Zero implies the table should remain a fixed size.
	 *	@param	cacheSize the size of the cache, in kilobytes.
	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	DatabaseException
	 *	@throw	OsException
	 */
	ITable *Create(const std::string &name, const TableSpecification::Properties properties, const FieldSpecification fieldSpecificationList[], const size_t nFields, const size_t initialMaxRecords, const size_t recordLength, const size_t nGrowRecords = TableSpecification::GROW_RECORDS_DEFAULT, const size_t cacheSize = TableSpecification::CACHE_COMPLETE);

	/**
	 *	@brief	Create a temporary table using supplied field specification structures.
	 *
	 *	@param	name name of table.
	 *	@param	properties table properties.
	 *	@param	fieldSpecificationList specification list for fields.
	 *	@param	initialMaxRecords initial maximum allowed number of records.
	 *	@param	recordLength length of record.
	 *	@param	nGrowRecords the number of records to grow the table by. Zero implies the table should remain a fixed size.
	 *	@param	cacheSize the size of the cache, in kilobytes.
	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	DatabaseException
	 *	@throw	OsException
	 */
	ITable *Create(const std::string &name, const TableSpecification::Properties properties, const FieldSpecificationList &fieldSpecificationList, const size_t initialMaxRecords, const size_t recordLength, const size_t nGrowRecords = TableSpecification::GROW_RECORDS_DEFAULT, const size_t cacheSize = TableSpecification::CACHE_COMPLETE);

	/**
	 *	@brief	Create a persistent database table using supplied field specification structures.
	 *
	 *	@param	name name of table.
	 *	@param	directory name of directory in which to store files.
	 *	@param	properties table properties.
	 *	@param	fieldSpecificationList specification list for fields.
	 *	@param	nFields number of table fields.
	 *	@param	initialMaxRecords initial maximum allowed number of records.
	 *	@param	recordLength length of record.
	 *	@param	nGrowRecords the number of records to grow the table by. Zero implies the table should remain a fixed size.
	 *	@param	cacheSize the size of the cache, in kilobytes.
	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	DatabaseException
	 *	@throw	OsException
	 */
	ITable *Create(const std::string &name, const std::string &directory, const TableSpecification::Properties properties, const FieldSpecification fieldSpecificationList[], const size_t nFields, const size_t initialMaxRecords, const size_t recordLength, const size_t nGrowRecords = TableSpecification::GROW_RECORDS_DEFAULT, const size_t cacheSize = TableSpecification::CACHE_COMPLETE);

	/**
	 *	@brief	Create a persistent database table using supplied field specification structures.
	 *
	 *	@param	name name of table.
	 *	@param	directory name of directory in which to store files.
	 *	@param	properties table properties.
	 *	@param	fieldSpecificationList specification list for fields.
	 *	@param	initialMaxRecords initial maximum allowed number of records.
	 *	@param	recordLength length of record.
	 *	@param	nGrowRecords the number of records to grow the table by. Zero implies the table should remain a fixed size.
	 *	@param	cacheSize the size of the cache, in kilobytes.
	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	DatabaseException
	 *	@throw	OsException
	 */
	ITable *Create(const std::string &name, const std::string &directory, const TableSpecification::Properties properties, const FieldSpecificationList &fieldSpecificationList, const size_t initialMaxRecords, const size_t recordLength, const size_t nGrowRecords = TableSpecification::GROW_RECORDS_DEFAULT, const size_t cacheSize = TableSpecification::CACHE_COMPLETE);

	/**
	 *	@brief	Get a reference to the singleton factory object.
	 */
	static UnmanagedTableFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	UnmanagedTableFactory();	

	static UnmanagedTableFactory	m_factory;	///< Singleton unmanaged table factory.
};

} // namespace Database

} // namespace Activ

#endif // !defined (ACTIV_DATABASE_UNMANAGED_TABLE_FACTORY_H)
