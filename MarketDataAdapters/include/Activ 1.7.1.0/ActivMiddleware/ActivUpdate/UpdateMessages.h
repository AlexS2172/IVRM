/**
 *	@file	UpdateMessages.h
 *
 *	@brief	Header file for update service messages.
 *
 *  $Log: $
 */

#if !defined (ACTIV_UPDATE_MESSAGES_H)
#define ACTIV_UPDATE_MESSAGES_H

namespace Activ
{

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_UPDATE message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_UPDATE,	UPDATE_MESSAGE_UPDATE_AVAILABLE);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_UPDATE,	UPDATE_MESSAGE_RESTART_SYSTEM);

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Subject used when publishing update messages.
 */
class UpdateSubject : public Subject
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	UpdateSubject(const std::string &type);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline UpdateSubject::UpdateSubject(const std::string &type) :
	Subject("/system/update/" + type)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Udpate available message.
 */
class UpdateAvailableMessage
{
public:
	/**
	 *	@brief	Deserialize the message.
	 *
	 *	@param	messageValidator message validator from which the data will be extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief	Serialize the message.
	 *
	 *	@param	messageBuilder message builder into which data will be placed.
	 *	@param	updateDescriptionFileName.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &updateDescriptionFileName);

	std::string	m_updateDescriptionFileName;					///< The update description file name.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UpdateAvailableMessage::Serialize(MessageBuilder &messageBuilder, const std::string &updateDescriptionFileName)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(updateDescriptionFileName));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode UpdateAvailableMessage::Deserialize(MessageValidator &messageValidator)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateString(&m_updateDescriptionFileName));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

} // namespace Activ

#endif // !defined (ACTIV_UPDATE_MESSAGES_H)
