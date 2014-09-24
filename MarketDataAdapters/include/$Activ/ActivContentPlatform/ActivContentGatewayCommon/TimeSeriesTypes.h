/**
 *  @file	TimeSeriesTypes.h
 *
 *  @brief	Types for time series data.
 *
 *  $Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_TIME_SERIES_TYPES_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_TIME_SERIES_TYPES_H

#include "ActivContentPlatform/ActivContentGatewayCommon/External.h"
#include "ActivContentPlatform/ActivContentGatewayCommon/Types.h"

#include "ActivMiddleware/ActivFieldTypes/DateTime.h"

namespace Activ
{

namespace ContentPlatform
{

typedef uint8_t SeriesType;										///< Series type type

#define ACTIV_DECLARE_CONSTANT_SERIES_TYPE(val, id, name) const SeriesType id = val;

/**
 *	@brief	Return series type as string.
 *
 *	@param	seriesType.
 *
 *	@return	String representation of \a seriesType.
 */
ACTIV_CONTENT_GATEWAY_COMMON_API std::string SeriesTypeToString(const SeriesType seriesType);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of series types for tick requests.
 */	
#define ACTIV_TIME_SERIES_TICK_SERIES_TYPE_LIST(d)						\
	d(0,	TICK_SERIES_TYPE,				"Ticks")

ACTIV_TIME_SERIES_TICK_SERIES_TYPE_LIST(ACTIV_DECLARE_CONSTANT_SERIES_TYPE)

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of series types for intraday requests. For INTRADAY_SERIES_TYPE_SPECIFIED_MINUTE, use m_specifiedInterval to specify minutes
 */	
#define ACTIV_TIME_SERIES_INTRADAY_SERIES_TYPE_LIST(d)					\
	d(20,	INTRADAY_SERIES_TYPE_1_MINUTE,	"1 Minute bars")			\
	d(21,	INTRADAY_SERIES_TYPE_5_MINUTE,	"5 Minute bars")			\
	d(22,	INTRADAY_SERIES_TYPE_SPECIFIED_MINUTE,	"Minute bars as specified")	

ACTIV_TIME_SERIES_INTRADAY_SERIES_TYPE_LIST(ACTIV_DECLARE_CONSTANT_SERIES_TYPE)

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of series types for history requests.
 */
#define ACTIV_TIME_SERIES_HISTORY_SERIES_TYPE_LIST(d)					\
	d(40,	HISTORY_SERIES_TYPE_DAILY,		"Daily bars")				\
	d(41,	HISTORY_SERIES_TYPE_WEEKLY,		"Weekly bars")				\
	d(42,	HISTORY_SERIES_TYPE_MONTHLY,	"Monthly bars")

ACTIV_TIME_SERIES_HISTORY_SERIES_TYPE_LIST(ACTIV_DECLARE_CONSTANT_SERIES_TYPE)

// ---------------------------------------------------------------------------------------------------------------------------------

typedef uint8_t RecordFilterType;								///< Record filter type type

#define ACTIV_DECLARE_CONSTANT_RECORD_FILTER_TYPE(val, id, name) const RecordFilterType id = val;

/**
 *	@brief	Return record filter type as string.
 *
 *	@param	recordFilterType.
 *
 *	@return	String representation of \a recordFilterType.
 */
ACTIV_CONTENT_GATEWAY_COMMON_API std::string RecordFilterTypeToString(const RecordFilterType recordFilterType);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of record filter types for tick requests.
 */
#define ACTIV_TIME_SERIES_TICK_RECORD_FILTER_TYPE_LIST(d)				\
	d(0,	TICK_RECORD_FILTER_TYPE_ALL,			"All")				\
	d(1,	TICK_RECORD_FILTER_TYPE_ALL_TRADES,		"All Trades")		\
	d(2,	TICK_RECORD_FILTER_TYPE_REGULAR_TRADES,	"Regular Trades")	\
	d(3,	TICK_RECORD_FILTER_TYPE_ALL_QUOTES,		"All Quotes")		\
	d(4,	TICK_RECORD_FILTER_TYPE_ALL_BIDS,		"All Bids")			\
	d(5,	TICK_RECORD_FILTER_TYPE_ALL_ASKS,		"Asks")

ACTIV_TIME_SERIES_TICK_RECORD_FILTER_TYPE_LIST(ACTIV_DECLARE_CONSTANT_RECORD_FILTER_TYPE)

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of record filter types for Intraday requests.
 */
#define ACTIV_TIME_SERIES_INTRADAY_RECORD_FILTER_TYPE_LIST(d)				\
	d(0,	INTRADAY_RECORD_FILTER_TYPE_REGULAR_TRADES,	"Regular Trades")	\
	d(1,	INTRADAY_RECORD_FILTER_TYPE_ALL,			"All")

ACTIV_TIME_SERIES_INTRADAY_RECORD_FILTER_TYPE_LIST(ACTIV_DECLARE_CONSTANT_RECORD_FILTER_TYPE)

// ---------------------------------------------------------------------------------------------------------------------------------

typedef uint8_t FieldFilterType;								///< Field filter type type

#define ACTIV_DECLARE_CONSTANT_FIELD_FILTER_TYPE(val, id, name) const FieldFilterType id = val;

/**
 *	@brief	Return field filter type as string.
 *
 *	@param	fieldFilterType.
 *
 *	@return	String representation of \a recordFilterType.
 */
ACTIV_CONTENT_GATEWAY_COMMON_API std::string FieldFilterTypeToString(const FieldFilterType fieldFilterType);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of field filter types for intraday requests.
 */
#define ACTIV_TIME_SERIES_INTRADAY_FIELD_FILTER_TYPE_LIST(d)			\
	d(0,	INTRADAY_FIELD_FILTER_TYPE_FULL_BAR,	"Full Bar")			\
	d(1,	INTRADAY_FIELD_FILTER_TYPE_MINI_BAR,	"Mini Bar")

ACTIV_TIME_SERIES_INTRADAY_FIELD_FILTER_TYPE_LIST(ACTIV_DECLARE_CONSTANT_FIELD_FILTER_TYPE)

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of field filter types for history requests.
 */
#define ACTIV_TIME_SERIES_HISTORY_FIELD_FILTER_TYPE_LIST(d)				\
	d(20,	HISTORY_FIELD_FILTER_TYPE_FULL_BAR,	"Full Bar")				\
	d(21,	HISTORY_FIELD_FILTER_TYPE_MINI_BAR,	"Mini Bar")

ACTIV_TIME_SERIES_HISTORY_FIELD_FILTER_TYPE_LIST(ACTIV_DECLARE_CONSTANT_FIELD_FILTER_TYPE)

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of period types.
 */
#define ACTIV_TIME_SERIES_PERIOD_TYPE_LIST(d)							\
	d(0,	PERIOD_TYPE_NOW,				"Present Time")				\
	d(1,	PERIOD_TYPE_INFINITY,			"Infinity")					\
	d(2,	PERIOD_TYPE_LOCAL_DATE_TIME,	"Local Date Time")			\
	d(3,	PERIOD_TYPE_UTC_DATE_TIME,		"UTC Date Time")			\
	d(4,	PERIOD_TYPE_TRADING_DAY_COUNT,	"Trading Day Count")		\
	d(5,	PERIOD_TYPE_DATA_POINT_COUNT,	"Data Point Count")			\
	d(6,	PERIOD_TYPE_DATA_POINT_ADDRESS,	"Data Point Address")

typedef uint8_t PeriodType;										///< Period type type

#define ACTIV_DECLARE_CONSTANT_PERIOD_TYPE(val, id, name)	const PeriodType id = val;

ACTIV_TIME_SERIES_PERIOD_TYPE_LIST(ACTIV_DECLARE_CONSTANT_PERIOD_TYPE)

#undef ACTIV_DECLARE_CONSTANT_PERIOD_TYPE

/**
 *	@brief	Return period type as string.
 *
 *	@param	periodType.
 *
 *	@return	String representation of \a periodType.
 */
ACTIV_CONTENT_GATEWAY_COMMON_API std::string PeriodTypeToString(const PeriodType periodType);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Address of an individual data point.
 */
class DataPointAddress
{
public:
	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder message builder.
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
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Deserialize(MessageValidater &messageValidater);

	typedef uint32_t SectorNumber;
	typedef uint16_t SequenceNumber;
	typedef uint16_t Flags;										///< Request flags type

	DateTime		m_dateTime;
	SectorNumber	m_sectorNumber;
	SequenceNumber	m_sequenceNumber;
	Flags			m_internalFlag;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Period of a series.
 */
class Period
{
public:
	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder messageBuilder.
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
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief	Check if the Period is an absolute type.
	 *
	 *	@return	true/false
	 */
	bool IsAbsolutePeriod() const;

	/**
	 *	@brief	Check if the Period is a relative type.
	 *
	 *	@return	true/false
	 */
	bool IsRelativePeriod() const;

	/**
	 *	@brief	Check if the Period is a count of data points.
	 *
	 *	@return	true/false
	 */
	bool IsRelativeCount() const;

	PeriodType			m_periodType;							///< Type of this period
	DateTime			m_dateTime;								///< Date and time for this period
	uint16_t			m_count;								///< Count for this period
	DataPointAddress	m_dataPointAddress;						///< Data point address
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Period::IsAbsolutePeriod() const
{
	return	PERIOD_TYPE_NOW == m_periodType
			|| PERIOD_TYPE_INFINITY == m_periodType
			|| PERIOD_TYPE_LOCAL_DATE_TIME == m_periodType
			|| PERIOD_TYPE_UTC_DATE_TIME == m_periodType
			|| PERIOD_TYPE_DATA_POINT_ADDRESS == m_periodType;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Period::IsRelativePeriod() const
{
	return	PERIOD_TYPE_TRADING_DAY_COUNT == m_periodType;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool Period::IsRelativeCount() const
{
	return PERIOD_TYPE_DATA_POINT_COUNT == m_periodType; 
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of Period.
 */
typedef std::vector<Period> PeriodList;

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Request parameters.
 */
class TimeSeriesRequestParameters
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API TimeSeriesRequestParameters();

	/**
	 *	@brief	Clear down period list.
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

	typedef uint16_t Flags;										///< Request flags type

	static const Flags FLAG_NONE = 0x0000;
	static const Flags FLAG_REVERSE = 0x0001;					///< Reverse series flag

	static const size_t DEFAULT_MAX_FIELD_SETS = 1000;			///< Default max number of results in one response

	SymbolId			m_symbolId;								///< Symbol to request on
	Flags				m_flags;								///< Request flags
	size_t				m_maxFieldSets;							///< Max results to return in one response
	SeriesType			m_seriesType;							///< Series type
	RecordFilterType	m_recordFilterType;						///< Filter to apply to records
	FieldFilterType		m_fieldFilterType;						///< Filter to apply to fields
	PeriodList			m_periodList;							///< List of periods to apply to
	uint8_t				m_delayPeriod;							///< Delay in minutes
	uint8_t				m_specifiedInterval;					///< for series type of INTRADAY_SERIES_TYPE_SPECIFIED_MINUTE only.  60/m_specifiedInterval should be an integer
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Cookie passed between history manager and client for multi part requests.
 */
class TimeSeriesCookie
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
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief	Get the symbol.
	 *
	 *	@return	Reference to the symbol.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API const SymbolId &GetSymbolId() const;

	/**
	 *	@brief	Get the data point address.
	 *
	 *	@return	Reference to the data point address.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API const DataPointAddress &GetDataPointAddress() const;

public:
	TimeSeriesRequestParameters	m_requestParameters;
	DataPointAddress			m_dataPointAddress;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Time series response parameters.
 *
 *	@param	FIELD_SET_TYPE type of result list.
 */
template <typename FIELD_SET_TYPE>
class TimeSeriesResponseParametersTemplate
{
public:
	typedef FIELD_SET_TYPE FieldSetList;						///< Field set list type

	/**
	 *	@brief	Clear down response field set list.
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
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API StatusCode Deserialize(MessageValidater &messageValidater);

	FieldSetList	m_fieldSetList;
};

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_TIME_SERIES_TYPES_H)
