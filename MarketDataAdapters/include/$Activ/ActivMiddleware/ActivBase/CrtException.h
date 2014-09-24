/**
 *	@file	CrtException.h
 *
 *	@brief	Header file for c runtime exception class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CRT_EXCEPTION_H)
#define ACTIV_CRT_EXCEPTION_H

#include "ActivMiddleware/ActivBase/Exception.h"

namespace Activ
{

/**
 *	@brief	C runtime exception class.
 *
 *  CrtException encapsulates errors from c run time library calls.
 */
class ACTIV_BASE_API CrtException : public Exception
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
	CrtException(const char *file, const char *function, uint_fast32_t line, const uint_fast32_t errorCode);

	std::string ToString() const;
};

} // namespace Activ

#endif // !defined (ACTIV_CRT_EXCEPTION_H)
