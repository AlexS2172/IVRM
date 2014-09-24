/**
 *	@file	BaseException.h
 *
 *	@brief	Header file for the base exception class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_EXCEPTION_H)
#define ACTIV_BASE_EXCEPTION_H

#include "ActivMiddleware/ActivBase/Exception.h"

/**
 *	@brief	The list of possible errors and their strings.
 *
 *	Note this list is converted to an enum using the ACTIV_DECLARE_ELEMENT_1 macro in Activ.h,
 *	and the strings are unpicked using ACTIV_DECLARE_ELEMENT_2.
 */
#define ACTIV_BASE_EXCEPTION_LIST(d)														\
	d(ERROR_INVALID_REGISTRY_VALUE_TYPE, "invalid registry value type")						\
	d(ERROR_FAILED_TO_ALLOC_SHARED_MUTEX, "failed to allocate shared mutex")				\
	d(ERROR_FAILED_TO_ALLOC_FAST_SHARED_MUTEX, "failed to allocate fast shared mutex")		\
	d(ERROR_FAILED_TO_ALLOC_PRIVATE_MUTEX, "failed to allocate private mutex")				\
	d(ERROR_FAILED_TO_INIT_PRIVATE_MUTEX, "failed to initialize private mutex")				\
	d(ERROR_FAILED_TO_ALLOC_SHARED_EVENT, "failed to allocate shared event")				\
	d(ERROR_FAILED_TO_ALLOC_PRIVATE_EVENT, "failed to allocate private event")				\
	d(ERROR_FAILED_TO_ALLOC_SHARED_SEMAPHORE, "failed to allocate shared semaphore")		\
	d(ERROR_FAILED_TO_ALLOC_PRIVATE_SEMAPHORE, "failed to allocate private semaphore")		\
	d(ERROR_FAILED_TO_ALLOC_VERSION, "failed to allocate version")							\
	d(ERROR_FAILED_TO_ALLOC_SHARED_MEMORY, "failed to allocate shared memory")				\
	d(ERROR_FAILED_TO_ALLOC_METERED_SECTION, "failed to allocate metered section")			\
	d(ERROR_FAILED_TO_ALLOC_UUID, "failed to allocate Uuid")								\
	d(ERROR_THREAD_ALREADY_STARTED, "thread already started")

namespace Activ
{

/**
 *	@brief	ActivBase exception class.
 *
 *  An exception of class BaseException is thrown only by classes inside ActivBase.
 */
class BaseException : public Exception
{
public:
	/**
	 *	@brief	Enumeration of all BaseException errors.
	 */
	enum
	{
		ACTIV_BASE_EXCEPTION_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_ERRORS
	};

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	file the file which raised the exception.
	 *	@param	function the function which raised the exception.
	 *	@param	line the line number which raised the exception.
	 *	@param	errorCode the exception specific error code.
	 */
	ACTIV_BASE_API BaseException(const char *file, const char *function, const uint_fast32_t line, const uint_fast32_t errorCode);

	ACTIV_BASE_API virtual std::string ToString() const;
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_EXCEPTION_H)
