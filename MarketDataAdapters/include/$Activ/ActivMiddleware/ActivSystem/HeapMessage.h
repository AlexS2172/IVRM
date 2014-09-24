/**
 *	@file	HeapMessage.h
 *
 *	@brief	Header file for heap message class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_HEAP_MESSAGE_H)
#define ACTIV_SYSTEM_HEAP_MESSAGE_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/Address.h"
#include "ActivMiddleware/ActivSystem/RequestId.h"
#include "ActivMiddleware/ActivSystem/Subject.h"
#include "ActivMiddleware/ActivIp/IpAddress.h"
#include "ActivMiddleware/ActivBase/ActivException.h"

namespace Activ
{

class MessageImpl;
class SystemImpl;
class ScatterBufferHelper;
class MessageBuilder;
class IHeap;

/**
 *	@brief	Heap message class.
 */ 
class HeapMessage
{
private:
	/**
	 *	@brief	Proxy class for non-const copy construction.
	 */
	class Proxy
	{
	public:
		/**
		 *	@brief	Constructor.
		 */
		Proxy(MessageImpl *pMessage, SystemImpl *pSystem);

		MessageImpl * const	m_pMessage;
		SystemImpl * const	m_pSystem;
	};

public:
	/**
	 *	@brief	Heap message iterator class.
	 */
	ACTIV_DECLARE_NESTED_CLASS(Iterator)
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SYSTEM_API Iterator();

		/**
		 *	@brief	Dereference operator.
		 *
		 *	@return	Reference to the current message.
		 */
		ACTIV_SYSTEM_API MessageImpl &operator*();

		/**
		 *	@brief	Member access operator.
		 *
		 *	@return	Pointer to the current message.
		 */
		ACTIV_SYSTEM_API MessageImpl *operator->();

		/**
		 *	@brief	Prefix increment operator.
		 *
		 *	@return	Reference to the updated object.
		 */
		ACTIV_SYSTEM_API Iterator &operator++();

		/**
		 *	@brief	Postfix increment operator.
		 *
		 *	@return	Copy of the original object.
		 */
		ACTIV_SYSTEM_API const Iterator operator++(int);

		/**
		*	@brief	Equality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are equal.
		*/
		ACTIV_SYSTEM_API bool operator==(const Iterator &rhs) const;

		/**
		*	@brief	Inequality operator.
		*
		*	@param	rhs the object to test against.
		*
		*	@return	whether the two objects are not equal.
		*/
		ACTIV_SYSTEM_API bool operator!=(const Iterator &rhs) const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	pHeapMessage the underlying heap message object.
		 *	@param	pMessage the current message object.
		 */
		Iterator(const HeapMessage *pHeapMessage, MessageImpl *pMessage);

		const HeapMessage	*m_pHeapMessage;					///< The underlying heap message object
		MessageImpl			*m_pMessage;						///< The current message object

		friend class HeapMessage;
	};

	class WriteHelper; // Forward declaration.

	/**
	 *	@brief	Heap message read helper class.
	 */
	ACTIV_DECLARE_NESTED_CLASS(ReadHelper)
	{
	public:
		/**
		 *	@brief	Default constructor.
		 *
		 *	@param	pHeapMessage the underlying heap message object.
		 *	@param	initialGetBufferSize the initial size of the get buffer.
		 *
		 *	@throw	ActivException
		 */
		ACTIV_SYSTEM_API ReadHelper(const HeapMessage *pHeapMessage = 0, const size_t initialGetBufferSize = 0);

		/**
		 *	@brief	Copy constructor.
		 *
		 *	@param	rhs the value to assign to this object.
		 *
		 *	@throw	ActivException
		 */
		ACTIV_SYSTEM_API ReadHelper(const ReadHelper &rhs);

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	rhs the value to assign to this object.
		 */
		ACTIV_SYSTEM_API ReadHelper(const WriteHelper &rhs);

		/**
		 *	@brief	Destructor.
		 */
		ACTIV_SYSTEM_API ~ReadHelper();

		/**
		 *	@brief	Assignment operator.
		 *
		 *	@param	rhs the value to assign to this object.
		 *
		 *	@return	a reference to this object.
		 *
		 *	@throw	ActivException
		 */
		ACTIV_SYSTEM_API ReadHelper& operator=(const ReadHelper &rhs);

		/**
		 *	@brief	Initialize with a new heap message object.
		 *
		 *	@param	pHeapMessage the underlying heap message object.
		 */
		ACTIV_SYSTEM_API void Initialize(const HeapMessage *pHeapMessage);

		/**
		 *	@brief	Initialize with the existing heap message object.
		 */
		ACTIV_SYSTEM_API void Initialize();

		/**
		 *	@brief	Copy data from the message.
		 *
		 *	@param	pData the address to copy to.
		 *	@param	dataLength the length to copy.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 */
		ACTIV_SYSTEM_API StatusCode Copy(void *pData, const size_t dataLength);

		/**
		 *	@brief	Copy data from the message.
		 *
		 *	@param	writeHelper the place to copy to.
		 *	@param	dataLength the length to copy.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_FAILURE
		 */
		ACTIV_SYSTEM_API StatusCode Copy(WriteHelper &writeHelper, const size_t dataLength);

		/**
		 *	@brief	Copy data from the message.
		 *
		 *	@param	messageBuilder the message builder.
		 *	@param	dataLength the length to copy.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval ...
		 */
		ACTIV_SYSTEM_API StatusCode Copy(MessageBuilder &messageBuilder, const size_t dataLength);

		/**
		 *	@brief	Get data from the message.
		 *
		 *	@param	pData reference to the address of the data.
		 *	@param	dataLength the length to get.
		 *	@param	pGetBuffer buffer to be used if data not contiguous.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_HEAP_EMPTY
		 */
		ACTIV_SYSTEM_API StatusCode Get(const void *&pData, const size_t dataLength, void *pGetBuffer = 0);

		/**
		 *	@brief	Get data from the message.
		 *
		 *	@param	pData reference to the address of the data.
		 *	@param	dataLength the length to get.
		 *	@param	dataLengthRead the length read.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 */
		ACTIV_SYSTEM_API StatusCode Get(const void *&pData, const size_t dataLength, size_t &dataLengthRead);

		/**
		 *	@brief	Get data from the message.
		 *
		 *	@param	scatterBufferHelper the scatter buffer helper.
		 *	@param	dataLength the length to get.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_HEAP_EMPTY
		 */
		ACTIV_SYSTEM_API StatusCode Get(ScatterBufferHelper &scatterBufferHelper, const size_t dataLength);

		/**
		 *	@brief	Get the the message length.
		 *
		 *	@return the message length.
		 */
		ACTIV_SYSTEM_API size_t GetLength() const;

		/**
		 *	@brief	Get the offset within the message.
		 *
		 *	@return the offset within the message.
		 */
		ACTIV_SYSTEM_API size_t GetOffset() const;

		/**
		 *	@brief	Increment the offset within the message.
		 *
		 *	@param	increment the value to increment the offset by.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 */
		ACTIV_SYSTEM_API StatusCode IncrementOffset(const size_t increment);

		/**
		 *	@brief	Set the offset within the message.
		 *
		 *	@param	offset the value to set the offset to.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 */
		ACTIV_SYSTEM_API StatusCode SetOffset(const size_t offset);

		/**
		*	@brief	Adjust the get buffer size.
		*
		*	@param	getBufferSize the size of the get buffer.
		*
		*	@retval STATUS_CODE_SUCCESS
		*	@retval STATUS_CODE_HEAP_EMPTY
		*/
		ACTIV_SYSTEM_API StatusCode AdjustGetBufferSize(const size_t getBufferSize) const;

	private:
		/**
		 *	@brief	Left->right data copy to the message.
		 *
		 *	@param	writeHelper the place to copy from.
		 *	@param	dataLength the length to copy.
		 */
		void LeftToRightCopy(WriteHelper &writeHelper, size_t &dataLength);

		/**
		 *	@brief	Recursive data copy to the message.
		 *
		 *	@param	writeHelper the place to copy from.
		 *	@param	dataLength the length to copy.
		 */
		void RecursiveCopy(WriteHelper &writeHelper, size_t &dataLength);

		const HeapMessage	*m_pHeapMessage;					///< The underlying heap message object
		Iterator			m_iterator;							///< The iterator (used for caching current position)
		size_t				m_absoluteOffset;					///< The offset within the heap message object
		size_t				m_relativeOffset;					///< The offset within the current message object

		mutable void		*m_pGetBuffer;						///< Pointer to get buffer
		mutable size_t		m_getBufferSize;					///< Total size of the get buffer

		friend class WriteHelper;
	};

	/**
	 *	@brief	Heap message write helper class.
	 */
	ACTIV_DECLARE_NESTED_CLASS(WriteHelper)
	{
	public:
		/**
		 *	@brief	Default constructor.
		 *
		 *	@param	pHeapMessage the underlying heap message object.
		 */
		ACTIV_SYSTEM_API WriteHelper(HeapMessage *pHeapMessage = 0);

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	rhs the value to assign to this object.
		 */
		ACTIV_SYSTEM_API WriteHelper(const ReadHelper &rhs);

		/**
		 *	@brief	Initialize with a new heap message object.
		 *
		 *	@param	pHeapMessage the underlying heap message object.
		 */
		ACTIV_SYSTEM_API void Initialize(HeapMessage *pHeapMessage);

		/**
		 *	@brief	Initialize with the existing heap message object.
		 */
		ACTIV_SYSTEM_API void Initialize();

		/**
		 *	@brief	Copy data to the message.
		 *
		 *	@param	pData the address to copy from.
		 *	@param	dataLength the length to copy.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_FAILURE
		 */
		ACTIV_SYSTEM_API StatusCode Copy(const void *pData, const size_t dataLength);

		/**
		 *	@brief	Copy data to the message.
		 *
		 *	@param	readHelper the place to copy from.
		 *	@param	dataLength the length to copy.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_FAILURE
		 */
		ACTIV_SYSTEM_API StatusCode Copy(ReadHelper &readHelper, const size_t dataLength);

		/**
		 *	@brief	Get data from the message.
		 *
		 *	@param	pData reference to the address of the data.
		 *	@param	dataLength the length got.
		 *	@param	dataLengthWritten the length written.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_FAILURE
		 */
		ACTIV_SYSTEM_API StatusCode Get(void *&pData, const size_t dataLength, size_t &dataLengthWritten);

		/**
		 *	@brief	Get the the message length.
		 *
		 *	@return the message length.
		 */
		ACTIV_SYSTEM_API size_t GetLength() const;

		/**
		 *	@brief	Get the offset within the message.
		 *
		 *	@return the offset within the message.
		 */
		ACTIV_SYSTEM_API size_t GetOffset() const;

		/**
		 *	@brief	Increment the offset within the message.
		 *
		 *	@param	increment the value to increment the offset by.
		 *	@param	updateLength whether the message length can be updated.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_FAILURE
		 */
		ACTIV_SYSTEM_API StatusCode IncrementOffset(const size_t increment, const bool updateLength = false);

		/**
		 *	@brief	Set the offset within the message.
		 *
		 *	@param	offset the value to set the offset to.
		 *	@param	updateLength whether the message length can be updated.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_FAILURE
		 */
		ACTIV_SYSTEM_API StatusCode SetOffset(const size_t offset, const bool updateLength = false);

	private:
		/**
		 *	@brief	Left->right data copy to the message.
		 *
		 *	@param	readHelper the place to copy from.
		 *	@param	dataLength the length to copy.
		 */
		void LeftToRightCopy(ReadHelper &readHelper, size_t &dataLength);

		/**
		 *	@brief	Recursive data copy to the message.
		 *
		 *	@param	readHelper the place to copy from.
		 *	@param	dataLength the length to copy.
		 */
		void RecursiveCopy(ReadHelper &readHelper, size_t &dataLength);

		HeapMessage		*m_pHeapMessage;						///< The underlying heap message object
		Iterator		m_iterator;								///< The iterator (used for caching current position)
		size_t			m_absoluteOffset;						///< The offset within the heap message object
		size_t			m_relativeOffset;						///< The offset within the current message object

		friend class ReadHelper;
	};

	/**
	 *	@brief	Status for a single HeapMessage, ignoring chaining.
	 */
	class Stats
	{
	public:
		HeapId		m_heapId;
		MessageId	m_messageId;
		BucketId	m_bucketId;
		MessageType	m_type;
		StatusCode	m_statusCode;
		Address		m_sourceAddress;
		Address		m_destinationAddress;
		Address		m_returnAddress;
		IpAddress	m_ipAddress;
		RequestId	m_requestId;
		Subject		m_subject;
		size_t		m_totalSpace;
		size_t		m_freeSpace;
		std::string	m_hexDump;									///< Hex dump of message body.
	};

	/**
	 *	@brief	Default Constructor.
	 */
	ACTIV_SYSTEM_API HeapMessage();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	proxy Proxy object to construct from.
	 */
	ACTIV_SYSTEM_API HeapMessage(Proxy proxy);

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	ACTIV_SYSTEM_API HeapMessage(HeapMessage &rhs);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_SYSTEM_API ~HeapMessage();

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	proxy Proxy object to assign from.
	 *
	 *	@return	a reference to this object.
	 */
	ACTIV_SYSTEM_API HeapMessage &operator=(Proxy proxy);

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	ACTIV_SYSTEM_API HeapMessage &operator=(HeapMessage &rhs);

	/**
	*	@brief	Equality operator.
	*
	*	@return	whether the two objects are equal.
	*/
	ACTIV_SYSTEM_API bool operator==(const HeapMessage &rhs) const;

	/**
	*	@brief	Inequality operator.
	*
	*	@return	whether the two objects are not equal.
	*/
	ACTIV_SYSTEM_API bool operator!=(const HeapMessage &rhs) const;

	/**
	 *	@brief	Logical not operator.
	 *
	 *	@return	whether a message is attached to the object.
	 */
	ACTIV_SYSTEM_API bool operator!() const;

	/**
	 *	@brief	Boolean cast operator.
	 *
	 *	@return	whether a message is attached to the object.
	 */
	ACTIV_SYSTEM_API operator bool() const;

	/**
	 *	@brief	Get the heap id.
	 *
	 *	@return	The heap id.
	 */
	ACTIV_SYSTEM_API const HeapId GetHeapId() const;

	/**
	 *	@brief	Get the message type.
	 *
	 *	@return	The message type.
	 */
	ACTIV_SYSTEM_API const MessageType GetType() const;

	/**
	 *	@brief	Set the message type.
	 *
	 *	@param	type the message type.
	 */
	ACTIV_SYSTEM_API void SetType(const MessageType type);

	/**
	 *	@brief	Get the message status code.
	 *
	 *	@return	The message status code.
	 */
	ACTIV_SYSTEM_API StatusCode GetStatusCode() const;

	/**
	 *	@brief	Set the message status code.
	 *
	 *	@param	statusCode the new message status code.
	 */
	ACTIV_SYSTEM_API void SetStatusCode(const StatusCode statusCode);

	/**
	 *	@brief	Get the source address.
	 *
	 *	@return	The source address.
	 */
	ACTIV_SYSTEM_API Address &GetSourceAddress() const;

	/**
	 *	@brief	Set the source address.
	 *
	 *	@param	sourceAddress the source address.
	 */
	ACTIV_SYSTEM_API void SetSourceAddress(const Address &sourceAddress);

	/**
	 *	@brief	Get the destination address.
	 *
	 *	@return	The destination address.
	 */
	ACTIV_SYSTEM_API Address &GetDestinationAddress() const;

	/**
	 *	@brief	Set the destination address.
	 *
	 *	@param	destinationAddress the destination address.
	 */
	ACTIV_SYSTEM_API void SetDestinationAddress(const Address &destinationAddress);

	/**
	 *	@brief	Get the return address.
	 *
	 *	@return	The return address.
	 */
	ACTIV_SYSTEM_API Address &GetReturnAddress() const;

	/**
	 *	@brief	Set the return address.
	 *
	 *	@param	returnAddress the return address.
	 */
	ACTIV_SYSTEM_API void SetReturnAddress(const Address &returnAddress);

	/**
	 *	@brief	Get the ip address.
	 *
	 *	@return	The ip address stored in the message.
	 */
	ACTIV_SYSTEM_API IpAddress &GetIpAddress() const;

	/**
	 *	@brief	Set the ip address.
	 *
	 *	@param	ipAddress the ip address to set.
	 */
	ACTIV_SYSTEM_API void SetIpAddress(const IpAddress &ipAddress);

	/**
	 *	@brief	Get the request id.
	 *
	 *	@return	A reference to the request id.
	 */
	ACTIV_SYSTEM_API RequestId &GetRequestId() const;

	/**
	 *	@brief	Set the syncronous request id.
	 *
	 *	@param	requestId the synchronous request id.
	 */
	ACTIV_SYSTEM_API void SetRequestId(const RequestId requestId);
	
	/**
	 *	@brief	Get a reference to the message subject.
	 *
	 *	@return	Reference to the message subject.
	 */
	ACTIV_SYSTEM_API Subject &GetSubject() const;

	/**
	 *	@brief	Set the message subject.
	 *
	 *	@param	subject reference to subject to set.
	 */
	ACTIV_SYSTEM_API void SetSubject(const Subject &subject);

	/**
	 *	@brief	Return the message to its heap.
	 */
	ACTIV_SYSTEM_API void Return();

	/**
	 *	@brief	Release the message pointer.
	 */
	ACTIV_SYSTEM_API void Release();

	/**
	 *	@brief	Get the message length.
	 *
	 *	@param	The current message length.
	 */
	ACTIV_SYSTEM_API size_t GetLength() const;

	/**
	 *	@brief	Increment the message length.
	 *
	 *	@param	length the amount to increment the message length by.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_SYSTEM_API StatusCode IncrementLength(size_t length);

	/**
	 *	@brief	Set the message length.
	 *
	 *	@param	length the length of the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_SYSTEM_API StatusCode SetLength(size_t length);

	/**
	 *	@brief	Append data.
	 *
	 *	@param	pData the address of the data to append to the message.
	 *	@param	dataLength the length of the data to append to the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_SYSTEM_API StatusCode Append(const void *pData, const size_t dataLength);

	/**
	 *	@brief	Copy data.
	 *
	 *	@param	offset the offset at which to copy data from the message.
	 *	@param	pData the address to copy data to.
	 *	@param	dataLength the length to copy.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_SYSTEM_API StatusCode Copy(const size_t offset, void *pData, const size_t dataLength) const;

	/**
	 *	@brief	Copy data.
	 *
	 *	@param	pData the address to copy data to.
	 *	@param	dataSize the size of data.
	 *	@param	dataLength the length copied.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_SYSTEM_API StatusCode Copy(void *pData, const size_t dataSize, size_t &dataLength) const;

	/**
	 *	@brief	Copy data.
	 *
	 *	@param	offset the offset at which to copy data from the message.
	 *	@param	messageBuilder the message builder.
	 *	@param	dataLength the length to copy.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode Copy(const size_t offset, MessageBuilder &messageBuilder, const size_t dataLength) const;

	/**
	 *	@brief	Copy data.
	 *
	 *	@param	messageBuilder the message builder.
	 *	@param	dataLength the length copied.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode Copy(MessageBuilder &messageBuilder, size_t &dataLength) const;

	/**
	 *	@brief	Erase data.
	 *
	 *	@param	offset the offset at which to erase in the message.
	 *	@param	dataLength the length of the data to erase from the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_SYSTEM_API StatusCode Erase(const size_t offset, const size_t dataLength);

	/**
	 *	@brief	Get data.
	 *
	 *	If the requested data is in a contiguous block, then no copying is required and pData
	 *	will be point to within the message. If not, the requested data will be copied into
	 *	either pGetBuffer (if it's non zero) or a newly allocated block of memory and pData
	 *	will point to that.
	 *
	 *	@param	offset the offset at which to copy data from the message.
	 *	@param	pData reference to the address of the data.
	 *	@param	dataLength the length to get.
	 *	@param	pGetBuffer buffer to be used if data not contiguous.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode Get(const size_t offset, const void *&pData, const size_t dataLength, void *pGetBuffer = 0) const;

	/**
	 *	@brief	Get data.
	 *
	 *	If the requested data is in a contiguous block, then no copying is required and pData
	 *	will be point to within the message. If not, the requested data will be copied into
	 *	either pGetBuffer (if it's non zero and big enough) or a newly allocated block of
	 *	memory and pData will point to that.
	 *
	 *	@param	pData reference to the address of the data.
	 *	@param	dataLength the length got.
	 *	@param	pGetBuffer buffer to be used if data not contiguous.
	 *	@param	getBufferSize the size of the get buffer.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode Get(const void *&pData, size_t &dataLength, void *pGetBuffer = 0, const size_t getBufferSize = 0) const;

	/**
	 *	@brief	Get data.
	 *
	 *	If the requested data is in a contiguous block, then dataLengthRead will be dataLength,
	 *	otherwise dataLengthRead will be the size of the available contiguous portion. In
	 *	both cases, pData will point to within the message.
	 *
	 *	@param	offset the offset at which to copy data from the message.
	 *	@param	pData reference to the address of the data.
	 *	@param	dataLength the length to get.
	 *	@param	dataLengthRead the length read.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_SYSTEM_API StatusCode Get(const size_t offset, const void *&pData, const size_t dataLength, size_t &dataLengthRead) const;

	/**
	 *	@brief	Get data.
	 *
	 *	@param	offset the offset at which to copy data from the message.
	 *	@param	scatterBufferHelper the scatter buffer helper.
	 *	@param	dataLength the length to get.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode Get(const size_t offset, ScatterBufferHelper &scatterBufferHelper, const size_t dataLength) const;

	/**
	 *	@brief	Get data.
	 *
	 *	@param	scatterBufferHelper the scatter buffer helper.
	 *	@param	dataLength the length got.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode Get(ScatterBufferHelper &scatterBufferHelper, size_t &dataLength) const;

	/**
	 *	@brief	Insert data.
	 *
	 *	@param	offset the offset at which to insert in the message.
	 *	@param	pData the address of the data to place in the message.
	 *	@param	dataLength the length of the data to place in the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_SYSTEM_API StatusCode Insert(const size_t offset, const void *pData, const size_t dataLength);

	/**
	 *	@brief	Overwrite data.
	 *
	 *	@param	offset the offset at which to overwrite in the message.
	 *	@param	pData the address of the data to place in the message.
	 *	@param	dataLength the length of the data to place in the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_SYSTEM_API StatusCode Overwrite(const size_t offset, const void *pData, const size_t dataLength);

	/**
	 *	@brief	Extend the message.
	 *
	 *	@param	size the size to extend the message by.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode Extend(const size_t size);

	/**
	 *	@brief	Attach another message to this object.
	 *
	 *	@param	hMessage reference to the message to be attached.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 */
	ACTIV_SYSTEM_API StatusCode Attach(HeapMessage &hMessage);

	/**
	 *	@brief	Clone another message.
	 *
	 *	@param	sourceHeapMessage reference to the message to be cloned.
	 *	@param	pHeap heap to get message copy from. Default is same heap as \a sourceHeapMessage.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_SYSTEM_API StatusCode Clone(const HeapMessage &sourceHeapMessage, IHeap * const pHeap = 0);

	/**
	 *	@brief	Build a hex dump of a message contents.
	 *
	 *	@param	bytesPerLine number of bytes to display per line.
	 *
	 *	@return	Formatted hex dump of the message.
	 */
	ACTIV_SYSTEM_API std::string ToHexDumpString(const size_t bytesPerLine = 16) const;

	/**
	 *	@brief	Is a message attached.
	 *
	 *	@return	whether a message is attached to this object.
	 */
	ACTIV_SYSTEM_API bool IsMessageAttached() const;

	/**
	 *	@brief	Get an iterator to the first message in the container.
	 *
	 *	@return an iterator object.
	 */
	ACTIV_SYSTEM_API Iterator Begin() const;

	/**
	 *	@brief	Get an iterator that points to beyond the last message in the container.
	 *
	 *	@return an iterator object.
	 */
	ACTIV_SYSTEM_API Iterator End() const;

	/**
	 *	@brief	Cast to Proxy object operator.
	 */
	ACTIV_SYSTEM_API operator Proxy();
	
private:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pMessage the underlying message.
	 *	@param	pSystem the owning system.
	 */
	HeapMessage(MessageImpl *pMessage, SystemImpl *pSystem);

	MessageImpl *		m_pMessage;								///< the underlying message
	SystemImpl *		m_pSystem;								///< the owning system

	mutable ReadHelper	m_readHelper;							///< read helper
	WriteHelper			m_writeHelper;							///< write helper

	friend class HeapMessageAccessor;
	friend class PrivateHeap;
	friend class SharedHeap;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::Proxy::Proxy(MessageImpl *pMessage, SystemImpl *pSystem) :
	m_pMessage(pMessage),
	m_pSystem(pSystem)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::HeapMessage() :
	m_pMessage(0),
	m_pSystem(0),
	m_readHelper(this),
	m_writeHelper(this)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::HeapMessage(HeapMessage &rhs) :
	m_pMessage(rhs.m_pMessage),
	m_pSystem(rhs.m_pSystem),
	m_readHelper(this),
	m_writeHelper(this)
{
	rhs.Release();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::HeapMessage(Proxy proxy) :
	m_pMessage(proxy.m_pMessage),
	m_pSystem(proxy.m_pSystem),
	m_readHelper(this),
	m_writeHelper(this)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::~HeapMessage()
{
	Return();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage &HeapMessage::operator=(Proxy proxy)
{
	Return();

	m_pMessage = proxy.m_pMessage;
	m_pSystem = proxy.m_pSystem;

	m_readHelper.Initialize(this);
	m_writeHelper.Initialize(this);

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage &HeapMessage::operator=(HeapMessage &rhs)
{
	if (&rhs != this)
	{
		Return();

		m_pMessage = rhs.m_pMessage;
		m_pSystem = rhs.m_pSystem;

		m_readHelper.Initialize(this);
		m_writeHelper.Initialize(this);

		rhs.Release();
	}

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HeapMessage::operator==(const HeapMessage &rhs) const
{
	return ((m_pMessage == rhs.m_pMessage) && (m_pSystem == rhs.m_pSystem));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HeapMessage::operator!=(const HeapMessage &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HeapMessage::operator!() const
{
	return !IsMessageAttached();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::operator bool() const
{
	return IsMessageAttached();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void HeapMessage::Release()
{
	m_pMessage = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HeapMessage::IsMessageAttached() const
{
	return (0 != m_pMessage);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::HeapMessage(MessageImpl *pMessage, SystemImpl *pSystem) :
	m_pMessage(pMessage),
	m_pSystem(pSystem),
	m_readHelper(this),
	m_writeHelper(this)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::Iterator HeapMessage::Begin() const
{
	return Iterator(this, m_pMessage);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::Iterator HeapMessage::End() const
{
	return Iterator(this, 0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::operator HeapMessage::Proxy()
{
	MessageImpl *pMessage = m_pMessage;
	SystemImpl *pSystem = m_pSystem;
	Release();

	return Proxy(pMessage, pSystem);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::Iterator::Iterator() :
	m_pHeapMessage(0),
	m_pMessage(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline MessageImpl &HeapMessage::Iterator::operator*()
{
	return *m_pMessage;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline MessageImpl *HeapMessage::Iterator::operator->()
{
	return m_pMessage;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const HeapMessage::Iterator HeapMessage::Iterator::operator++(int)
{
	Iterator oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HeapMessage::Iterator::operator==(const HeapMessage::Iterator &rhs) const
{
	return ((m_pHeapMessage == rhs.m_pHeapMessage) && (m_pMessage == rhs.m_pMessage));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool HeapMessage::Iterator::operator!=(const HeapMessage::Iterator &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::Iterator::Iterator(const HeapMessage *pHeapMessage, MessageImpl *pMessage) :
	m_pHeapMessage(pHeapMessage),
	m_pMessage(pMessage)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::ReadHelper::ReadHelper(const HeapMessage *pHeapMessage, const size_t initialGetBufferSize) :
	m_pHeapMessage(pHeapMessage),
	m_absoluteOffset(0),
	m_relativeOffset(0),
	m_pGetBuffer(NULL),
	m_getBufferSize(0)
{
	if (0 != m_pHeapMessage)
		m_iterator = m_pHeapMessage->Begin();

	if (0 != initialGetBufferSize)
	{
		StatusCode statusCode = AdjustGetBufferSize(initialGetBufferSize);

		if (STATUS_CODE_SUCCESS != statusCode)
			ACTIV_THROW(ActivException, statusCode);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::ReadHelper::ReadHelper(const ReadHelper &rhs) :
	m_pHeapMessage(rhs.m_pHeapMessage),
	m_iterator(rhs.m_iterator),
	m_absoluteOffset(rhs.m_absoluteOffset),
	m_relativeOffset(rhs.m_relativeOffset),
	m_pGetBuffer(NULL),
	m_getBufferSize(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::ReadHelper::ReadHelper(const WriteHelper &rhs) :
	m_pHeapMessage(rhs.m_pHeapMessage),
	m_iterator(rhs.m_iterator),
	m_absoluteOffset(rhs.m_absoluteOffset),
	m_relativeOffset(rhs.m_relativeOffset),
	m_pGetBuffer(NULL),
	m_getBufferSize(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::ReadHelper::~ReadHelper()
{
	std::free(m_pGetBuffer);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::ReadHelper& HeapMessage::ReadHelper::operator=(const ReadHelper &rhs)
{
	if (&rhs != this)
	{
		m_pHeapMessage = rhs.m_pHeapMessage;
		m_iterator = rhs.m_iterator;
		m_absoluteOffset = rhs.m_absoluteOffset;
		m_relativeOffset = rhs.m_relativeOffset;
	}

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void HeapMessage::ReadHelper::Initialize(const HeapMessage *pHeapMessage)
{
	m_pHeapMessage = pHeapMessage;

	Initialize();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void HeapMessage::ReadHelper::Initialize()
{
	m_iterator = ((0 != m_pHeapMessage) ? m_pHeapMessage->Begin() : Iterator());
	m_absoluteOffset = 0;
	m_relativeOffset = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t HeapMessage::ReadHelper::GetLength() const
{
	return m_pHeapMessage->GetLength();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t HeapMessage::ReadHelper::GetOffset() const
{
	return m_absoluteOffset;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::WriteHelper::WriteHelper(HeapMessage *pHeapMessage) :
	m_pHeapMessage(pHeapMessage),
	m_absoluteOffset(0),
	m_relativeOffset(0)
{
	if (0 != m_pHeapMessage)
		m_iterator = m_pHeapMessage->Begin();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline HeapMessage::WriteHelper::WriteHelper(const ReadHelper &rhs) :
	m_pHeapMessage(const_cast<HeapMessage *>(rhs.m_pHeapMessage)),
	m_iterator(rhs.m_iterator),
	m_absoluteOffset(rhs.m_absoluteOffset),
	m_relativeOffset(rhs.m_relativeOffset)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void HeapMessage::WriteHelper::Initialize(HeapMessage *pHeapMessage)
{
	m_pHeapMessage = pHeapMessage;

	Initialize();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void HeapMessage::WriteHelper::Initialize()
{
	m_iterator = ((0 != m_pHeapMessage) ? m_pHeapMessage->Begin() : Iterator());
	m_absoluteOffset = 0;
	m_relativeOffset = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t HeapMessage::WriteHelper::GetLength() const
{
	return m_pHeapMessage->GetLength();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t HeapMessage::WriteHelper::GetOffset() const
{
	return m_absoluteOffset;
}

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_HEAP_MESSAGE_H)
