/**
 *	@file	DirectoryWatchApi.h
 *
 *	@brief	Header file for the directory watch api.
 *
 *	$Log: $
 */

#if (!defined ACTIV_FRAMEWORK_DIRECTORY_WATCH_API_H)
#define ACTIV_FRAMEWORK_DIRECTORY_WATCH_API_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/TimeHelper.h"

#include <vector>

namespace Activ
{

class Component;

/**
 *	@brief	Api to the agent's directory watching service.
 */
class DirectoryWatchApi
{
public:
	/**
	 *	@brief	Message type received in OnMessage() by the component adding a directory watch when a change happens.
	 */
	static const MessageType MESSAGE_TYPE = ROUTER_MESSAGE_USER_NOTIFY_DIRECTORY_WATCH_EVENTS;

	typedef uint32_t WatchType;
	
	static const WatchType WATCH_TYPE_RECURSIVE = 0x00000001;	///< watch sub-directories?
	static const WatchType WATCH_TYPE_ADDS_DELETES = 0x00000002;///< watch for file & directory additions / deletions
	static const WatchType WATCH_TYPE_ATTRIBUTES = 0x00000004;	///< watch for changes in attributes (size, access times etc.) of files & directories

	/**
	 *	@brief	List of event types that can be received.
	 */
	enum EventType
	{
		EVENT_TYPE_ADDED,										///< file or directory has been added
		EVENT_TYPE_DELETED,										///< file or directory has been deleted
		EVENT_TYPE_MODIFIED										///< some attribute of the file or directory has changed
	};

	/**
	 *	@brief	File/directory change event.
	 */
	class Event
	{
	public:
		std::string	m_path;										///< path of file or directory that has changed
		EventType	m_eventType;								///< type of event that has happened
	};

	typedef std::vector<Event> EventList;						///< list of events

	/**
	 *	@brief	Add a directory watch.
	 *
	 *	Change notifications will be received in the OnMessage() callback of @a component, with message type
	 *	DirectoryWatchApi::MESSAGE_TYPE. Call DirectoryWatchApi::Deserialize() to get a list of Event objects.
	 *
	 *	@param	component the component wanting to create the watch.
	 *	@param	directoryPath path of a directory to watch.
	 *	@param	watchType type of watch to start.
	 *	@param	watchAddress reference to an Address object to receive the address of the new watch.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	STATUS_CODE_NOT_A_DIRECTORY path provided isn't a directory or doesn't exist
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode AddWatch(const Component &component, const std::string &directoryPath, const WatchType watchType, Address &watchAddress);

	/**
	 *	@brief	Cancel a directory watch.
	 *
	 *	Note the cancel is asynchronous: you may still have some events from the watch queued up to be processed after
	 *	calling this api.
	 *
	 *	@param	component the component that created the watch.
	 *	@param	watchAddress reference to an Address to holding the address of a watch. This is cleared down on success.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER watchAddress is not valid
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API static StatusCode CancelWatch(const Component &component, Address &watchAddress);

	/**
	 *	@brief	Deserialize a directory watch message.
	 *
	 *	@param	heapMessage message containing the serialized events.
	 *	@param	eventList to receive the deserialized events.
	 *
	 *	@return	STATUS_CODE_SUCCESS or an error status code.
	 */
	ACTIV_FRAMEWORK_API static StatusCode Deserialize(const HeapMessage &heapMessage, EventList &eventList);
};

} // namespace Activ

#endif // (!defined ACTIV_FRAMEWORK_DIRECTORY_WATCH_API_H)
