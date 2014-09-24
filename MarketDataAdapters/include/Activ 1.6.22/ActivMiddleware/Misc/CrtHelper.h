/**
 *  @file	CrtHelper.h
 *
 *	@brief	Work arounds for crt disparities across platforms.
 *
 *	$Log: $
 */

#if (!defined ACTIV_CRT_HELPER_H)
#define ACTIV_CRT_HELPER_H

#include "ActivMiddleware/Activ.h"

#include <time.h>

namespace Activ
{

#if (defined ACTIV_PLATFORM_WINDOWS)

/**
 *	@brief	Convert a time value and correct for the local time zone.
 *
 *	@param	pTime pointer to time to convert (e.g. from time()).
 *	@param	pTm pointer to tm structure to receive converted time.
 *
 *	@return	pTm.
 */
inline tm *localtime_r(const time_t *pTime, tm *pTm)
{
	*pTm = *::localtime(pTime);

	return pTm;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Convert a time value and store as UTC.
 *
 *	@param	pTime pointer to time to convert (e.g. from time()).
 *	@param	pTm pointer to tm structure to receive converted time.
 *
 *	@return	pTm.
 */
inline tm *gmtime_r(const time_t *pTime, tm *pTm)
{
	*pTm = *::gmtime(pTime);

	return pTm;
}

#elif (defined ACTIV_PLATFORM_POSIX)

using ::localtime_r;
using ::gmtime_r;

#else
#error Configure for this platform
#endif

} // namespace Activ

#endif // (!defined ACTIV_CRT_HELPER_H)
