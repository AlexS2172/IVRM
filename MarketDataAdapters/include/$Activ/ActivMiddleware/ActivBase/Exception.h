/**
 *	@file	Exception.h
 *
 *	@brief	Header file for the exception class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_EXCEPTION_H)
#define ACTIV_EXCEPTION_H

#include "ActivMiddleware/ActivBase/External.h"

#include <iostream>

/**
 *	@brief	Construct an exception.
 */
#define ACTIV_EXCEPTION(t, e)			t(__FILE__, __FUNCTION__, __LINE__, e)

/**
 *	@brief	Raise an exception.
 *
 *	Throw an exception of type t with error code e.
 *	The file, function and line number throwing the exception are automatically generated.
 */
#define ACTIV_THROW(t, e)				throw ACTIV_EXCEPTION(t, e)

/**
 *	@brief	Rethrow an exception.
 */
#define ACTIV_RETHROW(e)				if (true)																\
										{																		\
											e.AddException(new (std::nothrow) ACTIV_EXCEPTION(Exception, 0));	\
											throw;																\
										}																		\
										else

/**
 *	@brief	Throw exception if result of expression tests true using operator against statusCode
 */
#define ACTIV_THROW_ERROR_IF(statusCode, operator, expression, exception)			\
	if (true)																		\
	{																				\
		if (statusCode operator expression)											\
			throw exception;														\
	}																				\
	else

// ---------------------------------------------------------------------------------------------------------------------------------

namespace Activ
{

/**
 *	@brief	Base Activ exception class.
 */
class ACTIV_BASE_API Exception
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
	Exception(const char *file, const char *function, const uint_fast32_t line, const uint_fast32_t errorCode = 0);

	/**
	 *	@brief	Copy constructor.
	 */
	Exception(const Exception &rhs);

	/**
	 *	@brief	Virtual destructor.
	 */
	virtual ~Exception();

	/**
	 *	@brief	Convert the exception error code to a string.
	 *
	 *	Classes derived from Exception are required to implement ToString() to convert
	 *	their error codes to a readable string.
	 *
	 *	@return	the error code as a string.
	 */
	virtual std::string ToString() const;

	/**
	 *	@brief	Stream output method.
	 *
	 *	ToStream() allows writing of the exception to an ostream object. The file, function, line,
	 *	error code and error string will be written to the ostream.
	 *
	 *	@param	os the ostream to write the exception to.
	 */
	virtual void ToStream(std::ostream &os) const;

	/**
	 *	@brief	Attach another exception to the chain.
	 *
	 *	@param	pException the exception to add to the chain.
	 */
	void AddException(Exception *pException) const;

	const char * const		m_file;					///< The file that raised the exception.
	const char * const		m_function;				///< The function that raised the excetpion.
	const uint_fast32_t		m_line;					///< The line number that raised the exception.
	const uint_fast32_t		m_errorCode;			///< The exception specific error code.

private:
	/**
	 *	@brief	Display the exception history chain.
	 */
	void DisplayChain(std::ostream &os) const;

	mutable const Exception	*m_pNextException;		///< Can only be a heap allocated exception.
};

} // namespace Activ

/**
 *	@brief	Global operator<< to dump an exception to an ostream.
 *
 *	operator<< allows writing of the exception to an ostream object. The file, function, line,
 *	error code and error string will be written to the ostream.
 *
 *	@param	os the ostream to write the exception to.
 *	@param	e the exception to write.
 *
 *	@return	the ostream \a os.
 */
ACTIV_BASE_API std::ostream &operator<<(std::ostream &os, const Activ::Exception &e);

#endif // !defined (ACTIV_EXCEPTION_H)
