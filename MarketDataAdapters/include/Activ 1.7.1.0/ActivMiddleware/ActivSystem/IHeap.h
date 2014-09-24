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
#include "ActivMiddleware/ActivFieldTypes/DateTime.h"

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
		size_t	m_blockSize;									///< size of message body in bucket
		size_t	m_numBlocks;									///< number of messages in bucket
	};

	typedef std::vector<BucketInfo> BucketInfoVector;			///< bucket info vector type
	static const BucketInfoVector emptyBucketInfoVector;		///< empty bucket info vector

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
			 *	@param	messageValidator.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidator &messageValidator);

			size_t	m_minSize;									///< minimum size of request
			size_t	m_maxSize;									///< maximum size of request
			size_t	m_count;									///< number of requests >= m_minSize and <= m_maxSize
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
			 *	@param	messageValidator.
			 *
			 *	@retval	STATUS_CODE_SUCCESS
			 *	@retval	...
			 */
			ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidator &messageValidator);

			BucketInfo	m_bucketInfo;
			size_t		m_numFreeBlocks;						///< number of free blocks
			size_t		m_lowFreeBlocks;						///< lowest number of free blocks
			size_t		m_firstRunFails;						///< number of fails when most ideal sized block
			size_t		m_secondRunFails;						///< number of fails when > ideal sized block
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
		 *	@param	messageValidator.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidator &messageValidator);

		typedef std::vector<RequestDistributionStats> DistributionVector;
		typedef std::vector<BucketStats> BucketStatsVector;
	
		HeapId				m_id;
		ProcessId			m_creatorProcessId;
		std::string			m_name;
		size_t				m_memoryUsage;						///< mem usage of this heap
		size_t				m_totalNumberOfBlocks;

		size_t				m_totalFails;						///< number of GetMessage() requests that have failed
		DateTime			m_firstFailLocalDateTime;			///< local date/time of first fail
		DateTime			m_lastFailLocalDateTime;			///< local date/time of last fail

		size_t				m_totalCurrentFreeBlocks;			///< current free blocks across all buckets
		size_t				m_totalLowFreeBlocks;				///< low of free blocks across all buckets
		size_t				m_idealBucketFails;					///< number of requests for a message larger than largest block size
		size_t				m_mutexContentionCount;				///< approx. contentions on the heap's mutex
		BucketStatsVector	m_bucketStatsVector;				///< stats per bucket

		// IHeap::SetRequestDistributionLogging(true) needs to be called for the following to be populated:
		size_t				m_maxRequestSize;					///< size of max request
		DistributionVector	m_distributionVector;				///< request stats distribution

	private:
		static const byte_t	currentSerializedVersion = 2;
		static const byte_t firstFailDefined = 0x01;
		static const byte_t lastFailDefined = 0x02;
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
	 *	@param	shouldEnable true to enable.
	 */
	virtual void SetRequestDistributionLogging(const bool shouldEnable) = 0;

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
