/**
 *	@file	RelationshipIds.h
 *
 *	@brief	Header file for the relationship ids.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_RELATIONSHIP_IDS_H)
#define ACTIV_FEED_RELATIONSHIP_IDS_H

#include "ActivMiddleware/Activ.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

namespace Activ
{

namespace ContentPlatform
{

namespace Feed
{

const RelationshipType RELATIONSHIP_TYPE_GET_EQUAL = 0;
const RelationshipType RELATIONSHIP_TYPE_GET_MULTIPLE_PATTERN_MATCH_SOURCE = 1;
const RelationshipType RELATIONSHIP_TYPE_GET_MULTIPLE_PATTERN_MATCH_TARGET = 2;

const RelationshipId RELATIONSHIP_ID_NONE = 0;
const RelationshipId RELATIONSHIP_ID_SECTOR = 101;
const RelationshipId RELATIONSHIP_ID_INDEX = 102;
const RelationshipId RELATIONSHIP_ID_LISTING = 104;
const RelationshipId RELATIONSHIP_ID_SECURITY = 105;
const RelationshipId RELATIONSHIP_ID_COMPANY = 106;
const RelationshipId RELATIONSHIP_ID_OPTION_DELIVERABLE = 107;
const RelationshipId RELATIONSHIP_ID_OPTION_ROOT = 108;
const RelationshipId RELATIONSHIP_ID_FUTURE_ROOT = 109;
const RelationshipId RELATIONSHIP_ID_FUTURE_OPTION_ROOT = 110;
const RelationshipId RELATIONSHIP_ID_SYNTHETIC = 111;
const RelationshipId RELATIONSHIP_ID_OPTION = 113;
const RelationshipId RELATIONSHIP_ID_PRIMARY_LISTING = 114;
const RelationshipId RELATIONSHIP_ID_ANALYTICS = 115;
const RelationshipId RELATIONSHIP_ID_FUTURE_OPTION_EXPIRATION_ROOT = 116;
const RelationshipId RELATIONSHIP_ID_FUTURE = 117;
const RelationshipId RELATIONSHIP_ID_FOREX = 118;
const RelationshipId RELATIONSHIP_ID_FUNDAMENTAL = 119;
const RelationshipId RELATIONSHIP_ID_FUTURE_OPTION = 120;
const RelationshipId RELATIONSHIP_ID_UNDERLYING = 121;

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_FEED_RELATIONSHIP_IDS_H)
