/**
 *  @file	HeapMessageDeserializer.h
 *
 *	@brief	Header file for the heap message deserializer template class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_HEAP_MESSAGE_DESERIALIZER_H)
#define ACTIV_HEAP_MESSAGE_DESERIALIZER_H

#include "ActivMiddleware/ActivSystem/HeapMessage.h"
#include "ActivMiddleware/ActivMessageHandler/MessageValidater.h"

namespace Activ
{

/**
 *	@brief	Template class to wrap up deserializing a HeapMessage.
 *
 *	@param	MESSAGE_TYPE type of message to deserialize. Must have a StatusCode Deserialize(MessageValidater &) method.
 *	@param	BUFFER_SIZE default size of the internal buffer to use when message body is not contiguous.
 */
template<typename MESSAGE_TYPE, size_t BUFFER_SIZE>
class HeapMessageDeserializer
{
public:
	/**
	 *	@brief	Initialize by getting a pointer to a contiguous block containing the contents of a HeapMessage.
	 *
	 *	@param	heapMessage the HeapMessage to initialize from.
	 *	@param	messageVersion optional message version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 */
	StatusCode Initialize(const HeapMessage &heapMessage, const MessageHandler::MessageVersion messageVersion = MessageHandler::MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Deserialize from the current position.
	 *
	 *	@param	message object to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MESSAGE_TYPE &message);

	/**
	 *	@brief	Deserialize a the contents of a HeapMessage into an object.
	 *
	 *	Use this method if the HeapMessage contains only one instance of a message.
	 *
	 *	@param	heapMessage the HeapMessage to initialize from.
	 *	@param	message object to deserialize into.
	 *	@param	messageVersion optional message version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	StatusCode operator()(const HeapMessage &heapMessage, MESSAGE_TYPE &message, const MessageHandler::MessageVersion messageVersion = MessageHandler::MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Has all data been deserialized from the HeapMessage?
	 *
	 *	@retval	true
	 *	@retval	false
	 */
	bool IsEndOfMessage() const;

private:
	byte_t						m_buffer[BUFFER_SIZE];
	const void					*m_pData;
	size_t						m_dataLength;
	MessageValidater			m_validater;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename MESSAGE_TYPE, size_t BUFFER_SIZE>
inline StatusCode HeapMessageDeserializer<MESSAGE_TYPE, BUFFER_SIZE>::Initialize(const HeapMessage &heapMessage, const MessageHandler::MessageVersion messageVersion)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, heapMessage.Get(m_pData, m_dataLength, m_buffer, sizeof(m_buffer)));
	m_validater.Initialize(m_pData, m_dataLength, messageVersion);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename MESSAGE_TYPE, size_t BUFFER_SIZE>
inline StatusCode HeapMessageDeserializer<MESSAGE_TYPE, BUFFER_SIZE>::Deserialize(MESSAGE_TYPE &message)
{
	return message.Deserialize(m_validater);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename MESSAGE_TYPE, size_t BUFFER_SIZE>
inline StatusCode HeapMessageDeserializer<MESSAGE_TYPE, BUFFER_SIZE>::operator()(const HeapMessage &heapMessage, MESSAGE_TYPE &message, const MessageHandler::MessageVersion messageVersion)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Initialize(heapMessage, messageVersion));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Deserialize(message));

	return IsEndOfMessage() ? STATUS_CODE_SUCCESS : STATUS_CODE_INVALID_LENGTH;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename MESSAGE_TYPE, size_t BUFFER_SIZE>
inline bool HeapMessageDeserializer<MESSAGE_TYPE, BUFFER_SIZE>::IsEndOfMessage() const
{
	return m_validater.IsEndOfMessage();
}

} // namespace Activ

#endif // !defined (ACTIV_HEAP_MESSAGE_DESERIALIZER_H)
