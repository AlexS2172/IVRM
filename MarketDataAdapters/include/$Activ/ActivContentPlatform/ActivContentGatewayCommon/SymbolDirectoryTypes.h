/**
 *  @file	SymbolDirectoryTypes.h
 *
 *  @brief	Types for symbol directory data.
 *
 *  $Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_SYMBOL_DIRECTORY_TYPES_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_SYMBOL_DIRECTORY_TYPES_H

#include "ActivContentPlatform/ActivContentGatewayCommon/External.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/Types.h"
#include "ActivContentPlatform/ActivFeedCommon/EntityTypes.h"

namespace Activ
{

namespace ContentPlatform
{

/**
 *	@brief	Get Symbols Request parameters.
 */
class SymbolDirectoryRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API SymbolDirectoryRequestParameters();

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Deserialize(MessageValidater &messageValidater);

	typedef std::vector<Feed::EntityType>	EntityTypeList;

	enum FilterType
	{
		FILTER_TYPE_FULL,										///< No filtering
		FILTER_TYPE_ENTITY_TYPE_INCLUDE_LIST,					///< Only include these Entity Types in the response
		FILTER_TYPE_ENTITY_TYPE_EXCLUDE_LIST,					///< Exclude these Entity Types from the response
	};

	enum MatchType
	{
		MATCH_TYPE_ANY,											///< Match to any string
		MATCH_TYPE_EXACT,										///< Exact match on string
	};

	Feed::FieldId		m_fieldId;								///< Field to search on
	std::string			m_searchString;							///< String to search on
	FilterType			m_filterType;							///< Filter type
	MatchType			m_matchType;							///< Match type
	EntityTypeList		m_entityTypeList;						///< Entity type list to filter on

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

	typedef uint16_t Flags;										///< Response flags type

	static const Flags FLAG_NONE = 0x0000;
	static const Flags FLAG_SAME_AS_LAST = 0x0001;				///< The field type and match data are the same as the previous response

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
	ACTIV_CONTENT_GATEWAY_COMMON_API void Clear();

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder, const byte_t version) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Deserialize(MessageValidater &messageValidater);

	Feed::FieldId			m_fieldId;					///< Field searched on
	SymbolResponseList		m_symbolResponseList;		///< List of all found symbols (+ info)
};

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_SYMBOL_DIRECTORY_TYPES_H)
