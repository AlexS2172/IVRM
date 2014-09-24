/**
 *	@file	ActivFeedDefinitions.h
 *
 *	@brief	Header file for activ feed definitions.
 *
 *  $Log: $
 */

#if !defined (ACTIV_ACTIV_FEED_DEFINITIONS_H)
#define ACTIV_ACTIV_FEED_DEFINITIONS_H

#include "ActivMiddleware/Activ.h"

#include <vector>

namespace Activ
{

namespace ContentPlatform
{

namespace Feed
{

typedef uint8_t EventType;							///< The event type type definition.

typedef uint16_t FieldId;							///< The field id type definition.
typedef std::vector<FieldId> FieldIdList;			///< The field id list type defintion.

typedef uint8_t IndexNumber;						///< The index number type definition.
typedef uint8_t IndexType;							///< The index type definition.
typedef uint8_t IndexProperties;					///< The index properties definition.

typedef uint16_t PermissionId;						///< The permission id type definition.
typedef std::vector<PermissionId> PermissionIdList;	///< The permission id list type definition.
typedef uint8_t PermissionLevel;					///< The permission level type definition.

typedef uint8_t RelationshipType;					///< The relationship type definition.
typedef uint16_t RelationshipId;					///< The relationship id type definition.

typedef uint16_t TableId;							///< The table id type definition.
typedef uint8_t TableVersion;						///< The table version type definition.
typedef uint16_t TableNo;							///< The table number type definition.
typedef std::vector<TableNo> TableNoList;			///< The table number list type definition.

typedef uint16_t UpdateId;							///< The update id type definition.

typedef uint32_t Context;							///< The symbol context type definition.

typedef uint16_t CugId;								///< The cug id type definition.
typedef std::vector<CugId> CugIdList;				///< The cug id list type definition.

const TableNo TABLE_NO_UNDEFINED = ~0;				///< Table number undefined.
const TableNo TABLE_NO_CONTRIBUTION_BASE = 5000;	///< Starting value for the table number of any contributed tables.
const TableNo TABLE_NO_MAX = 8191;					///< Maximum allowed table number.

const Context CONTEXT_UNDEFINED = ~0;				///< Context undefined.

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_ACTIV_FEED_DEFINITIONS_H)
