/**
 *	@file	PostMessageHelper.h
 *
 *	@brief	Header file for PostMessageHelper.
 *
 *  $Log: $
 */

#if (!defined ACTIV_SYSTEM_POST_MESSAGE_HELPER_H)
#define ACTIV_SYSTEM_POST_MESSAGE_HELPER_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/Port.h"

namespace Activ
{

/**
 *	@brief	Helper class to post messages.
 *
 *	This class provides a similar interface to the Port message posting functions. In addition,
 *	it will post multiple messages to a target endpoint in one go, saving multiple acquiring
 *	of locks, kernel transitions, etc.
 *
 *	The intended use is when the processing of a single message will result in the sending of
 *	multiple messages to the same destination endpoint. For example:
 *
 *	MyPort::ProcessMyMessage(heapMessage)
 *	{
 *		for (...)
 *		{
 *			HeapMessage heapMessage2;
 *			heapMessage2.Clone(heapMessage);
 *			m_postMessageHelper.PostMessage(*this, m_targetAddress, MY_TYPE, heapMessage2);
 *		}
 *		m_postMessageHelper.Flush();
 *	}
 *
 *	The class is not thread-safe and is only intended to be used from a single thread.
 *
 *	Note that if the target endpoint for successive PostMessage() calls is not the same, an
 *	implicit Flush() call is made.
 */
class PostMessageHelper
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pSystem.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SYSTEM_API PostMessageHelper(ISystem * const pSystem);
	
	/**
	 *	@brief	Destructor.
	 */
	ACTIV_SYSTEM_API ~PostMessageHelper();
		
	/**
	 *	@brief	Post a message, setting destination address and type.
	 *
	 *	@param	port reference to the Port to post from.
	 *	@param	destinationAddress the target address.
	 *	@param	messageType the message type.
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to post.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode PostMessage(Port &port, const Address &destinationAddress, const MessageType messageType,
											const MessageBuilder &messageBuilder);

	/**
	 *	@brief	Post a message, setting destination address and type.
	 *
	 *	@param	port reference to the Port to post from.
	 *	@param	destinationAddress the target address.
	 *	@param	messageType the message type.
	 *	@param	pMessage the message buffer.
	 *	@param	messageLength the length of the message buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 */
	ACTIV_SYSTEM_API StatusCode PostMessage(Port &port, const Address &destinationAddress, const MessageType messageType, 
											const void *pMessage = 0, const size_t messageLength = 0);

	/**
	 *	@brief	Post a message, setting destination address and type.
	 *
	 *	@param	port reference to the Port to post from.
	 *	@param	destinationAddress the target address.
	 *	@param	messageType the message type.
	 *	@param	message the message.
	 */
	ACTIV_SYSTEM_API void PostMessage(Port &port, const Address &destinationAddress, const MessageType messageType, HeapMessage &message);

	/**
	 *	@brief	Post a fully initialized message (type, src, dest addresses set as appropriate).
	 *
	 *	@param	message the message.
	 */
	ACTIV_SYSTEM_API void PostMessage(HeapMessage &heapMessage);
											
	/**
	 *	@brief	Flush the queued messages to their target endpoint.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT
	 */
	ACTIV_SYSTEM_API StatusCode Flush();
	
	/**
	 *	@brief	Get the stats for the buffering endpoint.
	 *
	 *	@param	stats reference to an IEndPoint::Stats object.
	 */
	ACTIV_SYSTEM_API void GetStats(IEndPoint::Stats &stats) const;

private:
	ISystem * const		m_pSystem;
	IEndPoint * const	m_pTempEndPoint;
	
	EndPointId			m_targetEndPointId;
};

} // namespace Activ

#endif // (!defined ACTIV_SYSTEM_POST_MESSAGE_HELPER_H)
