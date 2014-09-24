/**
 *  @file	MessageValidater.h
 *
 *	@brief	Header file for the message validater class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MESSAGE_VALIDATER_H)
#define ACTIV_MESSAGE_VALIDATER_H

#include "ActivMiddleware/ActivMessageHandler/MessageHandler.h"

#include <float.h>
#include <vector>
#include <string>
#include <limits>

namespace Activ
{

/**
 *	@brief	Message validater class.
 */
class MessageValidater : public MessageHandler
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	pMessage the address of the message.
	 *	@param	length the length of the message.
	 *	@param	messageVersion optional version.
	 */
	MessageValidater(const void *pMessage = 0, const size_t length = 0, const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytes(const size_t length, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytes(const size_t length, const byte_t lower, const byte_t upper, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytes(const size_t length, const std::vector<byte_t> &set, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytes(const size_t length, const std::vector<std::string> &set, const void **ppData = 0);

	/**
	 *	@brief	Validate bytes and copy.
	 *
	 *	@param	length the length of the buffer.
	 *	@param	pCopy the address to copy the data to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytesCopy(const size_t length, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytesCopy(const size_t length, const byte_t lower, const byte_t upper, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytesCopy(const size_t length, const std::vector<byte_t> &set, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateBytesCopy(const size_t length, const std::vector<std::string> &set, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateAsciiHexadecimalIntegral(const size_t size, const Endian endian, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateAsciiHexadecimalIntegralCopy(const size_t size, const Endian endian, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiIntegral(const size_t length, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiIntegral(const size_t length, const int64_t lower, const int64_t upper, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, const int64_t lower, const int64_t upper, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiIntegralCopy(const size_t length, const std::vector<int64_t> &set, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiReal(const size_t length, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiReal(const size_t length, int64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, int64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, int64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, int64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, int64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, int64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedBinaryIntegral(const size_t size, const Endian endian, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedBinaryIntegralCopy(const size_t size, const Endian endian, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateString(std::string *pString, const void **ppData = 0, size_t *pDataLength = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateString(const byte_t lower, const byte_t upper, std::string *pString, const void **ppData = 0, size_t *pDataLength = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateString(const std::vector<byte_t> &set, const void **ppData = 0, size_t *pDataLength = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateString(const std::vector<byte_t> &set, std::string *pString, const void **ppData = 0, size_t *pDataLength = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateStringCopy(std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateStringCopy(const byte_t lower, const byte_t upper, std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateStringCopy(const std::vector<byte_t> &set, std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateStringCopy(const std::vector<std::string> &set, std::string *pString, void *pCopy, const size_t copySize, size_t *pCopyLength);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiIntegral(const size_t length, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, const uint64_t lower, const uint64_t upper, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiIntegralCopy(const size_t length, const std::vector<uint64_t> &set, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiReal(const size_t length, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiReal(const size_t length, uint64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, uint64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, uint64_t *pNumerator, uint64_t *pDivisor, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, uint64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, uint64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, uint64_t *pNumerator, uint64_t *pDivisor, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedBinaryIntegral(const size_t size, const Endian endian, const void **ppData = 0);

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
	ACTIV_MESSAGE_HANDLER_API StatusCode ValidateUnsignedBinaryIntegralCopy(const size_t size, const Endian endian, void *pCopy);

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
	ACTIV_MESSAGE_HANDLER_API static StatusCode CopySignedAsciiRealToNumeratorAndDivisor(int64_t &numerator, uint64_t &divisor, const void * const pData, const size_t length);

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
	ACTIV_MESSAGE_HANDLER_API static StatusCode CopyUnsignedAsciiRealToNumeratorAndDivisor(uint64_t &numerator, uint64_t &divisor, const void * const pData, const size_t length);

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
	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateAsciiHexadecimalIntegralT(const size_t size, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint8_t>(const size_t size, const Endian endian, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint16_t>(const size_t size, const Endian endian, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint32_t>(const size_t size, const Endian endian, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint64_t>(const size_t size, const Endian endian, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<size_t>(const size_t size, const Endian endian, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateAsciiHexadecimalIntegralT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint8_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint16_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint32_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint64_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<size_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateAsciiHexadecimalIntegralT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData = 0);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint8_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint16_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint32_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<uint64_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralT<size_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateAsciiHexadecimalIntegralCopyT(const size_t size, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint8_t>(const size_t size, const Endian endian, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint16_t>(const size_t size, const Endian endian, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint32_t>(const size_t size, const Endian endian, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint64_t>(const size_t size, const Endian endian, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<size_t>(const size_t size, const Endian endian, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateAsciiHexadecimalIntegralCopyT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint8_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint16_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint32_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint64_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<size_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateAsciiHexadecimalIntegralCopyT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint8_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint16_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint32_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<uint64_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateAsciiHexadecimalIntegralCopyT<size_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiIntegralT(const size_t length, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int8_t>(const size_t length, int8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int16_t>(const size_t length, int16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int32_t>(const size_t length, int32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int64_t>(const size_t length, int64_t *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiIntegralT(const size_t length, const int64_t lower, const int64_t upper, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int8_t>(const size_t length, const int64_t lower, const int64_t upper, int8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int16_t>(const size_t length, const int64_t lower, const int64_t upper, int16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int32_t>(const size_t length, const int64_t lower, const int64_t upper, int32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int64_t>(const size_t length, const int64_t lower, const int64_t upper, int64_t *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiIntegralT(const size_t length, const std::vector<int64_t> &set, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int8_t>(const size_t length, const std::vector<int64_t> &set, int8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int16_t>(const size_t length, const std::vector<int64_t> &set, int16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int32_t>(const size_t length, const std::vector<int64_t> &set, int32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralT<int64_t>(const size_t length, const std::vector<int64_t> &set, int64_t *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiIntegralCopyT(const size_t length, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int8_t>(const size_t length, int8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int16_t>(const size_t length, int16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int32_t>(const size_t length, int32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int64_t>(const size_t length, int64_t *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiIntegralCopyT(const size_t length, const int64_t lower, const int64_t upper, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int8_t>(const size_t length, const int64_t lower, const int64_t upper, int8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int16_t>(const size_t length, const int64_t lower, const int64_t upper, int16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int32_t>(const size_t length, const int64_t lower, const int64_t upper, int32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int64_t>(const size_t length, const int64_t lower, const int64_t upper, int64_t *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiIntegralCopyT(const size_t length, const std::vector<int64_t> &set, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int8_t>(const size_t length, const std::vector<int64_t> &set, int8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int16_t>(const size_t length, const std::vector<int64_t> &set, int16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int32_t>(const size_t length, const std::vector<int64_t> &set, int32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiIntegralCopyT<int64_t>(const size_t length, const std::vector<int64_t> &set, int64_t *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiRealT(const size_t length, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<float>(const size_t length, float *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<double>(const size_t length, double *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<long double>(const size_t length, long double *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiRealT(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<float>(const size_t length, const long double lower, const long double upper, float *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<double>(const size_t length, const long double lower, const long double upper, double *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<long double>(const size_t length, const long double lower, const long double upper, long double *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiRealT(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<float>(const size_t length, const std::vector<long double> &set, float *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<double>(const size_t length, const std::vector<long double> &set, double *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealT<long double>(const size_t length, const std::vector<long double> &set, long double *pValue, const void **ppData);)

	template <typename T> ACTIV_MESSAGE_HANDLER_API StatusCode ValidateSignedAsciiRealCopyT(const size_t length, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<float>(const size_t length, float *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<double>(const size_t length, double *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<long double>(const size_t length, long double *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiRealCopyT(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<float>(const size_t length, const long double lower, const long double upper, float *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<double>(const size_t length, const long double lower, const long double upper, double *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<long double>(const size_t length, const long double lower, const long double upper, long double *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedAsciiRealCopyT(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<float>(const size_t length, const std::vector<long double> &set, float *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<double>(const size_t length, const std::vector<long double> &set, double *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedAsciiRealCopyT<long double>(const size_t length, const std::vector<long double> &set, long double *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedBinaryIntegralT(const size_t size, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int8_t>(const size_t size, const Endian endian, int8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int16_t>(const size_t size, const Endian endian, int16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int32_t>(const size_t size, const Endian endian, int32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int64_t>(const size_t size, const Endian endian, int64_t *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedBinaryIntegralT(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int8_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int16_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int32_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int64_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int64_t *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedBinaryIntegralT(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int8_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int16_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int32_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralT<int64_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int64_t *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedBinaryIntegralCopyT(const size_t size, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int8_t>(const size_t size, const Endian endian, int8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int16_t>(const size_t size, const Endian endian, int16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int32_t>(const size_t size, const Endian endian, int32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int64_t>(const size_t size, const Endian endian, int64_t *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedBinaryIntegralCopyT(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int8_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int16_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int32_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int64_t>(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, int64_t *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateSignedBinaryIntegralCopyT(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int8_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int16_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int32_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateSignedBinaryIntegralCopyT<int64_t>(const size_t size, const std::vector<int64_t> &set, const Endian endian, int64_t *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiIntegralT(const size_t length, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint8_t>(const size_t length, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint16_t>(const size_t length, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint32_t>(const size_t length, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint64_t>(const size_t length, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<size_t>(const size_t length, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiIntegralT(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint8_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint16_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint32_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint64_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<size_t>(const size_t length, const uint64_t lower, const uint64_t upper, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiIntegralT(const size_t length, const std::vector<uint64_t> &set, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint8_t>(const size_t length, const std::vector<uint64_t> &set, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint16_t>(const size_t length, const std::vector<uint64_t> &set, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint32_t>(const size_t length, const std::vector<uint64_t> &set, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<uint64_t>(const size_t length, const std::vector<uint64_t> &set, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralT<size_t>(const size_t length, const std::vector<uint64_t> &set, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiIntegralCopyT(const size_t length, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint8_t>(const size_t length, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint16_t>(const size_t length, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint32_t>(const size_t length, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint64_t>(const size_t length, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<size_t>(const size_t length, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiIntegralCopyT(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint8_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint16_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint32_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint64_t>(const size_t length, const uint64_t lower, const uint64_t upper, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<size_t>(const size_t length, const uint64_t lower, const uint64_t upper, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiIntegralCopyT(const size_t length, const std::vector<uint64_t> &set, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint8_t>(const size_t length, const std::vector<uint64_t> &set, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint16_t>(const size_t length, const std::vector<uint64_t> &set, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint32_t>(const size_t length, const std::vector<uint64_t> &set, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<uint64_t>(const size_t length, const std::vector<uint64_t> &set, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiIntegralCopyT<size_t>(const size_t length, const std::vector<uint64_t> &set, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiRealT(const size_t length, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<float>(const size_t length, float *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<double>(const size_t length, double *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<long double>(const size_t length, long double *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiRealT(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<float>(const size_t length, const long double lower, const long double upper, float *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<double>(const size_t length, const long double lower, const long double upper, double *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<long double>(const size_t length, const long double lower, const long double upper, long double *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiRealT(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<float>(const size_t length, const std::vector<long double> &set, float *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<double>(const size_t length, const std::vector<long double> &set, double *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealT<long double>(const size_t length, const std::vector<long double> &set, long double *pValue, const void **ppData);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiRealCopyT(const size_t length, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<float>(const size_t length, float *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<double>(const size_t length, double *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<long double>(const size_t length, long double *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiRealCopyT(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<float>(const size_t length, const long double lower, const long double upper, float *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<double>(const size_t length, const long double lower, const long double upper, double *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<long double>(const size_t length, const long double lower, const long double upper, long double *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedAsciiRealCopyT(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<float>(const size_t length, const std::vector<long double> &set, float *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<double>(const size_t length, const std::vector<long double> &set, double *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedAsciiRealCopyT<long double>(const size_t length, const std::vector<long double> &set, long double *pValue, void *pCopy);)

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedBinaryIntegralT(const size_t size, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint8_t>(const size_t size, const Endian endian, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint16_t>(const size_t size, const Endian endian, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint32_t>(const size_t size, const Endian endian, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint64_t>(const size_t size, const Endian endian, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<size_t>(const size_t size, const Endian endian, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedBinaryIntegralT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint8_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint16_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint32_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint64_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<size_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedBinaryIntegralT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint8_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint8_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint16_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint16_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint32_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint32_t *pValue, const void **ppData);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<uint64_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint64_t *pValue, const void **ppData);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralT<size_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, size_t *pValue, const void **ppData);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedBinaryIntegralCopyT(const size_t size, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint8_t>(const size_t size, const Endian endian, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint16_t>(const size_t size, const Endian endian, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint32_t>(const size_t size, const Endian endian, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint64_t>(const size_t size, const Endian endian, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<size_t>(const size_t size, const Endian endian, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedBinaryIntegralCopyT(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint8_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint16_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint32_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint64_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<size_t>(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, size_t *pValue, void *pCopy);)
#endif

	template <typename T> ACTIV_TEMPLATE_API(ACTIV_MESSAGE_HANDLER_API) StatusCode ValidateUnsignedBinaryIntegralCopyT(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy);
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint8_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint8_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint16_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint16_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint32_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint32_t *pValue, void *pCopy);)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<uint64_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, uint64_t *pValue, void *pCopy);)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode ValidateUnsignedBinaryIntegralCopyT<size_t>(const size_t size, const std::vector<uint64_t> &set, const Endian endian, size_t *pValue, void *pCopy);)
#endif

	const void	*m_pMessage;	///< The address of the message.
	size_t		m_length;		///< The length of the message.
	size_t		m_offset;		///< The current offset within the message.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline MessageValidater::MessageValidater(const void *pMessage, const size_t length, const MessageVersion messageVersion)
{
	Initialize(pMessage, length, messageVersion);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void MessageValidater::Initialize(const void *pMessage, const size_t length, const MessageVersion messageVersion)
{
	m_pMessage = pMessage;
	m_length = length;
	
	Reset(messageVersion);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void MessageValidater::Reset(const MessageVersion messageVersion)
{
	MessageHandler::Reset(messageVersion);
	m_offset = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline const void *MessageValidater::GetMessage() const
{
	return m_pMessage;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t MessageValidater::GetLength() const
{
	return m_length;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t MessageValidater::GetOffset() const
{
	return m_offset;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::SetOffset(const size_t offset)
{
	if (offset > m_length)
		return STATUS_CODE_INVALID_PARAMETER;

	m_offset = offset;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool MessageValidater::IsEndOfMessage() const
{
	return (m_length == m_offset);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegral(size, lower, upper, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegral(size, set, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegralCopy(size, lower, upper, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateAsciiHexadecimalIntegralCopy(size, set, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiIntegral(const size_t length, const int64_t lower, const int64_t upper, const void **ppData)
{
	int64_t value;

	return ValidateSignedAsciiIntegral(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiIntegral(const size_t length, const std::vector<int64_t> &set, const void **ppData)
{
	int64_t value;

	return ValidateSignedAsciiIntegral(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiIntegralCopy(const size_t length, const int64_t lower, const int64_t upper, void *pCopy)
{
	int64_t value;

	return ValidateSignedAsciiIntegralCopy(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiIntegralCopy(const size_t length, const std::vector<int64_t> &set, void *pCopy)
{
	int64_t value;

	return ValidateSignedAsciiIntegralCopy(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, const void **ppData)
{
	long double value;

	return ValidateSignedAsciiRealT(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, const void **ppData)
{
	long double value;

	return ValidateSignedAsciiRealT(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, void *pCopy)
{
	long double value;

	return ValidateSignedAsciiRealCopyT(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, void *pCopy)
{
	long double value;

	return ValidateSignedAsciiRealCopyT(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedBinaryIntegral(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, const void **ppData)
{
	int64_t value;

	return ValidateSignedBinaryIntegral(size, lower, upper, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedBinaryIntegral(const size_t size, const std::vector<int64_t> &set, const Endian endian, const void **ppData)
{
	int64_t value;

	return ValidateSignedBinaryIntegral(size, set, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedBinaryIntegralCopy(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, void *pCopy)
{
	int64_t value;

	return ValidateSignedBinaryIntegralCopy(size, lower, upper, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateSignedBinaryIntegralCopy(const size_t size, const std::vector<int64_t> &set, const Endian endian, void *pCopy)
{
	int64_t value;

	return ValidateSignedBinaryIntegralCopy(size, set, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateString(const void **ppData, size_t *pDataLength)
{
	return ValidateString(0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateString(const byte_t lower, const byte_t upper, const void **ppData, size_t *pDataLength)
{
	return ValidateString(lower, upper, 0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateString(const std::vector<byte_t> &set, const void **ppData, size_t *pDataLength)
{
	return ValidateString(set, 0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateString(const std::vector<std::string> &set, const void **ppData, size_t *pDataLength)
{
	return ValidateString(set, 0, ppData, pDataLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateStringCopy(void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateStringCopy(const byte_t lower, const byte_t upper, void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(lower, upper, 0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateStringCopy(const std::vector<byte_t> &set, void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(set, 0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateStringCopy(const std::vector<std::string> &set, void *pCopy, const size_t copySize, size_t *pCopyLength)
{
	return ValidateStringCopy(set, 0, pCopy, copySize, pCopyLength);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegral(const size_t length, const uint64_t lower, const uint64_t upper, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegral(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegral(const size_t length, const std::vector<uint64_t> &set, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegral(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegralCopy(const size_t length, const uint64_t lower, const uint64_t upper, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegralCopy(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegralCopy(const size_t length, const std::vector<uint64_t> &set, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedAsciiIntegralCopy(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, const void **ppData)
{
	long double value;

	return ValidateUnsignedAsciiReal(length, lower, upper, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, const void **ppData)
{
	long double value;

	return ValidateUnsignedAsciiReal(length, set, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, void *pCopy)
{
	long double value;

	return ValidateUnsignedAsciiRealCopy(length, lower, upper, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, void *pCopy)
{
	long double value;

	return ValidateUnsignedAsciiRealCopy(length, set, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegral(size, lower, upper, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, const void **ppData)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegral(size, set, endian, &value, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegralCopy(size, lower, upper, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, void *pCopy)
{
	uint64_t value;

	return ValidateUnsignedBinaryIntegralCopy(size, set, endian, &value, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageValidater::Get(const size_t offset, const void *&pData, const size_t length) const
{
	if (0 == length)
		return STATUS_CODE_SUCCESS;

	if (offset + length > m_length)
		return STATUS_CODE_INVALID_PARAMETER;

	pData = static_cast<const byte_t *>(m_pMessage) + offset;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidater::CopyAsciiHexadecimalIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian)
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

template <typename T> StatusCode MessageValidater::CopySignedAsciiIntegralToSignedBinaryIntegral(T &to, const void * const pData, size_t length)
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

template <typename T> StatusCode MessageValidater::CopySignedAsciiRealToBinaryReal(T &to, const void * const pData, size_t length)
{
	int64_t numerator;
	uint64_t divisor;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CopySignedAsciiRealToNumeratorAndDivisor(numerator, divisor, pData, length));

	to = static_cast<T>(numerator) / static_cast<T>(divisor);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidater::CopySignedBinaryIntegralToSignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian)
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

template <typename T> StatusCode MessageValidater::CopyUnsignedAsciiIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t length)
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

template <typename T> StatusCode MessageValidater::CopyUnsignedAsciiRealToBinaryReal(T &to, const void * const pData, size_t length)
{
	uint64_t numerator;
	uint64_t divisor;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, CopyUnsignedAsciiRealToNumeratorAndDivisor(numerator, divisor, pData, length));

	to = static_cast<T>(numerator) / static_cast<T>(divisor);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T> StatusCode MessageValidater::CopyUnsignedBinaryIntegralToUnsignedBinaryIntegral(T &to, const void * const pData, size_t size, const Endian endian)
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
inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateAsciiHexadecimalIntegralT<T>(size, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateAsciiHexadecimalIntegralT<T>(size, lower, upper, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateAsciiHexadecimalIntegralT<T>(size, set, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateAsciiHexadecimalIntegralCopyT<T>(size, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateAsciiHexadecimalIntegralCopyT<T>(size, lower, upper, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateAsciiHexadecimalIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateAsciiHexadecimalIntegralCopyT<T>(size, set, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiIntegral(const size_t length, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiIntegralT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiIntegral(const size_t length, const int64_t lower, const int64_t upper, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiIntegralT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiIntegral(const size_t length, const std::vector<int64_t> &set, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiIntegralT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiIntegralCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiIntegralCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiIntegralCopy(const size_t length, const int64_t lower, const int64_t upper, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiIntegralCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiIntegralCopy(const size_t length, const std::vector<int64_t> &set, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiIntegralCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiReal(const size_t length, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiRealT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiReal(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiRealT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiReal(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData)
{
	return ValidateSignedAsciiRealT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiRealCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiRealCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiRealCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy)
{
	return ValidateSignedAsciiRealCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedBinaryIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateSignedBinaryIntegralT<T>(size, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedBinaryIntegral(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateSignedBinaryIntegralT<T>(size, lower, upper, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedBinaryIntegral(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateSignedBinaryIntegralT<T>(size, set, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedBinaryIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateSignedBinaryIntegralCopyT<T>(size, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedBinaryIntegralCopy(const size_t size, const int64_t lower, const int64_t upper, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateSignedBinaryIntegralCopyT<T>(size, lower, upper, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateSignedBinaryIntegralCopy(const size_t size, const std::vector<int64_t> &set, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateSignedBinaryIntegralCopyT<T>(size, set, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegral(const size_t length, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiIntegralT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegral(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiIntegralT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegral(const size_t length, const std::vector<uint64_t> &set, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiIntegralT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegralCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiIntegralCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegralCopy(const size_t length, const uint64_t lower, const uint64_t upper, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiIntegralCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiIntegralCopy(const size_t length, const std::vector<uint64_t> &set, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiIntegralCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiReal(const size_t length, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiRealT<T>(length, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiReal(const size_t length, const long double lower, const long double upper, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiRealT<T>(length, lower, upper, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiReal(const size_t length, const std::vector<long double> &set, T *pValue, const void **ppData)
{
	return ValidateUnsignedAsciiRealT<T>(length, set, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiRealCopy(const size_t length, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiRealCopyT<T>(length, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiRealCopy(const size_t length, const long double lower, const long double upper, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiRealCopyT<T>(length, lower, upper, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedAsciiRealCopy(const size_t length, const std::vector<long double> &set, T *pValue, void *pCopy)
{
	return ValidateUnsignedAsciiRealCopyT<T>(length, set, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegral(const size_t size, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateUnsignedBinaryIntegralT<T>(size, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegral(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateUnsignedBinaryIntegralT<T>(size, lower, upper, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegral(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, const void **ppData)
{
	return ValidateUnsignedBinaryIntegralT<T>(size, set, endian, pValue, ppData);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegralCopy(const size_t size, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateUnsignedBinaryIntegralCopyT<T>(size, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegralCopy(const size_t size, const uint64_t lower, const uint64_t upper, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateUnsignedBinaryIntegralCopyT<T>(size, lower, upper, endian, pValue, pCopy);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline StatusCode MessageValidater::ValidateUnsignedBinaryIntegralCopy(const size_t size, const std::vector<uint64_t> &set, const Endian endian, T *pValue, void *pCopy)
{
	return ValidateUnsignedBinaryIntegralCopyT<T>(size, set, endian, pValue, pCopy);
}

} // namespace Activ

#endif // !defined (ACTIV_MESSAGE_VALIDATER_H)
