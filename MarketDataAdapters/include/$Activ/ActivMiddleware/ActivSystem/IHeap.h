/**
 *	@file	IHeap.h
 *
 *	@brief	Header file for abstract heap class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_IHEAP_H)
#define ACTIV_SYSTEM_IHEAP_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/HeapMessage.h"
#include "ActivMiddleware/ActivBase/IniFileParser.h"

#include <vector>

namespace Activ
{

class ISystem;

/**
 *	@brief	Abstract heap interface class.
 */
class IHeap
{
public:
	/**
	 *	@brief	Heap bucket size information.
	 */
	class BucketInfo
	{
	public:
		size_t	m_blockSize;									///< Size of message body in bucket
		size_t	m_numBlocks;									///< Number of messages in bucket
	};

	typedef std::vector<BucketInfo> BucketInfoVector;			///< Bucket info vector type
	static const BucketInfoVector emptyBucketInfoVector;		///< Empty bucket info vector

	/**
	 *	@brief	Heap stats.
	 */
	class Stats
	{
	public:
		/**
		 *	@brief	Request distribution stats.
		 */
		class RequestDistributionStats
		{
		public:
			/**
			 *	@brief	Serialize.
			 *
			 *	@param	messageBuilder.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SYSTEM_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

			/**
			 *	@brief	Deserialize.
			 *
			 *	@param	messageValidater.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidater &messageValidater);

			size_t	m_minSize;									///< Minimum size of request
			size_t	m_maxSize;									///< Maximum size of request
			size_t	m_count;									///< Number of requests >= m_minSize and <= m_maxSize
		};

		/**
		 *	@brief	Individual Bucket stats.
		 */
		class BucketStats
		{
		public:
			/**
			 *	@brief	Serialize.
			 *
			 *	@param	messageBuilder.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SYSTEM_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

			/**
			 *	@brief	Deserialize.
			 *
			 *	@param	messageValidater.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidater &messageValidater);

			BucketInfo	m_bucketInfo;
			size_t		m_numFreeBlocks;						///< Number of free blocks
			size_t		m_lowFreeBlocks;						///< Lowest number of free blocks
			size_t		m_firstRunFails;						///< Number of fails when most ideal sized block
			size_t		m_secondRunFails;						///< Number of fails when > ideal sized block
		};

		/**
		 *	@brief	Serialize.
		 *
		 *	@param	messageBuilder.
		 *
		 * 	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SYSTEM_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

		/**
		 *	@brief	Deserialize.
		 *
		 *	@param	messageValidater.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidater &messageValidater);

		typedef std::vector<RequestDistributionStats> DistributionVector;
		typedef std::vector<BucketStats> BucketStatsVector;

		HeapId				m_id;
		ProcessId			m_creatorProcessId;
		std::string			m_name;
		size_t				m_memoryUsage;						///< Mem usage of this heap
		size_t				m_totalNumberOfBlocks;
		size_t				m_totalFails;						///< Number of GetMessage() requests that have failed
		size_t				m_idealBucketFails;					///< Number of requests for a message larger than largest block size
		size_t				m_maxRequestSize;					///< Size of max request
		DistributionVector	m_distributionVector;				///< Request stats distribution
		BucketStatsVector	m_bucketStatsVector;				///< Stats per buckets
	};

	/**
	 *	@brief	Destructor.
	 */
	virtual ~IHeap() = 0;

	/**
	 *	@brief	Get a message or chain of messages from the heap.
	 *
	 *	@param	size the maximum number of bytes the message will hold.
	 *
	 *	@return	a HeapMessage object. If message allocation failed,
	 *			the HeapMessage will not have an attached message.
	 */
	virtual HeapMessage GetMessage(const size_t size) = 0;

	/**
	 *	@brief	Get a message or chain of messages from the heap.
	 *
	 *	@param	size the maximum number of bytes the message will hold.
	 *	@param	pData pointer to data to copy into the message (up to \a size bytes).
	 *
	 *	@return	a HeapMessage object. If message allocation or data copy failed,
	 *			the HeapMessage will not have an attached message.
	 */
	virtual HeapMessage GetMessage(const size_t size, const void *pData) = 0;

	/**
	 *	@brief	Get a message or chain of messages from the heap and block if the heap is empty.
	 *
	 *	@param	size the maximum number of bytes the message will hold.
	 *	@param	pData pointer to data to copy into the message (up to \a size bytes).
	 *	@param	timeoutPeriod timeout.
	 *
	 *	@return	a HeapMessage object. If message allocation or data copy failed,
	 *			the HeapMessage will not have an attached message.
	 */
	virtual HeapMessage GetMessage(const size_t size, const void *pData, const TimeoutPeriod timeoutPeriod) = 0;

	/**
	 *	@brief	Get the name of the heap.
	 *
	 *	@return	A string object containing the name of the heap.
	 */
	virtual std::string GetName() const = 0;

	/**
	 *	@brief	Get the unique system wide heap id.
	 *
	 *	@return	The heap id.
	 */
	virtual HeapId GetId() const = 0;

	/**
	 *	@brief	Get the system that created the heap.
	 *
	 *	@return	An ISystem interface.
	 */
	virtual ISystem *GetSystem() const = 0;

	/**
	 *	@brief	Get heap stats.
	 *
	 *	@param	stats reference to a stats object.
	 */
	virtual void GetStats(Stats &stats) const = 0;

	/**
	 *	@brief	Reset heap stats.
	 */
	virtual void ResetStats() = 0;

	/**
	 *	@brief	Get request distribution logging state.
	 *
	 *	@return	true if request distribution loggin is enabled.
	 */
	virtual bool GetRequestDistributionLogging() const = 0;

	/**
	 *	@brief	Enable or disable request distribution logging.
	 *
	 *	@param	isEnabled true to enable.
	 */
	virtual void SetRequestDistributionLogging(const bool isEnabled) = 0;

	/**
	 *	@brief	Parse an ini file section and build vector of bucket infos.
	 *
	 *	Keys in the section are of the form SIZE = NUMBER_OF_BLOCKS. The vector is NOT cleared initially,
	 *	and only has BucketInfo values appended if the whole section is parsed successfully.
	 *
	 *	@param	sectionIterator iterator to an ini file section.
	 *	@param	bucketInfoVector vector to populate.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_SYSTEM_API static StatusCode ReadBucketInfoVector(const IniFileParser::ConstSectionIterator &sectionIterator, IHeap::BucketInfoVector &bucketInfoVector);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IHeap::~IHeap()
{
}

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_IHEAP_H)
