/**
 *	@file	PermissionLevels.h
 *
 *	@brief	Header file for the permission levels.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_PERMISSION_LEVELS_H)
#define ACTIV_FEED_PERMISSION_LEVELS_H

#include "ActivMiddleware/Activ.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

namespace Activ
{

namespace Feed
{

const PermissionLevel PERMISSION_LEVEL_DEFAULT = ~0;

const PermissionLevel PERMISSION_LEVEL_REALTIME = 0;
const PermissionLevel PERMISSION_LEVEL_DELAYED = 1;
const PermissionLevel PERMISSION_LEVEL_COUNT = 2;

/**
 *	Union of permission level data.
 */
union PermissionLevelData
{
	byte_t	m_delayPeriod;								///< for PERMISSION_LEVEL_DELAYED, delay period in minutes
};

} // namespace Feed

} // namespace Activ

#endif // !defined (ACTIV_FEED_PERMISSION_LEVELS_H)
