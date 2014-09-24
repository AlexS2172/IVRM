/**
 *	@file	IniFileParser.h
 *
 *	@brief	Header file for ini file parser class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_INI_FILE_PARSER_H)
#define ACTIV_BASE_INI_FILE_PARSER_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/Exception.h"

#include <string>
#include <vector>

namespace Activ
{

/**
 *	@brief	Ini file parser.
 *
 *	Note all section and key searches are case insensitive.
 */
class IniFileParser
{
private:
	typedef std::pair<std::string, std::string> Key;				///< Key is pair<name, value>
	typedef std::vector<Key> KeyList;								///< List of keys
	typedef std::pair<std::string, KeyList> Section;				///< Section is pair<name, list of keys>
	typedef std::vector<Section> SectionList;						///< Ini file is a list of sections

public:
	/**
	 *	@brief	Exception generated by parse errors.
	 */
	class ParseException : public Exception
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	offset.
		 *	@param	validPart.
		 */
		ACTIV_BASE_API ParseException(const char *file, const char *function, const uint_fast32_t line, const size_t offset, const std::string &validPart);

		ACTIV_BASE_API virtual std::string ToString() const;

		const size_t		m_offset;								///< Offset in the file where parsing failed
		const std::string	m_validPart;							///< Parsing failed at end of this
	};

	static const char SECTION_DELIMITER = ':';						///< Sub-sections are delimited with this, eg [Section:SubSection]
	static const size_t MATCH_LENGTH_COMPLETE = static_cast<size_t>(~0);

	class ConstSectionIterator;										///< Forward reference to const section iterator.

	/**
	 *	@brief	Const iterator to section keys.
	 */
	ACTIV_DECLARE_NESTED_CLASS(ConstKeyIterator)
	{
	public:
		/**
		 *	@brief	Get the key name.
		 *
		 *	@return	The key name.
		 */
		ACTIV_BASE_API const std::string GetName() const;

		/**
		 *	@brief	Get string value for this key.
		 *
		 *	@param	value reference to a string variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 */
		ACTIV_BASE_API StatusCode GetStringValue(std::string &value) const;

		/**
		 *	@brief	Get a signed integral value for this key.
		 *
		 *	@param	value reference to a variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_OUT_OF_RANGE parsed value is out of range of type T
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a signed value
		 */
		template<typename T>
		ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode GetSignedValue(T &value) const;

		// Allowed instantiations for supported signed types
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedValue<int8_t>(int8_t &value) const;)
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedValue<int16_t>(int16_t &value) const;)
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedValue<int32_t>(int32_t &value) const;)
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedValue<int64_t>(int64_t &value) const;)

		/**
		 *	@brief	Get an unsigned integral value for this key.
		 *
		 *	@param	value reference to a variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_OUT_OF_RANGE parsed value is out of range of type T
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not an unsigned value
		 */
		template<typename T>
		ACTIV_TEMPLATE_API(ACTIV_BASE_API) StatusCode GetUnsignedValue(T &value) const;

		// Allowed instantiations for supported unsigned types
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedValue<uint8_t>(uint8_t &value) const;)
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedValue<uint16_t>(uint16_t &value) const;)
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedValue<uint32_t>(uint32_t &value) const;)
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedValue<uint64_t>(uint64_t &value) const;)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedValue<size_t>(size_t &value) const;)
#endif

		/**
		 *	@brief	Get a real value for this key.
		 *
		 *	@param	value reference to a variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a real value.
		 */
		ACTIV_BASE_API StatusCode GetRealValue(double &value) const;

		/**
		 *	@brief	Get boolean value for this key.
		 *
		 *	@param	value reference to a boolean variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a boolean value
		 */
		ACTIV_BASE_API StatusCode GetBooleanValue(bool &value) const;

		/**
		 *	@brief	Advance to the next key in the section.
		 *
		 *	@return	*this.
		 */
		ACTIV_BASE_API ConstKeyIterator &operator++();

		/**
		 *	@brief	Equality operator.
		 *
		 *	@param	rhs KeyIterator to compare to.
		 *
		 *	@return	boolean.
		 */
		ACTIV_BASE_API bool operator==(const ConstKeyIterator &rhs) const;

		/**
		 *	@brief	Inequality operator.
		 *
		 *	@param	rhs KeyIterator to compare to.
		 *
		 *	@return	boolean.
		 */
		ACTIV_BASE_API bool operator!=(const ConstKeyIterator &rhs) const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	section section the key is in.
		 *	@param	iterator underlying iterator to key.
		 *	@param	matchLength length of section name to match on when advancing.
		 */
		ConstKeyIterator(const Section &section, const KeyList::const_iterator &iterator, const size_t matchLength);

		const Section &			m_section;						///< Section the key is in
		KeyList::const_iterator	m_iterator;						///< Underlying stl iterator
		const size_t			m_matchLength;					///< Length to match on, or all

		friend class ConstSectionIterator;						///< Allow section iterator access
	};

	/**
	 *	@brief	Const iterator to ini file sections.
	 */
	ACTIV_DECLARE_NESTED_CLASS(ConstSectionIterator)
	{
	public:
		/**
		 *	@brief	Get the complete name of the section.
		 *
		 *	@return	The section name.
		 */
		ACTIV_BASE_API const std::string &GetName() const;

		/**
		 *	@brief	Get just the sub section name (for a top level section this == GetName()).
		 *
		 *	@return	The sub section name.
		 */
		ACTIV_BASE_API std::string GetSubName() const;

		/**
		 *	@brief	Get an iterator to the first key in the section. If there are no keys, will equal
		 *			EndKey().
		 *
		 *	@return	Iterator to the first key in this section.
		 */
		ACTIV_BASE_API ConstKeyIterator BeginKey() const;

		/**
		 *	@brief	Get an iterator to past the last key in the section.
		 *
		 *	@return	Iterator to past the last key in the section.
		 */
		ACTIV_BASE_API ConstKeyIterator EndKey() const;

		/**
		 *	@brief	Find a key by name.
		 *
		 *	@param	keyName key to find.
		 *	@param	matchLength how much of \a keyName to match on. Default is complete match
		 *
		 *	@return	Iterator to the key. Will equal EndKey() if not found.
		 */
		ACTIV_BASE_API ConstKeyIterator FindKey(const std::string &keyName, const size_t matchLength = MATCH_LENGTH_COMPLETE) const;

		/**
		*	@brief	Get an iterator to the first sub section in the section.
		*
		*	@return	Iterator to the first sub section in the section. If the section contains no sub-sections,
					will be equal to EndSubSection().
		*/
		ACTIV_BASE_API ConstSectionIterator BeginSubSection() const;

		/**
		*	@brief	Get an iterator to past the last sub section in the section.
		*
		*	@return	Iterator to past the last sub section in the section.
		*/
		ACTIV_BASE_API ConstSectionIterator EndSubSection() const;

		/**
		 *	@brief	Find a sub section of this section. Sub section names are delimited with sectionDelimiter.
		 *
		 *	@param	subSectionName name of the sub section.
		 *	@param	matchLength how much of \a subSectionName to match on. Default is complete match
		 *
		 *	@return	Iterator to sub section or EndSection() if not found.
		 */
		ACTIV_BASE_API ConstSectionIterator FindSubSection(const std::string &subSectionName, const size_t matchLength = MATCH_LENGTH_COMPLETE) const;

		/**
		 *	@brief	Get an iterator to past the last section in the ini file.
		 *
		 *	@return	Iterator to past the last section in the ini file.
		 */
		ACTIV_BASE_API ConstSectionIterator EndSection() const;

		/**
		 *	@brief	Get string value for a key in this section.
		 *
		 *	@param	keyName key to get value of.
		 *	@param	value reference to a string variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a string value
		 *	@retval	STATUS_CODE_NOT_FOUND key not found
		 */
		ACTIV_BASE_API StatusCode GetStringValue(const std::string &keyName, std::string &value) const;

		/**
		 *	@brief	Get a signed integral value for a key in this section.
		 *
		 *	@param	keyName key to get value of.
		 *	@param	value reference to a variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_OUT_OF_RANGE parsed value is out of range of type T
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a signed value
		 *	@retval	STATUS_CODE_NOT_FOUND key not found
		 */
		template<typename T>
		StatusCode GetSignedValue(const std::string &keyName, T &value) const;

		/**
		 *	@brief	Get an unsigned integral value for a key in this section.
		 *
		 *	@param	keyName key to get value of.
		 *	@param	value reference to a variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_OUT_OF_RANGE parsed value is out of range of type T
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not an unsigned value
		 *	@retval	STATUS_CODE_NOT_FOUND key not found
		 */
		template<typename T>
		StatusCode GetUnsignedValue(const std::string &keyName, T &value) const;

		/**
		 *	@brief	Get a real value for a key in this section.
		 *
		 *	@param	keyName key to get value of.
		 *	@param	value reference to a variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a real value.
		 */
		ACTIV_BASE_API StatusCode GetRealValue(const std::string &keyName, double &value) const;

		/**
		 *	@brief	Get boolean value for a key in this section.
		 *
		 *	@param	keyName key to get value of.
		 *	@param	value reference to a boolean variable to receive the value.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a boolean value
		 *	@retval	STATUS_CODE_NOT_FOUND key not found
		 */
		ACTIV_BASE_API StatusCode GetBooleanValue(const std::string &keyName, bool &value) const;

		/**
		 *	@brief	Advance to the next section in the ini file.
		 *
		 *	@return	*this.
		 */
		ACTIV_BASE_API ConstSectionIterator &operator++();

		/**
		 *	@brief	Equality operator.
		 *
		 *	@param	rhs SectionIterator to compare to.
		 *
		 *	@return	boolean.
		 */
		ACTIV_BASE_API bool operator==(const ConstSectionIterator &rhs) const;

		/**
		 *	@brief	Inequality operator.
		 *
		 *	@param	rhs SectionIterator to compare to.
		 *
		 *	@return	boolean.
		 */
		ACTIV_BASE_API bool operator!=(const ConstSectionIterator &rhs) const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	iniFileParser ini file the section is in.
		 *	@param	iterator underlying iterator to key.
		 *	@param	matchLength length of section name to match on when advancing.
		 */
		ConstSectionIterator(const IniFileParser &iniFileParser, const SectionList::const_iterator &iterator, const size_t matchLength);

		const IniFileParser &		m_iniFileParser;			///< Inifile this section is in
		SectionList::const_iterator	m_iterator;					///< Underlying stl iterator
		const size_t				m_matchLength;				///< Length to match on, or all

		friend class IniFileParser;								///< Allow IniFileParser access
	};

	/**
	 *	@brief	Constructor. Parses the ini file.
	 *
	 *	@param	iniFileName ini file to parse.
	 *
	 *	@throw	ActivException
	 *	@throw	ParseException
	 */
	ACTIV_BASE_API IniFileParser(const std::string &iniFileName);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API ~IniFileParser();

	/**
	 *	@brief	Get an iterator to the first section in the ini file.
	 *
	 *	@return	Iterator to the first section in the ini file. If the ini file contains no sections, will be
	 *			equal to EndSection().
	 */
    ACTIV_BASE_API ConstSectionIterator BeginSection() const;

	/**
	 *	@brief	Get an iterator to past the last section in the ini file.
	 *
	 *	@return	Iterator to past the last section in the ini file.
	 */
	ACTIV_BASE_API ConstSectionIterator EndSection() const;

	/**
	 *	@brief	Find a section in the ini file by name.
	 *
	 *	@param	sectionName name of the section name to find.
	 *	@param	matchLength how much of \a sectionName to match on. Default is complete match
	 *
	 *	@return	Iterator to the section. Will equal EndSection() if the section doesn't exist.
	 */
	ACTIV_BASE_API ConstSectionIterator FindSection(const std::string &sectionName, const size_t matchLength = MATCH_LENGTH_COMPLETE) const;

	/**
	 *	@brief	Get string value for a (section, key).
	 *
	 *	@param	sectionName section to look for key.
	 *	@param	keyName key to get value of.
	 *	@param	value reference to a string variable to receive the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a string value.
	 *	@retval	STATUS_CODE_NOT_FOUND key not found.
	 */
	ACTIV_BASE_API StatusCode GetStringValue(const std::string &sectionName, const std::string &keyName, std::string &value) const;

	/**
	 *	@brief	Get a signed integral value for a (section, key).
	 *
	 *	@param	sectionName section to look for key.
	 *	@param	keyName key to get value of.
	 *	@param	value reference to a variable to receive the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE parsed value is out of range of type T
	 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a signed value
	 *	@retval	STATUS_CODE_NOT_FOUND key not found.
	 */
	template<typename T>
	StatusCode GetSignedValue(const std::string &sectionName, const std::string &keyName, T &value) const;

	/**
	 *	@brief	Get an unsigned integral value for a (section, key).
	 *
	 *	@param	sectionName section to look for key.
	 *	@param	keyName key to get value of.
	 *	@param	value reference to a variable to receive the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_OUT_OF_RANGE parsed value is out of range of type T
	 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not an unsigned value
	 *	@retval	STATUS_CODE_NOT_FOUND key not found.
	 */
	template<typename T>
	StatusCode GetUnsignedValue(const std::string &sectionName, const std::string &keyName, T &value) const;

	/**
	 *	@brief	Get a real value for a (section, key).
	 *
	 *	@param	sectionName section to look for key.
	 *	@param	keyName key to get value of.
	 *	@param	value reference to a variable to receive the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a real value.
	 */
	ACTIV_BASE_API StatusCode GetRealValue(const std::string &sectionName, const std::string &keyName, double &value) const;

	/**
	 *	@brief	Get boolean value for a (section, key).
	 *
	 *	@param	sectionName section to look for key.
	 *	@param	keyName key to get value of.
	 *	@param	value reference to a boolean variable to receive the value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT parsed value is not a boolean value.
	 *	@retval	STATUS_CODE_NOT_FOUND key not found.
	 */
	ACTIV_BASE_API StatusCode GetBooleanValue(const std::string &sectionName, const std::string &keyName, bool &value) const;

	/**
	 *	@brief	Build a section name from a list of sub-section names.
	 *
	 *	@param	count the count of sub-sections.
	 *	@param	... list of \a count sub-section names of type const char *.
	 *
	 *	@return	the section name in the form A:B:C.
	 */
	ACTIV_BASE_API static std::string GetSectionName(const size_t count, ...);

	ACTIV_DECLARE_NESTED_CLASS(Grammar);						///< Grammar class

private:
	SectionList	m_sectionList;									///< Parsed list of sections
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode IniFileParser::GetSignedValue(const std::string &sectionName, const std::string &keyName, T &value) const
{
	ConstSectionIterator iter = FindSection(sectionName);

	if (iter == EndSection())
		return STATUS_CODE_NOT_FOUND;

	return iter.GetSignedValue(keyName, value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode IniFileParser::GetUnsignedValue(const std::string &sectionName, const std::string &keyName, T &value) const
{
	ConstSectionIterator iter = FindSection(sectionName);

	if (iter == EndSection())
		return STATUS_CODE_NOT_FOUND;

	return iter.GetUnsignedValue(keyName, value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode IniFileParser::ConstSectionIterator::GetSignedValue(const std::string &keyName, T &value) const
{
	ConstKeyIterator iter = FindKey(keyName);

	if (iter == EndKey())
		return STATUS_CODE_NOT_FOUND;

	return iter.GetSignedValue(value);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode IniFileParser::ConstSectionIterator::GetUnsignedValue(const std::string &keyName, T &value) const
{
	ConstKeyIterator iter = FindKey(keyName);

	if (iter == EndKey())
		return STATUS_CODE_NOT_FOUND;

	return iter.GetUnsignedValue(value);
}

} // namepsace Activ

#endif // !defined (ACTIV_BASE_INI_FILE_PARSER_H)
