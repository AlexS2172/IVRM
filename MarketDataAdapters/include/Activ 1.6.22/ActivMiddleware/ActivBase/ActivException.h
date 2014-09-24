/**
 *	@file	ActivException.h
 *
 *	@brief	Header file for the Activ exception class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_ACTIV_EXCEPTION_H)
#define ACTIV_ACTIV_EXCEPTION_H

#include "ActivMiddleware/ActivBase/Exception.h"

namespace Activ
{

/**
 *	@brief	Throw result if result of expression tests true using operator against statusCode
 */
#define ACTIV_THROW_RESULT_IF(statusCode, operator, expression)														\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if (statusCode operator thisIsTheResultOfTheCall)															\
			ACTIV_THROW(Activ::ActivException, thisIsTheResultOfTheCall);											\
	}																												\
	else

/**
 *	@brief	Throw result if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_THROW_RESULT_IF_AND_IF(statusCode1, statusCode2, operator, expression)								\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((statusCode1 operator thisIsTheResultOfTheCall) && (statusCode2 operator thisIsTheResultOfTheCall))		\
			ACTIV_THROW(Activ::ActivException, thisIsTheResultOfTheCall);											\
	}																												\
	else

/**
 *	@brief	Throw result if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_THROW_RESULT_IF_OR_IF(statusCode1, statusCode2, operator, expression)									\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((statusCode1 operator thisIsTheResultOfTheCall) || (statusCode2 operator thisIsTheResultOfTheCall))		\
			ACTIV_THROW(Activ::ActivException, thisIsTheResultOfTheCall);											\
	}																												\
	else

/**
 *	@brief	Activ exception class.
 */
class ActivException : public Exception
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
	ACTIV_BASE_API ActivException(const char *file, const char *function, const uint_fast32_t line, const uint_fast32_t errorCode);

	ACTIV_BASE_API virtual std::string ToString() const;
};

} // namespace Activ

#endif // !defined (ACTIV_ACTIV_EXCEPTION_H)
