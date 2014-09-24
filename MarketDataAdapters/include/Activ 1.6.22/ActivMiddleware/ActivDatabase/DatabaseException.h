/**
 *	@file	DatabaseException.h
 *
 *	@brief	Header file for the database exception class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_DATABASE_EXCEPTION_H)
#define ACTIV_DATABASE_DATABASE_EXCEPTION_H

#include "ActivMiddleware/Activ.h"

#include "ActivMiddleware/ActivBase/Exception.h"

#include "ActivMiddleware/ActivDatabase/External.h"

#include <string>

namespace Activ
{

namespace Database
{

/**
 *	@brief	The list of database errors.
 */
#define ACTIV_DATABASE_EXCEPTION_LIST(d)														\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_DATA_STORE, "failed to allocate data store")				\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_DATA_STORE_CACHE, "failed to allocate data store cache")	\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_DATABASE, "failed to allocate database")					\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_FIELD_LIST, "failed to allocate field list")				\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_INDEX, "failed to allocate index")							\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_KEY, "failed to allocate key")								\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_MEMORY_MANAGER, "failed to allocate memory manager")		\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_RECORD, "failed to allocate record")						\
	d(DATABASE_ERROR_FAILED_TO_ALLOC_TABLE, "failed to allocate table")							\
	d(DATABASE_ERROR_FAILED_TO_CREATE_FILE, "failed to create file")							\
	d(DATABASE_ERROR_NOT_CREATED, "not created")												\
	d(DATABASE_ERROR_NOT_SUPPORTED, "not supported")											\
	d(DATABASE_ERROR_INVALID_PARAMETER, "invalid parameter")									\
	d(DATABASE_ERROR_ALREADY_CREATED, "already created")

/**
 *	@brief	Database exception class.
 */
class DatabaseException : public Exception
{
public:
	/**
	 *	@brief	Enumeration of all database exception errors.
	 */
	enum Errors
	{
		ACTIV_DATABASE_EXCEPTION_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUMBER_OF_ERRORS
	};

	/**
	 *	@brief	Constructor that takes an error code.
	 *
	 *	@param	file the file which raised the exception.
	 *	@param	function the function which raised the exception.
	 *	@param	line the line number which raised the exception.
	 *	@param	errorCode the exception specific error code.
	 */
	ACTIV_DATABASE_API DatabaseException(const char file[], const char function[], const uint_fast32_t line, const uint_fast32_t errorCode);

	/**
	 *	@brief	Constructor that takes an error code and error string.
	 *
	 *	@param	file the file which raised the exception.
	 *	@param	function the function which raised the exception.
	 *	@param	line the line number which raised the exception.
	 *	@param	errorCode the exception specific error code.
	 *	@param	format pointer to a sprintf-style format string.
	 *	@param	... variable arguments for the format string.
	 */
	ACTIV_DATABASE_API DatabaseException(const char file[], const char function[], const uint_fast32_t line, const uint_fast32_t errorCode, const char format[], ...);

	/**
	 *	@brief	Convert the exception error code to a C-style string.
	 *
	 *	@return	the error code as a string.
	 */
	ACTIV_DATABASE_API virtual std::string ToString() const;

private:
	std::string m_errorString;		///< String containing extra error information.
};

}	// namespace Database

}	// namespace Activ

#endif // !defined (ACTIV_DATABASE_DATABASE_EXCEPTION_H)
