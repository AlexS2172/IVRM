/**
 *  @file	MessageHandler.h
 *
 *	@brief	Header file for the base message handler class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MESSAGE_HANDLER_H)
#define ACTIV_MESSAGE_HANDLER_H

#include "ActivMiddleware/ActivMessageHandler/External.h"

namespace Activ
{

/**
 *	@brief	Message validater class.
 */
class ACTIV_MESSAGE_HANDLER_API MessageHandler
{
public:
	typedef uint16_t StringLength;								///< Definition of the string length type.
	typedef uint8_t MessageVersion;								///< Definition of the version type.

	static const byte_t MIN_PRINTABLE_ASCII = 0x20;				///< The minimum printable ascii character.
	static const byte_t MAX_PRINTABLE_ASCII = 0x7e;				///< The maximum printable ascii character.
	static const MessageVersion MESSAGE_VERSION_UNDEFINED = ~0;	///< Undefined version value.

	/**
	 *	@brief	Endian enumeration.
	 */
	enum Endian
	{
		ENDIAN_LITTLE,											///< Little endian 
		ENDIAN_BIG,												///< Big endian 
#if defined(ACTIV_LITTLE_ENDIAN)
		ENDIAN_LOCAL = ENDIAN_LITTLE,							///< Local endian 
#else
		ENDIAN_LOCAL = ENDIAN_BIG,								///< Local endian 
#endif
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	messageVersion optional version.
	 */
	MessageHandler(const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED);

	/**
	 *	@brief	Destructor.
	 */
	virtual ~MessageHandler() = 0;

	/**
	 *	@brief	Reset.
	 *
	 *	@param	messageVersion optional version.
	 */
	virtual void Reset(const MessageVersion messageVersion = MESSAGE_VERSION_UNDEFINED) = 0;

	/**
	 *	@brief	Get the version.
	 *
	 *	@return	The version.
	 */
	MessageVersion GetMessageVersion() const;

	/**
	 *	@brief	Set the version.
	 *
	 *	@param	messageVersion the version.
	 */
	void SetMessageVersion(const MessageVersion messageVersion);

private:
	MessageVersion	m_messageVersion;
};

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageHandler::MessageHandler(const MessageVersion messageVersion)
{
	MessageHandler::Reset(messageVersion);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline MessageHandler::~MessageHandler()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void MessageHandler::Reset(const MessageVersion messageVersion)
{
	SetMessageVersion(messageVersion);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline MessageHandler::MessageVersion MessageHandler::GetMessageVersion() const
{
	return m_messageVersion;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void MessageHandler::SetMessageVersion(const MessageVersion messageVersion)
{
	m_messageVersion = messageVersion;
}

} // namespace Activ

#endif // !defined (ACTIV_MESSAGE_HANDLER_H)
