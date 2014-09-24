/**
 *	@file	SystemException.h
 *
 *	@brief	Header file for ActivSystem exception class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_EXCEPTION_H)
#define ACTIV_SYSTEM_EXCEPTION_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivBase/Exception.h"

/**
 *	@brief	The list of possible errors and their strings.
 *
 *	Note this list is converted to an enum using the ACTIV_DECLARE_ELEMENT_1 macro in Activ.h,
 *	and the strings are unpicked using ACTIV_DECLARE_ELEMENT_2.
 */
#define ACTIV_SYSTEM_EXCEPTION_LIST(d)														\
	d(ERROR_FAILED_TO_ALLOC_MESSAGE_ARRAY, "failed to allocate message array")				\
	d(ERROR_FAILED_TO_ALLOC_SYSTEM, "failed to allocate system")							\
	d(ERROR_FAILED_TO_ALLOC_PROCESS, "failed to allocate process")							\
	d(ERROR_FAILED_TO_ALLOC_PROCESS_TABLE, "failed to allocate process table")				\
	d(ERROR_PROCESS_DOES_NOT_EXIST, "process does not exist")								\
	d(ERROR_FAILED_TO_ALLOC_ENDPOINT, "failed to allocate endpoint")						\
	d(ERROR_FAILED_TO_ALLOC_ENDPOINT_TABLE, "failed to allocate endpoint table")			\
	d(ERROR_ENDPOINT_DOES_NOT_EXIST, "endpoint does not exist")								\
	d(ERROR_FAILED_TO_ALLOC_HEAP, "failed to allocate heap")								\
	d(ERROR_FAILED_TO_ALLOC_HEAP_TABLE, "failed to allocate heap table")					\
	d(ERROR_HEAP_DOES_NOT_EXIST, "heap does not exist")										\
	d(ERROR_INVALID_NUMBER_OF_BUCKETS, "invalid number of buckets")							\
	d(ERROR_INVALID_BLOCK_SIZE, "invalid block size (probably 0)")							\
	d(ERROR_INVALID_HEAP_ID, "invalid heap id")												\
	d(ERROR_INVALID_ENDPOINT_ID, "invalid endpoint id")										\
	d(ERROR_INVALID_PROCESS_ID, "invalid process id")										\
	d(ERROR_PORT_IN_USE, "port in use")														\
	d(ERROR_INVALID_PROCESS_STATE, "invalid process state")									\
	d(ERROR_UNKNOWN_SYSTEM_LOG_MODE, "unknown system log mode")								\
	d(ERROR_INVALID_MEMORY_MODEL, "invalid memory model")

namespace Activ
{

/**
 *	@brief	ActivSystem exception class.
 *
 *  An exception of class SystemException is thrown only by classes inside ActivSystem.
 */
class SystemException : public Exception
{
public:
	/**
	 *  @brief	Enumeration of all SystemException errors.
	 */
	enum
	{
		ACTIV_SYSTEM_EXCEPTION_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_ERRORS,
	};

	/**
	 *  @brief	Default constructor.
	 *
	 *  @param	file the file which raised the exception
	 *	@param	function the function which raised the exception
	 *	@param	line the line number which raised the exception
	 *	@param	errorCode the exception specific error code
	 */
	ACTIV_SYSTEM_API SystemException(const char *file, const char *function, const uint_fast32_t line, const uint_fast32_t errorCode);

	ACTIV_SYSTEM_API std::string ToString() const;
};

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_EXCEPTION_H)
