/**
 *  @file	NewsTypes.h
 *
 *  @brief	Types for news data.
 *
 *	$Log: $
 *
 */

#if (!defined ACTIV_FEED_API_COMMON_NEWS_TYPES_H)
#define ACTIV_FEED_API_COMMON_NEWS_TYPES_H

#include "ActivContentPlatform/ActivFeedApiCommon/External.h"
#include "ActivContentPlatform/ActivFeedApiCommon/Types.h"
#include "ActivContentPlatform/ActivFeedApiCommon/RealtimeTypes.h"
#include "ActivContentPlatform/ActivFeedCommon/UpdateIds.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

/**
 *	@brief	Type for counting records per response
 */
typedef uint8_t RecordsPerResponseCount;

#define ACTIV_DECLARE_FLAGS(value, id)	static const Flags id = value;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	News request parameters.
 */
class NewsRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FEED_API_COMMON_API NewsRequestParameters();

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder, const byte_t version) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator message validator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Deserialize(MessageValidator &messageValidator, const byte_t version);

	#define ACTIV_NEWS_REQUEST_FLAG_LIST(d)											\
		d(0x00,	FLAG_NONE)															\
		d(0x01,	FLAG_SUBSCRIBE)	/** Subscribe to this news query */

	typedef uint8_t Flags;										///< Response flags type
	ACTIV_NEWS_REQUEST_FLAG_LIST(ACTIV_DECLARE_FLAGS)

	static const RecordsPerResponseCount DEFAULT_MAX_RECORDS_PER_RESPONSE = 64;

	std::string					m_query;							///< The query string
	Date						m_startDate;						///< The start date of the query
	Date						m_endDate;							///< The end date of the query
	std::string					m_startSymbol;						///< The symbol to start from
	uint16_t					m_numberOfRecords;					///< Number of news records to return
	RecordsPerResponseCount		m_maxRecordsPerResponse;			///< Maximum number of records in each response
	Feed::FieldIdList			m_fieldIdList;						///< field list
	Flags						m_flags;							///< Request flags
	Feed::PermissionLevel		m_permissionLevel;					///< Requested permission level
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	News story class.
 */
class NewsStory
{
public:
	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator message validator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	std::string				m_symbol;						///< symbol for story
	FieldData				m_fieldData;					///< serialized field data
	Feed::PermissionIdList	m_permissionIdList;				///< list of permission id's of the story
};

typedef std::vector<NewsStory> NewsStoryList;
typedef SerializableObjectContainerHelper<NewsStoryList, uint16_t> NewsStoryListSerializer;
typedef SerializableObjectContainerHelper<Feed::PermissionIdList, Feed::PermissionId> PermissionIdListSerializer;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	News response parameters.
 */
class NewsResponseParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FEED_API_COMMON_API NewsResponseParameters();

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder, const byte_t version) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator message validator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Deserialize(MessageValidator &messageValidator);

	NewsStoryList		m_storyList;
	SubscriptionCookie	m_subscriptionCookie;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	News update class.
 */
class NewsUpdate
{
public:
	#define ACTIV_NEWS_UPDATE_FLAG_LIST(d)																	\
		d(0x00,	FLAG_NONE)										/** Nominal update */							\
		d(0x01,	FLAG_DELETE)									/** Update is a delete for a record */

	typedef uint8_t Flags;										///< Update flags
	ACTIV_NEWS_UPDATE_FLAG_LIST(ACTIV_DECLARE_FLAGS)

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder, const byte_t version) const;

	/**
	 *	@brief	Serialize without the subscription cookie list. Use SubscriptionCookieSerializer::Serialize() to attach the subscription cookie list.
	 *
	 *	@param	messageBuilder.
	 *	@param	version.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode SerializeWithoutSubscriptionCookieList(MessageBuilder &messageBuilder, const uint8_t version) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator message validator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief	Convert flags to a printable string. Mainly for debugging purposes.
	 *
	 *	@param	flags update flags.
	 *
	 *	@return	String representation of \a flags.
	 */
	ACTIV_FEED_API_COMMON_API static std::string FlagsToString(const Flags flags);

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

	uint8_t					m_version;
	Flags					m_flags;						///< Update flags
	NewsStory				m_newsStory;					///< The news story
	Feed::UpdateId			m_updateId;						///< Update id
	SubscriptionCookieList	m_subscriptionCookieList;		///< List of subscription cookies this news story matches

	static const uint8_t currentVersion = 1;

	typedef SerializableObjectContainerHelper<SubscriptionCookieList, uint32_t, 1> SubscriptionCookieListSerializer;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool NewsUpdate::IsNewRecord() const
{
	return Feed::IsUpdateIdNewRecord(m_updateId);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool NewsUpdate::IsDelete() const
{
	return m_flags & FLAG_DELETE;
}

#undef ACTIV_DECLARE_FLAGS

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_COMMON_NEWS_TYPES_H)
