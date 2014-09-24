/**
 *	@file	FieldSet.h
 *
 *	@brief	Header file for the Field Set base class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_COMMON_FIELD_SET_H)
#define ACTIV_CONTENT_GATEWAY_COMMON_FIELD_SET_H

#include "ActivContentPlatform/ActivContentGatewayCommon/External.h"

#include "ActivContentPlatform/ActivFeedCommon/FieldIds.h"

#include "ActivMiddleware/ActivFieldTypes/IFieldType.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"

#include "ActivMiddleware/Misc/SerializableContainerHelper.h"

#define HISTORY_SYMBOLSIZE 20

namespace Activ
{

//class BitBuilder;
//class BitValidater;

class Date;
class Time;

namespace ContentPlatform
{

/**
 *	@brief	The list of field set types and their string representation.
 */
#define ACTIV_FIELD_SET_TYPE_LIST(d)								\
	d(FIELD_SET_TYPE_NULL,				"Null"					)	\
	d(FIELD_SET_TYPE_TICK,				"Tick"					)	\
	d(FIELD_SET_TYPE_INTRADAY_BAR,		"IntradayBar"			)	\
	d(FIELD_SET_TYPE_HISTORY_BAR,		"HistoryBar"			)	\
	d(FIELD_SET_TYPE_CLOSING_BAR,		"ClosingBar"			)	\
	d(FIELD_SET_TYPE_OPTION_BAR,		"OptionBar"				)


enum FieldSetType
{
	ACTIV_FIELD_SET_TYPE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
	NUMBER_OF_FIELD_SET_TYPES,
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	The list of field set types and their string representation.
 */
#define ACTIV_FIELD_SET_FILTER_STATUS_LIST(d)				\
	d(FILTER_STATUS_OK,					"Ok"			)	\
	d(FILTER_STATUS_IGNORE,				"Ignore"		)	\
	d(FILTER_STATUS_DONE,				"Done"			)

// ---------------------------------------------------------------------------------------------------------------------------------

//	@brief	Bar price types.

#define BASE_TYPE_LIST(d)									\
	d(	BASE_TYPE_LOW,					"Low"			)	\
	d(	BASE_TYPE_OPEN,					"Open"			)	\
	d(	BASE_TYPE_HIGH,					"High"			)	\
	d(	BASE_TYPE_CLOSE,				"Close"			)

// ---------------------------------------------------------------------------------------------------------------------------------
/**
 *	@brief	Update return codes.
 */
#define UPDATE_FIELDSET_STATUS_LIST(d)						\
	d(UPDATE_STATUS_OK,					"Ok"			)	\
	d(UPDATE_STATUS_FLUSH,				"Flush"			)	\
	d(UPDATE_STATUS_IGNORE,				"Ignore"		)

// ---------------------------------------------------------------------------------------------------------------------------------
/**
 *	@brief	Update return codes.
 */
#define FIELDSETLIST_FIND_MODE_LIST(d)												\
	d(FIND_MODE_LATER,				"The first one later in time"				)	\
	d(FIND_MODE_EQUAL_OR_LATER,		"The first one match or later in time"		)	\
	d(FIND_MODE_EQUAL,				"exact match in time"						)	\
	d(FIND_MODE_EQUAL_OR_EARLIER,	"The first one match or earlierer in time"	)	\
	d(FIND_MODE_EARLIER,			"The first earlier in time"					)	


/**
 *	@brief	Field Set base class.
 */
class FieldSet
{
public:
	static const size_t											SIZEOFBYTE = sizeof(byte_t);
	ACTIV_CONTENT_GATEWAY_COMMON_API static const std::string	DEFAULT_STRING_FIELD_SEPARATER;

	/**
	 *	@brief	Update return status.
	 */
	enum UpdateFieldSetStatus
	{
		UPDATE_FIELDSET_STATUS_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUMBER_OF_UPDATE_STATUS,
	};


	enum FilterStatus
	{
		ACTIV_FIELD_SET_FILTER_STATUS_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUMBER_OF_FIELD_SET_FILTERS,
	};

	enum FieldSetListFindMode
	{
		FIELDSETLIST_FIND_MODE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUMBER_OF_FIELD_SET_LIST_FIND_MODES,
	};

	/**
	 *	@brief	Constructor.
	 */
	FieldSet();
	
	/**
	 *	@brief	Destructor.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual ~FieldSet();

	/**
	 *	@brief	Reset all values in the field set.
	 */
	virtual void Reset();

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	fieldId field's id.
	 *	@param	field actual field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode GetField(const Feed::FieldId fieldId, IFieldType &field) const;

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	fieldId field's id.
	 *	@param	field actual field.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FIELD
	 *	@retval	STATUS_CODE_UNDEFINED_FIELD
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode SetField(const Feed::FieldId fieldId, const IFieldType &field);

	/**
	 *	@brief	Get the field set type.
	 *
	 *	@return	the field type.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual FieldSetType GetType() const;

	/**
	 *	@brief	Convert the object to a string.
	 *
	 *	@return	the object as a string.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual std::string ToString(const std::string &deliminator = DEFAULT_STRING_FIELD_SEPARATER) const;

	/**
	 *	@brief	Set the object value from a string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *  @retval ......
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode FromString(const std::string &inputStr, const std::string &deliminator = DEFAULT_STRING_FIELD_SEPARATER);

	/**
	 *	@brief	Serialize the object into the supplied message builder.
	 *
	 *	@param	messageBuilder the message builder to store a serialized version of the object in.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize the object from the supplied message validater.
	 *
	 *	@param	messageValidater the message validater from which the object will be extracted.
	 *	@param	maxSerializedBodyLength the maximum serialized body length.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief Get the serialized length of the object.
	 *
	 *	@return	the serialized length of the object.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API virtual size_t GetSerializedLength() const;

	/**
	 *	@brief	split addjust
	 *
	 *	@param	sharesBefore Shares before adjusting
	 *	@param	sharesAfter Shares after adjusting
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 */
	StatusCode ProcessSplit(const int sharesBefore, const int sharesAfter);

	/**
	 *	@brief	Get the maximum serialized length.
	 *
	 *	@return	the maximum serialized length.
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API static size_t GetMaxSerializedLength();

	/**
	 *	@brief Set the sequence number used to identify the position in the block
	 *
	 *	@param	sequence sequence number to set to
	 */
	void SetSequence(byte_t sequence);

	/**
	 *	@brief Get the sequence number used to identify the position in the block
	 *
	 *	@retval	sequence number
	 */
	byte_t GetSequence() const;

	/**
	 *	@brief	Compress a field set.
	 *
	 *	@param	previous previous field set in this stream.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Compress(const FieldSet &previous, BitBuilder &builder) const;

	/**
	 *	@brief	Decompress a field set.
	 *
	 *	@param	previous previous field set in this stream.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API virtual StatusCode Decompress(const FieldSet &previous, BitValidater &validater);

	/**
	 *	@brief	Compress a date.
	 *
	 *	@param	previous previous date.
	 *	@param	date current date.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Compress(const Date &previous, const Date &date, BitBuilder &builder);

	/**
	 *	@brief	Compress a date.
	 *
	 *	@param	previous previous date.
	 *	@param	date current date.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode CompressWithCheck(const Date &previous, const Date &date, BitBuilder &builder);

	/**
	 *	@brief	Decompress a date.
	 *
	 *	@param	previous previous date.
	 *	@param	date current date.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Decompress(const Date &previous, Date &date, BitValidater &validater);

	/**
	 *	@brief	Decompress a date.
	 *
	 *	@param	previous previous date.
	 *	@param	date current date.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode DecompressWithCheck(const Date &previous, Date &date, BitValidater &validater);

	/**
	 *	@brief	Compress a time.
	 *
	 *	@param	previous previous time.
	 *	@param	time current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Compress(const Time &previous, const Time &time, BitBuilder &builder);

	/**
	 *	@brief	Compress a time.
	 *
	 *	@param	previous previous time.
	 *	@param	time current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode CompressWithCheck(const Time &previous, const Time &time, BitBuilder &builder);

	/**
	 *	@brief	Decompress a time.
	 *
	 *	@param	previous previous time.
	 *	@param	time current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Decompress(const Time &previous, Time &time, BitValidater &validater);

	/**
	 *	@brief	Decompress a time.
	 *
	 *	@param	previous previous time.
	 *	@param	time current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode DecompressWithCheck(const Time &previous, Time &time, BitValidater &validater);

	/**
	 *	@brief	Compress a uint64_t based on a previous value.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Compress(const uint64_t &previous, const uint64_t &current, BitBuilder &builder);

	/**
	 *	@brief	Compress a uint64_t based on a previous value w/equal test.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode CompressWithCheck(const uint64_t &previous, const uint64_t &current, BitBuilder &builder);

	/**
	 *	@brief	Decompress a uint64_t based on a previous value.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Decompress(const uint64_t &previous, uint64_t &current, BitValidater &validater);

	/**
	 *	@brief	Decompress a uint64_t based on a previous value w/equal test.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode DecompressWithCheck(const uint64_t &previous, uint64_t &current, BitValidater &validater);

	/**
	 *	@brief	Compress a Rational::Denominator based on a previous value.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Compress(const Rational::Denominator &previous, const Rational::Denominator &current, BitBuilder &builder);

	/**
	 *	@brief	Compress a Rational::Denominator based on a previous value w/equal test.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode CompressWithCheck(const Rational::Denominator &previous, const Rational::Denominator &current, BitBuilder &builder);

	/**
	 *	@brief	Decompress a Rational::Denominator based on a previous value.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Decompress(const Rational::Denominator &previous, Rational::Denominator &current, BitValidater &validater);

	/**
	 *	@brief	Decompress a Rational::Denominator based on a previous value w/equal test.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode DecompressWithCheck(const Rational::Denominator &previous, Rational::Denominator &current, BitValidater &validater);

	/**
	 *	@brief	Compress a Rational based on a previous value.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Compress(const Rational &previous, const Rational &current, BitBuilder &builder);

	/**
	 *	@brief	Compress a Rational based on a previous value w/equal test.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	builder bit builder for compression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode CompressWithCheck(const Rational &previous, const Rational &current, BitBuilder &builder);

	/**
	 *	@brief	Compress a Rational based on a previous value.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode Decompress(const Rational &previous, Rational &current, BitValidater &validater);

	/**
	 *	@brief	Decompress a Rational based on a previous value w/equal test.
	 *
	 *	@param	previous previous value.
	 *	@param	current current time.
	 *	@param	validater bit validater for decompression.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_BUFFER_OVERFLOW
	 *	@retval	...
	 */
	//ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode DecompressWithCheck(const Rational &previous, Rational &current, BitValidater &validater);

	ACTIV_CONTENT_GATEWAY_COMMON_API static char *RationalToDoubleString(const Rational &rational, char *buffer);

	/**
	 *	@brief	helper function to read in delimiated date
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode ReadDate(const std::string &input,  const std::string &deliminator, std::string::size_type &startingPos, Date &dateOut);

	/**
	 *	@brief	helper function to read in delimiated dateTime
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode ReadDateTime(const std::string &input,  const std::string &deliminator, std::string::size_type &startPos, DateTime &dateTimeOut);

	/**
	 *	@brief	helper function to read in delimiated integer
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	static StatusCode ReadInteger(const std::string &input,  const std::string &deliminator, std::string::size_type &startPos, uint32_t &uintOut);

	/**
	 *	@brief	helper function to read delimiated double string into rational
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_COMMON_API static StatusCode ReadRationalFromDoubleString(const std::string &input,  const std::string &deliminator, std::string::size_type &startPos, Rational &rationalOut);

	static StatusCode FindContentSubstring(const std::string &input,  const std::string &deliminator, std::string::size_type &startPos, std::string &subStringOut);

private:
	byte_t m_sequence;	
};

template <typename OBJECT_TYPE, typename COUNT_TYPE>
class FieldSetList : public std::list<OBJECT_TYPE>
{
public:
	typedef typename std::list<OBJECT_TYPE>::iterator Iterator;
	typedef typename std::list<OBJECT_TYPE>::const_iterator ConstIterator;
	typedef SerializableObjectContainerHelper<FieldSetList, COUNT_TYPE> Serializer;
	
	/**
	 *	@brief	find in the list the specified date time
	 *
	 *	@param	dateTime dateTime to find
	 *	@param  isReverse is the list in reverse order (more recent at the end)
	 *	@param	findMode find mode
	 *
	 *	@retval	the iterator found
	 *	@retval	end() is not found
	 *	@throw STATUS_CODE_INVALID_PARAMETER
	 */
	Iterator Find(const DateTime &dateTime, const bool isReverse, const FieldSet::FieldSetListFindMode findMode, const Iterator &startingPos)
	{
		//TODO: add updateID match
		if(isReverse)
			return ReverseFind(dateTime, findMode, startingPos);

		//we are dealing with a list that is the latest data first
		Iterator iter, iterEnd = std::list<OBJECT_TYPE>::end(), lastIter = std::list<OBJECT_TYPE>::end();

		switch(findMode)
		{
		case FieldSet::FIND_MODE_LATER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) <= dateTime)
					break;
				else
					lastIter = iter;
			}
			break;
		case FieldSet::FIND_MODE_EQUAL_OR_LATER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) < dateTime)
					break;
				else
					lastIter = iter;
			}
			break;
		case FieldSet::FIND_MODE_EQUAL:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) == dateTime)
					return iter;
			}
			break;
		case FieldSet::FIND_MODE_EQUAL_OR_EARLIER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) <= dateTime)
					return iter;
			}
			break;
		case FieldSet::FIND_MODE_EARLIER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) < dateTime)
					return iter;
			}
			break;
		default:
			ACTIV_THROW(ActivException, STATUS_CODE_INVALID_PARAMETER);
		}
		return lastIter;
	}

	/**
	 *	@brief	merge the fromList to this list, replace entry use fromList if duplicate exist
	 *
	 *	@param	isReverse if the list is in reverse order (both should have the same order
	 *	@param	fromList the lsit to be merged
	 *
	 *	@retval	the iterator found
	 *	@retval	end() is not found
	 *	@throw STATUS_CODE_INVALID_PARAMETER
	 */
	void MergeFieldSets(const bool isReverse, const FieldSetList<OBJECT_TYPE, COUNT_TYPE> &fromList);

protected:
	Iterator ReverseFind(const DateTime &dateTime, const FieldSet::FieldSetListFindMode findMode, const Iterator &startingPos)
	{
		//TODO: add updateID match
		//we are dealing with a list that is the earliest data first
		Iterator iter, iterEnd = std::list<OBJECT_TYPE>::end(), lastIter = std::list<OBJECT_TYPE>::end();

		switch(findMode)
		{
		case FieldSet::FIND_MODE_LATER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) > dateTime)
					return iter;
			}
			break;
		case FieldSet::FIND_MODE_EQUAL_OR_LATER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) >= dateTime)
					return iter;
			}
			break;
		case FieldSet::FIND_MODE_EQUAL:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) == dateTime)
					return iter;
			}
			break;
		case FieldSet::FIND_MODE_EQUAL_OR_EARLIER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) > dateTime)
					break;
				else
					lastIter = iter;
			}
			break;
		case FieldSet::FIND_MODE_EARLIER:
			for(iter = startingPos; iter != iterEnd; ++iter)
			{
				if( (*iter) >= dateTime)
					break;
				else
					lastIter = iter;
			}
			break;
		default:
			ACTIV_THROW(ActivException, STATUS_CODE_INVALID_PARAMETER);
		}
		return lastIter;
	}

	void ReverseMergeFieldSets(const FieldSetList<OBJECT_TYPE, COUNT_TYPE> &fromList);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldSet::FieldSet()
{
	Reset();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FieldSet::Reset()
{
	m_sequence = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FieldSet::SetSequence(byte_t sequence)
{
	m_sequence = sequence;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline byte_t FieldSet::GetSequence() const
{
	return m_sequence;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldSet::FindContentSubstring(const std::string &input,  const std::string &deliminator, std::string::size_type &startPos, std::string &subStringOut)
{
	if(startPos == std::string::npos)
		return STATUS_CODE_INVALID_PARAMETER;

	std::string::size_type endPos = input.find(deliminator, startPos);
	subStringOut.assign(input, startPos, endPos - startPos);
	if(endPos == std::string::npos)
		startPos = endPos;
	else
		startPos = endPos + deliminator.length();
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldSet::ReadInteger(const std::string &input,  const std::string &deliminator, std::string::size_type &startPos, uint32_t &intOut)
{
	std::string subString;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, FieldSet::FindContentSubstring(input, deliminator, startPos, subString));
	intOut = static_cast<uint32_t>(std::atoi(subString.c_str()));
	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldSet::ProcessSplit(const int sharesBefore, const int sharesAfter)
{
	return STATUS_CODE_NOT_SUPPORTED;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename OBJECT_TYPE, typename COUNT_TYPE>
inline void FieldSetList<OBJECT_TYPE, COUNT_TYPE>::MergeFieldSets(const bool isReverse, const FieldSetList<OBJECT_TYPE, COUNT_TYPE> &fromList)
{
	if(isReverse)
		return ReverseMergeFieldSets(fromList);
	ConstIterator fromIter, fromEnd = fromList.end();
	Iterator foundPosition = std::list<OBJECT_TYPE>::begin();

	//this list is in normal, or from most recent to earlier
	DateTime dateTime;
	for(fromIter = fromList.begin(); fromIter != fromEnd; ++fromIter)
	{
		fromIter->GetDateTime(dateTime);
		foundPosition = Find(dateTime, isReverse, FieldSet::FIND_MODE_EQUAL_OR_EARLIER, foundPosition);
		if(foundPosition == std::list<OBJECT_TYPE>::end())
		{
			//we did not find anything that is earlier, add to the front
			push_back(*fromIter);
		}
		else if((*foundPosition) == dateTime) 
		{
			// we are at the same time, replace use fromList
			*foundPosition = *fromIter;
		}
		else
		{
			//the item is eariler, just insert
			foundPosition = insert(foundPosition, *fromIter);
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename OBJECT_TYPE, typename COUNT_TYPE>
inline void FieldSetList<OBJECT_TYPE, COUNT_TYPE>::ReverseMergeFieldSets(const FieldSetList<OBJECT_TYPE, COUNT_TYPE> &fromList)
{
	ConstIterator fromIter, fromEnd = fromList.end();
	Iterator foundPosition = std::list<OBJECT_TYPE>::begin();

	//this list is in reverse, or from earliest to more recent
	DateTime dateTime;
	for(fromIter = fromList.begin(); fromIter != fromEnd; ++fromIter)
	{
		fromIter->GetDateTime(dateTime);
		foundPosition = Find(dateTime, true, FieldSet::FIND_MODE_EQUAL_OR_LATER, foundPosition);
		if(foundPosition == std::list<OBJECT_TYPE>::end())
		{
			//we did not find anything that is later, add to the end
			push_back(*fromIter);
		}
		else if((*foundPosition) == dateTime) 
		{
			// we are at the same time, replace use fromList
			*foundPosition = *fromIter;
		}
		else
		{
			//the item is eariler, just insert
			foundPosition = insert(foundPosition, *fromIter);
		}
	}
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_GATEWAY_COMMON_FIELD_SET_H)
