/**
 *	@file	TableTypes.h
 *
 *	@brief	Header file for the table types.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_TABLE_TYPES_H)
#define ACTIV_FEED_TABLE_TYPES_H

#include "ActivMiddleware/Activ.h"

#include <vector>
#include <string>

/**
 *	@brief	The list of table types and their string representation.
 
 *	  TYPE							NAME
 */
#define ACTIV_TABLE_TYPE_LIST(d)						\
	d(TABLE_TYPE_REALTIME,			Realtime)			\
	d(TABLE_TYPE_NAVIGATION,		Navigation)			\
	d(TABLE_TYPE_PERMISSION,		Permission)			\
	d(TABLE_TYPE_CORPORATE_ACTION,	CorporateAction)	\
	d(TABLE_TYPE_NEWS,				News)				\
	d(TABLE_TYPE_INTERNAL,			Internal)			\
	d(TABLE_TYPE_REALTIME_LEVEL_2,	RealtimeLevel2)		\
	d(TABLE_TYPE_ALIAS,				Alias)

/**
 *	@brief	The table type enumeration declaration.
 */
#define ACTIV_DECLARE_TABLE_TYPE_ENUMERATIONS(id, name)	id,
#define ACTIV_DECLARE_TABLE_TYPE_STRINGS(id, name)		ACTIV_STRINGIZE(name),

namespace Activ
{

namespace Feed
{

typedef uint8_t TableType;			///< The table type type definition.

/**
 *	@brief	Enumeration of all table types.
 */
enum
{
	ACTIV_TABLE_TYPE_LIST(ACTIV_DECLARE_TABLE_TYPE_ENUMERATIONS)
	NUMBER_OF_TABLE_TYPES,
	TABLE_TYPE_UNKNOWN = static_cast<TableType>(~0),
};

/**
 *	@brief	Convert a table type to a string.
 *
 *	@param	tableType the table type.
 *
 *	@return	the string corresponding to the table type.
 */
inline std::string TableTypeToString(const TableType tableType)
{
	static const char * const strs[] = { ACTIV_TABLE_TYPE_LIST(ACTIV_DECLARE_TABLE_TYPE_STRINGS) };

	if (tableType < ACTIV_ARRAY_SIZE(strs))
		return std::string(strs[tableType]);

	return std::string("Unknown");
}

/**
 *	@brief	Convert a string to a table type.
 *
 *	@param	name the name of the table type
 *
 *	@return	the table type that corresponds to the name (or TABLE_TYPE_UNKNOWN).
 */
inline TableType StringToTableType(const std::string &name)
{
	const std::string strs[] = { ACTIV_TABLE_TYPE_LIST(ACTIV_DECLARE_TABLE_TYPE_STRINGS) };

	for (TableType i = 0; i < NUMBER_OF_TABLE_TYPES; ++i)
	{
		if(name == strs[i])
			return i;
	}

	return TABLE_TYPE_UNKNOWN;
}

} // namespace Feed

} // namespace Activ

#endif // !defined(ACTIV_FEED_TABLE_TYPES_H)
