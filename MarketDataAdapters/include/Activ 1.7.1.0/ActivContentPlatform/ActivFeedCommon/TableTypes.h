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
 *
 *	  TYPE							NAME				ABRV NAME
 */
#define ACTIV_TABLE_TYPE_LIST(d)								\
	d(TABLE_TYPE_REALTIME,			Realtime,			RT)		\
	d(TABLE_TYPE_NAVIGATION,		Navigation,			Nav)	\
	d(TABLE_TYPE_PERMISSION,		Permission,			Perm)	\
	d(TABLE_TYPE_CORPORATE_ACTION,	CorporateAction,	CA)		\
	d(TABLE_TYPE_NEWS,				News,				News)	\
	d(TABLE_TYPE_INTERNAL,			Internal,			Intl)	\
	d(TABLE_TYPE_REALTIME_LEVEL_2,	RealtimeLevel2,		RTL2)	\
	d(TABLE_TYPE_ALIAS,				Alias,				Als)	\
	d(TABLE_TYPE_METADATA,			Metadata,			Mtda)

/**
 *	@brief	The table type enumeration declaration.
 */
#define ACTIV_DECLARE_TABLE_TYPE_ENUMERATIONS(id, name, abbreviatedName)			id,
#define ACTIV_DECLARE_TABLE_TYPE_STRINGS(id, name, abbreviatedName)					ACTIV_STRINGIZE(name),
#define ACTIV_DECLARE_TABLE_TYPE_ABBREVIATED_STRINGS(id, name, abbreviatedName)		ACTIV_STRINGIZE(abbreviatedName),

namespace Activ
{

namespace ContentPlatform
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
 *	@brief	Convert a table type to an abbreviated string.
 *
 *	@param	tableType the table type.
 *
 *	@return	the abbreviated string corresponding to the table type.
 */
inline std::string TableTypeToAbbreviatedString(const TableType tableType)
{
	static const char * const strs[] = { ACTIV_TABLE_TYPE_LIST(ACTIV_DECLARE_TABLE_TYPE_ABBREVIATED_STRINGS) };

	if (tableType < ACTIV_ARRAY_SIZE(strs))
		return std::string(strs[tableType]);

	return std::string("Unkn");
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
		if (name == strs[i])
			return i;
	}

	return TABLE_TYPE_UNKNOWN;
}

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_FEED_TABLE_TYPES_H)
