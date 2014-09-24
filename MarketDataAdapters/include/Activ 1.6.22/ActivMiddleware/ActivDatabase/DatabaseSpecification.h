/**
 *  @file	DatabaseSpecification.h
 *
 *	@brief	Header file for the database specification class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_DATABASE_SPECIFICATION_H)
#define ACTIV_DATABASE_DATABASE_SPECIFICATION_H

#include "ActivMiddleware/Misc/StlHelper.h"

#include "ActivMiddleware/ActivDatabase/External.h"

#include "ActivMiddleware/ActivDatabase/TableSpecification.h"
#include "ActivMiddleware/ActivDatabase/IndexSpecification.h"

#include <string>

namespace Activ
{

class MessageBuilder;
class MessageValidater;

namespace Database
{

typedef std::vector<IndexSpecification> IndexSpecificationList;					///< Index specification list definition. Defined here as well as Definitions.h because of a cyclic dependency.

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Database specification structure.
 */
class DatabaseSpecification
{
public:
	static const uint8_t VERSION = 1;				///< Version of the specification.

	typedef uint16_t TableId;						///< Unique identifier for tables.
	const static TableId UNDEFINED_TABLE_ID = ~0;	///< Undefined table id.

	/**
	 *	@brief	Database table specification structure.
	 */
	class DatabaseTableSpecification
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	tableId the id of the table.
		 *	@param	tableSpecification the specification of the table
		 *	@param	indexSpecificationList the list of index specifications.
		 *
		 *	@throw	DatabaseException
		 */
		ACTIV_DATABASE_API DatabaseTableSpecification(const TableId tableId = UNDEFINED_TABLE_ID, const TableSpecification &tableSpecification = TableSpecification(), const IndexSpecificationList &indexSpecificationList = IndexSpecificationList());

		/**
		 *	@brief	Serialize the database specification into a message.
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
		 *	@brief	Deserialize a database specification from a message.
		 *
		 *	@param	messageValidater the message validater from which the database specification is extracted.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_HEAP_EMPTY
		 *	@retval	STATUS_CODE_NOT_INITIALIZED
		 */
		ACTIV_DATABASE_API StatusCode Deserialize(MessageValidater &messageValidater);

		/**
		 *	@brief	Convert the specification to a C-style string.
		 *
		 *	@return	the specification as a string.
		 */
		ACTIV_DATABASE_API std::string ToString() const;

		TableId					m_tableId;					///< The id used for the table in this database.
		TableSpecification		m_tableSpecification;		///< The table specification.
		IndexSpecificationList	m_indexSpecificationList;	///< The list of index specifications for the table.
	};
	typedef std::vector<DatabaseTableSpecification>	DatabaseTableSpecificationList;	///< Database table specification list type definition.

	/**
	 *	@brief	Properties.
	 */
	enum
	{
		PROPERTY_NONE = 0x00000000,
		PROPERTY_CREATE_MUTEX = 0x00000001		///< Create a mutex to control access to the database. Locking occurs over the whole database.
	};
	typedef uint32_t Properties;				///< Definition of the properties of the database.

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name set equal to the name of the table.
	 *	@param	properties set equal to the database properties.
	 *	@param	databaseTableSpecificationList the list database table specifications.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API DatabaseSpecification(const std::string &name = std::string(), const Properties properties = PROPERTY_NONE, const DatabaseTableSpecificationList &databaseTableSpecificationList = DatabaseTableSpecificationList());

	/**
	 *	@brief	Serialize the database specification into a message.
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
	 *	@brief	Deserialize a database specification from a message.
	 *
	 *	@param	messageValidater the message validater from which the database specification is extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief	Should create mutex.
	 *
	 *	@param	properties the properties to check.
	 *
	 *	@return	true if the mutex should be created.
	 */
	ACTIV_DATABASE_API static bool ShouldCreateMutex(const Properties properties);

	/**
	 *	@brief	Convert the specification to a C-style string.
	 *
	 *	@return	the specification as a string.
	 */
	ACTIV_DATABASE_API std::string ToString() const;

	std::string							m_name;								///< Name.
	Properties							m_properties;						///< Properties.
	DatabaseTableSpecificationList		m_databaseTableSpecificationList;	///< The list of database table specifications.
};

// ---------------------------------------------------------------------------------------------------------------------------------

}	// namespace Database

}	// namespace Activ

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Global operator<< to dump a database specification to an ostream.
 *
 *	@param	os the ostream to write the database specification to.
 *	@param	rhs the field specification to write.
 *
 *	@return	the ostream \a os.
 */
ACTIV_DATABASE_API std::ostream &operator<<(std::ostream &os, const Activ::Database::DatabaseSpecification &rhs);

// ---------------------------------------------------------------------------------------------------------------------------------

#endif	// !defined(ACTIV_DATABASE_DATABASE_SPECIFICATION_H)
