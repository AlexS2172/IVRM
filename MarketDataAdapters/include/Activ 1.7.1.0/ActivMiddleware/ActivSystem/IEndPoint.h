/**
 *	@file	IEndPoint.h
 *
 *	@brief	Header file for abstract endpoint class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_IENDPOINT_H)
#define ACTIV_IENDPOINT_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/HeapMessage.h"

#include <string>

namespace Activ
{

class ISystem;
class SyncRequestId;

/**
 *	@brief	Endpoint data notification types.
 */
#define ACTIV_IENDPOINT_NOTIFICATION_TYPE_LIST(d)		\
	d(NOTIFICATION_TYPE_NONE, "none")					\
	d(NOTIFICATION_TYPE_SEMAPHORE, "semaphore")			\
	d(NOTIFICATION_TYPE_EVENT, "event")					\
	d(NOTIFICATION_TYPE_SOCKET, "socket")

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Endpoint states.
 */
#define ACTIV_IENDPOINT_STATE_LIST(d)					\
	d(STATE_ACCEPTING_MESSAGES, "accepting")			\
	d(STATE_IGNORING_MESSAGES, "ignoring")

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Abstract endpoint interface class.
 */
class IEndPoint
{
public:
	/**
	 *	@brief	Endpoint data notification types.
	 */
	enum NotificationType
	{
		ACTIV_IENDPOINT_NOTIFICATION_TYPE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NOTIFICATION_TYPE_DEFAULT = -1,
#if (defined ACTIV_PLATFORM_WINDOWS)
		NOTIFICATION_TYPE_MULTIPLEX = NOTIFICATION_TYPE_DEFAULT
#elif (defined ACTIV_PLATFORM_POSIX)
		NOTIFICATION_TYPE_MULTIPLEX = NOTIFICATION_TYPE_SOCKET
#else
#error Configure
#endif
	};

	/**
	 *	@brief	Endpoint states.
	 */
	enum State
	{
		ACTIV_IENDPOINT_STATE_LIST(ACTIV_DECLARE_ELEMENT_1_WITH_COMMA)
		NUM_STATES
	};

	/**
	 *	@brief	Stats for an endpoint.
	 */
	class Stats
	{
	public:
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

		ProcessId			m_creatorProcessId;					///< pid of the process that created the endpoint
		NotificationType	m_notificationType;					///< message notification type
		bool				m_hasMutex;							///< does the endpoint have a mutex?
		size_t				m_mutexContentionCount;				///< approx. contentions on the endpoint's mutex
		State				m_state;

		size_t				m_messageCount;						///< current number of messages on endpoint
		size_t				m_messageCountHigh;					///< all-time high number of messages on endpoint
		size_t				m_messageCountDailyHigh;			///< daily high number of messages on endpoint
		size_t				m_messageCountLastHigh;				///< high number of messages on endpoint in last stats interval

		size_t				m_byteCount;						///< current number of bytes on endpoint
		size_t				m_byteCountHigh;					///< all-time high number of bytes on endpoint
		size_t				m_byteCountDailyHigh;				///< daily high number of bytes on endpoint
		size_t				m_byteCountLastHigh;				///< high number of bytes on endpoint in last stats interval

	private:
		static const byte_t	currentSerializedVersion = 3;
	};

	/**
	 *	@brief	Destructor.
	 */
	virtual ~IEndPoint() = 0;

	/**
	 *	@brief	Add a message to the tail of the endpoint.
	 *
	 *	@param	heapMessage the message to add.
	 *
	 *	@retval	STATUS_CODE_SUCCESS \a heapMessage added to the endpoint.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages. \a heapMessage returned to its heap.
	 *	@retval	STATUS_CODE_FULL the endpoint maximum byte count has been reached. \a heapMessage returned to its heap.
	 */
	virtual StatusCode PostMessage(HeapMessage &heapMessage) = 0;

	/**
	 *	@brief	Add a chain of messages to the tail of the endpoint.
	 *
	 *	@param	pEndPoint the chain of messages to add.
	 *	@param	minBytesToRemove minimum number of bytes from pEndPoint to add.
	 *
	 *	@retval	STATUS_CODE_SUCCESS \a heapMessage added to the endpoint.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages. All messages returned to their heap.
	 *	@retval	STATUS_CODE_FULL the endpoint maximum byte count has been reached. All messages returned to their heap.
	 */
	virtual StatusCode PostMessages(IEndPoint * const pEndPoint, size_t minBytesToRemove = 0) = 0;

	/**
	 *	@brief	Get a message from the head of the endpoint.
	 *
	 *	@param	heapMessage reference to a HeapMessage object to receive the head message.
	 *	@param	timeout how long to wait for a message on the endpoint, in ms.
	 *			Will be ignored if the object was not created with a data notification mechanism.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_TIMEOUT no messages on this endpoint in the timeout period, \a heapMessage unaffected. Note this
	 *			return value is possible even with \a timeout set to ACTIV_WAIT_INFINITE. \a heapMessage will only contain
	 *			a retrieved message if the return value is STATUS_CODE_SUCCESS.
	 */
	virtual StatusCode GetMessage(HeapMessage &heapMessage, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) = 0;

	/**
	 *	@brief	Get multiple messages from the endpoint and add to the tail of a second endpoint.
	 *
	 *	@param	pEndPoint the endpoint to add messages to.
	 *	@param	timeout how long to wait for a message on this endpoint, in ms.
	 *			Will be ignored if the object was not created with a data notification mechanism.
	 *	@param	minBytesToRemove the minimum number of bytes to remove from endpoint (0 to remove all messages).
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_DISABLED \a pEndPoint is not accepting messages, all returned to their heap.
	 *	@retval	STATUS_CODE_TIMEOUT no messages on this endpoint in the timeout period, \a pEndPoint unaffected.
	 */
	virtual StatusCode GetMessages(IEndPoint * const pEndPoint, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE, size_t minBytesToRemove = 0) = 0;

	/**
	 *	@brief	Remove all messages from the endpoint and return them to their heaps.
	 */
	virtual void Clear() = 0;

	/**
	 *	@brief	Set the state of the endpoint.
	 *
	 *	@param	state the state to set the endpoint to.
	 */
	virtual void SetState(const State state) = 0;

	/**
	 *	@brief	Get the current state of the endpoint.
	 *
	 *	@return	The state of the endpoint.
	 */
	virtual State GetState() const = 0;

	/**
	 *	@brief	Get the system that created the endpoint.
	 *
	 *	@return	An ISystem interface.
	 */
	virtual ISystem *GetSystem() const = 0;

	/**
	 *	@brief	Get endpoint stats.
	 *
	 *	@param	stats reference to a Stats object.
	 */
	virtual void GetStats(Stats &stats) const = 0;

	/**
	 *	@brief	Clear all high message and byte counts.
	 */
	virtual void ClearAllHighCounts() = 0;

	/**
	 *	@brief	Clear daily high message and byte counts.
	 */
	virtual void ClearDailyHighCounts() = 0;

	/**
	 *	@brief	Clear last high message and byte counts.
	 */
	virtual void ClearLastHighCounts() = 0;

	/**
	 *	@brief	Set the maximum bytes allowed on an endpoint.
	 *
	 *	By default there is no limit, but if set to a non 0 value, posting a message to an endpoint which takes
	 *	the number of bytes on the endpoint to over maxByteCount, the call will return STATUS_CODE_FULL.
	 *
	 *	@param	maxByteCount max bytes to allow on the endpoint, or 0 for no limit.
	 */
	virtual void SetMaxByteCount(const size_t maxByteCount) = 0;

	/**
	 *	@brief	Get the underlying message notification handle, if available.
	 *
	 *	@param	handle reference to receive the handle.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_SUPPORTED the notification type for this endpoint does not support an exportable handle.
	 */
	virtual StatusCode GetNotificationHandle(HANDLE &handle) const = 0;

	/**
	 *	@brief	Return an endpoint notification type in string form.
	 *
	 *	@param	notificationType to convert.
	 *
	 *	@return	Notification type state as a string.
	 */
	ACTIV_SYSTEM_API static std::string NotificationTypeToString(const NotificationType notificationType);
	
	/**
	 *	@brief	Convert string to notification type.
	 *
	 *	@param	str the string to convert.
	 *	@param	notificationType to receive the notification type value.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_SYSTEM_API static StatusCode StringToNotificationType(const std::string &str, NotificationType &notificationType);

	/**
	 *	@brief	Return an endpoint state in string form.
	 *
	 *	@param	state to convert.
	 *
	 *	@return	Endpoint state as a string.
	 */
	ACTIV_SYSTEM_API static std::string StateToString(const State state);
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Endpoint interface class with sync call support.
 */
class ISyncEndPoint : public IEndPoint
{
public:
	/**
	 *	@brief	Stats for a shared endpoint.
	 */
	class Stats : public IEndPoint::Stats
	{
	public:
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

		std::string	m_name;
		EndPointId	m_id;
	};

	/**
	 *	@brief	Send a message to the endpoint and wait for a response.
	 *
	 *	@param	heapMessage the message to send.
	 *	@param	timeout how long to wait for the response, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS \a heapMessage now contains the response message.
	 *	@retval	STATUS_CODE_TIMEOUT no message received in the timeout period.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages, \a heapMessage returned to it's heap.
	 */
	virtual StatusCode SendMessage(HeapMessage &heapMessage, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) = 0;

	/**
	 *	@brief	Send a message to the endpoint and initialize a SyncRequestId that can be used to wait for the response.
	 *
	 *	@param	heapMessage the message to send.
	 *	@param	syncRequestId reference to a syncRequestId object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS \a syncRequestId can be used to wait or poll for the response message.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages, \a heapMessage returned to it's heap.
	 */
	virtual StatusCode SendMessage(HeapMessage &heapMessage, SyncRequestId &syncRequestId) = 0;

	/**
	 *	@brief	Get the name of the endpoint.
	 *
	 *	@return	A string containing the unique system wide name of the endpoint.
	 */
   	virtual std::string GetName() const = 0;

	/**
	 *	@brief	Get the id of the endpoint.
	 *
	 *	@return	The unique system wide id of the endpoint.
	 */
	virtual EndPointId GetId() const = 0;

	/**
	 *	@brief	Get the process id of the creator of the endpoint.
	 *
	 *	@return	The process id of the creator of the endpoint.
	 */
	virtual ProcessId GetCreatorProcessId() const = 0;

	virtual void GetStats(Stats &stats) const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IEndPoint::~IEndPoint()
{
}

} // namespace Activ

#endif // !defined (ACTIV_IENDPOINT_H)
