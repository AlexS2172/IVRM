/**
 *  @file	RealtimeTypes.h
 *
 *  @brief	Types for realtime data.
 *
 *  $Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_REALTIME_TYPES_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_REALTIME_TYPES_H

#include "ActivContentPlatform/ActivContentGatewayCommon/External.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/Types.h"
#include "ActivContentPlatform/ActivFeedCommon/FieldIds.h"
#include "ActivContentPlatform/ActivFeedCommon/RelationshipIds.h"
#include "ActivContentPlatform/ActivFeedCommon/UpdateIds.h"
#include "ActivContentPlatform/ActivFeedCommon/EventTypes.h"
#include "ActivContentPlatform/ActivFeedCommon/PermissionLevels.h"

#include "ActivMiddleware/Misc/SerializableBuffer.h"

#include "boost/integer_traits.hpp"

namespace Activ
{

namespace ContentPlatform
{

typedef uint8_t RequestBlockCount;								///< Type for counting request blocks
typedef uint8_t ResponseBlockCount;								///< Type for counting response blocks
typedef std::vector<Feed::TableNo> TableNoList;					///< Type of list of table numbers

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Parameters for specifying subscription options.
 */
class SubscribeParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	m_type is set to TYPE_NONE
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API SubscribeParameters();

	enum Type
	{
		TYPE_NONE,												///< Don't subscribe
		TYPE_ADD_OR_DELETE,										///< Only subscribe to adds or deletes
		TYPE_EVENT_TYPE_FILTER_INCLUDE_LIST,					///< Subscribe to adds, deletes + the event types in m_eventTypeList
		TYPE_EVENT_TYPE_FILTER_EXCLUDE_LIST,					///< Subscribe to adds, deletes + the event types not in m_eventTypeList
		TYPE_FULL,												///< Subscribe to all updates
	};

	typedef std::vector<Feed::EventType> EventTypeList;			///< Type of list of event types

	Type			m_type;										///< Type of subscription
	EventTypeList	m_eventTypeList;							///< List of event types to filter by, if TYPE_EVENT_TYPE_LIST

	class Serializer;											///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Parameters for SubscribeToFeed() method.
 */
class SubscribeFeedParameters : public SubscribeParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	m_type is set to TYPE_FULL
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API SubscribeFeedParameters();

	Feed::PermissionLevel	m_permissionLevel;						///< Permission level to subscribe to feed with

	class Serializer;												///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Parameters for SubscribeToTable() method.
 */
class SubscribeTableParameters : public SubscribeParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	m_type is set to TYPE_FULL
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API SubscribeTableParameters();

	TableNoList				m_tableNumberList;						///< List of table numbers to subscribe to
	Feed::PermissionLevel	m_permissionLevel;						///< Permission level to subscribe to the tables with
	RequestId				m_updateRequestId;						///< Id of this request for Workstation Service use

	class Serializer;												///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request block.
 */
class RequestBlock
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API RequestBlock();

	/**
	 *	@brief	Clear down.
	 *
	 *	Clears field list, resets flags and relationship id to RELATIONSHIP_ID_NONE.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API void Clear();

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder message builder.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Deserialize(MessageValidater &messageValidater);

	typedef uint16_t Flags;										///< flags type

	static const Flags FLAG_NONE = 0x0000;
	static const Flags FLAG_ALL_FIELDS = 0x0001;				///< return all fields permissioned for

	Flags					m_flags;							///< flags for this request block
	Feed::RelationshipId	m_relationshipId;					///< relationship id
	FieldIdList				m_fieldIdList;						///< field list
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of RequestBlock in contiguous form.
 */
typedef std::vector<RequestBlock> RequestBlockList;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters that apply to all requests.
 */
class RealtimeRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	m_flags is set to FLAG_NONE
	 *	m_permissionLevel is set to PERMISSION_LEVEL_DEFAULT
	 *	m_maxResponseBlocks is set to DEFAULT_MAX_RESPONSE_BLOCKS
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API RealtimeRequestParameters();

	/**
	 *	@brief	Clear down request block list. All other members are left alone.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API void Clear();

	typedef uint16_t Flags;										///< Flags that apply to all request types
	static const Flags FLAG_NONE = 0x0000;
	static const Flags FLAG_IGNORE_ALIAS = 0x0001;				///< Any aliasing will be ignored, the response will be the alias record

	static const ResponseBlockCount DEFAULT_MAX_RESPONSE_BLOCKS = 64;

	Flags					m_flags;							///< Flags for the request
	Feed::PermissionLevel	m_permissionLevel;					///< Requested permission level
	ResponseBlockCount		m_maxResponseBlocks;				///< Max response blocks
	RequestBlockList		m_requestBlockList;					///< List of request blocks for this request
	SubscribeParameters		m_subscribeParameters;				///< Options for subscribing to results of request

	class Serializer;											///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters for table number based requests
 */
class TableNumberRequestParameters : public RealtimeRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API TableNumberRequestParameters();

	Feed::TableNo	m_tableNumber;								///< Table to search
	uint16_t		m_numberOfRecords;							///< Number of records to return

	class Serializer;											///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters for SymbolId based requests.
 */
class SymbolIdRequestParameters : public RealtimeRequestParameters
{
public:
	SymbolId	m_symbolId;

	class Serializer;											///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters for SymbolId based requests with count (for get next, previous)
 */
class SymbolIdCountRequestParameters : public RealtimeRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API SymbolIdCountRequestParameters();

	SymbolId	m_symbolId;										///< Symbol to start from
	uint16_t	m_numberOfRecords;								///< Number of records to return

	class Serializer;											///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters for SymbolIdList based requests.
 */
class SymbolIdListRequestParameters : public RealtimeRequestParameters
{
public:
	SymbolIdList	m_symbolIdList;

	class Serializer;											///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters for pattern match on symbol based requests.
 */
class PatternMatchRequestParameters : public RealtimeRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API PatternMatchRequestParameters();

	typedef uint16_t RequestFlags;									///< Flags for this request only
	static const RequestFlags REQUEST_FLAG_NONE = 0x0000;
	static const RequestFlags REQUEST_FLAG_WATCH = 0x0001;			///< Watch for new matches to the pattern
	static const RequestFlags REQUEST_FLAG_REVERSE_ORDER = 0x0002;	///< Return the results in reverse order

	Feed::TableNo	m_tableNumber;
	std::string		m_symbolPattern;
	RequestFlags	m_requestFlags;

	class Serializer;												///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters for pattern match on symbol based requests.
 */
class PatternMatchListRequestParameters : public RealtimeRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API PatternMatchListRequestParameters();

	typedef uint16_t RequestFlags;									///< Flags for this request only
	static const RequestFlags REQUEST_FLAG_NONE = 0x0000;
	static const RequestFlags REQUEST_FLAG_WATCH = 0x0001;			///< Watch for new matches to the pattern
	static const RequestFlags REQUEST_FLAG_REVERSE_ORDER = 0x0002;	///< Return the results in reverse order

	Feed::TableNo				m_tableNumber;
	std::vector<std::string>	m_symbolPatternList;
	RequestFlags				m_requestFlags;

	class Serializer;												///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters for match request.
 */
class MatchRequestParameters : public RealtimeRequestParameters
{
public:
	/**
	 *	@brief	Match types.
	 */
	enum MatchType
	{
		MATCH_TYPE_COMPOSITE,									///< Look for composite symbol, or first regional
	};

	SymbolId	m_symbolId;										///< Partial symbol to search for
	MatchType	m_matchType;									///< Match type

	class Serializer;											///< Hidden class to (de) serialize this class
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Opaque field data block.
 */
class FieldData
{
public:
	/**
	 *	@brief	Is field data empty?
	 *
	 *	@return	true if there is no field data.
	 */
	bool IsEmpty() const;

	typedef SerializableBuffer<uint16_t> Buffer;				///< Serialized field data buffer type

	MessageHandler::MessageVersion	m_messageVersion;			///< Version of serialized field data
	Buffer							m_buffer;					///< Buffer containing field data
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldData::IsEmpty() const
{
	return 0 == m_buffer.m_dataLength;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Response block.
 */
class ResponseBlock
{
public:
	typedef uint8_t Status;										///< Status type

	/**
	 *	@brief	List of response block status values.
	 */
	#define ACTIV_RESPONSE_BLOCK_STATUS_LIST(d)											\
		d(0,	STATUS_SUCCESS, "success")												\
		d(1,	STATUS_STALE, "stale")													\
		d(2,	STATUS_SOURCE_NOT_FOUND, "source not found")							\
		d(3,	STATUS_SOURCE_NOT_PERMISSIONED, "source not permissioned")				\
		d(4,	STATUS_RELATIONSHIP_NOT_FOUND, "relationship not found")				\
		d(5,	STATUS_RELATIONSHIP_NOT_PERMISSIONED, "relationship not permissioned")	\
		d(6,	STATUS_NAVIGATION_NOT_FOUND, "navigation not found")					\
		d(7,	STATUS_NAVIGATION_NOT_PERMISSIONED, "navigation not permissioned")		\
		d(8,	STATUS_TARGET_NOT_FOUND, "target not found")							\
		d(9,	STATUS_TARGET_NOT_PERMISSIONED, "target not permissioned")				\
		d(10,	STATUS_INTERNAL_ERROR, "internal error")								\
		d(11,	STATUS_INVALID_SOURCE_TABLE, "invalid source table")					\
		d(12,	STATUS_PERMISSION_LEVEL_NOT_AVAILABLE, "permission level not available")

	/**
	 *	@brief	Return string representation of a response status.
	 *
	 *	@param	status value to convert.
	 *
	 *	@return	String representation of \a status.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API static std::string StatusToString(const Status status);

	/**
	 *	@brief	Is this a valid response?
	 *
	 *	A valid response is either STATUS_SUCCESS or STATUS_STALE.
	 *
	 *	@return	True if status is STATUS_SUCCESS or STATUS_STALE.
	 */
	bool IsValidResponse() const;

	/**
	 *	@brief	Is response key defined in this response block?
	 *
	 *	Some error response status values will not have m_responseKey defined.
	 *
	 *	@return	True if \a m_responseKey is set.
	 */
	bool IsResponseKeyDefined() const;

	Status						m_status;						///< status
	SymbolId					m_requestKey;					///< fully qualified request key
	SymbolId					m_responseKey;					///< response key
	Feed::RelationshipId		m_relationshipId;				///< relationship id
	Feed::PermissionLevel		m_permissionLevel;				///< permission level of the record
	Feed::PermissionLevelData	m_permissionLevelData;			///< permission level specific data (eg delay period)
	FieldData					m_fieldData;					///< field data

	/**
	 *	@brief	Macro to declare constant response statuses.
	 */
	#define ACTIV_DECLARE_RESPONSE_BLOCK_STATUS(value, id, name)	static const Status id = value;

	/**
	 *	@brief	Status code constants.
	 */
	ACTIV_RESPONSE_BLOCK_STATUS_LIST(ACTIV_DECLARE_RESPONSE_BLOCK_STATUS)

	#undef ACTIV_DECLARE_RESPONSE_BLOCK_STATUS
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool ResponseBlock::IsValidResponse() const
{
	switch (m_status)
	{
		case STATUS_SUCCESS:
		case STATUS_STALE:
			return true;

		default:
			return false;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool ResponseBlock::IsResponseKeyDefined() const
{
	switch (m_status)
	{
		case STATUS_SUCCESS:
		case STATUS_STALE:
		case STATUS_TARGET_NOT_FOUND:
		case STATUS_TARGET_NOT_PERMISSIONED:
		case STATUS_PERMISSION_LEVEL_NOT_AVAILABLE:
			return true;

		default:
			return false;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of response blocks.
 */
typedef std::vector<ResponseBlock> ResponseBlockList;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Cookie used to unsubscribe from a request's response keys.
 */
typedef uint32_t SubscriptionCookie;

const SubscriptionCookie SUBSCRIPTION_COOKIE_UNDEFINED = ~0;

/**
 *	@brief	List of subscription cookies.
 */
typedef std::vector<SubscriptionCookie> SubscriptionCookieList;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Info returned about a subscription.
 */
class SubscriptionInfo
{
public:
	SymbolId	m_symbolId;
	uint32_t	m_subscriptionCount;
};

/**
 *	@brief	List of subscription infos.
 */
typedef std::vector<SubscriptionInfo> SubscriptionInfoList;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Response parameters from a realtime request.
 */
class RealtimeResponseParameters
{
public:
	/**
	 *	@brief	Clear down response block list.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API void Clear();

	ResponseBlockList	m_responseBlockList;					///< List of response blocks from the request
	SubscriptionCookie	m_subscriptionCookie;					///< Cookie that can be used to unsubscribe from response keys
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Record update.
 */
class RecordUpdate
{
public:
	typedef uint8_t Flags;										///< Update flags

	static const Flags FLAG_NONE = 0x00;						///< Nominal update
	static const Flags FLAG_DELETE = 0x01;						///< Update is a delete for a record
	static const Flags FLAG_STALE = 0x02;						///< Update is potentially out of date
	static const Flags FLAG_REFRESH = 0x04;						///< Update is a refresh
	static const Flags FLAG_REQUEST_KEY = 0x08;					///< Update contains requested key
	static const Flags FLAG_RELATIONSHIP_ID = 0x10;				///< Update contains relationship id

	/**
	 *	@brief	Convert flags to a printable string. Mainly for debugging purposes.
	 *
	 *	@param	flags update flags.
	 *
	 *	@return	String representation of \a flags.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API static std::string FlagsToString(const Flags flags);

	/**
	 *	@brief	Is the update a new record update?
	 *
	 *	@return	true if new record.
	 */
	bool IsNewRecord() const;

	/**
	 *	@brief	Is the update a delete?
	 *
	 *	@return	true if a delete.
	 */
	bool IsDelete() const;

	Flags						m_flags;						///< Update flags
	SymbolId					m_symbolId;						///< Symbol of the updated record
	SymbolId					m_requestKey;					///< Original requested key (if FLAG_REQUEST_KEY)
	Feed::RelationshipId		m_relationshipId;				///< Relationship id (if FLAG_RELATIONSHIP_ID)
	Feed::UpdateId				m_updateId;						///< Update id
	Feed::EventType				m_eventType;					///< Event type
	Feed::PermissionLevel		m_permissionLevel;				///< Permission level of the update
	Feed::PermissionLevelData	m_permissionLevelData;			///< Permission level specific data (eg delay period)
	FieldData					m_fieldData;					///< Field data
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool RecordUpdate::IsNewRecord() const
{
	return Feed::IsUpdateIdNewRecord(m_updateId);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool RecordUpdate::IsDelete() const
{
	return m_flags & FLAG_DELETE;
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_REALTIME_TYPES_H)
