/**
 *  @file	Definitions.h
 *
 *	@brief	Header file for the database definition information.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_DEFINITIONS_H)
#define ACTIV_DATABASE_DEFINITIONS_H

#include "ActivMiddleware/Activ.h"

#include "ActivMiddleware/ActivDatabase/FieldSpecification.h"
#include "ActivMiddleware/ActivDatabase/IndexSpecification.h"
#include "ActivMiddleware/ActivDatabase/TableSpecification.h"
#include "ActivMiddleware/ActivDatabase/DatabaseSpecification.h"

namespace Activ
{

namespace Database
{

// Used as a unique identifier for records and keys in data storage areas.
typedef int32_t DataId;															///< Data identifier definition.
const DataId UNDEFINED_ID = -1;													///< Undefined id.

// Mode is a user defined value which can be used to indicate different states
// of database or table operation. For example, it can be used to indicate that
// a database is recovering from a failure and the contents shouldn't necessarily
// be trusted.
typedef byte_t Mode;															///< Mode type definition.

// Default value used when doing partial matches on keys in tables.
const size_t FIELD_COMPARE_LENGTH_WHOLE = ~0;									///< Compare the whole length of a field.

typedef std::vector<FieldSpecification> FieldSpecificationList;					///< Field specification list definition.
typedef std::vector<FieldSpecification::Id> FieldIdList;						///< Field identifier list definition.
typedef std::vector<IndexSpecification> IndexSpecificationList;					///< Index specification list definition.
typedef std::vector<IndexSpecification::Field> IndexFieldSpecificationList;		///< Index field specification list.

} // namespace Database

} // namespace Activ

#endif	// !defined (ACTIV_DATABASE_DEFINITIONS_H)
