/**
 *  @file	Document.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_INDEXER_DOCUMENT_H)
#define ACTIV_INDEXER_DOCUMENT_H

#include "ActivMiddleware/ActivIndexer/External.h"
#include "ActivMiddleware/ActivIndexer/Index.h"
#include "ActivMiddleware/ActivIndexer/Stemmer.h"
#include "ActivMiddleware/ActivIndexer/Typedefs.h"

#include "ActivMiddleware/Misc/StlHelper.h"

#include <algorithm>
#include <set>
#include <map>
#include <locale>

#include "boost/scoped_array.hpp"

namespace Activ
{

namespace Indexer
{

class Document
{
friend class Index;
public:

	/**
	 *	@brief	Map type used for storing document values
	 */
	typedef std::multimap<std::string, ByteVector> ValueMap; 

	/**
	 *	@brief	Default constructor
	 */
	ACTIV_INDEXER_API Document(const std::locale &documentLocale = Index::GetDefaultLocale());

	/**
	 *	@brief	Default destructor
	 */
	ACTIV_INDEXER_API ~Document();

	/**
	 *	@brief	Reset the document object
	 */
	ACTIV_INDEXER_API void Reset(void);

	/**
	 *	@brief	Add data to the document
	 */
	ACTIV_INDEXER_API StatusCode AddData(const void *pData, size_t length, bool copy = false);

	/**
	 *	@brief	Add a search term to the document
	 *	If you are calling AddTerm() from multiple threads, make sure you have a seperate stemmer per thread
	 *	or provide suitable locking around the calls to AddTerm()
	 *
	 *	@param	term		The term to add
	 *	@param	pStemmer	Pointer to a stemmer to use to stem the term (or zero for no stemming).
	 */
	void AddTerm(std::string &term, Stemmer *pStemmer = 0);

	/**
	 *	@brief	Add a custom value to the document [multiple values with the same name are allowed (max 255)].
	 *
	 *	@param	name		The name of this value (e.g. "category", "magazine")
	 *	@param	pValueData	Pointer to the data of the value
	 *	@param	length		The length of the data (max = 65535)
	 *	@param	indexed		Whether the value is searchable in the index
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH	Data length too long
	 *	@retval	STATUS_CODE_FULL			Too many values with same name
	 */
	ACTIV_INDEXER_API StatusCode AddValue(const std::string &name, const void *pValueData, size_t length, bool indexed);

	/**
	 *	@brief	Add a custom value to the document [multiple values with the same name are allowed (max 255)].
	 *
	 *	@param	name		The name of this value (e.g. "category", "magazine")
	 *	@param	valueString	String containging the value data
	 *	@param	indexed		Whether the value is searchable in the index
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH	Data length too long
	 *	@retval	STATUS_CODE_FULL			Too many values with same name
	 */
	StatusCode AddValue(const std::string &name, const std::string &valueString, bool indexed);

	/**
	 *	@brief	Add a date to the document
	 *	NOTE: this adds a value with the name "date". You should avoid adding other values with the name "date"
	 *
	 *	@param	date	The date to add to the document
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH	Data length too long
	 *	@retval	STATUS_CODE_FULL			Too many values with same name
	 */
	StatusCode AddDate(const Activ::Date &date);

	/**
	 *	@brief	Get documentID of this document
	 */
	DocumentId GetDocumentId(void) const;

	/**
	 *	@brief	Get pointer to the document data
	 */
	const void *GetData(void) const;

	/**
	 *	@brief	Get length of document data
	 */
	size_t GetLength(void) const;

	/**
	 *	@brief	Get the term list
	 */
	const StringSet &GetTerms(void) const;

	/**
	 *	@brief	Get the term list
	 */
	const ValueMap &GetValues(void) const;
	
	/**
	 *	@brief	Add a term to the global list of stop word.
	 *
	 *	The default list of stop words is:
	 *	"a", "am", "an", "and", "are", "as", "at", "be", "be", "but", "by", "for", "got", "had", "has",
	 *	"i", "if", "in", "is", "it", "nor", "not", "of", "or", "so", "that", "the", "to", "too", "we"
	 */
	static void AddStopWord(const std::string &term);

	/**
	 *	@brief Convert a value to a term that can be used to search the index
	 */
	ACTIV_INDEXER_API static std::string ConvertValueToIndexTerm(const std::string &name, const ByteVector &valueData);

	/**
	 *	@brief Convert a value to a term that can be used to search the index
	 */
	ACTIV_INDEXER_API static std::string ConvertValueToIndexTerm(const std::string &name, const std::string &valueData);

private:
	DocumentId								m_documentId;	///< Document ID of this document

	const void *							m_pData;		///< Pointer to document data
	size_t									m_length;		///< Length of document data

	boost::scoped_array<byte_t>				m_buffer;		///< Temporary buffer
	size_t									m_bufferLength;	///< Size of temporary buffer
	
	StringSet								m_termList;		///< List of terms associated with this document
	ValueMap								m_valueMap;		///< List of values

	std::locale								m_locale;		///< Locale for this document

	ACTIV_INDEXER_API static StringHashSet	m_stopWords;	///< List of terms to ignore
};

// --------------------------------------------------------------------------------------------------------------------

inline void Document::AddTerm(std::string &term, Stemmer *pStemmer)
{
	// Make lowercase according to the document locale
	Indexer::StringToLowerWithLocale(term, m_locale);

	if (0 == m_stopWords.count(term))
	{
		const std::string &stemmedTerm = ((0 == pStemmer) ? term : pStemmer->StemWord(term));
		m_termList.insert(stemmedTerm);
	}
}

// --------------------------------------------------------------------------------------------------------------------

inline StatusCode Document::AddValue(const std::string &name, const std::string &valueString, bool indexed)
{
	return AddValue(name, valueString.data(), valueString.length(), indexed);
}

// --------------------------------------------------------------------------------------------------------------------

inline StatusCode Document::AddDate(const Activ::Date &date)
{
	uint32_t julianDate = HostToLittleEndianOrder(date.Get());
	return AddValue("date", &julianDate, sizeof(julianDate), false);
}

// --------------------------------------------------------------------------------------------------------------------

inline DocumentId Document::GetDocumentId(void) const
{
	return m_documentId;
}

// --------------------------------------------------------------------------------------------------------------------

inline const void *Document::GetData(void) const
{
	return m_pData;
}

// --------------------------------------------------------------------------------------------------------------------

inline size_t Document::GetLength(void) const
{
	return m_length;
}

// --------------------------------------------------------------------------------------------------------------------

inline const StringSet &Document::GetTerms(void) const
{
	return m_termList;
}

// --------------------------------------------------------------------------------------------------------------------

inline const Document::ValueMap &Document::GetValues(void) const
{
	return m_valueMap;
}

// --------------------------------------------------------------------------------------------------------------------

inline void Document::AddStopWord(const std::string &term)
{
	m_stopWords.insert(term);
}

} // namespace Indexer

} // namespace Activ

#endif // !defined (ACTIV_INDEXER_DOCUMENT_H)
