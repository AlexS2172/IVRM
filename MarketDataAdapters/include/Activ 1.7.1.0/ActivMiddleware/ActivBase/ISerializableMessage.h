/**
 *  @file	ISerializableMessage.h
 *
 *	@brief	Header file for serializable message interface class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_ISERIALIZABLE_MESSAGE_H)
#define ACTIV_BASE_ISERIALIZABLE_MESSAGE_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

class MessageValidator;
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
	 *	@param	messageValidator message validator from which the data will be extracted.
	 */
	virtual StatusCode Deserialize(MessageValidator &messageValidator) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ISerializableMessage::~ISerializableMessage()
{
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_ISERIALIZABLE_MESSAGE_H)
