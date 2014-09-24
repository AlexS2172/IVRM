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

class Query;
class IQueryNode;

class Document
{
friend class Index;
public:

	/**
	 *	@brief	Default constructor
	 *
	 *	@param	documentLocale	Locale of document
	 */
	ACTIV_INDEXER_API Document(const std::locale &documentLocale);

	/**
	 *	@brief	Default destructor
	 */
	ACTIV_INDEXER_API ~Document();

	/**
	 *	@brief	Reset the document object (clears document data, terms and values)
	 */
	ACTIV_INDEXER_API void Reset(void);

	/**
	 *	@brief	Change the document locale
	 */
	const std::locale &GetLocale(void);

	/**
	 *	@brief	Change the document locale
	 */
	void SetLocale(const std::locale &documentLocale);

	/**
	 *	@brief	Add data to the document
	 */
	ACTIV_INDEXER_API StatusCode AddData(const void *pData, size_t length, bool copy = false);

	/**
	 *	@brief	Clear the document data (doesn't clear the values)
	 */
	StatusCode ClearData(void);

	/**
	 *	@brief	Add a search term to the document
	 *	If you are calling AddTerm() from multiple threads, make sure you have a seperate stemmer per thread
	 *	or provide suitable locking around the calls to AddTerm()
	 *
	 *	@param	term			The term to add
	 *	@param	stopped			Whether to check the term against the list of stop words.
	 *	@param	caseSensitive	Whether the term is added case sensitively (case sensitive terms will not be stemmed)
	 *	@param	pStemmer		Pointer to a stemmer to use to stem the term (or zero for no stemming).
	 */
	void AddTerm(const std::string &term, bool stopped, bool caseSensitive = false, const Stemmer *pStemmer = 0);

	/**
	 *	@brief	Add a custom value to the document [multiple values with the same name are allowed (max 255)].
	 *	If you are calling AddValue() from multiple threads, make sure you have a seperate stemmer per thread
	 *	or provide suitable locking around the calls to AddTerm()
	 *
	 *	@param	name			The name of this value (e.g. "category", "magazine")
	 *	@param	pValueData		Pointer to the data of the value
	 *	@param	length			The length of the data (max = 65535)
	 *	@param	indexed			Whether the value is searchable in the index
	 *	@param	stopped			Whether to check the data against the list of stop words (only used if indexed == true)
	 *	@param	caseSensitive	Whether the term is added case sensitively (case sensitive values will not be stemmed)
	 *	@param	pStemmer		Pointer to a stemmer to use to stem the data (or zero for no stemming) (only used if indexed == true).
	 *	@param	onlyIndex		If true, then the value is added to the index, but not to the list of document values
	 *							ignored if indexed == false
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH	Data length too long
	 *	@retval	STATUS_CODE_FULL			Too many values with same name
	 */
	ACTIV_INDEXER_API StatusCode AddValue(const std::string &name, const void *pValueData, size_t length, bool indexed, bool stopped, bool caseSensitive = false, const Stemmer *pStemmer = 0, bool onlyIndex = false);

	/**
	 *	@brief	Add a custom value to the document [multiple values with the same name are allowed (max 255)].
	 *	If you are calling AddValue() from multiple threads, make sure you have a seperate stemmer per thread
	 *	or provide suitable locking around the calls to AddTerm()
	 *
	 *	@param	name			The name of this value (e.g. "category", "magazine")
	 *	@param	valueData		ByteVector containging the value data
	 *	@param	length			The length of the data (max = 65535)
	 *	@param	indexed			Whether the value is searchable in the index
	 *	@param	stopped			Whether to check the data against the list of stop words (only used if indexed == true)
	 *	@param	caseSensitive	Whether the term is added case sensitively (case sensitive values will not be stemmed)
	 *	@param	pStemmer		Pointer to a stemmer to use to stem the data (or zero for no stemming) (only used if indexed == true).
	 *	@param	onlyIndex		If true, then the value is added to the index, but not to the list of document values
	 *							ignored if indexed == false
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH	Data length too long
	 *	@retval	STATUS_CODE_FULL			Too many values with same name
	 */
	StatusCode AddValue(const std::string &name, const ByteVector &valueData, bool indexed, bool stopped, bool caseSensitive = false, const Stemmer *pStemmer = 0, bool onlyIndex = false);

	/**
	 *	@brief	Add a custom value to the document [multiple values with the same name are allowed (max 255)].
	 *	If you are calling AddValue() from multiple threads, make sure you have a seperate stemmer per thread
	 *	or provide suitable locking around the calls to AddTerm()
	 *
	 *	@param	name			The name of this value (e.g. "category", "magazine")
	 *	@param	valueData		String containging the value data
	 *	@param	indexed			Whether the value is searchable in the index
	 *	@param	stopped			Whether to check the data against the list of stop words (only used if indexed == true)
	 *	@param	caseSensitive	Whether the term is added case sensitively (case sensitive values will not be stemmed)
	 *	@param	pStemmer		Pointer to a stemmer to use to stem the data (or zero for no stemming) (only used if indexed == true).
	 *	@param	onlyIndex		If true, then the value is added to the index, but not to the list of document values
	 *							ignored if indexed == false
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_LENGTH	Data length too long
	 *	@retval	STATUS_CODE_FULL			Too many values with same name
	 */
	StatusCode AddValue(const std::string &name, const std::string &valueData, bool indexed, bool stopped, bool caseSensitive = false, const Stemmer *pStemmer = 0, bool onlyIndex = false);

	/**
	 *	@brief	Clear all the values with given name
	 *
	 *	@param	name	The name of this value (e.g. "category", "magazine")
	 */
	ACTIV_INDEXER_API void ClearValues(const std::string &name);

	/**
	 *	@brief	Does the document match a given query object?
	 *
	 *	@param	query		Query object to match against.  The query *MUST* have been Prepare()'d
	 *	@param	result		Whether the document matches or not is returned in this parameter
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval ...
	 */
	StatusCode MatchesQuery(const Query &query, bool &result) const;

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
	 *	@brief	Get the document locale
	 */
	const std::locale &GetLocale(void) const;

	/**
	 *	@brief	Add a term to the global list of stop word.
	 *
	 *	The default list of stop words is:
	 *	"a", "am", "an", "and", "are", "as", "at", "be", "but", "by", "for", "got", "had", "has", "have",
	 *	"i", "if", "in", "is", "it", "nor", "not", "of", "on", "or", "so", "than", "that", "the", "to", "too",
	 *	"was", "we", "with"
	 */
	static void AddStopWord(const std::string &term);

	/**
	 *	@brief	Check if a word is a stop word
	 *	NB this function is called automatically by \a AddTerm()
	 *
	 *	@param	term	Term to check against stop words list
	 */
	static bool IsStopWord(const std::string &term);

	/**
	 *	@brief Convert a value to a term that can be used to search the index
	 */
	static std::string ConvertValueToIndexTerm(const std::string &name, const ByteVector &valueData);

	/**
	 *	@brief Convert a value to a term that can be used to search the index
	 */
	static std::string ConvertValueToIndexTerm(const std::string &name, const std::string &valueData);

	/**
	 *	@brief Convert a value to a term that can be used to search the index
	 */
	ACTIV_INDEXER_API static std::string ConvertValueToIndexTerm(const std::string &name, const void *pData, size_t dataLength);

private:
	/**
	 *	@brief	Does the document match a given query node and all its sub-nodes?
	 *
	 *	@param	query		Query node to match against
	 *	@param	result		Whether the document matches or not is returned in this parameter
	 */
	ACTIV_INDEXER_API StatusCode MatchesQuery(const IQueryNode *pQuery, bool &result) const;

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
// --------------------------------------------------------------------------------------------------------------------

inline const std::locale &Document::GetLocale(void)
{
	return m_locale;
}

// --------------------------------------------------------------------------------------------------------------------

inline void Document::SetLocale(const std::locale &documentLocale)
{
	m_locale = documentLocale;
}

// --------------------------------------------------------------------------------------------------------------------

inline StatusCode Document::ClearData(void)
{
	return AddData(0, 0, false);
}

// --------------------------------------------------------------------------------------------------------------------

inline void Document::AddTerm(const std::string &term, bool stopped, bool caseSensitive, const Stemmer *pStemmer)
{
	// Make lowercase according to the document locale
	std::string lowercaseTerm(term);
	StringToLowerWithLocale(lowercaseTerm, m_locale);

	if (stopped && IsStopWord(lowercaseTerm))
		return;

	if (caseSensitive)
	{
		m_termList.insert(term);
	}
	else
	{
		const std::string &stemmedTerm = ((0 == pStemmer) ? lowercaseTerm : pStemmer->StemWord(lowercaseTerm));
		m_termList.insert(stemmedTerm);
	}
}

// --------------------------------------------------------------------------------------------------------------------

inline StatusCode Document::AddValue(const std::string &name, const ByteVector &valueData, bool indexed, bool stopped, bool caseSensitive, const Stemmer *pStemmer, bool onlyIndex)
{
	return AddValue(name, &(valueData[0]), valueData.size(), indexed, stopped, caseSensitive, pStemmer, onlyIndex);
}

// --------------------------------------------------------------------------------------------------------------------

inline StatusCode Document::AddValue(const std::string &name, const std::string &valueData, bool indexed, bool stopped, bool caseSensitive, const Stemmer *pStemmer, bool onlyIndex)
{
	return AddValue(name, valueData.data(), valueData.length(), indexed, stopped, caseSensitive, pStemmer, onlyIndex);
}

// --------------------------------------------------------------------------------------------------------------------

inline StatusCode Document::MatchesQuery(const Query &query, bool &result) const
{
	return MatchesQuery(query.m_rootNode, result);
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

inline const ValueMap &Document::GetValues(void) const
{
	return m_valueMap;
}

// --------------------------------------------------------------------------------------------------------------------

inline const std::locale &Document::GetLocale(void) const
{
	return m_locale;
}

// --------------------------------------------------------------------------------------------------------------------

inline void Document::AddStopWord(const std::string &term)
{
	m_stopWords.insert(term);
}

// --------------------------------------------------------------------------------------------------------------------

inline bool Document::IsStopWord(const std::string &term)
{
	return (0 != m_stopWords.count(term));
}

// --------------------------------------------------------------------------------------------------------------------

inline std::string Document::ConvertValueToIndexTerm(const std::string &name, const ByteVector &valueData)
{
	return ConvertValueToIndexTerm(name, &valueData[0], valueData.size());
}

// --------------------------------------------------------------------------------------------------------------------

inline std::string Document::ConvertValueToIndexTerm(const std::string &name, const std::string &valueData)
{
	return ConvertValueToIndexTerm(name, valueData.data(), valueData.length());
}

} // namespace Indexer

} // namespace Activ

#endif // !defined (ACTIV_INDEXER_DOCUMENT_H)
