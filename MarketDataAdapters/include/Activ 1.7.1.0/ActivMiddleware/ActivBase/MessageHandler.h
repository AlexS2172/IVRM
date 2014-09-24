/**
 *  @file	MessageHandler.h
 *
 *	@brief	Header file for the base message handler class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_MESSAGE_HANDLER_H)
#define ACTIV_BASE_MESSAGE_HANDLER_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Base class for validator and builder.
 */
class MessageHandler
{
public:
	/**
	 *	@brief	Helper class to scope changes to message version.
	 */
	class ScopedVersion
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	Caches the current message version to restore when this object is destructed.
		 *
		 *	@param	messageHandler MessageHandler to set version of.
		 *	@param	newVersion version to set.
		 */
		ScopedVersion(MessageHandler &messageHandler, const byte_t newVersion);

		/**
		 *	@brief	Destructor.
		 *
		 *	The original message version is restored.
		 */
		~ScopedVersion();

	private:
		MessageHandler &m_messageHandler;
		const byte_t	m_oldVersion;
	};

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
#if (defined ACTIV_LITTLE_ENDIAN)
		ENDIAN_LOCAL = ENDIAN_LITTLE							///< Local endian 
#else
		ENDIAN_LOCAL = ENDIAN_BIG								///< Local endian 
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
	 *	@param	messageVersion the new version to set.
	 *
	 *	@return	The previous version.
	 */
	MessageVersion SetMessageVersion(const MessageVersion messageVersion);

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

inline MessageHandler::MessageVersion MessageHandler::SetMessageVersion(const MessageVersion messageVersion)
{
	const MessageVersion oldMessageVersion = m_messageVersion;
	
	m_messageVersion = messageVersion;
	
	return oldMessageVersion;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline MessageHandler::ScopedVersion::ScopedVersion(MessageHandler &messageHandler, const byte_t newVersion) :
	m_messageHandler(messageHandler),
	m_oldVersion(m_messageHandler.SetMessageVersion(newVersion))
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline MessageHandler::ScopedVersion::~ScopedVersion()
{
	(void) m_messageHandler.SetMessageVersion(m_oldVersion);
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_MESSAGE_HANDLER_H)
