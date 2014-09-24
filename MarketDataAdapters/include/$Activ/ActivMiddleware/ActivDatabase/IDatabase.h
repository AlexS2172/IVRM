/**
 *  @file	IDatabase.h
 *
 *	@brief	Header file for the database interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_IDATABASE_H)
#define ACTIV_DATABASE_IDATABASE_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/ITable.h"

namespace Activ
{

namespace Database
{

/**
 *	@brief	Abstract base database class.
 */
class ACTIV_DATABASE_API IDatabase
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	IDatabase();

	/**
	 *	@brief	Default destructor.
	 */
	virtual ~IDatabase() = 0;

	/**
	 *	@brief	Attach a table to the database.
	 *
	 *	The database object will be responsible for deleting the table object if this call succeeds.
	 *
	 *	@param	tableId the identifier for this table. Must be unique for each table in the database.
	 *	@param	pTable a pointer to the table to be added.
	 */
	virtual StatusCode AttachTable(const ITable::Id tableId, ITable *pTable) = 0;

	/**
	 *	@brief	Detach a table from the database.
	 *
	 *	The database object is no longer responsible for deleting the table object if this call succeeds.
	 *
	 *	@param	tableId the identifier for the table.
	 *	@param	pTable the detached table pointer is stored in this variable.
	 */
	virtual StatusCode DetachTable(const ITable::Id tableId, ITable *&pTable) = 0;

	/**
	 *	@brief	Get the table matching the specified identifier.
	 *
	 *	@param	tableId the identifier of the table to be retrieved.
	 *	@param	pTable the retrieved table pointer is stored in this variable.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetTable(const ITable::Id tableId, ITable *&pTable) const = 0;

	/**
	 *	@brief	Get the first table in the database.
	 *
	 *	@param	tableId the retrieved table identifier is stored in this variable.
	 *	@param	pTable the retrieved table pointer is stored in this variable.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetFirst(ITable::Id &tableId, ITable *&pTable) const = 0; 

	/**
	 *	@brief	Get the next table in the database.
	 *
	 *	@param	tableId the retrieved table identifier is stored in this variable.
	 *	@param	pTable the retrieved table pointer is stored in this variable.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	virtual StatusCode GetNext(ITable::Id &tableId, ITable *&pTable) const = 0;

	/**
	 *	@brief	Get the name of the database.
	 *
	 *	@return	the name of the database
	 */
	virtual std::string GetName() const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IDatabase::IDatabase()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IDatabase::~IDatabase()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

} // namespace Database

} // namespace Activ

#endif // !defined (ACTIV_DATABASE_IDATABASE_H)
