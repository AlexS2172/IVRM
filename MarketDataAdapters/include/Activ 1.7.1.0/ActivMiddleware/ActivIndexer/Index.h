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
#include "ActivMiddleware/ActivIndexer/Query.h"

#include "ActivMiddleware/ActivBase/IniFileParser.h"
#include "ActivMiddleware/Misc/StlHelper.h"

#include <string>
#include "boost/scoped_ptr.hpp"

#if defined USE_GOOGLE_MAPS
#if defined (ACTIV_COMPILER_VISUAL_C)
// Disable crappy warnings from google sparse hash map in vc++
#pragma warning(push)
#pragma warning(disable : 4267 4244)
#endif

#include "google/sparse_hash_map"

#if defined (ACTIV_COMPILER_VISUAL_C)
#pragma warning(pop)
#endif
#endif


namespace Activ
{

class Date;
class LocalMq;
class MessageBuilder;
class MqService;
class UiIo;
class UiMenu;

namespace Indexer
{

class Document;
class Stemmer;
struct IndexInfo;
struct DocumentRange;

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
		ACTIV_INDEXER_API Settings();

		ACTIV_INDEXER_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		std::string		m_dataDir;				///< Directory where data is held
		uint32_t		m_maxNumQueues;			///< Maximum number of separate data queues

		uint32_t		m_queueDataFileSize;	///< Size of each sub-file in data queues
		uint32_t		m_queueIndexFileSize;	///< Size of each sub-file in data queue indexes
	};

	/**
	 *	@brief	Reindexing callback functor used in constructor.
	 *
	 *	The Reindex Callback is called when the document index and document data are out of step.
	 *	The index should be able to be reconstructed from the data.
	 *
	 *	To provide your own callback, derive a class from this one and overload the () operator.
	 *
	 *	The overloaded function should simply populate the terms of the document using AddTerm()
	 *	and return STATUS_CODE_SUCCESS to signal that this has been done successfully and the
	 *	document is ready to be reindexed.  If you wish to add a document value as an indexable term use:
	 *	document.AddTerm(Document::ConvertValueToIndexTerm(valueName, valueData));
	 */
	struct ReindexCallback
	{
		virtual StatusCode operator()(Document &document) const = 0;

		virtual ~ReindexCallback()
		{
		}
	};

	/**
	 *	@brief	Default constructor
	 *
	 *	@param	settings			Settings for the index
	 *	@param	defaultLocale		Default locale
	 *	@param	pReindexCallback	Pointer to reindex callback functor. The callback should populate the terms of the document.
	 */
	ACTIV_INDEXER_API Index(const Settings &settings, const std::locale &defaultLocale, const ReindexCallback *pReindexCallback = 0);

	/**
	 *	@brief	Default destructor
	 */
	ACTIV_INDEXER_API ~Index();

	/**
	 *	@brief	Add a new data 'queue' to the index
	 *
	 *	@param	queueDate		Date of queue to add
	 *	@param	bufferSize		Size of buffer for old data. E.g. if buffer size is 1000, you can insert 1000
								records into the previous queue before it fills up
	 *	@param	dropOldQueues	Whether to drop old queues if the number of queues > settings.m_maxNumQueues
	 *							If false, you can drop the old queues later on by calling DropOldDataQueues()
	 */
	ACTIV_INDEXER_API StatusCode AddDataQueue(const Date &queueDate, uint32_t bufferSize, bool dropOldQueues);

	/**
	 *	@brief	Drop old queues if the number of queues > settings.m_maxNumQueues
	 */
	ACTIV_INDEXER_API StatusCode DropOldDataQueues(void);

	/**
	 *	@brief	Get the date of the current data queue
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_INDEXER_API StatusCode GetCurrentDataQueueDate(Date &queueDate) const;

	/**
	 *	@brief	Get the date of the first data queue
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_INDEXER_API StatusCode GetFirstDataQueueDate(Date &queueDate) const;

	/**
	 *	@brief	Get a list of all the avaliable queues
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_INDEXER_API StatusCode GetAvailableQueues(std::vector<Date> &queueDates) const;

	/**
	 *	@brief	Get the range of document ids in the index
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_INDEXER_API StatusCode GetDocumentIds(DocumentId &firstDocId, DocumentId &nextDocId) const;

	/**
	 *	@brief	Get the range of document ids contined in a specific queue
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_INDEXER_API StatusCode GetDocumentIdsForQueue(const Date &queueDate, DocumentId &firstDocId, DocumentId &nextDocId) const;

	/**
	 *	@brief	Add a document to the index
	 *
	 *	@param	document	The document to add
	 *	@param	queueDate	The date of the queue to add the document to (adds to latest queue if uninitialized)
	 */
	ACTIV_INDEXER_API StatusCode AddDocument(Document &document, const Date &queueDate);

	/**
	 *	@brief	Delete a document from the index
	 *	The document will only be marked as deleted, and queries will still return the document,
	 *	however any attempt to retrieve the document will return in STATUS_CODE_DELETED
	 *
	 *	@param	documentId	Id of the documewnt to delete
	 */
	ACTIV_INDEXER_API StatusCode DeleteDocument(DocumentId documentId);

	/**
	 *	@brief	Update a document in the index
	 *	If the new document is the same size or smaller than the existing one, and doesn't need to
	 *	be reindexed, then it will be overwritten. If the new document is bigger than the existing one
	 *	or does need to be reindexed, then it will be deleted and re-added with the new	data
	 *	(this will result in a new document ID)
	 *
	 *	@param	documentId		Document to update
	 *	@param	document		New document data
	 *	@param	needsReindex	Have any indexable terms/values changed?
	 */
	ACTIV_INDEXER_API StatusCode UpdateDocument(DocumentId documentId, Document &document, bool needsReindex = true);

	/**
	 *	@brief	Update a document in the index and move it to a new queue
	 *	(this will result in a new document ID)
	 *
	 *	@param	documentId		Document to update
	 *	@param	document		New document data
	 *	@param	newQueueDate	The date of the queue to move the document to
	 */
	ACTIV_INDEXER_API StatusCode UpdateDocumentAndMove(DocumentId documentId, Document &document, const Date &newQueueDate);

	/**
	 *	@brief	Compact the index (merge the tempory index into the main one)
	 */
	ACTIV_INDEXER_API StatusCode CompactIndex(void);

	/**
	 *	@brief	Perform a query on the index.
	 *
	 *	@param	query		The query to perform.  The query *MUST* be Prepare()'d before calling this function
	 *	@param	docList		Will hold the list of document IDs that match the query
	 */
	StatusCode Query(const Indexer::Query &query, DocumentList &docList);

	/**
	 *	@brief	Get a list of documents that a term occurs in
	 *
	 *	@param	term			Term to search for
	 *	@param	docList			Will hold the list of document IDs that match the query
	 *	@param	pStemmer		Pointer to stemmer used to stem the terms of the query (or zero for no stemming)
	 *	@param	caseSensitive	Whether to search for the term case sensitively
	 *	@param	pLocale			The locale that the query terms are in (or zero to use default)
	 */
	ACTIV_INDEXER_API StatusCode QueryTerm(const std::string &term, DocumentList &docList, const Stemmer *pStemmer = 0, bool caseSensitive = false, const std::locale *pLocale = 0);

	/**
	 *	@brief	Get a list of documents that a value occurs in
	 *
	 *	@param	valueName		The name of the value to find
	 *	@param	valueData		The data of the value to find
	 *	@param	docList			Will hold the list of document IDs that match the query
	 *	@param	pStemmer		Pointer to stemmer used to stem the value data in the query (or zero for no stemming)	 
	 *	@param	caseSensitive	Whether to search for the term case sensitively
	 *	@param	pLocale			The locale that the value is in (or zero to use default)
	 */
	ACTIV_INDEXER_API StatusCode QueryValue(const std::string &valueName, const std::string &valueData, DocumentList &docList, const Stemmer *pStemmer = 0, bool caseSensitive = false, const std::locale *pLocale = 0);

	/**
	 *	@brief	Find a term in the index
	 *
	 *	@param	term			The term to find
	 *	@param	pStemmer		Pointer to stemmer used to stem the word (or zero for no stemming)
	 *	@param	caseSensitive	Whether to search for the term case sensitively
	 *	@param	pLocale			The locale that the term is in (or zero to use default)
	 *
	 *	@retval	true			The term was found
	 *	@retval	false			The term was not found
	 */
	ACTIV_INDEXER_API bool FindTerm(const std::string &term, const Stemmer *pStemmer = 0, bool caseSensitive = false, const std::locale *pLocale = 0) const;

	/**
	 *	@brief	Find a value in the index
	 *
	 *	@param	valueName		The name of the value to find
	 *	@param	valueData		The data of the value to find
	 *	@param	pStemmer		Pointer to stemmer used to stem the value data (or zero for no stemming)
	 *	@param	caseSensitive	Whether to search for the term case sensitively
	 *	@param	pLocale			The locale that the value is in (or zero to use default)
	 *
	 *	@retval	true			The value was found
	 *	@retval	false			The value was not found
	 */
	ACTIV_INDEXER_API bool FindValue(const std::string &valueName, const std::string &valueData, const Stemmer *pStemmer = 0, bool caseSensitive = false, const std::locale *pLocale = 0) const;

	/**
	 *	@brief	Get a document (the terms of the document will not be populated)
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
	 *	@brief	Get all document values
	 */
	ACTIV_INDEXER_API StatusCode GetDocumentValues(DocumentId documentId, ValueMap &valueData);

	/**
	 *	@brief	Write the index to disk
	 */
	ACTIV_INDEXER_API StatusCode WriteIndexToDisk(void);

	/**
	 *	@brief	Add all indexer options to a menu
	 */
	ACTIV_INDEXER_API void UiAddAllOptions(UiMenu &uiMenu);

	/**
	 *	@brief	Display some statistics about the index
	 */
	ACTIV_INDEXER_API void UiDisplayStatistics(const UiIo &uiIo);

	/**
	 *	@brief	Display some statistics about a term
	 */
	ACTIV_INDEXER_API void UiDisplayTermStatistics(const UiIo &uiIo);

	/**
	 *	@brief	Display most popular terms
	 */
	ACTIV_INDEXER_API void UiDisplayPopularTerms(const UiIo &uiIo);

	/**
	 *	@brief	Display stats about queues
	 */
	ACTIV_INDEXER_API void UiDisplayQueues(const UiIo &uiIo);

	/**
	 *	@brief	Set the default locale for new documents / queries
	 */
	void SetDefaultLocale(const std::locale &defaultLocale);

	/**
	 *	@brief	Get the default locale for new documents / queries
	 */
	const std::locale &GetDefaultLocale(void);

	/**
	 *	@brief	Get the default name of index files
	 */
	static const std::string &GetDefaultIndexFilename(void);

private:
#if defined USE_GOOGLE_MAPS
#define HASH_MAP google::sparse_hash_map
#else
#define HASH_MAP Activ::hash_map
#endif

	typedef uint64_t BlockType;
	typedef HASH_MAP<std::string, GolombDocumentList<BlockType> *, StringHashCompare<> > TermHashMap;
	typedef HASH_MAP<std::string, DeltaDocumentList<BlockType> *, StringHashCompare<> > TempTermMap;
	typedef std::vector<LocalMq *> DocumentQueueList;
	typedef std::map<uint32_t, LocalMq *> DocumentQueueMap;

	/**
	 *	@brief	Perform cleanup work
	 */
	void Cleanup(void);

	/**
	 *	@brief	Add a document to a specified queue
	 */
	StatusCode AddDocument(Document &document, LocalMq &queue);

	/**
	 *	@brief	Add document terms to the index
	 */
	StatusCode AddTerms(const Document &document);

	/**
	 *	@brief	Check the range of docIds from the index against a given first/next pair
	 *			Any missing documents will be reindexed and any erroneus documents will be removed
	 */
	void ValidateIndexDocRange(const DocumentRange &indexRange, DocumentId firstDocumentId, DocumentId nextDocumentId, const ReindexCallback *pReindexCallback);

	/**
	 *	@brief	Get the queue that a document lives in
	 */
	StatusCode GetQueueForDocumentId(DocumentId documentId, LocalMq *&pQueue);

	/**
	 *	@brief	Check if a queue is full
	 */
	bool IsQueueFull(const LocalMq &queue);

	/**
	 *	@brief	Merge a temp map entry into the main map
	 */
	StatusCode MergeTermList(const TempTermMap::iterator &tempTermIter);

	/**
	 *	@brief	Faster but more unreliable version of CompactIndex()
	 */
	void CompactIndexFast(void);

	/**
	 *	@brief	Read the index from disk
	 */
	StatusCode ReadIndexFromDisk(IndexInfo &indexInfo);

	/**
	 *	@brief	Reindex a range of documents
	 */
	StatusCode Reindex(const DocumentId firstDocumentId, const DocumentId endDocumentId, const ReindexCallback *pReindexCallback);

	/**
	 *	@brief	Reindex a document
	 */
	StatusCode Reindex(const DocumentId documentId, const ReindexCallback &reindexCallback);

	/**
	 *	@brief	Remove some document Ids from the index
	 */
	StatusCode RemoveDocumentsFromIndex(DocumentId firstDocumentId, DocumentId lastDocumentId);

	/**
	 *	@brief	Recalculate document offsets
	 */
	StatusCode RecalculateDocumentOffsets(DocumentId prevFirstDocumentId, DocumentId newFirstDocumentId);

	/**
	 *	@brief	Perform a query on a node and all its subnodes.
	 *
	 *	@param	pQuery		Pointer to the root node of the query to perform.  The query *MUST* be reduced before calling this function
	 *	@param	docList		Will hold the list of document IDs that match the query
	 */
	ACTIV_INDEXER_API StatusCode Query(const IQueryNode *pQuery, DocumentList &docList);

	/**
	 *	@brief	Get a list of documents that a stemmed term occurs in
	 */
	StatusCode QueryStemmedTerm(const std::string &term, DocumentList &docList);

	/**
	 *	@brief	Find a term that has already been stemmed / converted to lowercase
	 */
	bool FindStemmedTerm(const std::string &stemmedTerm) const;

	const Settings					m_settings;				///< The settings for the index
	std::locale						m_defaultLocale;		///< Default locale for documents
	boost::scoped_ptr<MqService>	m_mqService;			///< Pointer to the MQ Service for storing document data
	DocumentId						m_firstDocumentId;		///< Document ID of the first document in the index
	DocumentId						m_nextDocumentId;		///< Document ID of next document to be added
	TermHashMap						m_termMap;				///< Main map of term -> doc IDs
	TempTermMap						m_tempTermMap;			///< Tempory map of term -> doc IDs
	DocumentQueueList				m_documentQueues;		///< List of all open document queues
	DocumentQueueMap				m_documentQueueMap;		///< Map of queue date -> queue

	// Static variables...

	static const byte_t							m_currentIndexVersion;	///< Current version for index files
	ACTIV_INDEXER_API static const std::string	m_defaultIndexFilename;	///< Default filename for index files
};

// --------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Index::Query(const Indexer::Query &query, DocumentList &docList)
{
	return Query(query.m_rootNode, docList);
}

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

// --------------------------------------------------------------------------------------------------------------------

inline const std::string &Index::GetDefaultIndexFilename(void)
{
	return m_defaultIndexFilename;
}

} // namespace Indexer

} // namespace Activ

#endif // !defined (ACTIV_INDEXER_INDEX_H)
