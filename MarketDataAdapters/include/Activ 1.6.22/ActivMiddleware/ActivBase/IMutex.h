/**
 *  @file	IMutex.h
 *
 *	@brief	Header file for the mutex interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IMUTEX_H)
#define ACTIV_IMUTEX_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Mutex interface class.
 */
class IMutex
{
public:
    /**
     *	@brief	Destructor.
     */
	virtual ~IMutex() = 0;

	/**
     *	@brief	Try to obtain the mutex lock.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
     */
	virtual StatusCode Try() const = 0;
	
    /**
     *	@brief	Acquire the mutex lock.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_SUPPORTED
     */
	virtual StatusCode Acquire() const = 0;

    /**
     *	@brief	Release the mutex lock.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
     */
	virtual StatusCode Release() const = 0;

	/**
	 *	@brief	Get approximate contention count for the mutex.
	 *
	 *	The count is approximate as it doesn't use an atomic counter - if multiple threads attempt to lock
	 *	the mutex at the "same" time and the mutex is already locked, the counter may not be incremented correctly.
	 *
	 *	Note not all mutex implementations may provide this value, in which case this function will return 0. For
	 *	mutexes that do, the value is incremented if Try() fails or Acquire() blocks.
	 *
	 *	This is provided for informational purposes only.
	 *
	 *	@return	approximate contention count.
	 */
	virtual size_t GetContentionCount() const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IMutex::~IMutex()
{
}

} // namespace Activ

#endif // !defined (ACTIV_IMUTEX_H)
