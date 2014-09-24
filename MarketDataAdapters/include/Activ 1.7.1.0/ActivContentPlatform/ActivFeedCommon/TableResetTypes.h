/**
 *	@file	TableResetTypes.h
 *
 *	@brief	Header file for the table reset types.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_TABLE_RESET_TYPES_H)
#define ACTIV_FEED_TABLE_RESET_TYPES_H

#include "ActivMiddleware/Activ.h"

#include <vector>
#include <string>

/**
 *	@brief	The list of table reset types and their string representation.
 
 *	  TYPE							NAME
 */
#define ACTIV_TABLE_RESET_TYPE_LIST(d)				\
	d(RESET_TYPE_NONE,			None)				\
	d(RESET_TYPE_INTERNAL,		Internal)			\
	d(RESET_TYPE_EXTERNAL,		External)

/**
 *	@brief	The table reset type enumeration declaration.
 */
#define ACTIV_DECLARE_TABLE_RESET_TYPE_ENUMERATIONS(id, name)	id,
#define ACTIV_DECLARE_TABLE_RESET_TYPE_STRINGS(id, name)		ACTIV_STRINGIZE(name),

namespace Activ
{

namespace ContentPlatform
{

namespace Feed
{

typedef uint8_t TableResetType;			///< The table type type definition.

/**
 *	@brief	Enumeration of all table reset types.
 */
enum
{
	ACTIV_TABLE_RESET_TYPE_LIST(ACTIV_DECLARE_TABLE_RESET_TYPE_ENUMERATIONS)
	NUMBER_OF_TABLE_RESET_TYPES,
	TABLE_RESET_TYPE_UNKNOWN = static_cast<TableResetType>(~0),
};

/**
 *	@brief	Convert a table reset type to a string.
 *
 *	@param	tableResetType the table reset type.
 *
 *	@return	the string corresponding to the table reset type.
 */
inline std::string TableResetTypeToString(const TableResetType tableResetType)
{
	static const char * const strs[] = { ACTIV_TABLE_RESET_TYPE_LIST(ACTIV_DECLARE_TABLE_RESET_TYPE_STRINGS) };

	if (tableResetType < ACTIV_ARRAY_SIZE(strs))
		return std::string(strs[tableResetType]);

	return std::string("Unknown");
}

/**
 *	@brief	Convert a string to a table reset type.
 *
 *	@param	name the name of the table reset type
 *
 *	@return	the table reset type that corresponds to the name (or TABLE_RESET_TYPE_UNKNOWN).
 */
inline TableResetType StringToTableResetType(const std::string &name)
{
	const std::string strs[] = { ACTIV_TABLE_RESET_TYPE_LIST(ACTIV_DECLARE_TABLE_RESET_TYPE_STRINGS) };

	for (TableResetType i = 0; i < NUMBER_OF_TABLE_RESET_TYPES; ++i)
	{
		if(name == strs[i])
			return i;
	}

	return TABLE_RESET_TYPE_UNKNOWN;
}

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_FEED_TABLE_RESET_TYPES_H)
