/**
 *	@file	StatusCodes.h
 *
 *	@brief	Status codes and helper macros.
 *
 *	$Log: $
 */

#if !defined (ACTIV_STATUS_CODES_H)
#define ACTIV_STATUS_CODES_H

namespace Activ
{

/**
 *	@brief	Return if result of expression tests true using operator against statusCode
 */
#define ACTIV_RETURN_IF(statusCode, operator, expression)															\
	if (true)																										\
	{																												\
		if (expression operator statusCode)																			\
			return;																									\
	}																												\
	else

/**
 *	@brief	Return if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_RETURN_IF_AND_IF(statusCode1, statusCode2, operator, expression)										\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) && (thisIsTheResultOfTheCall operator statusCode2))		\
			return;																									\
	}																												\
	else

/**
 *	@brief	Return if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_RETURN_IF_OR_IF(statusCode1, statusCode2, operator, expression)										\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) || (thisIsTheResultOfTheCall operator statusCode2))		\
			return;																									\
	}																												\
	else

/**
 *	@brief	Return result if result of expression tests true using operator against statusCode
 */
#define ACTIV_RETURN_RESULT_IF(statusCode, operator, expression)													\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if (thisIsTheResultOfTheCall operator statusCode)															\
			return thisIsTheResultOfTheCall;																		\
	}																												\
	else

/**
 *	@brief	Return result if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_RETURN_RESULT_IF_AND_IF(statusCode1, statusCode2, operator, expression)								\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) && (thisIsTheResultOfTheCall operator statusCode2))		\
			return thisIsTheResultOfTheCall;																		\
	}																												\
	else

/**
 *	@brief	Return result if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_RETURN_RESULT_IF_OR_IF(statusCode1, statusCode2, operator, expression)								\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) || (thisIsTheResultOfTheCall operator statusCode2))		\
			return thisIsTheResultOfTheCall;																		\
	}																												\
	else

/**
 *	@brief	Return a value if result of expression tests true using operator against statusCode
 */
#define ACTIV_RETURN_VALUE_IF(statusCode, operator, expression, value)												\
	if (true)																										\
	{																												\
		if (expression operator statusCode)																			\
			return value;																							\
	}																												\
	else

/**
 *	@brief	Return a value if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_RETURN_VALUE_IF_AND_IF(statusCode1, statusCode2, operator, expression, value)							\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) && (thisIsTheResultOfTheCall operator statusCode2))		\
			return value;																							\
	}																												\
	else

/**
 *	@brief	Return a value if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_RETURN_VALUE_IF_OR_IF(statusCode1, statusCode2, operator, expression, value)							\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) || (thisIsTheResultOfTheCall operator statusCode2))		\
			return value;																							\
	}																												\
	else

/**
 *	@brief	Continue if result of expression tests true using operator against statusCode
 */
#define ACTIV_CONTINUE_IF(statusCode, operator, expression)															\
	if (true)																										\
	{																												\
		if (expression operator statusCode)																			\
			continue;																								\
	}																												\
	else

/**
 *	@brief	Continue if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_CONTINUE_IF_AND_IF(statusCode1, statusCode2, operator, expression)									\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) && (thisIsTheResultOfTheCall operator statusCode2))		\
			continue;																								\
	}																												\
	else

/**
 *	@brief	Continue if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_CONTINUE_IF_OR_IF(statusCode1, statusCode2, operator, expression)										\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) || (thisIsTheResultOfTheCall operator statusCode2))		\
			continue;																								\
	}																												\
	else

/**
 *	@brief	Continue if result of expression tests true using operator against statusCode
 */
#define ACTIV_BREAK_IF(statusCode, operator, expression)															\
	if (true)																										\
	{																												\
		if (expression operator statusCode)																			\
			break;																									\
	}																												\
	else

/**
 *	@brief	Continue if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_BREAK_IF_AND_IF(statusCode1, statusCode2, operator, expression)										\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) && (thisIsTheResultOfTheCall operator statusCode2))		\
			break;																									\
	}																												\
	else

/**
 *	@brief	Continue if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_BREAK_IF_OR_IF(statusCode1, statusCode2, operator, expression)										\
	if (true)																										\
	{																												\
		const Activ::StatusCode thisIsTheResultOfTheCall = expression;												\
		if ((thisIsTheResultOfTheCall operator statusCode1) || (thisIsTheResultOfTheCall operator statusCode2))		\
			break;																									\
	}																												\
	else

/**
 *	@brief	Return true if result of expression tests true using operator against statusCode
 */
#define ACTIV_RETURN_TRUE_IF(statusCode, operator, expression)							ACTIV_RETURN_VALUE_IF(statusCode, operator, expression, true)

/**
 *	@brief	Return true if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_RETURN_TRUE_IF_AND_IF(statusCode1, statusCode2, operator, expression)		ACTIV_RETURN_VALUE_IF_AND_IF(statusCode1, statusCode2, operator, expression, true)

/**
 *	@brief	Return true if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_RETURN_TRUE_IF_OR_IF(statusCode1, statusCode2, operator, expression)		ACTIV_RETURN_VALUE_IF_OR_IF(statusCode1, statusCode2, operator, expression, true)

/**
 *	@brief	Return false if result of expression tests true using operator against statusCode
 */
#define ACTIV_RETURN_FALSE_IF(statusCode, operator, expression)							ACTIV_RETURN_VALUE_IF(statusCode, operator, expression, false)

/**
 *	@brief	Return false if result of expression tests true using operator against statusCode1 and statusCode2
 */
#define ACTIV_RETURN_FALSE_IF_AND_IF(statusCode1, statusCode2, operator, expression)	ACTIV_RETURN_VALUE_IF_AND_IF(statusCode1, statusCode2, operator, expression, false)

/**
 *	@brief	Return false if result of expression tests true using operator against statusCode1 or statusCode2
 */
#define ACTIV_RETURN_FALSE_IF_OR_IF(statusCode1, statusCode2, operator, expression)		ACTIV_RETURN_VALUE_IF_OR_IF(statusCode1, statusCode2, operator, expression, false)

/**
 *	@brief	The list of possible status codes and their string representations.
 *
 *	These are network constant values.
 */
#define ACTIV_STATUS_CODE_LIST(d)											\
	d(0,	STATUS_CODE_SUCCESS, "success")									\
	d(1,	STATUS_CODE_FAILURE, "failure")									\
	d(2,	STATUS_CODE_EXIT, "exit")										\
	d(3,	STATUS_CODE_SHUTTING_DOWN, "shutting down")						\
	d(4,	STATUS_CODE_UNHANDLED_EXCEPTION, "unhandled exception")			\
	d(5,	STATUS_CODE_UNRECOGNIZED, "unrecognized")						\
	d(6,	STATUS_CODE_NOT_FOUND, "not found")								\
	d(7,	STATUS_CODE_TIMEOUT, "timeout")									\
	d(8,	STATUS_CODE_UNCHANGED, "unchanged")								\
	d(9,	STATUS_CODE_PENDING, "pending")									\
	d(10,	STATUS_CODE_ACCESS_DENIED, "access denied")						\
	d(11,	STATUS_CODE_BUFFER_OVERFLOW, "buffer overflow")					\
	d(12,	STATUS_CODE_BUFFER_UNDERRUN, "buffer underrun")					\
	d(13,	STATUS_CODE_NOT_SUPPORTED, "not supported")						\
	d(14,	STATUS_CODE_NOT_INITIALIZED, "not initialized")					\
	d(15,	STATUS_CODE_MISSING_ARGS, "missing args")						\
	d(16,	STATUS_CODE_NOT_OPEN, "not open")								\
	d(17,	STATUS_CODE_ALREADY_OPEN, "already open")						\
	d(18,	STATUS_CODE_DISABLED, "disabled")								\
	d(19,	STATUS_CODE_INVALID_ENDPOINT, "invalid endpoint")				\
	d(20,	STATUS_CODE_INVALID_PORT, "invalid port")						\
	d(21,	STATUS_CODE_INVALID_VERSION, "invalid version")					\
	d(22,	STATUS_CODE_INVALID_LENGTH, "invalid length")					\
	d(23,	STATUS_CODE_INVALID_PARAMETER, "invalid parameter")				\
	d(24,	STATUS_CODE_INVALID_NAME, "invalid name")						\
	d(25,	STATUS_CODE_INVALID_FORMAT, "invalid format")					\
	d(26,	STATUS_CODE_INVALID_INDEX, "invalid index")						\
	d(27,	STATUS_CODE_INVALID_KEY, "invalid key")							\
	d(28,	STATUS_CODE_INVALID_FIELD_TYPE, "invalid field type")			\
	d(29,	STATUS_CODE_INVALID_FIELD, "invalid field")						\
	d(30,	STATUS_CODE_INVALID_RECORD, "invalid record")					\
	d(31,	STATUS_CODE_INVALID_TABLE, "invalid table")						\
	d(32,	STATUS_CODE_INVALID_PROTOCOL, "invalid protocol")				\
	d(33,	STATUS_CODE_UNDEFINED_FIELD, "undefined field")					\
	d(34,	STATUS_CODE_NO_PRIMARY_KEY, "no primary key")					\
	d(35,	STATUS_CODE_MISMATCH, "mismatch")								\
	d(36,	STATUS_CODE_OUT_OF_RANGE, "out of range")						\
	d(37,	STATUS_CODE_STALE, "stale data")								\
	d(38,	STATUS_CODE_DELETED, "item deleted")							\
	d(39,	STATUS_CODE_EMPTY, "empty")										\
	d(40,	STATUS_CODE_FULL, "full")										\
	d(41,	STATUS_CODE_HEAP_EMPTY, "heap empty")							\
	d(42,	STATUS_CODE_WRONG_STATE, "wrong state")							\
	d(43,	STATUS_CODE_NOT_CONNECTED, "not connected")						\
	d(44,	STATUS_CODE_ALREADY_CONNECTED, "already connected")				\
	d(45,	STATUS_CODE_ALREADY_DEFINED, "already defined")					\
	d(46,	STATUS_CODE_NOT_CREATED, "not created")							\
	d(47,	STATUS_CODE_ALREADY_EXISTS, "already exists")					\
	d(48,	STATUS_CODE_IN_USE, "in use")									\
	d(49,	STATUS_CODE_END_OF_DATA, "end of data")							\
	d(50,	STATUS_CODE_ALREADY_LISTENING, "already listening")				\
	d(51,	STATUS_CODE_NOT_SHAREABLE, "not shareable")						\
	d(52,	STATUS_CODE_CONNECTION_REFUSED, "connection refused")			\
	d(53,	STATUS_CODE_INTERRUPTED, "interrupted")							\
	d(54,	STATUS_CODE_IGNORED, "ignored")									\
	d(55,	STATUS_CODE_TOO_MANY_REQUESTS, "too many requests outstanding")	\
	d(56,	STATUS_CODE_INVALID_ENDPOINT_PORT, "invalid endpoint-port")		\
	d(57,	STATUS_CODE_HOST_NOT_FOUND, "host not found")					\
	d(58,	STATUS_CODE_INVALID_USER, "invalid user")						\
	d(59,	STATUS_CODE_INVALID_PASSWORD, "invalid password")				\
	d(60,	STATUS_CODE_INVALID_URL, "invalid url")

/**
 *	@brief	Status code type.
 */
typedef byte_t StatusCode;

/**
 *	@brief	Macro to declare constant status code.
 */
#define ACTIV_DECLARE_STATUS_CODE(value, id, name)							\
	const StatusCode id = value;

/**
 *	@brief	Status code constants.
 */
ACTIV_STATUS_CODE_LIST(ACTIV_DECLARE_STATUS_CODE)

#undef ACTIV_DECLARE_STATUS_CODE

} // namespace Activ

#endif // !defined (ACTIV_STATUS_CODES_H)
