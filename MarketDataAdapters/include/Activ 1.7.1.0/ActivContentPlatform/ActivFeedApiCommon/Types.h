/**
 *  @file	Types.h
 *
 *  @brief	General content gateway types.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_COMMON_TYPES_H)
#define ACTIV_FEED_API_COMMON_TYPES_H

#include "ActivContentPlatform/ActivFeedApiCommon/External.h"
#include "ActivContentPlatform/MessageClasses.h"
#include "ActivContentPlatform/ActivFeedCommon/TableNumbers.h"
#include "ActivContentPlatform/ActivFeedCommon/PermissionLevels.h"

#include "ActivMiddleware/Misc/SerializableContainerHelper.h"

namespace Activ
{

class MessageValidator;
class MessageBuilder;

namespace ContentPlatform
{

namespace FeedApi
{

/**
 *	@brief	Content gateway request types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_UNSUBSCRIBE_COOKIE_SYMBOL);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_FIRST);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_LAST);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_NEXT);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_PREVIOUS);
ACTIV_DECLARE_MESSAGE_TYPE(6,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_EQUAL);
ACTIV_DECLARE_MESSAGE_TYPE(7,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_MULTIPLE_EQUAL);
ACTIV_DECLARE_MESSAGE_TYPE(8,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_MULTIPLE_PATTERN_MATCH);
ACTIV_DECLARE_MESSAGE_TYPE(9,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_SUBSCRIBE_FEED);
ACTIV_DECLARE_MESSAGE_TYPE(10,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_UNSUBSCRIBE_FEED);
ACTIV_DECLARE_MESSAGE_TYPE(11,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_UNSUBSCRIBE_COOKIE);
ACTIV_DECLARE_MESSAGE_TYPE(12,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_FIRST_HISTORY);
ACTIV_DECLARE_MESSAGE_TYPE(13,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_NEXT_HISTORY);
ACTIV_DECLARE_MESSAGE_TYPE(14,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_FIRST_INTRADAY);
ACTIV_DECLARE_MESSAGE_TYPE(15,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_NEXT_INTRADAY);
ACTIV_DECLARE_MESSAGE_TYPE(16,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_FIRST_TICKS);
ACTIV_DECLARE_MESSAGE_TYPE(17,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_NEXT_TICKS);
ACTIV_DECLARE_MESSAGE_TYPE(18,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_UNSUBSCRIBE_ALL);
ACTIV_DECLARE_MESSAGE_TYPE(19,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_MULTIPLE_PATTERN_MATCH_LIST);
ACTIV_DECLARE_MESSAGE_TYPE(20,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_SUBSCRIPTION_INFO);
ACTIV_DECLARE_MESSAGE_TYPE(21,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_MATCH);
ACTIV_DECLARE_MESSAGE_TYPE(22,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_SUBSCRIBE_TABLE);
ACTIV_DECLARE_MESSAGE_TYPE(23,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_SYMBOLS);
ACTIV_DECLARE_MESSAGE_TYPE(24,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_MULTIPLE_MATCH);
ACTIV_DECLARE_MESSAGE_TYPE(25,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_SERVER_INFO);
ACTIV_DECLARE_MESSAGE_TYPE(26,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_PERMISSION_INFO);
ACTIV_DECLARE_MESSAGE_TYPE(27,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_REQUEST_GET_NEWS_STORIES);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Content gateway message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_RECORD_UPDATE);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_REPORT);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_GREETING);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_UNSUBSCRIBE_ALL);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_UNSUBSCRIBE_COOKIE_LIST);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_UNSUBSCRIBE_COOKIE_SYMBOL_LIST);
ACTIV_DECLARE_MESSAGE_TYPE(6,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_GET_USER_ID_COUNT);
ACTIV_DECLARE_MESSAGE_TYPE(7,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_USER_ID_COUNT);
ACTIV_DECLARE_MESSAGE_TYPE(8,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_SUB_LOGON);
ACTIV_DECLARE_MESSAGE_TYPE(9,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_SUB_LOGOFF);
ACTIV_DECLARE_MESSAGE_TYPE(10,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_FAILOVER);
ACTIV_DECLARE_MESSAGE_TYPE(11,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_STATS);
ACTIV_DECLARE_MESSAGE_TYPE(12,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_INTERNAL_LOGOFF);
ACTIV_DECLARE_MESSAGE_TYPE(13,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_CONTENT_GATEWAY_INFO_UPDATE);
ACTIV_DECLARE_MESSAGE_TYPE(14,	MESSAGE_CLASS_CONTENT_GATEWAY,	GATEWAY_MESSAGE_NEWS_UPDATE);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Symbol id.
 */
class SymbolId
{
public:
	typedef std::string Symbol;									///< underlying symbol type

	/**
	 *	@brief	Default constructor. Table number is set to TABLE_NO_UNDEFINED and context to CONTEXT_UNDEFINED.
	 */
	SymbolId();

	/**
	 *	@brief	Constructor. Table number is set to TABLE_NO_UNDEFINED and context to CONTEXT_UNDEFINED.
	 *
	 *	@param	symbol symbol string.
	 *	@param	context symbol context.
	 */
	SymbolId(const Symbol &symbol);

	/**
	 *	@brief	Constructor. Context is set to CONTEXT_UNDEFINED.
	 *
	 *	@param	tableNumber table number.
	 *	@param	symbol symbol string.
	 */
	SymbolId(const Feed::TableNo tableNumber, const Symbol &symbol);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	tableNumber table number.
	 *	@param	symbol symbol string.
	 *	@param	context symbol context.
	 */
	SymbolId(const Feed::TableNo tableNumber, const Symbol &symbol, const Feed::Context context);

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs SymbolId to compare to.
	 *
	 *	@return	true if this < rhs. Only m_symbol takes part in the comparison as it is unique.
	 */
	bool operator<(const SymbolId &rhs) const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs SymbolId to compare to.
	 *
	 *	@return	true if this == rhs. Only m_symbol takes part in the comparison as it is unique.
	 */
	bool operator==(const SymbolId &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs SymbolId to compare to.
	 *
	 *	@return	true if this != rhs. Only m_symbol takes part in the comparison as it is unique.
	 */
	bool operator!=(const SymbolId &rhs) const;

	Feed::TableNo	m_tableNumber;								///< table number
	Feed::Context	m_context;									///< context
	Symbol			m_symbol;									///< symbol string
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline SymbolId::SymbolId() :
	m_tableNumber(Feed::TABLE_NO_UNDEFINED),
	m_context(Feed::CONTEXT_UNDEFINED)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SymbolId::SymbolId(const Symbol &symbol) :
	m_tableNumber(Feed::TABLE_NO_UNDEFINED),
	m_context(Feed::CONTEXT_UNDEFINED),
	m_symbol(symbol)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SymbolId::SymbolId(const Feed::TableNo tableNumber, const Symbol &symbol) :
	m_tableNumber(tableNumber),
	m_context(Feed::CONTEXT_UNDEFINED),
	m_symbol(symbol)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SymbolId::SymbolId(const Feed::TableNo tableNumber, const Symbol &symbol, const Feed::Context context) :
	m_tableNumber(tableNumber),
	m_context(context),
	m_symbol(symbol)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool SymbolId::operator==(const SymbolId &rhs) const
{
	return m_symbol == rhs.m_symbol;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool SymbolId::operator<(const SymbolId &rhs) const
{
	return m_symbol < rhs.m_symbol;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool SymbolId::operator!=(const SymbolId &rhs) const
{
	return !(*this == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of SymbolId.
 */
typedef std::vector<SymbolId> SymbolIdList;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of field status values.
 */
#define ACTIV_FIELD_STATUS_TYPE_LIST(d)							\
	d(0,	FIELD_STATUS_DEFINED,			"defined")			\
	d(1,	FIELD_STATUS_UNDEFINED,			"undefined")		\
	d(2,	FIELD_STATUS_NOT_FOUND,			"not found")		\
	d(3,	FIELD_STATUS_NOT_PERMISSIONED,	"not permissioned")

typedef uint8_t FieldStatus;									///< Field status type

#define ACTIV_DECLARE_CONSTANT_FIELD_STATUS(val, id, name)	const FieldStatus id = val;

ACTIV_FIELD_STATUS_TYPE_LIST(ACTIV_DECLARE_CONSTANT_FIELD_STATUS)

#undef ACTIV_DECLARE_CONSTANT_FIELD_STATUS

/**
 *	@brief	Return field status as string.
 *
 *	@param	fieldStatus.
 *
 *	@return	String representation of \a fieldStatus.
 */
ACTIV_FEED_API_COMMON_API std::string FieldStatusToString(const FieldStatus fieldStatus);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of user type values.
 */
#define ACTIV_USER_TYPE_LIST(d)										\
	d(0,	USER_TYPE_API,					"Api")					\
	d(1,	USER_TYPE_WORKSTATION,			"Workstation")			\
	d(2,	USER_TYPE_ISV,					"Isv")					\
	d(3,	USER_TYPE_WORKSTATION_SERVICE,	"Workstation Service")

typedef uint8_t UserType;									///< User type type

#define ACTIV_DECLARE_CONSTANT_USER_TYPE(val, id, name)	const UserType id = val;

ACTIV_USER_TYPE_LIST(ACTIV_DECLARE_CONSTANT_USER_TYPE)

#undef ACTIV_DECLARE_CONSTANT_USER_TYPE

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Return user type as string.
 *
 *	@param	userType.
 *
 *	@return	String representation of \a userType.
 */
ACTIV_FEED_API_COMMON_API std::string UserTypeToString(const UserType userType);

// ---------------------------------------------------------------------------------------------------------------------------------

#define ACTIV_DECLARE_FLAGS(value, id)	static const Flags id = value;

/**
 *	@brief	Information about the ContentGateway.
 */
class ContentGatewayInfo
{
public:
	typedef std::vector<Feed::PermissionLevel>	PermissionLevelList;

	#define ACTIV_CONTENT_GATEWAY_INFO_FLAG_LIST(d)																					\
		d(0x00,	FLAG_NONE)										/** Nothing has changed */											\
		d(0x01,	FLAG_HISTORY_SERVICE_CHANGED)					/** The availabilty of the history service has changed */			\
		d(0x02,	FLAG_SYMBOL_DIRECTORY_SERVICE_CHANGED)			/** The availabilty of the symbol directory service has changed */	\
		d(0x04,	FLAG_NEWS_SERVER_SERVICE_CHANGED)				/** The availabilty of the news server service has changed */

	typedef uint8_t Flags;										///< Update flags
	ACTIV_CONTENT_GATEWAY_INFO_FLAG_LIST(ACTIV_DECLARE_FLAGS)

	Flags				m_flags;
	ISystem::SystemInfo	m_systemInfo;							///< info about the CG machine
	bool				m_isHistoryServiceAvailable;
	bool				m_isSymbolDirectoryServiceAvailable;
	bool				m_isNewsServerServiceAvailable;
	PermissionLevelList	m_permissionLevelList;					///< available permission levels on the CG
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Information about client's permissioning.
 */
class PermissionInfo
{
public:
	/**
	 *	@brief	Information about a particular permission id for the client.
	 */
	class PermissionIdInfo
	{
	public:
		enum Type
		{
			TYPE_SOME_FIELDS,									///< Permissioned fields are in m_fieldIdList
			TYPE_ALL_FIELDS										///< All fields are permissioned, m_fieldIdList will be empty
		};

		Type				m_type;								///< Partial or full permissioning?
		std::string			m_name;								///< Name of permission id
		Feed::FieldIdList	m_fieldIdList;						///< List of fields permissioned for this permission id, if TYPE_SOME_FIELDS
	};

	typedef std::map<Feed::PermissionId, PermissionIdInfo> PermissionIdInfoMap;

	PermissionIdInfoMap		m_permissionLevelInfo[Feed::PERMISSION_LEVEL_COUNT];
	uint32_t				m_subscriptionQuota;				///< Maximum number of subscriptions allowed
};

#undef ACTIV_DECLARE_FLAGS

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_COMMON_TYPES_H)
