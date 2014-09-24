/**
 *	@file	Uuid.h
 *
 *	@brief	Header file for the Uuid class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_UUID_H)
#define ACTIV_BASE_UUID_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/BaseException.h"

#if (defined ACTIV_PLATFORM_WINDOWS)
#include <rpc.h>
#elif (defined ACTIV_PLATFORM_LINUX) || (defined ACTIV_PLATFORM_DARWIN) || (defined ACTIV_PLATFORM_SOLARIS)
#include <uuid/uuid.h>
#elif (defined ACTIV_PLATFORM_BSD)
#include <uuid.h>
#else
#error Implement for this platform
#endif

#include <string>
#include <algorithm>

namespace Activ
{

/**
 *	@brief	Uuid class.
 */
class Uuid
{
public:
#if (defined ACTIV_PLATFORM_WINDOWS)
	static const size_t UUID_SIZE = sizeof(UUID);				///< size of the binary representation of a uuid
#else
	static const size_t UUID_SIZE = sizeof(uuid_t);
#endif

	/**
	 *	@brief	Default constructor that initializes with a new uuid.
	 *
	 *	@throw	BaseException
	 */
	ACTIV_BASE_API Uuid();

	/**
	 *	@brief	Constructor that initializes the uuid from a binary representation.
	 *
	 *	@param	pBuffer buffer containing the binary representation.
	 *	@param	bufferSize the size of the buffer.
	 *
	 *	@throw	BaseException
	 *	@throw	StatusCodeException
	 */
	ACTIV_BASE_API Uuid(const byte_t *pBuffer, const size_t bufferSize);

	/**
	 *	@brief	Constructor that initializes the uuid from a string representation.
	 *
	 *	@param	uuidString string representation of the uuid.
	 *
	 *	@throw	BaseException
	 *	@throw	StatusCodeException
	 */
	ACTIV_BASE_API Uuid(std::string &uuidString);

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_BASE_API bool operator==(const Uuid &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	BaseException
	 */
	ACTIV_BASE_API bool operator!=(const Uuid &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	BaseException
	 */
	ACTIV_BASE_API bool operator<(const Uuid &rhs) const;

	/**
	 *	@brief	Less than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	BaseException
	 */
	ACTIV_BASE_API bool operator<=(const Uuid &rhs) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	BaseException
	 */
	ACTIV_BASE_API bool operator>(const Uuid &rhs) const;

	/**
	 *	@brief	Greater than or equal to operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	BaseException
	 */
	ACTIV_BASE_API bool operator>=(const Uuid &rhs) const;

	/**
	 *	@brief	Set the value of the uuid from a binary representation.
	 *
	 *	@param	pBuffer buffer containing the binary representation.
	 *	@param	bufferSize the size of the buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode Set(const byte_t *pBuffer, const size_t bufferSize);

	/**
	 *	@brief	Set the value of the uuid from a string representation.
	 *
	 *	@param	uuidString string representation of the Uuid.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode Set(const std::string &uuidString);

	/**
	 *	@brief	Get a binary representation of the Uuid.
	 *
	 *	@param	pBuffer buffer for the binary representation.
	 *	@param	bufferSize the size of the buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 */
	ACTIV_BASE_API StatusCode GetBinary(byte_t *pBuffer, const size_t bufferSize) const;

	/**
	 *	@brief	Get a string representation of the Uuid.
	 *
	 *	@return	string representation of the Uuid.
	 */
	ACTIV_BASE_API std::string ToString() const;

	ACTIV_BASE_API static const Uuid emptyUuid;			///< Empty uuid object.

private:
	std::string	m_uuidString;

#if (defined ACTIV_PLATFORM_WINDOWS)
	UUID		m_uuid;
#else
	uuid_t		m_uuid;
#endif
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_UUID_H)
