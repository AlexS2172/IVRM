/**
 *	@file	IpException.h
 *
 *	@brief	Header file for the Ip exception class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_EXCEPTION_H)
#define ACTIV_IP_EXCEPTION_H

#include "ActivMiddleware/ActivIp/External.h"
#include "ActivMiddleware/ActivBase/Exception.h"

namespace Activ
{

/**
 *	@brief	The list of possible errors and their strings.
 *
 *	Note this list is converted to an enum using the ACTIV_DECLARE_ELEMENT_1 macro in Activ.h,
 *	and the strings are unpicked using ACTIV_DECLARE_ELEMENT_2.
 */
#define ACTIV_IP_EXCEPTION_LIST(d)														\
	d(ERROR_UNACCEPTABLE_WINSOCK_VERSION, "unacceptable winsock version")				\
	d(ERROR_INVALID_FUNCTION_POINTER_LENGTH, "invalid function pointer length")			\
	d(ERROR_IN_WRONG_STATE, "in wrong state to perform requested operation")			\
	d(ERROR_MULTIPLE_CLOSES_PERFORMED, "multiple closes performed on socket")			\
	d(ERROR_ALREADY_LISTENING, "already listening")										\
	d(ERROR_NOT_LISTENING, "not listening")												\
	d(ERROR_RX_BUFFER_SIZE_NOT_SET, "rx buffer size not set correctly")					\
	d(ERROR_TX_BUFFER_SIZE_NOT_SET, "tx buffer size not set correctly")					\
	d(ERROR_MANAGED_OBJECTS_STILL_EXIST, "managed objects still exist")					\
	d(ERROR_ADDRESS_FAMILIES_DO_NOT_MATCH, "address families do not match")				\
	d(ERROR_INVALID_ADDRESS_FAMILY, "invalid address family")							\
	d(ERROR_INTERFACE_INDEX_MISMATCH, "interface index mismatch")						\
	d(ERROR_FAILED_TO_POST_READ, "failed to post read")

/**
 *	@brief	Ip exception class.
 */
class ACTIV_IP_API IpException : public Exception
{
public:
	/**
	 *	@brief	Enumeration of all IpException errors.
	 */
	enum
	{
		ACTIV_IP_EXCEPTION_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_ERRORS,
	};

	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	file the file which raised the exception.
	 *	@param	function the function which raised the exception.
	 *	@param	line the line number which raised the exception.
	 *	@param	errorCode the exception specific error code.
	 */
	IpException(const char *file, const char *function, const uint_fast32_t line, const uint_fast32_t errorCode);

	std::string ToString() const;
};

} // namespace Activ

#endif // !defined (ACTIV_IP_EXCEPTION_H)
