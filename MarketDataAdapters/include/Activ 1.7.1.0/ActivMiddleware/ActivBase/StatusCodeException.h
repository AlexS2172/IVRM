/**
 *	@file	StatusCodeException.h
 *
 *	@brief	Header file for the StatusCode exception class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_STATUS_CODE_EXCEPTION_H)
#define ACTIV_BASE_STATUS_CODE_EXCEPTION_H

#include "ActivMiddleware/ActivBase/Exception.h"

namespace Activ
{

/**
 *	@brief	Throw result if result of expression tests true using operator against statusCode.
 */
#define ACTIV_THROW_RESULT_IF(statusCode, operator, expression)														\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if (statusCode operator thisIsTheResultOfTheCall)															\
			ACTIV_THROW(Activ::StatusCodeException, thisIsTheResultOfTheCall);											\
	}																												\
	else

/**
 *	@brief	Throw result if result of expression tests true using operator against statusCode1 and statusCode2.
 */
#define ACTIV_THROW_RESULT_IF_AND_IF(statusCode1, statusCode2, operator, expression)								\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((statusCode1 operator thisIsTheResultOfTheCall) && (statusCode2 operator thisIsTheResultOfTheCall))		\
			ACTIV_THROW(Activ::StatusCodeException, thisIsTheResultOfTheCall);											\
	}																												\
	else

/**
 *	@brief	Throw result if result of expression tests true using operator against statusCode1 or statusCode2.
 */
#define ACTIV_THROW_RESULT_IF_OR_IF(statusCode1, statusCode2, operator, expression)									\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((statusCode1 operator thisIsTheResultOfTheCall) || (statusCode2 operator thisIsTheResultOfTheCall))		\
			ACTIV_THROW(Activ::StatusCodeException, thisIsTheResultOfTheCall);											\
	}																												\
	else

/**
 *	@brief	StatusCode exception class.
 */
class StatusCodeException : public Exception
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	file the file which raised the exception.
	 *	@param	function the function which raised the exception.
	 *	@param	line the line number which raised the exception.
	 *	@param	errorCode the exception specific error code, in this case a StatusCode value.
	 */
	ACTIV_BASE_API StatusCodeException(const char *file, const char *function, const uint_fast32_t line, const uint_fast32_t errorCode);

	ACTIV_BASE_API virtual std::string ToString() const;
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_STATUS_CODE_EXCEPTION_H)
