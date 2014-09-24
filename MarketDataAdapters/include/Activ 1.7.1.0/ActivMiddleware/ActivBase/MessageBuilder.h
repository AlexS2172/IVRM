/**
 *  @file	MessageBuilder.h
 *
 *	@brief	Header file for the message builder class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_MESSAGE_BUILDER_H)
#define ACTIV_BASE_MESSAGE_BUILDER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/MessageHandler.h"
#include "ActivMiddleware/ActivBase/StatusCodeException.h"

#include "boost/integer_traits.hpp"

#include <limits>
#include <string>

namespace Activ
{

/**
 *	@brief	Message builder class.
 */
class MessageBuilder : public MessageHandler
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	initialSize the initial size of the message.
	 *	@param	shouldAdjustSize indicates whether the message size should be adjusted.
	 *	@param	messageVersion optional version.
	 *
	 *	@throw	StatusCodeException
	 */
	MessageBuilder(size_t initialSize = 0, const bool shouldAdjustSize = true, const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pMessage the address of the message.
	 *	@param	size the size of the message.
	 *	@param	shouldAdjustSize indicates whether the message size should be adjusted.
	 *	@param	messageVersion optional version.
	 */
	MessageBuilder(void *pMessage, const size_t size, const bool shouldAdjustSize = true, const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	MessageBuilder(const MessageBuilder &rhs);

	/**
	 *	@brief	Destructor.
	 */
	~MessageBuilder();

	/**
	 *	@brief	Addition operator.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	const MessageBuilder operator+(const MessageBuilder &rhs) const;

	/**
	 *	@brief	Addition assignment operator.
	 *
	 *	@param	rhs the value to add to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	MessageBuilder& operator+=(const MessageBuilder &rhs);

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 *
	 *	@throw	StatusCodeException
	 */
	MessageBuilder& operator=(const MessageBuilder &rhs);

	/**
	 *	@brief	Initialize with a new message.
	 *
	 *	@param	pMessage the address of the message.
	 *	@param	size the size of the message.
	 *	@param	shouldAdjustSize indicates whether the message size should be adjusted.
	 *	@param	messageVersion optional version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode Initialize(void *pMessage, const size_t size, const bool shouldAdjustSize = true, const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Initialize with a new message.
	 *
	 *	@param	initialSize the initial size of the message.
	 *	@param	shouldAdjustSize indicates whether the message size should be adjusted.
	 *	@param	messageVersion optional version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Initialize(size_t initialSize, const bool shouldAdjustSize = true, const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Reset.
	 *
	 *	@param	messageVersion optional version.
	 */
	void Reset(const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Get the address of the message.
	 *
	 *	@return	the address of the message.
	 */
	const void *GetMessage() const;

	/**
	 *	@brief	Get the size of the message.
	 *
	 *	@return	the size of the message.
	 */
	size_t GetSize() const;

	/**
	 *	@brief	Set the size of the message.
	 *
	 *	@param	size the size of the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode SetSize(const size_t size);

	/**
	 *	@brief	Get the length of the message.
	 *
	 *	@return	the length of the message.
	 */
	size_t GetLength() const;

	/**
	 *	@brief	Set the length of the message.
	 *
	 *	@param	length the length of the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode SetLength(const size_t length);

	/**
	 *	@brief	Append a buffer.
	 *
	 *	@param	pBuffer the address of the buffer to place in the message.
	 *	@param	length the length of the buffer to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const void * const pBuffer, const size_t length);

	/**
	 *	@brief	Append a binary integral value.
	 *
	 *	@param	pValue the address of the value to place in the message.
	 *	@param	size the size of the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const void * const pValue, const size_t size, const Endian endian);

	/**
	 *	@brief	Append a character.
	 *
	 *	@param	character the character to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const char character);

	/**
	 *	@brief	Append a character.
	 *
	 *	@param	character the character to place in the message.
	 *	@param	count the number of character to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const char character, const size_t count);

	/**
	 *	@brief	Append a signed 8 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const int8_t value, const Endian endian);

	/**
	 *	@brief	Append a signed 16 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const int16_t value, const Endian endian);

	/**
	 *	@brief	Append a signed 32 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const int32_t value, const Endian endian);

	/**
	 *	@brief	Append a signed 64 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const int64_t value, const Endian endian);

	/**
	 *	@brief	Append a string.
	 *
	 *	@param	str the string to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const std::string &str);

	/**
	 *	@brief	Append an unsigned 8 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const uint8_t value, const Endian endian);

	/**
	 *	@brief	Append an unsigned 16 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const uint16_t value, const Endian endian);

	/**
	 *	@brief	Append an unsigned 32 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const uint32_t value, const Endian endian);

	/**
	 *	@brief	Append an unsigned 64 bit integral value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const uint64_t value, const Endian endian);

#if (defined ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	/**
	 *	@brief	Append an unsigned long value.
	 *
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Append(const size_t value, const Endian endian);
#endif

	/**
	 *	@brief	Append a boolean value.
	 *
	 *	@param	value the boolean value to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode AppendBoolean(const bool value);

	/**
	 *	@brief	Erase a buffer / binary integral value.
	 *
	 *	@param	offset the offset at which to erase in the message.
	 *	@param	length the length of the buffer to erase in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	StatusCode Erase(const size_t offset, const size_t length);

	/**
	 *	@brief	Insert a buffer.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	pBuffer the address of the buffer to place in the message.
	 *	@param	length the length of the buffer to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const void * const pBuffer, const size_t length);

	/**
	 *	@brief	Insert a binary integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	pValue the address of the value to place in the message.
	 *	@param	size the size of the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const void * const pValue, const size_t size, const Endian endian);

	/**
	 *	@brief	Insert a character.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	character the character to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const char character);

	/**
	 *	@brief	Insert a character.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	character the character to place in the message.
	 *	@param	count the number of characters to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const char character, const size_t count);

	/**
	 *	@brief	Insert a signed 8 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const int8_t value, const Endian endian);

	/**
	 *	@brief	Insert a signed 16 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const int16_t value, const Endian endian);

	/**
	 *	@brief	Insert a signed 32 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const int32_t value, const Endian endian);

	/**
	 *	@brief	Insert a signed 64 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const int64_t value, const Endian endian);

	/**
	 *	@brief	Insert a string.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	str the string to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const std::string &str);

	/**
	 *	@brief	Insert an unsigned 8 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const uint8_t value, const Endian endian);

	/**
	 *	@brief	Insert an unsigned 16 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const uint16_t value, const Endian endian);

	/**
	 *	@brief	Insert an unsigned 32 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const uint32_t value, const Endian endian);

	/**
	 *	@brief	Insert an unsigned 64 bit integral value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const uint64_t value, const Endian endian);

#if (defined ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	/**
	 *	@brief	Insert an unsigned long value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Insert(const size_t offset, const size_t value, const Endian endian);
#endif

	/**
	 *	@brief	Insert a boolean value.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	value the boolean value to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode InsertBoolean(const size_t offset, const bool value);

	/**
	 *	@brief	Overwrite a buffer.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	pBuffer the address of the buffer to place in the message.
	 *	@param	length the length of the buffer to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const void * const pBuffer, const size_t length);

	/**
	 *	@brief	Overwrite a binary integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	pValue the address of the value to place in the message.
	 *	@param	size the size of the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const void * const pValue, const size_t size, const Endian endian);

	/**
	 *	@brief	Overwrite a character.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	character the character to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const char character);

	/**
	 *	@brief	Overwrite a character.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	character the character to place in the message.
	 *	@param	count the number of character to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const char character, const size_t count);

	/**
	 *	@brief	Overwrite a signed 8 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const int8_t value, const Endian endian);

	/**
	 *	@brief	Overwrite a signed 16 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const int16_t value, const Endian endian);

	/**
	 *	@brief	Overwrite a signed 32 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const int32_t value, const Endian endian);

	/**
	 *	@brief	Overwrite a signed 64 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const int64_t value, const Endian endian);

	/**
	 *	@brief	Overwrite a string.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	str the string to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const std::string &str);

	/**
	 *	@brief	Overwrite an unsigned 8 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const uint8_t value, const Endian endian);

	/**
	 *	@brief	Overwrite an unsigned 16 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const uint16_t value, const Endian endian);

	/**
	 *	@brief	Overwrite an unsigned 32 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const uint32_t value, const Endian endian);

	/**
	 *	@brief	Overwrite an unsigned 64 bit integral value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const uint64_t value, const Endian endian);

#if (defined ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	/**
	 *	@brief	Overwrite an unsigned long value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the value to place in the message.
	 *	@param	endian the endian to copy the value in.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Overwrite(const size_t offset, const size_t value, const Endian endian);
#endif

	/**
	 *	@brief	Overwrite a boolean value.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	value the boolean value to place in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode OverwriteBoolean(const size_t offset, const bool value);

private:
	/**
	 *	@brief	Append a zero-terminated string (not implemented).
	 *
	 *	@param	str the string to place in the message.
	 *
	 *	@retval	STATUS_CODE_NOT_IMPLEMENTED
	 */
	StatusCode Append(const char str[]);

	/**
	 *	@brief	Insert a zero-terminated string (not implemented).
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	str the string to place in the message.
	 *
	 *	@retval	STATUS_CODE_NOT_IMPLEMENTED
	 */
	StatusCode Insert(const size_t offset, const char str[]);

	/**
	 *	@brief	Overwrite a zero-terminated string (not implemented).
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	str the string to place in the message.
	 *
	 *	@retval	STATUS_CODE_NOT_IMPLEMENTED
	 */
	StatusCode Overwrite(const size_t offset, const char str[]);

	/**
	 *	@brief	Append a buffer.
	 *
	 *	@param	pBuffer the address of the buffer to place in the message.
	 *	@param	length the length of the buffer to place in the message.
	 *	@param	reverseByteOrder whether the byte order should be reversed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode AppendBuffer(const void * const pBuffer, const size_t length, const bool reverseByteOrder = false);

	/**
	 *	@brief	Erase a buffer.
	 *
	 *	@param	offset the offset at which to erase in the message.
	 *	@param	length the length of the buffer to erase in the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_BASE_API StatusCode EraseBuffer(const size_t offset, const size_t length);

	/**
	 *	@brief	Insert a buffer.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	pBuffer the address of the buffer to place in the message.
	 *	@param	length the length of the buffer to place in the message.
	 *	@param	reverseByteOrder whether the byte order should be reversed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode InsertBuffer(const size_t offset, const void * const pBuffer, const size_t length, const bool reverseByteOrder = false);

	/**
	 *	@brief	Overwrite a buffer.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	pBuffer the address of the buffer to place in the message.
	 *	@param	length the length of the buffer to place in the message.
	 *	@param	reverseByteOrder whether the byte order should be reversed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode OverwriteBuffer(const size_t offset, const void * const pBuffer, const size_t length, const bool reverseByteOrder = false);

	/**
	 *	@brief	Adjusts the message size.
	 *
	 *	@param	size the size of message required.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_BASE_API StatusCode AdjustSize(const size_t size);

	/**
	 *	@brief	Get additional size required.
	 *
	 *	@param	size the amount of space required in the message.
	 *
	 *	@return the additional space required.
	 */
	size_t GetSpaceRequired(const size_t size) const;

	/**
	 *	@brief	Check for available space in the message.
	 *
	 *	@param	size the amount of space required in the message.
	 *
	 *	@return	whether there's space available in the message.
	 */
	bool IsSpaceAvailable(const size_t size) const;

	bool	m_shouldAdjustSize;	///< Indicates whether the message size should be adjusted.
	void	*m_pUserMessage;	///< The address of the user message.
	size_t	m_userMessageSize;	///< The size of the user message.
	void	*m_pMessage;		///< The address of the message.
	size_t	m_size;				///< The size of the message.
	size_t	m_length;			///< The length of the message.
};

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageBuilder::MessageBuilder(size_t initialSize, const bool shouldAdjustSize, const MessageVersion messageVersion) :
	MessageHandler(messageVersion),
	m_shouldAdjustSize(shouldAdjustSize),
	m_pUserMessage(NULL),
	m_userMessageSize(0),
	m_pMessage(NULL),
	m_size(0),
	m_length(0)
{
	if (0 != initialSize)
	{
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AdjustSize(initialSize));
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageBuilder::MessageBuilder(void *pMessage, const size_t size, const bool shouldAdjustSize, const MessageVersion messageVersion) :
	MessageHandler(messageVersion),
	m_shouldAdjustSize(shouldAdjustSize),
	m_pUserMessage(pMessage),
	m_userMessageSize(size),
	m_pMessage(pMessage),
	m_size(size),
	m_length(0)
{
}

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageBuilder::MessageBuilder(const MessageBuilder &rhs) :
	MessageHandler(rhs.GetMessageVersion()),
	m_shouldAdjustSize(rhs.m_shouldAdjustSize),
	m_pUserMessage(NULL),
	m_userMessageSize(0),
	m_pMessage(NULL),
	m_size(0),
	m_length(0)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AppendBuffer(rhs.m_pMessage, rhs.m_length));
}

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageBuilder::~MessageBuilder()
{
	if ((m_pUserMessage != m_pMessage) && (NULL != m_pMessage))
		std::free(m_pMessage);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline const MessageBuilder MessageBuilder::operator+(const MessageBuilder &rhs) const
{
	return MessageBuilder(*this) += rhs;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageBuilder& MessageBuilder::operator+=(const MessageBuilder &rhs)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AppendBuffer(rhs.m_pMessage, rhs.m_length));

	return *this;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageBuilder& MessageBuilder::operator=(const MessageBuilder &rhs)
{
	if (this != &rhs)
	{
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, SetLength(0));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, AppendBuffer(rhs.m_pMessage, rhs.m_length));
	}

	return *this;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Initialize(size_t initialSize, const bool shouldAdjustSize, const MessageVersion messageVersion)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AdjustSize(0));

	m_shouldAdjustSize = shouldAdjustSize;
	m_pUserMessage = NULL;
	m_userMessageSize = 0;
	m_pMessage = NULL;
	m_size = 0;

	if (0 != initialSize)
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AdjustSize(initialSize));
	}

	Reset(messageVersion);

	return STATUS_CODE_SUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Initialize(void *pMessage, const size_t size, const bool shouldAdjustSize, const MessageVersion messageVersion)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AdjustSize(0));

	m_shouldAdjustSize = shouldAdjustSize;
	m_pUserMessage = pMessage;
	m_userMessageSize = size;
	m_pMessage = pMessage;
	m_size = size;

	Reset(messageVersion);

	return STATUS_CODE_SUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline void MessageBuilder::Reset(const MessageVersion messageVersion)
{
	MessageHandler::Reset(messageVersion);
	m_length = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline const void *MessageBuilder::GetMessage() const
{
	return m_pMessage;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline size_t MessageBuilder::GetSize() const
{
	return m_size;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::SetSize(const size_t size)
{
	if (size < m_length)
		return STATUS_CODE_INVALID_PARAMETER;

	return AdjustSize(size);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline size_t MessageBuilder::GetLength() const
{
	return m_length;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::SetLength(const size_t length)
{
	if (length > m_length)
		return STATUS_CODE_INVALID_PARAMETER;

	m_length = length;

	return STATUS_CODE_SUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const void * const pBuffer, const size_t length)
{
	return AppendBuffer(pBuffer, length, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const void * const pValue, const size_t size, const Endian endian)
{
	return AppendBuffer(pValue, size, (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const char character)
{
	return AppendBuffer(&character, sizeof(character), false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const char character, const size_t count)
{
	return AppendBuffer(std::string(count, character).c_str(), count, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const int8_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const int16_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const int32_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const int64_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const std::string &str)
{
	if (str.length() > boost::integer_traits<StringLength>::const_max)
		return STATUS_CODE_INVALID_PARAMETER;

	StringLength length = static_cast<StringLength>(str.length());

#if (defined ACTIV_LITTLE_ENDIAN)
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AppendBuffer(&length, sizeof(length), false));
#else
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, AppendBuffer(&length, sizeof(length), true));
#endif

	return AppendBuffer(str.c_str(), length, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const uint8_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const uint16_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const uint32_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Append(const uint64_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

#if (defined ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
inline StatusCode MessageBuilder::Append(const size_t value, const Endian endian)
{
	return AppendBuffer(&value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}
#endif

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::AppendBoolean(const bool value)
{
	return Append(static_cast<uint8_t>(value ? 1 : 0), ENDIAN_LITTLE);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Erase(const size_t offset, const size_t length)
{
	return EraseBuffer(offset, length);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const void * const pBuffer, const size_t length)
{
	return InsertBuffer(offset, pBuffer, length, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const void * const pValue, const size_t size, const Endian endian)
{
	return InsertBuffer(offset, pValue, size, (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const char character)
{
	return InsertBuffer(offset, &character, sizeof(character), false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const char character, const size_t count)
{
	return InsertBuffer(offset, std::string(count, character).c_str(), count, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const int8_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const int16_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const int32_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const int64_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const std::string &str)
{
	if (str.length() > boost::integer_traits<StringLength>::const_max)
		return STATUS_CODE_INVALID_PARAMETER;

	StringLength length = static_cast<StringLength>(str.length());

#if (defined ACTIV_LITTLE_ENDIAN)
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, InsertBuffer(offset, &length, sizeof(length), false));
#else
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, InsertBuffer(offset, &length, sizeof(length), true));
#endif

	return InsertBuffer(offset + sizeof(length), str.c_str(), length, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const uint8_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const uint16_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const uint32_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Insert(const size_t offset, const uint64_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

#if (defined ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
inline StatusCode MessageBuilder::Insert(const size_t offset, const size_t value, const Endian endian)
{
	return InsertBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}
#endif

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::InsertBoolean(const size_t offset, const bool value)
{
	return Insert(offset, static_cast<uint8_t>(value ? 1 : 0), ENDIAN_LITTLE);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const void * const pBuffer, const size_t length)
{
	return OverwriteBuffer(offset, pBuffer, length, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const void * const pValue, const size_t size, const Endian endian)
{
	return OverwriteBuffer(offset, pValue, size, (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const char character)
{
	return OverwriteBuffer(offset, &character, sizeof(character), false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const char character, const size_t count)
{
	return OverwriteBuffer(offset, std::string(count, character).c_str(), count, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const int8_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const int16_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const int32_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const int64_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const std::string &str)
{
	if (str.length() > boost::integer_traits<StringLength>::const_max)
		return STATUS_CODE_INVALID_PARAMETER;

	StringLength length = static_cast<StringLength>(str.length());

#if (defined ACTIV_LITTLE_ENDIAN)
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, OverwriteBuffer(offset, &length, sizeof(length), false));
#else
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, OverwriteBuffer(offset, &length, sizeof(length), true));
#endif

	return OverwriteBuffer(offset + sizeof(length), str.c_str(), length, false);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const uint8_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const uint16_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const uint32_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::Overwrite(const size_t offset, const uint64_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}

// --------------------------------------------------------------------------------------------------------------------------------

#if (defined ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
inline StatusCode MessageBuilder::Overwrite(const size_t offset, const size_t value, const Endian endian)
{
	return OverwriteBuffer(offset, &value, sizeof(value), (ENDIAN_LOCAL == endian) ? false : true);
}
#endif

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode MessageBuilder::OverwriteBoolean(const size_t offset, const bool value)
{
	return Overwrite(offset, static_cast<uint8_t>(value ? 1 : 0), ENDIAN_LITTLE);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline size_t MessageBuilder::GetSpaceRequired(const size_t size) const
{
	return ((m_length + size <= m_userMessageSize) ? m_userMessageSize : (4 * (((std::max<size_t>(m_size + (m_size / 2), m_length + size) - 1) / 4) + 1)));
}

// --------------------------------------------------------------------------------------------------------------------------------

inline bool MessageBuilder::IsSpaceAvailable(const size_t size) const
{
	return (m_length + size <= m_size) ? true : false;
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_MESSAGE_BUILDER_H)
