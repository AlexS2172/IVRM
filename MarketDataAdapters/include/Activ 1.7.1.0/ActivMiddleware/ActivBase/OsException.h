/**
 *	@file	OsException.h
 *
 *	@brief	Header file for the os dependant exception class
 *
 *  Under Windows, OS exception encapsulates Windows error codes (see GetLastError()).
 *	Under Unix, the OS exception class is equivalent to CrtException.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_OS_EXCEPTION_H)
#define ACTIV_BASE_OS_EXCEPTION_H

#include "ActivMiddleware/ActivBase/Exception.h"

#if (!defined ACTIV_PLATFORM_WINDOWS)
#include "ActivMiddleware/ActivBase/CrtException.h"
namespace Activ
{
typedef CrtException OsException;
}
#else

namespace Activ
{

/**
 *	@brief	OS exception class
 */
class OsException : public Exception
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	file the file which raised the exception.
	 *	@param	function the function which raised the exception.
	 *	@param	line the line number which raised the exception.
	 *	@param	errorCode the exception specific error code.
	 */
	ACTIV_BASE_API OsException(const char *file, const char *function, uint_fast32_t line, const uint_fast32_t errorCode);

	ACTIV_BASE_API virtual std::string ToString() const;
};

} // namespace Activ

#endif // (defined ACTIV_PLATFORM_WINDOWS)

#endif // (!defined ACTIV_BASE_OS_EXCEPTION_H)
