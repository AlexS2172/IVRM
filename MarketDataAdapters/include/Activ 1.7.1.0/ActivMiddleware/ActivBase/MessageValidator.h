/**
 *  @file	MessageValidator.h
 *
 *	@brief	Header file for the message validator class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_MESSAGE_VALIDATOR_H)
#define ACTIV_BASE_MESSAGE_VALIDATOR_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/MessageHandler.h"

#include <float.h>
#include <vector>
#include <string>
#include <limits>

namespace Activ
{

/**
 *	@brief	Message validator class.
 */
class MessageValidator : public MessageHandler
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	pMessage the address of the message.
	 *	@param	length the length of the message.
	 *	@param	messageVersion optional version.
	 */
	MessageValidator(const void *pMessage = 0, const size_t length = 0, const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Initialize with a new contiguous message.
	 *
	 *	@param	pMessage the address of the message.
	 *	@param	length the length of the message.
	 *	@param	messageVersion optional version.
	 */
	void Initialize(const void *pMessage, const size_t length, const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Reset.
	 *
	 *	@param	messageVersion optional version.
	 */
	void Reset(const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Get the address of the message.
	 *
	 *	@return	The address of the message.
	 */
	const void *GetMessage() const;

	/**
	 *	@brief	Get the length of the message.
	 *
	 *	@return	the length of the message.
	 */
	size_t GetLength() const;

	/**
	 *	@brief	Get the current offset within the message.
	 *
	 *	@return	The offset within the message.
	 */
	size_t GetOffset() const;

	/**
	 *	@brief	Set the current offset within the message.
	 *
	 *	@param	offset the offset within the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode SetOffset(const size_t offset);

	/**
	 *	@brief	Is the end of message.
	 *
	 *	@return	whether the end of message has been reached.
	 */
	bool IsEndOfMessage() const;

	/**
	 *	@brief	Validate bytes.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateBytes(const size_t length, const void **ppData = 0);

	/**
	 *	@brief	Validate bytes.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateBytes(const size_t length, const byte_t lower, const byte_t upper, const void **ppData = 0);

	/**
	 *	@brief	Validate bytes.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateBytes(const size_t length, const std::vector<byte_t> &set, const void **ppData = 0);

	/**
	 *	@brief	Validate bytes.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateBytes(const size_t length, const std::vector<std::string> &set, const void **ppData = 0);

	/**
	 *	@brief	Validate bytes and copy.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateBytesCopy(const size_t length, void *pCopy);

	/**
	 *	@brief	Validate bytes and copy.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateBytesCopy(const size_t length, const byte_t lower, const byte_t upper, void *pCopy);

	/**
	 *	@brief	Validate bytes and copy.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateBytesCopy(const size_t length, const std::vector<byte_t> &set, void *pCopy);

	/**
	 *	@brief	Validate bytes and copy.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateBytesCopy(const size_t length, const std::vector<std::string> &set, void *pCopy);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateAsciiHexadecimalIntegral(const size_t size, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	template <typename T> StatusCode ValidateAsciiHexadecimalIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateAsciiHexadecimalIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateAsciiHexadecimalIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateAsciiHexadecimalIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateAsciiHexadecimalIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateAsciiHexadecimalIntegralCopy(const size_t size, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	template <typename T> StatusCode ValidateAsciiHexadecimalIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateAsciiHexadecimalIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateAsciiHexadecimalIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateAsciiHexadecimalIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate an ASCII hexadecimal integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateAsciiHexadecimalIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiIntegral(const size_t length, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiIntegral(const size_t length, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiIntegral(const size_t length, const int64_t lower, const int64_t upper, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiIntegral(const size_t length, const int64_t lower, const int64_t upper, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedAsciiIntegral(const size_t length, const std::vector<int64_t> &set, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiIntegral(const size_t length, const std::vector<int64_t> &set, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, const int64_t lower, const int64_t upper, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, const int64_t lower, const int64_t upper, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, const std::vector<int64_t> &set, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, const std::vector<int64_t> &set, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiReal(const size_t length, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiReal(const size_t length, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiReal(const size_t length, int64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, int64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, int64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiRealCopy(const size_t length, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, int64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, int64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, int64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

	/**
	 *	@brief	Validate a signed binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateSignedBinaryIntegral(const size_t size, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	template <typename T> StatusCode ValidateSignedBinaryIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedBinaryIntegral(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedBinaryIntegral(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedBinaryIntegral(const size_t size, const std::vector<int64_t> &set, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedBinaryIntegral(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate a signed binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateSignedBinaryIntegralCopy(const size_t size, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate a signed binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	template <typename T> StatusCode ValidateSignedBinaryIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedBinaryIntegralCopy(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate a signed binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedBinaryIntegralCopy(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a signed binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateSignedBinaryIntegralCopy(const size_t size, const std::vector<int64_t> &set, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate a signed binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateSignedBinaryIntegralCopy(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode ValidateString(const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	pString the location to copy the string to.
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateString(std::string *pString, const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateString(const byte_t lower, const byte_t upper, const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pString the location to copy the string to.
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateString(const byte_t lower, const byte_t upper, std::string *pString, const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateString(const std::vector<byte_t> &set, const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	pString the location to copy the string to.
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateString(const std::vector<byte_t> &set, std::string *pString, const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateString(const std::vector<std::string> &set, const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	pString the location to copy the string to.
	 *	@param	ppData the location to copy the address of the data to.
	 *	@param	pDataLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateString(const std::vector<std::string> &set, std::string *pString, const void **ppData = 0, size_t *pDataLength = 0);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode ValidateStringCopy(void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	pString the location to copy the string to.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateStringCopy(std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateStringCopy(const byte_t lower, const byte_t upper, void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pString the location to copy the string to.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateStringCopy(const byte_t lower, const byte_t upper, std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateStringCopy(const std::vector<byte_t> &set, void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	pString the location to copy the string to.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateStringCopy(const std::vector<byte_t> &set, std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateStringCopy(const std::vector<std::string> &set, void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate string and copy.
	 *
	 *	@param	set the set of allowed values.
	 *	@param	pString the location to copy the string to.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	copySize the size of data.
	 *	@param	pCopyLength pointer to the length of the data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateStringCopy(const std::vector<std::string> &set, std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

	/**
	 *	@brief	Validate an unsigned ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiIntegral(const size_t length, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiIntegral(const size_t length, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedAsciiIntegral(const size_t length, const uint64_t lower, const uint64_t upper, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiIntegral(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedAsciiIntegral(const size_t length, const std::vector<uint64_t> &set, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII integral value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiIntegral(const size_t length, const std::vector<uint64_t> &set, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pCopy the address to copy the data to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, const uint64_t lower, const uint64_t upper, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, const std::vector<uint64_t> &set, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII integral value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, const std::vector<uint64_t> &set, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiReal(const size_t length, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiReal(const size_t length, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiReal(const size_t length, uint64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, uint64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, uint64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, uint64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, uint64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned ASCII real value and copy.
	 *
	 *	@param	length the length of the value.
	 *	@param	set the set of allowed values.
	 *	@param	pNumerator the location to copy the numerator to.
	 *	@param	pDivisor the location to copy the divisor to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, uint64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

	/**
	 *	@brief	Validate an unsigned binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedBinaryIntegral(const size_t size, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	template <typename T> StatusCode ValidateUnsignedBinaryIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedBinaryIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedBinaryIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedBinaryIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned binary integral value.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	ppData the location to copy the address of the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedBinaryIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData = 0);

	/**
	 *	@brief	Validate an unsigned binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode ValidateUnsignedBinaryIntegralCopy(const size_t size, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate an unsigned binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	template <typename T> StatusCode ValidateUnsignedBinaryIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedBinaryIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate an unsigned binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedBinaryIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate an unsigned binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateUnsignedBinaryIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, void *pCopy);

	/**
	 *	@brief	Validate an unsigned binary integral value and copy.
	 *
	 *	@param	size the size of the value.
	 *	@param	set the set of allowed values.
	 *	@param	endian the endian of the value.
	 *	@param	pValue the location to copy the value to.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> StatusCode ValidateUnsignedBinaryIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy);

	/**
	 *	@brief	Validate a binary boolean value.
	 *
	 *	@param	pBoolean the location to copy the boolean value to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	StatusCode ValidateBoolean(bool *pBoolean);

	/**
	 *	@brief	Get data from the message.
	 *
	 *	@param	offset the offset within the message.
	 *	@param	pData reference to the address of the data.
	 *	@param	length the length to get.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Get(const size_t offset, const void *&pData, const size_t length) const;

	/**
	 *	@brief	Copy an ASCII hexadecimal integral value to a unsigned binary integral value.
	 *
	 *	@param	to the value to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	size the size of the data to copy from.
	 *	@param	endian the endian of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> static StatusCode CopyAsciiHexadecimalIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian);

	/**
	 *	@brief	Copy a signed ASCII integral value to a signed binary integral value.
	 *
	 *	@param	to the value to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	length the length of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> static StatusCode CopySignedAsciiIntegralToSignedBinaryIntegral(T &to, const void * const pData, size_t length);

	/**
	 *	@brief	Copy a signed ASCII real value to a binary real.
	 *
	 *	@param	to the value to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	length the length of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> static StatusCode CopySignedAsciiRealToBinaryReal(T &to, const void * const pData, size_t length);

	/**
	 *	@brief	Copy a signed ASCII real value to a numerator and divisor.
	 *
	 *	@param	numerator the numerator to be copied to.
	 *	@param	divisor the divisor to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	length the length of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API static StatusCode CopySignedAsciiRealToNumeratorAndDivisor(int64_t &numerator, uint64_t &divisor, const void * const pData, const size_t length);

	/**
	 *	@brief	Copy a signed binary integral value to a signed binary integral value.
	 *
	 *	@param	to the value to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	size the size of the data to copy from.
	 *	@param	endian the endian of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> static StatusCode CopySignedBinaryIntegralToSignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian);

	/**
	 *	@brief	Copy an unsigned ASCII integral value to an unsigned binary integral value.
	 *
	 *	@param	to the value to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	length the length of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> static StatusCode CopyUnsignedAsciiIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t length);

	/**
	 *	@brief	Copy an unsigned ASCII real value to a binary real.
	 *
	 *	@param	to the value to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	length the length of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> static StatusCode CopyUnsignedAsciiRealToBinaryReal(T &to, const void * const pData, size_t length);

	/**
	 *	@brief	Copy a unsigned ASCII real value to a numerator and divisor.
	 *
	 *	@param	numerator the numerator to be copied to.
	 *	@param	divisor the divisor to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	length the length of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	ACTIV_BASE_API static StatusCode CopyUnsignedAsciiRealToNumeratorAndDivisor(uint64_t &numerator, uint64_t &divisor, const void * const pData, const size_t length);

	/**
	 *	@brief	Copy an unsigned binary integral value to a unsigned binary integral value.
	 *
	 *	@param	to the value to be copied to.
	 *	@param	pData a pointer to the data to copy from.
	 *	@param	size the size of the data to copy from.
	 *	@param	endian the endian of the data to copy from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_OUT_OF_RANGE
	 */
	template <typename T> static StatusCode CopyUnsignedBinaryIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian);

private:
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateAsciiHexadecimalIntegralT(const size_t size, const Endian endian, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateAsciiHexadecimalIntegralT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateAsciiHexadecimalIntegralT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData = 0);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateAsciiHexadecimalIntegralCopyT(const size_t size, const Endian endian, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateAsciiHexadecimalIntegralCopyT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateAsciiHexadecimalIntegralCopyT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiIntegralT(const size_t length, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiIntegralT(const size_t length, const int64_t lower, const int64_t upper, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiIntegralT(const size_t length, const std::vector<int64_t> &set, T *pValue, const void **ppData);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiIntegralCopyT(const size_t length, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiIntegralCopyT(const size_t length, const int64_t lower, const int64_t upper, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiIntegralCopyT(const size_t length, const std::vector<int64_t> &set, T *pValue, void *pCopy);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiRealT(const size_t length, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiRealT(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiRealT(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiRealCopyT(const size_t length, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiRealCopyT(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedAsciiRealCopyT(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy);
	
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedBinaryIntegralT(const size_t size, const Endian endian, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedBinaryIntegralT(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedBinaryIntegralT(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, const void **ppData);
	
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedBinaryIntegralCopyT(const size_t size, const Endian endian, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedBinaryIntegralCopyT(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateSignedBinaryIntegralCopyT(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, void *pCopy);
	
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiIntegralT(const size_t length, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiIntegralT(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiIntegralT(const size_t length, const std::vector<uint64_t> &set, T *pValue, const void **ppData);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiIntegralCopyT(const size_t length, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiIntegralCopyT(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiIntegralCopyT(const size_t length, const std::vector<uint64_t> &set, T *pValue, void *pCopy);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiRealT(const size_t length, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiRealT(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiRealT(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiRealCopyT(const size_t length, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiRealCopyT(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedAsciiRealCopyT(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedBinaryIntegralT(const size_t size, const Endian endian, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedBinaryIntegralT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedBinaryIntegralT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData);

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedBinaryIntegralCopyT(const size_t size, const Endian endian, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedBinaryIntegralCopyT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy);
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode ValidateUnsignedBinaryIntegralCopyT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy);

	const void	*m_pMessage;	///< The address of the message.
	size_t		m_length;		///< The length of the message.
	size_t		m_offset;		///< The current offset within the message.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline MessageValidator::MessageValidator(const void *pMessage, const size_t length, const MessageVersion messageVersion)
{
	Initialize(pMessage, length, messageVersion);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void MessageValidator::Initialize(const void *pMessage, const size_t length, const MessageVersion messageVersion)
{
	m_pMessage = pMessage;
	m_length = length;
	
	Reset(messageVersion);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void MessageValidator::Reset(const MessageVersion messageVersion)
{
	MessageHandler::Reset(messageVersion);
	m_offset = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline const void *MessageValidator::GetMessage() const
{
	return m_pMessage;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t MessageValidator::GetLength() const
{
	return m_length;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t MessageValidator::GetOffset() const
{
	return m_offset;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::SetOffset(const size_t offset)
{
	if (offset > m_length)
		return STATUS_CODE_INVALID_PARAMETER;

	m_offset = offset;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool MessageValidator::IsEndOfMessage() const
{
	return (m_length == m_offset);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegral(size, lower, upper, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegral(size, set, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegralCopy(size, lower, upper, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegralCopy(size, set, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiIntegral(const size_t length, const int64_t lower, const int64_t upper, const void **ppData)
{
	int64_t value;

	return ValidateSignedAsciiIntegral(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiIntegral(const size_t length, const std::vector<int64_t> &set, const void **ppData)
{
	int64_t value;

	return ValidateSignedAsciiIntegral(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiIntegralCopy(const size_t length, const int64_t lower, const int64_t upper, void *pCopy)
{
	int64_t value;

	return ValidateSignedAsciiIntegralCopy(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiIntegralCopy(const size_t length, const std::vector<int64_t> &set, void *pCopy)
{
	int64_t value;

	return ValidateSignedAsciiIntegralCopy(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, const void **ppData)
{
	long double value;

	return ValidateSignedAsciiRealT(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, const void **ppData)
{
	long double value;

	return ValidateSignedAsciiRealT(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, void *pCopy)
{
	long double value;

	return ValidateSignedAsciiRealCopyT(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, void *pCopy)
{
	long double value;

	return ValidateSignedAsciiRealCopyT(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedBinaryIntegral(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, const void **ppData)
{
	int64_t value;

	return ValidateSignedBinaryIntegral(size, lower, upper, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedBinaryIntegral(const size_t size, const std::vector<int64_t> &set, const Endian endian, const void **ppData)
{
	int64_t value;

	return ValidateSignedBinaryIntegral(size, set, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedBinaryIntegralCopy(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, void *pCopy)
{
	int64_t value;

	return ValidateSignedBinaryIntegralCopy(size, lower, upper, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateSignedBinaryIntegralCopy(const size_t size, const std::vector<int64_t> &set, const Endian endian, void *pCopy)
{
	int64_t value;

	return ValidateSignedBinaryIntegralCopy(size, set, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateString(const void **ppData, size_t *pDataLength)
{
	return ValidateString(0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateString(const byte_t lower, const byte_t upper, const void **ppData, size_t *pDataLength)
{
	return ValidateString(lower, upper, 0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateString(const std::vector<byte_t> &set, const void **ppData, size_t *pDataLength)
{
	return ValidateString(set, 0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateString(const std::vector<std::string> &set, const void **ppData, size_t *pDataLength)
{
	return ValidateString(set, 0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateStringCopy(void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateStringCopy(const byte_t lower, const byte_t upper, void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(lower, upper, 0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateStringCopy(const std::vector<byte_t> &set, void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(set, 0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateStringCopy(const std::vector<std::string> &set, void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(set, 0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegral(const size_t length, const uint64_t lower, const uint64_t upper, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegral(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegral(const size_t length, const std::vector<uint64_t> &set, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegral(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegralCopy(const size_t length, const uint64_t lower, const uint64_t upper, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegralCopy(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegralCopy(const size_t length, const std::vector<uint64_t> &set, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegralCopy(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, const void **ppData)
{
	long double value;

	return ValidateUnsignedAsciiReal(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, const void **ppData)
{
	long double value;

	return ValidateUnsignedAsciiReal(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, void *pCopy)
{
	long double value;

	return ValidateUnsignedAsciiRealCopy(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, void *pCopy)
{
	long double value;

	return ValidateUnsignedAsciiRealCopy(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegral(size, lower, upper, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegral(size, set, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegralCopy(size, lower, upper, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegralCopy(size, set, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::ValidateBoolean(bool *pBoolean)
{
	uint8_t value;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ValidateUnsignedBinaryIntegral(sizeof(uint8_t), ENDIAN_LITTLE, &value));

	if (0 != pBoolean)
		*pBoolean = (0 != value);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidator::Get(const size_t offset, const void *&pData, const size_t length) const
{
	if (0 == length)
		return STATUS_CODE_SUCCESS;

	if (offset + length > m_length)
		return STATUS_CODE_INVALID_PARAMETER;

	pData = static_cast<const byte_t *>(m_pMessage) + offset;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidator::CopyAsciiHexadecimalIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian)
{
	static const T MAX_VALUE = std::numeric_limits<T>::max();

	if ((0 != size % 2) || (size > 2 * sizeof(uint64_t)))
		return STATUS_CODE_INVALID_PARAMETER;

	const byte_t * const pDataBuffer = static_cast<const byte_t * const>(pData);
	uint64_t value = 0;

	for (size_t i = 0; i < size; i += 2)
	{
		uint8_t nibble1, nibble2;

		if ((pDataBuffer[i] >= '0') && (pDataBuffer[i] <= '9'))
		{
			nibble1 = pDataBuffer[i] - '0';
		}
		else if ((pDataBuffer[i] >= 'A') && (pDataBuffer[i] <= 'F'))
		{
			nibble1 = pDataBuffer[i] - 'A' + 10;
		}
		else if ((pDataBuffer[i] >= 'a') && (pDataBuffer[i] <= 'f'))
		{
			nibble1 = pDataBuffer[i] - 'a' + 10;
		}
		else
		{
			return STATUS_CODE_INVALID_FORMAT;
		}

		if ((pDataBuffer[i+1] >= '0') && (pDataBuffer[i+1] <= '9'))
		{
			nibble2 = pDataBuffer[i+1] - '0';
		}
		else if ((pDataBuffer[i+1] >= 'A') && (pDataBuffer[i+1] <= 'F'))
		{
			nibble2 = pDataBuffer[i+1] - 'A' + 10;
		}
		else if ((pDataBuffer[i+1] >= 'a') && (pDataBuffer[i+1] <= 'f'))
		{
			nibble2 = pDataBuffer[i+1] - 'a' + 10;
		}
		else
		{
			return STATUS_CODE_INVALID_FORMAT;
		}

		value |= ((ENDIAN_LITTLE == endian) ? (static_cast<uint64_t>((nibble1 << (CHAR_BIT / 2)) | nibble2) << ((i / 2) * CHAR_BIT)) : (static_cast<uint64_t>((nibble1 << (CHAR_BIT / 2)) | nibble2) << ((((size - i) / 2) - 1) * CHAR_BIT)));
	}

	if (value > MAX_VALUE)
		return STATUS_CODE_OUT_OF_RANGE;

	to = static_cast<T>(value);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidator::CopySignedAsciiIntegralToSignedBinaryIntegral(T &to, const void * const pData, size_t length)
{
	static const T MAX_VALUE = std::numeric_limits<T>::max();
	static const size_t DIGITS_10 = std::numeric_limits<T>::digits10;

	const byte_t * const pDataBuffer = static_cast<const byte_t * const>(pData);
	bool negative = false;

	to = 0;

	for (size_t i = 0; i < length; ++i)
	{
		if (('+' == pDataBuffer[i]) || ('-' == pDataBuffer[i]))
		{
			if (i > 0)
				return STATUS_CODE_INVALID_FORMAT;

			if ('-' == pDataBuffer[i])
				negative = true;
		}
		else if ((pDataBuffer[i] >= '0') && (pDataBuffer[i] <= '9'))
		{
			if ((i >= DIGITS_10) && (to > ((MAX_VALUE - static_cast<T>(pDataBuffer[i] - '0')) / 10)))
				return STATUS_CODE_OUT_OF_RANGE;

			to = (10 * to) + static_cast<T>(pDataBuffer[i] - '0');
		}
		else
		{
			return STATUS_CODE_INVALID_FORMAT;
		}
	}

	if (negative)
		to *= -1;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidator::CopySignedAsciiRealToBinaryReal(T &to, const void * const pData, size_t length)
{
	int64_t numerator;
	uint64_t divisor;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CopySignedAsciiRealToNumeratorAndDivisor(numerator, divisor, pData, length));

	to = static_cast<T>(numerator) / static_cast<T>(divisor);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidator::CopySignedBinaryIntegralToSignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian)
{
	static const T MIN_VALUE = std::numeric_limits<T>::min();
	static const T MAX_VALUE = std::numeric_limits<T>::max();

	if (size > sizeof(int64_t))
		return STATUS_CODE_INVALID_PARAMETER;

	const byte_t * const pDataBuffer = static_cast<const byte_t * const>(pData);
	int64_t value = 0;

	if (ENDIAN_LITTLE == endian)
	{
		for (size_t i = 0; i < size; ++i)
		{
			value |= static_cast<int64_t>(pDataBuffer[i]) << (i * CHAR_BIT);
		}
	}
	else // (ENDIAN_BIG == endian)
	{
		for (size_t i = 0; i < size; ++i)
		{
			value |= static_cast<int64_t>(pDataBuffer[i]) << ((size - i - 1) * CHAR_BIT);
		}
	}

	if ((sizeof(int64_t) != size) && (value & (INT64_C(1) << (((size - 1) * CHAR_BIT) + (CHAR_BIT - 1)))))
	{
		value |= ((INT64_C(-1) >> ((sizeof(int64_t) - size) * CHAR_BIT)) << (size * CHAR_BIT));
	}

	if ((value < MIN_VALUE) || (value > MAX_VALUE))
		return STATUS_CODE_OUT_OF_RANGE;

	to = static_cast<T>(value);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidator::CopyUnsignedAsciiIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t length)
{
	static const T MAX_VALUE = std::numeric_limits<T>::max();
	static const size_t DIGITS_10 = std::numeric_limits<T>::digits10;

	const byte_t * const pDataBuffer = static_cast<const byte_t * const>(pData);

	to = 0;

	for (size_t i = 0; i < length; ++i)
	{
		if ((pDataBuffer[i] >= '0') && (pDataBuffer[i] <= '9'))
		{
			if ((i >= DIGITS_10) && (to > ((MAX_VALUE - static_cast<T>(pDataBuffer[i] - '0')) / 10)))
				return STATUS_CODE_OUT_OF_RANGE;

			to = (10 * to) + static_cast<T>(pDataBuffer[i] - '0');
		}
		else
		{
			return STATUS_CODE_INVALID_FORMAT;
		}
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidator::CopyUnsignedAsciiRealToBinaryReal(T &to, const void * const pData, size_t length)
{
	uint64_t numerator;
	uint64_t divisor;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CopyUnsignedAsciiRealToNumeratorAndDivisor(numerator, divisor, pData, length));

	to = static_cast<T>(numerator) / static_cast<T>(divisor);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidator::CopyUnsignedBinaryIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian)
{
	static const T MAX_VALUE = std::numeric_limits<T>::max();

	if (size > sizeof(uint64_t))
		return STATUS_CODE_INVALID_PARAMETER;

	const byte_t * const pDataBuffer = static_cast<const byte_t * const>(pData);
	uint64_t value = 0;

	if (ENDIAN_LITTLE == endian)
	{
		for (size_t i = 0; i < size; ++i)
		{
			value |= static_cast<uint64_t>(pDataBuffer[i]) << (i * CHAR_BIT);
		}
	}
	else // (ENDIAN_BIG == endian)
	{
		for (size_t i = 0; i < size; ++i)
		{
			value |= static_cast<uint64_t>(pDataBuffer[i]) << ((size - i - 1) * CHAR_BIT);
		}
	}

	if (value > MAX_VALUE)
		return STATUS_CODE_OUT_OF_RANGE;

	to = static_cast<T>(value);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateAsciiHexadecimalIntegralT<T>(size, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateAsciiHexadecimalIntegralT<T>(size, lower, upper, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateAsciiHexadecimalIntegralT<T>(size, set, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateAsciiHexadecimalIntegralCopyT<T>(size, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateAsciiHexadecimalIntegralCopyT<T>(size, lower, upper, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateAsciiHexadecimalIntegralCopyT<T>(size, set, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiIntegral(const size_t length, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiIntegralT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiIntegral(const size_t length, const int64_t lower, const int64_t upper, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiIntegralT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiIntegral(const size_t length, const std::vector<int64_t> &set, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiIntegralT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiIntegralCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiIntegralCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiIntegralCopy(const size_t length, const int64_t lower, const int64_t upper, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiIntegralCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiIntegralCopy(const size_t length, const std::vector<int64_t> &set, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiIntegralCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiReal(const size_t length, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiRealT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiRealT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiRealT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiRealCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiRealCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiRealCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiRealCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedBinaryIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateSignedBinaryIntegralT<T>(size, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedBinaryIntegral(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateSignedBinaryIntegralT<T>(size, lower, upper, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedBinaryIntegral(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateSignedBinaryIntegralT<T>(size, set, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedBinaryIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateSignedBinaryIntegralCopyT<T>(size, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedBinaryIntegralCopy(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateSignedBinaryIntegralCopyT<T>(size, lower, upper, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateSignedBinaryIntegralCopy(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateSignedBinaryIntegralCopyT<T>(size, set, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegral(const size_t length, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiIntegralT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegral(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiIntegralT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegral(const size_t length, const std::vector<uint64_t> &set, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiIntegralT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegralCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiIntegralCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegralCopy(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiIntegralCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiIntegralCopy(const size_t length, const std::vector<uint64_t> &set, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiIntegralCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiReal(const size_t length, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiRealT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiRealT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiRealT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiRealCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiRealCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiRealCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiRealCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateUnsignedBinaryIntegralT<T>(size, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateUnsignedBinaryIntegralT<T>(size, lower, upper, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateUnsignedBinaryIntegralT<T>(size, set, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateUnsignedBinaryIntegralCopyT<T>(size, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateUnsignedBinaryIntegralCopyT<T>(size, lower, upper, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidator::ValidateUnsignedBinaryIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateUnsignedBinaryIntegralCopyT<T>(size, set, endian, pValue, pCopy);
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_MESSAGE_VALIDATOR_H)
