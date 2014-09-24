/**
 *  @file	ISerializableMessage.h
 *
 *	@brief	Header file for serializable message interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_ISERIALIZABLE_MESSAGE_H)
#define ACTIV_ISERIALIZABLE_MESSAGE_H

#include "ActivMiddleware/ActivMessageHandler/External.h"

namespace Activ
{

class MessageValidater;
class MessageBuilder;

/**
 *	@brief	Serializable message interface.
 */
class ISerializableMessage
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~ISerializableMessage() = 0;

	/**
	 *	@brief	Serialize the message.
	 *
	 *	@param	messageBuilder message builder into which data will be placed.
	 */
	virtual StatusCode Serialize(MessageBuilder &messageBuilder) const = 0;

	/**
	 *	@brief	Deserialize the message.
	 *
	 *	@param	messageValidater message validater from which the data will be extracted.
	 */
	virtual StatusCode Deserialize(MessageValidater &messageValidater) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISerializableMessage::~ISerializableMessage()
{
}

} // namespace Activ

#endif // !defined (ACTIV_ISERIALIZABLE_MESSAGE_H)
