/**
 *  @file	SymbolDirectoryTypes.h
 *
 *  @brief	Types for symbol directory data.
 *
 *  $Log: $
 */

#if (!defined ACTIV_FEED_API_COMMON_SYMBOL_DIRECTORY_TYPES_H)
#define ACTIV_FEED_API_COMMON_SYMBOL_DIRECTORY_TYPES_H

#include "ActivContentPlatform/ActivFeedApiCommon/External.h"
#include "ActivContentPlatform/ActivFeedApiCommon/Types.h"
#include "ActivContentPlatform/ActivFeedCommon/EntityTypes.h"

namespace Activ
{

namespace ContentPlatform
{

namespace FeedApi
{

#define ACTIV_DECLARE_FLAGS(value, id)	static const Flags id = value;

/**
 *	@brief	Get Symbols Request parameters.
 */
class SymbolDirectoryRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FEED_API_COMMON_API SymbolDirectoryRequestParameters();

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator message validator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FEED_API_COMMON_API StatusCode Deserialize(MessageValidator &messageValidator);

	typedef std::vector<Feed::EntityType>	EntityTypeList;

	#define SYMBOL_DIRECTORY_FILTER_TYPE_LIST(d)																					\
		d(FILTER_TYPE_FULL)													/** No filtering */										\
		d(FILTER_TYPE_ENTITY_TYPE_INCLUDE_LIST)								/** Only include these Entity Types in the response */	\
		d(FILTER_TYPE_ENTITY_TYPE_EXCLUDE_LIST)								/** Exclude these Entity Types from the response */

	enum FilterType
	{
		SYMBOL_DIRECTORY_FILTER_TYPE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)	///< The filter types.
		NUMBER_OF_FILTER_TYPES												///< The number of filter types.
	};

	#define SYMBOL_DIRECTORY_MATCH_TYPE_LIST(d)															\
		d(MATCH_TYPE_ANY)													/** Match to any string */	\
		d(MATCH_TYPE_EXACT)													/** Exact match on string */

	enum MatchType
	{
		SYMBOL_DIRECTORY_MATCH_TYPE_LIST(ACTIV_DECLARE_VALUE_WITH_COMMA)	///< The match types.
		NUMBER_OF_MATCH_TYPES												///< The number of match types.
	};

	Feed::FieldId		m_fieldId;											///< Field to search on
	std::string			m_searchString;										///< String to search on
	FilterType			m_filterType;										///< Filter type, default FILTER_TYPE_FULL
	MatchType			m_matchType;										///< Match type, default MATCH_TYPE_ANY
	EntityTypeList		m_entityTypeList;									///< Entity type list to filter on

private:
	typedef SerializableObjectContainerHelper<EntityTypeList, uint16_t, 0> EntityTypeListSerializer;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Symbol directory response parameters.
 */
class SymbolResponse
{
public:
	/**
	 *	@brief	Checks if the fieldType and matchData are the same or the flag is set.
	 *
	 *	@param	symbolDirectorySymbolResponse the SymbolDirectorySymbolResponse to check against.
	 *
	 *	@retval	true if the same
	 */
	bool SameAsLastCheck(const SymbolResponse symbolResponse);

	#define ACTIV_SYMBOL_RESPONSE_FLAG_LIST(d)																		\
		d(0x0000,	FLAG_NONE)																						\
		d(0x0001,	FLAG_SAME_AS_LAST)	/** The field type and match data are the same as the previous response */

	typedef uint16_t Flags;				///< Response flags type
	ACTIV_SYMBOL_RESPONSE_FLAG_LIST(ACTIV_DECLARE_FLAGS)

	SymbolId			m_symbolId;		///< The found symbol
	Flags				m_flags;		///< Response Flags
	std::string			m_matchData;	///< The match data
	Feed::EntityType	m_entityType;	///< The entity type of the found symbol
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool SymbolResponse::SameAsLastCheck(const SymbolResponse symbolResponse)
{
	return (((m_flags & FLAG_SAME_AS_LAST) == FLAG_SAME_AS_LAST) || (m_matchData == symbolResponse.m_matchData));
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Symbol directory response parameters.
 */
class SymbolDirectoryResponseParameters
{
public:
	typedef std::vector<SymbolResponse>	SymbolResponseList;

	/**
	 *	@brief	Clear down response symbol list.
	 */
	ACTIV_FEED_API_COMMON_API void Clear();

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
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

	Feed::FieldId			m_fieldId;					///< Field searched on
	SymbolResponseList		m_symbolResponseList;		///< List of all found symbols (+ info)
};

#undef ACTIV_DECLARE_FLAGS

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_COMMON_SYMBOL_DIRECTORY_TYPES_H)
