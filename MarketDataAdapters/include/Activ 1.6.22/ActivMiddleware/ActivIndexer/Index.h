/**
 *  @file	Index.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_INDEXER_INDEX_H)
#define ACTIV_INDEXER_INDEX_H

#include "ActivMiddleware/ActivIndexer/External.h"
#include "ActivMiddleware/ActivIndexer/Typedefs.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/Misc/StlHelper.h"

#include <map>
#include <string>
#include "boost/scoped_ptr.hpp"

namespace Activ
{

class UiIo;
class MqService;
class LocalMq;

namespace Indexer
{

class Document;
class IQueryNode;
class Query;
class Stemmer;

template <typename BlockType> class DeltaDocumentList;
template <typename BlockType> class GolombDocumentList;

class Index
{
public:
	/**
	 *	@brief	Settings class
	 */
	class Settings
	{
	public:
		std::string		m_dataDir;		///< Directory where data is held
		uint32_t		m_maxNumQueues;	///< Maximum number of separate data queues
	};

	/**
	 *	@brief	Reindexing callback functor used in constructor.
	 *
	 *	The Reindex Callback is called when the document index and document data are out of step.
	 *	The index should be able to be reconstructed from the data.
	 *
	 *	This default callback simply returns STATUS_CODE_IGNORED which tells the constructor
	 *	not to bother reindexing the document.  To provide your own callback, derive a class
	 *	from this one and overload the () operator.
	 *
	 *	The overloaded function should simply populate the terms of the document using AddTerm()
	 *	and return STATUS_CODE_SUCCESS to signal that this has been done successfully and the
	 *	document is ready to be reindexed.  If you wish to add a document value as an indexable term use:
	 *	document.AddTerm(Document::ConvertValueToIndexTerm(valueName, valueData));
	 */
	struct ReindexCallback
	{
		virtual StatusCode operator()(Document &document) const
		{
			return STATUS_CODE_IGNORED;
		}
	};

	/**
	 *	@brief	Default constructor
	 *
	 *	@param	settings			Settings for the index
	 *	@param	pReindexCallback	Pointer to reindex callback functor. The callback should populate the terms of the document.
	 */
	ACTIV_INDEXER_API Index(const Settings &settings, ReindexCallback *pReindexCallback = 0);

	/**
	 *	@brief	Default destructor
	 */
	ACTIV_INDEXER_API ~Index();

	/**
	 *	@brief	Add a new data 'queue' to the index
	 */
	ACTIV_INDEXER_API StatusCode AddDataQueue(const Date &queueDate);

	/**
	 *	@brief	Get the date of the current data queue
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_INDEXER_API StatusCode GetCurrentDataQueueDate(Date &queueDate);

	/**
	 *	@brief	Add a document to the index
	 */
	ACTIV_INDEXER_API StatusCode AddDocument(Document &document);

	/**
	 *	@brief	Compact the index (merge the tempory index into the main one)
	 */
	ACTIV_INDEXER_API StatusCode CompactIndex(void);

	/**
	 *	@brief	Perform a query on the index.
	 *
	 *	@param	query		The query to perform.  The query *MUST* be reduced before calling this function
	 *	@param	docList		Will hold the list of document IDs that match the query
	 *	@param	pStemmer	Pointer to stemmer used to stem the terms of the query (or zero for no stemming)
	 *	@param	loc			The locale that the query terms are in
	 */
	ACTIV_INDEXER_API StatusCode Query(const Indexer::Query &query, DocumentList &docList, Stemmer *pStemmer = 0, const std::locale &loc = m_defaultLocale);

	/**
	 *	@brief	Get a list of documents that a term occurs in
	 *
	 *	@param	term		Term to search for
	 *	@param	docList		Will hold the list of document IDs that match the query
	 *	@param	pStemmer	Pointer to stemmer used to stem the terms of the query (or zero for no stemming)
	 *	@param	loc			The locale that the query terms are in
	 */
	ACTIV_INDEXER_API StatusCode QueryTerm(std::string &term, DocumentList &docList, Stemmer *pStemmer = 0, const std::locale &loc = m_defaultLocale);

	/**
	 *	@brief	Get a list of documents that a value occurs in
	 *
	 *	@param	valueName	The name of the value to find
	 *	@param	valueData	The data of the value to find
	 *	@param	docList		Will hold the list of document IDs that match the query
	 *	@param	loc			The locale that the value is in
	 */
	ACTIV_INDEXER_API StatusCode QueryValue(const std::string &valueName, const std::string &valueData, DocumentList &docList, const std::locale &loc = m_defaultLocale);

	/**
	 *	@brief	Find a term in the index
	 *
	 *	@param	term		The term to find
	 *	@param	pStemmer	Pointer to stemmer used to stem the word (or zero for no stemming)
	 *	@param	loc			The locale that the term is in
	 *
	 *	@retval	true		The term was found
	 *	@retval	false		The term was not found
	 */
	ACTIV_INDEXER_API bool FindTerm(std::string &term, Stemmer *pStemmer = 0, const std::locale &loc = m_defaultLocale) const;

	/**
	 *	@brief	Find a value in the index
	 *
	 *	@param	valueName	The name of the value to find
	 *	@param	valueData	The data of the value to find
	 *	@param	loc			The locale that the value is in
	 *
	 *	@retval	true		The value was found
	 *	@retval	false		The value was not found
	 */
	ACTIV_INDEXER_API bool FindValue(const std::string &valueName, const std::string &valueData, const std::locale &loc = m_defaultLocale) const;

	/**
	 *	@brief	Get a document
	 */
	ACTIV_INDEXER_API StatusCode GetDocument(DocumentId documentId, Document &document);

	/**
	 *	@brief	Get document data only
	 */
	ACTIV_INDEXER_API StatusCode GetDocumentData(DocumentId documentId, MessageBuilder &documentData);

	/**
	 *	@brief	Get a single document value only
	 */
	ACTIV_INDEXER_API StatusCode GetDocumentValue(DocumentId documentId, const std::string &valueName, std::vector<ByteVector> &valueData);

	/**
	 *	@brief	Write the index to disk
	 */
	ACTIV_INDEXER_API StatusCode WriteIndexToDisk(void);

	/**
	 *	@brief	Display some statistics about the index
	 */
	ACTIV_INDEXER_API void UiDisplayStatistics(const UiIo &uiIo);

	/**
	 *	@brief	Set the default locale for new documents / queries
	 */
	static void SetDefaultLocale(const std::locale &defaultLocale);

	/**
	 *	@brief	Get the default locale for new documents / queries
	 */
	static const std::locale &GetDefaultLocale(void);

private:
	typedef uint64_t BlockType;
	typedef hash_map<std::string, GolombDocumentList<BlockType> *, StringHashCompare<> > TermHashMap;
	typedef std::map<std::string, DeltaDocumentList<BlockType> * > TempTermMap;
	typedef std::vector<LocalMq *> DocumentQueueList;

	/**
	 *	@brief	Merge a temp map entry into the main map
	 */
	StatusCode MergeTermList(const TempTermMap::const_iterator &tempTermIter);

	/**
	 *	@brief	Read the index from disk
	 */
	StatusCode ReadIndexFromDisk(DocumentId &firstDocumentId, DocumentId &nextDocumentId);

	/**
	 *	@brief	Reindex a document
	 */
	StatusCode Reindex(DocumentId &documentId, ReindexCallback &reindexCallback);

	/**
	 *	@brief	Recalculate document offsets
	 */
	StatusCode RecalculateDocumentOffsets(DocumentId prevFirstDocumentId, DocumentId newFirstDocumentId);

	/**
	 *	@brief	Perform a query on a node and all its subnodes.
	 *
	 *	@param	pQuery		Pointer to the root node of the query to perform.  The query *MUST* be reduced before calling this function
	 *	@param	docList		Will hold the list of document IDs that match the query
	 *	@param	pStemmer	Pointer to stemmer used to stem the terms of the query (or zero for no stemming)
	 *	@param	loc			The locale that the query terms are in
	 */
	StatusCode Query(const IQueryNode *pQuery, DocumentList &docList, Stemmer *pStemmer = 0, const std::locale &loc = m_defaultLocale);

	const Settings					m_settings;			///< The settings for the index
	boost::scoped_ptr<MqService>	m_mqService;		///< Pointer to the MQ Service for storing document data
	DocumentId						m_firstDocumentId;	///< Document ID of the first document in the index
	DocumentId						m_nextDocumentId;	///< Document ID of next document to be added
	TermHashMap						m_termMap;			///< Main map of term -> doc IDs
	TempTermMap						m_tempTermMap;		///< Tempory map of term -> doc IDs
	DocumentQueueList				m_documentQueues;	///< List of all open document queues

	ACTIV_INDEXER_API static std::locale	m_defaultLocale;	///< Default locale for documents
};

// --------------------------------------------------------------------------------------------------------------------

inline void Index::SetDefaultLocale(const std::locale &defaultLocale)
{
	m_defaultLocale = defaultLocale;
}

// --------------------------------------------------------------------------------------------------------------------

inline const std::locale &Index::GetDefaultLocale(void)
{
	return m_defaultLocale;
}


} // namespace Indexer

} // namespace Activ

#endif // !defined (ACTIV_INDEXER_INDEX_H)
