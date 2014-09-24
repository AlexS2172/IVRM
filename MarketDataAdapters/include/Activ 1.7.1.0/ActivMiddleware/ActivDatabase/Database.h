/**
 *  @file	Database.h
 *
 *	@brief	Header file for the Database class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_DATABASE_H)
#define ACTIV_DATABASE_DATABASE_H

#include "ActivMiddleware/ActivBase/IMutex.h"
#include "ActivMiddleware/ActivBase/MutexHelper.h"

#include "ActivMiddleware/ActivDatabase/ITable.h"
#include "ActivMiddleware/ActivDatabase/DatabaseException.h"
#include "ActivMiddleware/ActivDatabase/DatabaseSpecification.h"

#include "ActivMiddleware/Misc/StlHelper.h"

#include <vector>
#include <set>

namespace Activ
{

namespace Database
{

/**
 *	@brief	Database class.
 */
class Database
{
private:
	typedef std::vector<ITable *> TablePtrList;						///< Definition of the table pointer list.
	typedef std::set<DatabaseSpecification::TableId> TableIdSet;	///< Definition of the table id set.

public:
	/**
	 *	@brief	Const iterator class.
	 */
	class ConstIterator
	{
	public:
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
		ACTIV_DATABASE_API const ConstIterator operator++(int);

		/**
		*	@brief	Equality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are equal.
		*/
		ACTIV_DATABASE_API bool operator==(const ConstIterator &rhs) const;

		/**
		*	@brief	Inequality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are not equal.
		*/
		ACTIV_DATABASE_API bool operator!=(const ConstIterator &rhs) const;

		/**
		 *	@brief	Get the table id.
		 *
		 *	@return	the table id.
		 */
		ACTIV_DATABASE_API DatabaseSpecification::TableId GetTableId() const;

		/**
		 *	@brief	Get the table.
		 *
		 *	@return	a pointer to the table.
		 */
		ACTIV_DATABASE_API const ITable *GetTable() const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	tablePtrList the table pointer list.
		 *	@param	iterator the iterator.
		 */
		ConstIterator(const TablePtrList &tablePtrList, const TableIdSet::const_iterator &iterator);

		const TablePtrList &		m_tablePtrList;	///< The table pointer list.
		TableIdSet::const_iterator	m_iterator;		///< The iterator.

		friend class Database;
	};

	/**
	 *	@brief	Iterator class.
	 */
	class Iterator
	{
	public:
		/**
		 *	@brief	Prefix increment operator.
		 *
		 *	@return	a reference to the updated object.
		 */
		ACTIV_DATABASE_API Iterator &operator++();

		/**
		 *	@brief	Postfix increment operator.
		 *
		 *	@return	a copy of the original object.
		 */
		ACTIV_DATABASE_API const Iterator operator++(int);

		/**
		*	@brief	Equality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are equal.
		*/
		ACTIV_DATABASE_API bool operator==(const Iterator &rhs) const;

		/**
		*	@brief	Inequality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are not equal.
		*/
		ACTIV_DATABASE_API bool operator!=(const Iterator &rhs) const;

		/**
		 *	@brief	Get the table id.
		 *
		 *	@return	the table id.
		 */
		ACTIV_DATABASE_API DatabaseSpecification::TableId GetTableId() const;

		/**
		 *	@brief	Get the table.
		 *
		 *	@return	a pointer to the table.
		 */
		ACTIV_DATABASE_API ITable *GetTable();

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	tablePtrList the table pointer list.
		 *	@param	iterator the iterator.
		 */
		Iterator(const TablePtrList &tablePtrList, const TableIdSet::iterator &iterator);

		const TablePtrList &		m_tablePtrList;	///< The table pointer list.
		TableIdSet::iterator		m_iterator;		///< The iterator.

		friend class Database;
	};

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	name the name of the database.
	 *	@param	properties the properties.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API Database(const std::string &name = std::string(), const DatabaseSpecification::Properties properties = DatabaseSpecification::PROPERTY_NONE);

	/**
	 *	@brief	Default destructor.
	 */
	ACTIV_DATABASE_API ~Database();

	/**
	 *	@brief	Attach a table to the database.
	 *
	 *	The database object will be responsible for deleting the table object if this call succeeds.
	 *
	 *	@param	tableId the identifier for this table. Must be unique for each table in the database.
	 *	@param	pTable a pointer to the table to be added.
	 */
	ACTIV_DATABASE_API StatusCode AttachTable(const DatabaseSpecification::TableId tableId, ITable *pTable);

	/**
	 *	@brief	Detach a table from the database.
	 *
	 *	The database object is no longer responsible for deleting the table object if this call succeeds.
	 *
	 *	@param	tableId the identifier for the table.
	 *	@param	pTable the detached table pointer is stored in this variable.
	 */
	ACTIV_DATABASE_API StatusCode DetachTable(const DatabaseSpecification::TableId tableId, ITable *&pTable);

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
	ACTIV_DATABASE_API StatusCode GetTable(const DatabaseSpecification::TableId tableId, ITable *&pTable) const;

	/**
	 *	@brief	Get record from first table with matching supplied key information.
	 *
	 *	The function assumes that the field ids and field types of the key fields supplied are consistent
	 *	across the tables to be searched in the database. It will only search a particular index if all of
	 *	the key fields can be copied from the supplied key. It also searches the primary index of any table
	 *	first.
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
	ACTIV_DATABASE_API StatusCode GetEqual(const Key &key, Record &record) const;

	/**
	 *	@brief	Get record from specified table and index matching supplied key information.
	 *
	 *	The function assumes that the field ids and field types of the key fields supplied are consistent
	 *	across the tables to be searched in the database. It will only search a particular index if all of
	 *	the key fields can be copied from the supplied key.
	 *
	 *	@param	pTable the table to be searched.
	 *	@param	indexNo the index to be searched.
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
	StatusCode GetEqual(const ITable *pTable, const IndexSpecification::Number indexNo, const Key &key, Record &record) const;

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
	ACTIV_DATABASE_API StatusCode SerializeSpecification(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Get the specification.
	 *
	 *	@return	the specification.
	 */
	ACTIV_DATABASE_API DatabaseSpecification GetSpecification() const;

	/**
	 *	@brief	Get the name of the database.
	 *
	 *	@return	the name of the database
	 */
	ACTIV_DATABASE_API std::string GetName() const;

	/**
	 *	@brief	Get a list of all table ids in the database.
	 *
	 *	@return	a list of table ids.
	 */
	TableIdList GetTableIdList() const;

	/**
	 *	@brief	Get the number of tables in the database.
	 *
	 *	@return	the number of tables.
	 */
	ACTIV_DATABASE_API size_t GetNumberOfTables() const;

	/**
	 *	@brief	Set mode.
	 *
	 *	@param	mode the new mode of the database.
	 */
	void SetMode(const Mode mode);

	/**
	 *	@brief	Set the search order of the tables in the database.
	 *
	 *	Any table that is attached to the database after a search order is set will be appended to the
	 *	end of the list.
	 *
	 *	@param	tableIdList the ordered list of table ids.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *
	 */
	StatusCode SetSearchOrder(const TableIdList &tableIdList);

	/**
	 *	@brief	Get search order.
	 *
	 *	@return	the ordered list of table ids.
	 */
	TableIdList GetSearchOrder() const;

	/**
	 *	@brief	Get mode.
	 *
	 *	@return	the mode of the database.
	 */
	Mode GetMode() const;

	/**
	 *	@brief	Get a const iterator to the first table in the database.
	 *
	 *	@return	a const iterator object.
	 */
	ConstIterator Begin() const;

	/**
	 *	@brief	Get a const iterator that points to beyond the last table in the list.
	 *
	 *	@return	a const iterator object.
	 */
	ConstIterator End() const;

	/**
	 *	@brief	Get an iterator to the first table in the database.
	 *
	 *	@return	an iterator object.
	 */
	Iterator Begin();

	/**
	 *	@brief	Get an iterator that points to beyond the last table in the list.
	 *
	 *	@return	an iterator object.
	 */
	Iterator End();

private:
	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	Database(const Database &rhs);

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	Database &operator=(const Database &rhs);

	/**
	 *	@brief	Is table id valid.
	 *
	 *	This function assumes the mutex has already been acquired.
	 *
	 *	@param	tableId the identifier for the table.
	 *
	 *	@return	true if a table exists in the database for the specified table id.
	 */
	bool IsTableIdValid(const DatabaseSpecification::TableId tableId) const;

	std::string							m_name;						///< The name of the database.
	DatabaseSpecification::Properties	m_properties;				///< The properties;
	Mode								m_mode;						///< The mode of the database.
	IMutex								*m_pMutex;					///< Lock around the database.
	TablePtrList						m_tablePtrList;				///< A sparsely populated array of table pointers.
	TableIdSet							m_tableIdSet;				///< A set of all table ids in the database.
	TableIdList							m_tableIdSearchOrderList;	///< The order which tables should be searched in.

	friend class ConstIterator;
	friend class Iterator;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline TableIdList Database::GetTableIdList() const
{
	return TableIdList(m_tableIdSet.begin(), m_tableIdSet.end());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Database::SetMode(const Mode mode)
{
	m_mode = mode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Mode Database::GetMode() const
{
	return m_mode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::ConstIterator Database::Begin() const
{
	return ConstIterator(m_tablePtrList, m_tableIdSet.begin());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::ConstIterator Database::End() const
{
	return ConstIterator(m_tablePtrList, m_tableIdSet.end());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::Iterator Database::Begin()
{
	return Iterator(m_tablePtrList, m_tableIdSet.begin());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::Iterator Database::End()
{
	return Iterator(m_tablePtrList, m_tableIdSet.end());
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::ConstIterator &Database::ConstIterator::operator++()
{
	++m_iterator;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Database::ConstIterator Database::ConstIterator::operator++(int)
{
	ConstIterator oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Database::ConstIterator::operator==(const Database::ConstIterator &rhs) const
{
	return (m_iterator == rhs.m_iterator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Database::ConstIterator::operator!=(const Database::ConstIterator &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DatabaseSpecification::TableId Database::ConstIterator::GetTableId() const
{
	return *m_iterator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const ITable *Database::ConstIterator::GetTable() const
{
	return m_tablePtrList[*m_iterator];
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::ConstIterator::ConstIterator(const TablePtrList &tablePtrList, const TableIdSet::const_iterator &iterator) :
	m_tablePtrList(tablePtrList),
	m_iterator(iterator)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::Iterator &Database::Iterator::operator++()
{
	++m_iterator;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Database::Iterator Database::Iterator::operator++(int)
{
	Iterator oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Database::Iterator::operator==(const Database::Iterator &rhs) const
{
	return (m_iterator == rhs.m_iterator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Database::Iterator::operator!=(const Database::Iterator &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline DatabaseSpecification::TableId Database::Iterator::GetTableId() const
{
	return *m_iterator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ITable *Database::Iterator::GetTable()
{
	return m_tablePtrList[*m_iterator];
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Database::Iterator::Iterator(const TablePtrList &tablePtrList, const TableIdSet::iterator &iterator) :
	m_tablePtrList(tablePtrList),
	m_iterator(iterator)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

} // namespace Database

} // namespace Activ

#endif // !defined (ACTIV_DATABASE_DATABASE_H)
