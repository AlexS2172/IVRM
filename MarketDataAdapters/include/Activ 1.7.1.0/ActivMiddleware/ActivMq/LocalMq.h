/**
 *  @file	LocalMq.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_LOCAL_MQ_H)
#define ACTIV_LOCAL_MQ_H

// ActivMq includes
#include "ActivMiddleware/ActivMq/External.h"
#include "ActivMiddleware/ActivMq/MqTypes.h"
#include "ActivMiddleware/ActivMq/MqRecord.h"

// ActivFieldTypes includes
#include "ActivMiddleware/ActivFieldTypes/Date.h"

namespace Activ
{

// Forward references
class MqService;
class SpanningFile;
class MessageBuilder;

class LocalMq
{
public:
	const static size_t m_messageHeaderSize = sizeof(MessageHeader);
	const static byte_t m_version0 = 0;

	const static uint32_t m_defaultIndexFileSize = 100000 * sizeof(struct MsnIndexEntry);
	const static uint32_t m_defaultIndexViewSize = 25;
	const static uint32_t m_defaultDataFileSize = 1024 * 1024 * 10; // 10Mb
	const static uint32_t m_defaultDataViewSize = m_defaultIndexViewSize * 2;

	/**
	 *	@brief	Settings class
	 */
	class Settings
	{
	public:
		ACTIV_MQ_API Settings();

		ACTIV_MQ_API ~Settings();

		std::string		m_queueName;		///< The name of the queue
		Date			m_queueDate;		///< The date of the queue

		uint32_t		m_indexFileSize;	///< Size of index sub-files
		uint32_t		m_indexViewSize;	///< View size (in pages) of index files
		uint32_t		m_dataFileSize;		///< Size of data sub-files
		uint32_t		m_dataViewSize;		///< View size (in pages) of data files

		MqMsn			m_msnOffset;		///< MSNs start from this value (ignored if queue already exists)
	};

	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_MQ_API LocalMq(MqService &mqService, const Settings &settings);

	/**
	 *	@brief	Default destructor.
	 */
	ACTIV_MQ_API ~LocalMq();

	/**
	 *	@brief	Add a message to the queue.
	 *
	 *	@param	pBuffer			Pointer to buffer containing message to add
	 *	@param	bufferLength	Length of message in buffer
	 *	@param	msn				The MSN of the new message is returned in this variable
	 */
	ACTIV_MQ_API StatusCode AddMessage(const void *pBuffer, const uint32_t bufferLength, MqMsn &msn);

	/**
	 *	@brief	Get a message from the queue into an MqRecord
	 *
	 *	@param	msn			MSN of record to get
	 *	@param	mqRecord	MqRecord to store data in
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_DELETED
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 */
	ACTIV_MQ_API StatusCode GetMessage(MqMsn msn, MqRecord &mqRecord) const;

	/**
	 *	@brief	Get a message from the queue into a MessageBuilder
	 *
	 *	@param	msn			MSN of record to get
	 *	@param	buffer		byte vector to store data in
	 *	@param	offset		Offset to start getting data from
	 *	@param	length		Ammount of data to get (0 = get all)
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_DELETED
	 *	@retval	STATUS_CODE_INVALID_RECORD
	 */
	ACTIV_MQ_API StatusCode GetMessage(MqMsn msn, MessageBuilder &messageBuilder, size_t offset = 0, size_t length = 0) const;

	/**
	 *	@brief	Delete a message from the queue.
	 */
	ACTIV_MQ_API StatusCode DeleteMessage(MqMsn msn);

	/**
	 *	@brief	Update a message.  The message cannot be longer than the existing one
	 *
	 *	@param	pBuffer			Pointer to buffer containing message to add
	 *	@param	bufferLength	Length of message in buffer
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_INVALID_LENGTH
	 */
	ACTIV_MQ_API StatusCode UpdateMessage(MqMsn msn, const void *pBuffer, const uint32_t bufferLength);

	/**
	 *	@brief	Get the directory where the queue data is held.
	 */
	ACTIV_MQ_API const std::string &GetMqServiceRootDirectory() const;

	/**
	 *	@brief	Get the first msn represented in the queue
	 */
	MqMsn GetFirstMsn(void) const;

	/**
	 *	@brief	Get the msn of the next record to be added to the queue
	 */
	MqMsn GetNextMsn(void) const;

	/**
	 *	@brief	Get the number of messages in the queue
	 */
	MqMsn GetNumMessages(void) const;

	/**
	 *	@brief	Get the name of the queue
	 */
	const std::string &GetQueueName(void) const;

	/**
	 *	@brief	Get the date of the queue
	 */
	const Date &GetQueueDate(void) const;

private:
	StatusCode ValidateIndex(void);

	StatusCode GetIndexEntry(MqMsn msn, MsnIndexEntry &indexEntry, bool useOffset = true) const;

	StatusCode GetMessage(MqMsn msn, ScatterBufferHelper *&mappedBuffers, bool useOffset = true) const;

	StatusCode GetMsnDetailsFromQueue(MqMsn &firstMsn, MqMsn &msnOffset) const;

	void CreateStoreDateTime(MessageHeader &messageHeader) const;

	MqService &		m_mqService;		///< Reference back to the owning object.
	Settings		m_settings;			///< The settings for the queue
	MqMsn			m_firstMsn;			///< The lowest msn stored in the queue
	MqMsn			m_nextMsn;			///< The msn of the next record to be added
	SpanningFile *	m_indexFile;		///< Pointer to index spanning file
	SpanningFile *	m_dataFile;			///< Pointer to data spanning file

	static MqMsn m_nextOrderNumber;		///< Like an msn, but valid across all open queues
};

// --------------------------------------------------------------------------------------------------------------------

inline MqMsn LocalMq::GetFirstMsn(void) const
{
	return m_firstMsn;
}

// --------------------------------------------------------------------------------------------------------------------

inline MqMsn LocalMq::GetNextMsn(void) const
{
	return m_nextMsn;
}

// --------------------------------------------------------------------------------------------------------------------

inline MqMsn LocalMq::GetNumMessages(void) const
{
	return m_nextMsn - m_firstMsn;
}

// --------------------------------------------------------------------------------------------------------------------

inline const std::string &LocalMq::GetQueueName(void) const
{
	return m_settings.m_queueName;
}

// --------------------------------------------------------------------------------------------------------------------

inline const Date &LocalMq::GetQueueDate(void) const
{
	return m_settings.m_queueDate;
}

} // namespace Activ

#endif // !defined (ACTIV_LOCAL_MQ_H)
