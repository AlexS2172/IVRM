/**
 *	@file	UpdateIds.h
 *
 *	@brief	Header file for the update ids.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_UPDATE_IDS_H)
#define ACTIV_FEED_UPDATE_IDS_H

#include "ActivMiddleware/Activ.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

namespace Activ
{

namespace ContentPlatform
{

namespace Feed
{

const UpdateId UPDATE_ID_BIT_RESET			= 0;																	///< The reset bit of an update id.

const UpdateId UPDATE_ID_BITMASK_RESET		= (1 << ((sizeof(UpdateId) * CHAR_BIT) - (UPDATE_ID_BIT_RESET + 1)));	///< The update id reset bitmask.
const UpdateId UPDATE_ID_BITMASK_COUNT		= static_cast<UpdateId>(~UPDATE_ID_BITMASK_RESET);						///< The update id count bitmask.

const UpdateId UPDATE_ID_COUNT_NEW_RECORD	= 0;																	///< The new record update id count .
const UpdateId UPDATE_ID_COUNT_UNKNOWN		= static_cast<UpdateId>(~0) ^ UPDATE_ID_BITMASK_RESET;					///< The unknown update id count.
const UpdateId UPDATE_ID_COUNT_MAXIMUM		= UPDATE_ID_COUNT_UNKNOWN;												///< The maximum allowed update id count.

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get the count of the update id.
 *
 *	@param	updateId the update id.
 *
 *	@return	the count of the update id.
 */
inline UpdateId GetUpdateIdCount(const UpdateId updateId)
{
	return (UPDATE_ID_BITMASK_COUNT & updateId);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get an incremented version of the update id.
 *
 *	@param	updateId the update id.
 *
 *	@return	the incremented version of the update id.
 */
inline UpdateId GetIncrementedUpdateId(const UpdateId updateId)
{
	UpdateId updateCount = GetUpdateIdCount(updateId);

	return ((UPDATE_ID_BITMASK_RESET & updateId) | ((UPDATE_ID_COUNT_MAXIMUM == ++updateCount) ? 1 : updateCount));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get a reset update id.
 *
 *	@param	updateId the update id.
 *
 *	@return	the reset update id.
 */
inline UpdateId GetResetUpdateId(const UpdateId updateId)
{
	return (UPDATE_ID_BITMASK_RESET ^ GetIncrementedUpdateId(updateId));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get the new record update id.
 *
 *	@return	the new record update id.
 */
inline UpdateId GetNewRecordUpdateId()
{
	return UPDATE_ID_COUNT_NEW_RECORD;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Get the unknown update id.
 *
 *	@return	the unknown update id.
 */
inline UpdateId GetUnknownUpdateId()
{
	return UPDATE_ID_COUNT_UNKNOWN;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Is the resit bit set in the update id.
 *
 *	@param	updateId the update id.
 *
 *	@return	whether the reset bit is set.
 */
inline bool IsUpdateIdResetBitSet(const UpdateId updateId)
{
	return (0 != (UPDATE_ID_BITMASK_RESET & updateId));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Is the value the new record update id.
 *
 *	@param	updateId the update id.
 *
 *	@return	whether the value is the new record update id.
 */
inline bool IsUpdateIdNewRecord(const UpdateId updateId)
{
	return (UPDATE_ID_COUNT_NEW_RECORD == GetUpdateIdCount(updateId));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Is the value the unknown update id.
 *
 *	@param	updateId the update id.
 *
 *	@return	whether the value is the unknown update id.
 */
inline bool IsUpdateIdUnknown(const UpdateId updateId)
{
	return (UPDATE_ID_COUNT_UNKNOWN == GetUpdateIdCount(updateId));
}

// ---------------------------------------------------------------------------------------------------------------------------------

} // namespace Feed

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_FEED_UPDATE_IDS_H)
