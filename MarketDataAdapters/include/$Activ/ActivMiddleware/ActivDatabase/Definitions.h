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

namespace Activ
{

namespace Database
{

typedef int32_t DataId;															///< Data identifier definition.
const DataId UNDEFINED_ID = -1;													///< Undefined id.

const size_t FIELD_COMPARE_LENGTH_WHOLE = ~0;									///< Compare the whole length of a field.

typedef std::vector<FieldSpecification> FieldSpecificationList;					///< Field specification list definition.
typedef std::vector<FieldSpecification::Id> FieldIdList;						///< Field identifier list definition.
typedef std::vector<IndexSpecification> IndexSpecificationList;					///< Index specification list definition.
typedef std::vector<IndexSpecification::Field> IndexFieldSpecificationList;		///< Index field specification list.

} // namespace Database

} // namespace Activ

#endif	// !defined (ACTIV_DATABASE_DEFINITIONS_H)
