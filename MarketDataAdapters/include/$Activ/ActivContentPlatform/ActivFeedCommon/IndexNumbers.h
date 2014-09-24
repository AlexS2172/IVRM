/**
 *	@file	IndexNumbers.h
 *
 *	@brief	Header file for the table template index numbers.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_INDEX_NUMBERS_H)
#define ACTIV_FEED_INDEX_NUMBERS_H

#include "ActivMiddleware/Activ.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

#include <vector>

/**
 *	@brief	The list of index types and their string representation.
 
 *	  TYPE							NAME
 */
#define ACTIV_INDEX_TYPE_LIST(d)			\
	d(INDEX_TYPE_RED_BLACK_TREE,	Tree)	\
	d(INDEX_TYPE_HASH,				Hash)

/**
 *	@brief	The index type enumeration declaration.
 */
#define ACTIV_DECLARE_INDEX_TYPE_ENUMERATIONS(id, name)	id,
#define ACTIV_DECLARE_INDEX_TYPE_STRINGS(id, name)		ACTIV_STRINGIZE(name),

/**
 *	@brief	The list of index properties and their string representation.
 
 *	  TYPE								NAME
 */
#define ACTIV_INDEX_PROPERTIES_LIST(d)					\
	d(INDEX_PROPERTY_NONE,				None)			\
	d(INDEX_PROPERTY_UNIQUE,			Unique)			\
	d(INDEX_PROPERTY_DUPLICATE,			Duplicate)		\
	d(INDEX_PROPERTY_PRIMARY_UNIQUE,	PrimaryUnique)
/**
 *	@brief	The index properties enumeration declaration.
 */
#define ACTIV_DECLARE_INDEX_PROPERTIES_ENUMERATIONS(id, name)	id,
#define ACTIV_DECLARE_INDEX_PROPERTIES_STRINGS(id, name)		ACTIV_STRINGIZE(name),

namespace Activ
{

namespace Feed
{

/**
 *	@brief	Enumeration of all index properties.
 */
enum
{
	ACTIV_INDEX_TYPE_LIST(ACTIV_DECLARE_INDEX_TYPE_ENUMERATIONS)
	NUMBER_OF_INDEX_TYPES,
	INDEX_TYPE_UNKNOWN = static_cast<IndexType>(~0),
};

/**
 *	@brief	Convert a index type to a string.
 *
 *	@param	indexType the index type.
 *
 *	@return	the string corresponding to the index type.
 */
inline std::string IndexTypeToString(const IndexType indexType)
{
	static const char * const strs[] = { ACTIV_INDEX_TYPE_LIST(ACTIV_DECLARE_INDEX_TYPE_STRINGS) };

	if (indexType < ACTIV_ARRAY_SIZE(strs))
		return std::string(strs[indexType]);

	return std::string("Unknown");
}

/**
 *	@brief	Convert a string to a index type.
 *
 *	@param	name the name of the index type
 *
 *	@return	the index type that corresponds to the name (or INDEX_TYPE_UNKNOWN).
 */
inline IndexType StringToIndexType(const std::string &name)
{
	const std::string strs[] = { ACTIV_INDEX_TYPE_LIST(ACTIV_DECLARE_INDEX_TYPE_STRINGS) };

	for (IndexType i = 0; i < NUMBER_OF_INDEX_TYPES; ++i)
	{
		if(name == strs[i])
			return i;
	}

	return INDEX_TYPE_UNKNOWN;
}

/**
 *	@brief	Enumeration of all index properties.
 */
enum
{
	ACTIV_INDEX_PROPERTIES_LIST(ACTIV_DECLARE_INDEX_PROPERTIES_ENUMERATIONS)
	NUMBER_OF_INDEX_PROPERTIES,
	INDEX_PROPERTY_UNKNOWN = static_cast<IndexProperties>(~0),
};

/**
 *	@brief	Convert an index property to a string.
 *
 *	@param	indexProperties the index properties.
 *
 *	@return	the string corresponding to the index properties.
 */
inline std::string IndexPropertiesToString(const IndexProperties indexProperties)
{
	static const char * const strs[] = { ACTIV_INDEX_PROPERTIES_LIST(ACTIV_DECLARE_INDEX_PROPERTIES_STRINGS) };

	if (indexProperties < ACTIV_ARRAY_SIZE(strs))
		return std::string(strs[indexProperties]);

	return std::string("Unknown");
}

/**
 *	@brief	Convert a string to an index property.
 *
 *	@param	name the name of the index property
 *
 *	@return	the index properties that corresponds to the name (or INDEX_PROPERTIES_UNKNOWN).
 */
inline IndexProperties StringToIndexProperties(const std::string &name)
{
	const std::string strs[] = { ACTIV_INDEX_PROPERTIES_LIST(ACTIV_DECLARE_INDEX_PROPERTIES_STRINGS) };

	for (IndexProperties i = 0; i < NUMBER_OF_INDEX_PROPERTIES; ++i)
	{
		if(name == strs[i])
			return i;
	}

	return INDEX_PROPERTY_UNKNOWN;
}

} // namespace Feed

} // namespace Activ

#endif // !defined(ACTIV_FEED_INDEX_NUMBERS_H)
