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

namespace ContentPlatform
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

/**
 *	@brief	Convert a permission level to a string.
 *
 *	@param	permisionLevel the permission level.
 *
 *	@return	the string corresponding to the permission level.
 */
inline std::string PermissionLevelToString(const PermissionLevel permissionLevel)
{
	switch (permissionLevel)
	{
		case PERMISSION_LEVEL_DEFAULT:
			return "PERMISSION_LEVEL_DEFAULT";

		case PERMISSION_LEVEL_REALTIME:
			return "PERMISSION_LEVEL_REALTIME";

		case PERMISSION_LEVEL_DELAYED:
			return "PERMISSION_LEVEL_DELAYED";
	}

	return "Unknown";
}

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_FEED_PERMISSION_LEVELS_H)
