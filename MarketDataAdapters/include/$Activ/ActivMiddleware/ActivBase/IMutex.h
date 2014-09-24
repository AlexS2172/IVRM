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
class ACTIV_BASE_API IMutex
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
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IMutex::~IMutex()
{
}

} // namespace Activ

#endif // !defined (ACTIV_IMUTEX_H)
