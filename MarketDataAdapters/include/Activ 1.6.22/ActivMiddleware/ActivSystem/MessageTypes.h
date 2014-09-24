/**
 *	@file	MessageTypes.h
 *
 *	@brief	Message types.
 *
 *	$Log: $
 * 
 */

#if !defined (ACTIV_MESSAGE_TYPES_H)
#define ACTIV_MESSAGE_TYPES_H

#include "ActivMiddleware/ActivSystem/External.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Message class type.
 */
typedef byte_t MessageClass;

/**
 *	@brief	List of available message classes used by ActivMiddleware.
 */
#define ACTIV_MESSAGE_CLASS_LIST(d)											\
	d(0,	MESSAGE_CLASS_SYSTEM,		"System")							\
	d(1,	MESSAGE_CLASS_TIMER,		"Timer")							\
	d(2,	MESSAGE_CLASS_LOGGER,		"Logger")							\
	d(3,	MESSAGE_CLASS_DIRECTORY,	"Directory")						\
	d(4,	MESSAGE_CLASS_ROUTER,		"Router")							\
	d(5,	MESSAGE_CLASS_PUBSUB,		"Pub-Sub")							\
	d(6,	MESSAGE_CLASS_MQ,			"Message-Queue")					\
	d(7,	MESSAGE_CLASS_UI,			"UI")								\
	d(8,	MESSAGE_CLASS_DATABASE,		"Database")							\
	d(9,	MESSAGE_CLASS_SERVICE,		"Service")							\
	d(10,	MESSAGE_CLASS_UDP,			"Udp")								\
	d(11,	MESSAGE_CLASS_TCP,			"Tcp")								\
	d(12,	MESSAGE_CLASS_MONITOR,		"Monitor")							\
	d(13,	MESSAGE_CLASS_UPDATE,		"Update")							\
	d(64,	MESSAGE_CLASS_USER_BASE,	"User")

/**
 *	@brief	Declare a constant message class value.
 */
#define ACTIV_DECLARE_MESSAGE_CLASS(value, messageClass, name)				\
	const Activ::MessageClass messageClass = value;

/**
 *	@brief	Declare list of constant message class values.
 */
ACTIV_MESSAGE_CLASS_LIST(ACTIV_DECLARE_MESSAGE_CLASS)

/**
 *	@brief	Return message class as a string.
 *
 *	@param	messageClass the message class.
 *
 *	@return	A string representation of the message class.
 */
ACTIV_SYSTEM_API std::string MessageClassToString(const MessageClass messageClass);

/**
 *	@brief	Convert string to message class. Case sensitive.
 *
 *	@param	str message class as string.
 *	@param	messageClass reference to receive the message class.
 *
 *	@retval	STATUS_CODE_SUCCESS \a messageClass set
 *	@retval	STATUS_CODE_NOT_FOUND \a str is not a known message class
 */
ACTIV_SYSTEM_API StatusCode StringToMessageClass(const std::string &str, MessageClass &messageClass);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Macro to make message type.
 *
 *	@param	messageClass the message class of the type.
 *	@param	subType the message subtype within \a messageClass.
 */
#define MakeMessageType(messageClass, subType)								\
	((static_cast<Activ::MessageType>((messageClass)) << 8) | (subType))

/**
 *	@brief	Get the message class from a message type.
 *
 *	@param	messageType message type.
 *
 *	@return	Class of \a messageType.
 */
inline MessageClass GetMessageClass(const MessageType messageType)
{
	return static_cast<MessageClass>(messageType >> 8);
}

/**
 *	@brief	Get the message subtype from from a message type.
 *
 *	@param	messageType message type.
 *
 *	@return	Subtype of \a messageType.
 */
inline byte_t GetSubType(const MessageType messageType)
{
	return static_cast<byte_t>(messageType & 0xff);
}

/**
 *	@brief	Declare a constant message type value.
 */
#define ACTIV_DECLARE_MESSAGE_TYPE(value, messageClass, name)				\
	const Activ::MessageType name = MakeMessageType(messageClass, value)

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_SYSTEM request types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_SYSTEM,	SYSTEM_REQUEST_FIND_PORT);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_SYSTEM,	SYSTEM_REQUEST_PING_PORT);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_SYSTEM,	SYSTEM_REQUEST_TOKEN);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_SYSTEM,	SYSTEM_REQUEST_GET_PORT_INFO_LIST);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_SYSTEM message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_THREAD_EXIT);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_CTRL_EVENT);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_SYSTEM_SHUTDOWN);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_PROCESS_STARTUP);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_PROCESS_SHUTDOWN);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_ENDPOINT_STARTUP);
ACTIV_DECLARE_MESSAGE_TYPE(6,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_ENDPOINT_SHUTDOWN);
ACTIV_DECLARE_MESSAGE_TYPE(7,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_ENDPOINT_DIE);
ACTIV_DECLARE_MESSAGE_TYPE(8,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_HEAP_STARTUP);
ACTIV_DECLARE_MESSAGE_TYPE(9,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_ENDPOINT_PAUSE);
ACTIV_DECLARE_MESSAGE_TYPE(10,	MESSAGE_CLASS_SYSTEM,	SYSTEM_MESSAGE_ENDPOINT_RESUME);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_TIMER request types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_TIMER,	TIMER_REQUEST_SET_TIMER);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_TIMER,	TIMER_REQUEST_GET_CURRENT_TIME_OBJECT);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_TIMER message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_TIMER,	TIMER_MESSAGE_CANCEL_TIMER);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_TIMER,	TIMER_MESSAGE_ON_TIMER);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_LOGGER message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_LOGGER,	LOGGER_MESSAGE_CLOSE_LOG_FILE);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_LOGGER,	LOGGER_MESSAGE_LOG_MESSAGE);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_LOGGER,	LOGGER_MESSAGE_SYSTEM_LOG_MESSAGE);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_LOGGER,	LOGGER_MESSAGE_SET_LOG_HEADER);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_LOGGER request types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_LOGGER,	LOGGER_REQUEST_OPEN_LOG_FILE);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	MESSAGE_CLASS_ROUTER request types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_ASYNC_CONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_ALLOCATE_CLIENT);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_INITIATE_CONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_INITIATE_DISCONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_START_LISTEN);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_ADD_DIRECTORY_WATCH);

ACTIV_DECLARE_MESSAGE_TYPE(7,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_INTERNAL_SYN);
ACTIV_DECLARE_MESSAGE_TYPE(8,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_INTERNAL_FIN);
ACTIV_DECLARE_MESSAGE_TYPE(9,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_NOTIFY_ACCEPT_CONNECTION);
ACTIV_DECLARE_MESSAGE_TYPE(10,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_UDP_INIT);
ACTIV_DECLARE_MESSAGE_TYPE(11,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_UDP_SHUTDOWN);
ACTIV_DECLARE_MESSAGE_TYPE(12,	MESSAGE_CLASS_ROUTER,	ROUTER_REQUEST_USER_GET_SOCKET_HANDLE);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	MESSAGE_CLASS_ROUTER message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_CONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_CONNECT_FAILED);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_CONNECT_ABORTED);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_ACCEPT);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_ACCEPT_FAILED);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_ACCEPT_ABORTED);
ACTIV_DECLARE_MESSAGE_TYPE(6,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_BREAK);
ACTIV_DECLARE_MESSAGE_TYPE(7,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_DISCONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(8,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_INITIATE_DISCONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(9,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_BREAK_CONNECTION);
ACTIV_DECLARE_MESSAGE_TYPE(10,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_BREAK_ACCEPTED);
ACTIV_DECLARE_MESSAGE_TYPE(11,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_STOP_LISTEN);
ACTIV_DECLARE_MESSAGE_TYPE(12,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_CIRCUIT_UP);
ACTIV_DECLARE_MESSAGE_TYPE(13,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_CIRCUIT_DOWN);
ACTIV_DECLARE_MESSAGE_TYPE(14,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_CIRCUIT_BREAK);
ACTIV_DECLARE_MESSAGE_TYPE(15,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_DATA);
ACTIV_DECLARE_MESSAGE_TYPE(16,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_CONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(17,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_DISCONNECT);
ACTIV_DECLARE_MESSAGE_TYPE(18,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_READ);
ACTIV_DECLARE_MESSAGE_TYPE(19,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_MULTICAST_JOIN);
ACTIV_DECLARE_MESSAGE_TYPE(20,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_MULTICAST_LEAVE);
ACTIV_DECLARE_MESSAGE_TYPE(21,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_SET_MULTICAST_TTL);
ACTIV_DECLARE_MESSAGE_TYPE(22,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_SET_UNICAST_TTL);
ACTIV_DECLARE_MESSAGE_TYPE(23,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_SET_MULTICAST_INTERFACE);
ACTIV_DECLARE_MESSAGE_TYPE(24,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_SET_RX_BUFFER_SIZE);
ACTIV_DECLARE_MESSAGE_TYPE(25,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_SET_TX_BUFFER_SIZE);
ACTIV_DECLARE_MESSAGE_TYPE(26,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_UDP_SOCKET_ERROR);
ACTIV_DECLARE_MESSAGE_TYPE(27,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_UDP_SOCKET_SHUTDOWN);
ACTIV_DECLARE_MESSAGE_TYPE(28,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_SET_MULTICAST_LOOPBACK);
ACTIV_DECLARE_MESSAGE_TYPE(29,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_UDP_CLEAR_MULTICAST_LOOPBACK);
ACTIV_DECLARE_MESSAGE_TYPE(30,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_SET_HEAP);
ACTIV_DECLARE_MESSAGE_TYPE(31,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_CIRCUIT_FAILED);
ACTIV_DECLARE_MESSAGE_TYPE(32,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_SOCKET_LISTENER_CLOSED);
ACTIV_DECLARE_MESSAGE_TYPE(33,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_CLEAR_SOCKET_STATS_HIGHS);
ACTIV_DECLARE_MESSAGE_TYPE(34,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_INTERNAL_DIRECTORY_WATCH_CLOSED);
ACTIV_DECLARE_MESSAGE_TYPE(35,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_CANCEL_DIRECTORY_WATCH);
ACTIV_DECLARE_MESSAGE_TYPE(36,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_DIRECTORY_WATCH_EVENTS);
ACTIV_DECLARE_MESSAGE_TYPE(37,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_ENABLE_SOCKET_STATS);
ACTIV_DECLARE_MESSAGE_TYPE(38,	MESSAGE_CLASS_ROUTER,	ROUTER_MESSAGE_USER_NOTIFY_SOCKET_STATS);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_UI request types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_UI,		UI_REQUEST_NEW_USER);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	List of MESSAGE_CLASS_UI message types.
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_UI,		UI_MESSAGE_REGISTER_COMPONENT);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_UI,		UI_MESSAGE_DEREGISTER_COMPONENT);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_UI,		UI_MESSAGE_USER_INPUT);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_UI,		UI_MESSAGE_DESTROY);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_UI,		UI_MESSAGE_OUTPUT);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_UI,		UI_MESSAGE_SWITCH_TO_COMPONENT);
ACTIV_DECLARE_MESSAGE_TYPE(6,	MESSAGE_CLASS_UI,		UI_MESSAGE_RETURN_TO_MAIN_COMPONENT);
ACTIV_DECLARE_MESSAGE_TYPE(7,	MESSAGE_CLASS_UI,		UI_MESSAGE_RESET_MENU);
ACTIV_DECLARE_MESSAGE_TYPE(8,	MESSAGE_CLASS_UI,		UI_MESSAGE_LOG_OUTPUT);
ACTIV_DECLARE_MESSAGE_TYPE(9,	MESSAGE_CLASS_UI,		UI_MESSAGE_SHARED_OUTPUT);
ACTIV_DECLARE_MESSAGE_TYPE(10,	MESSAGE_CLASS_UI,		UI_MESSAGE_SHARED_SESSION_CLOSED);

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Messages used internally by udp users (pub sub, udp transmitter etc).
 */
ACTIV_DECLARE_MESSAGE_TYPE(0,	MESSAGE_CLASS_UDP,		UDP_MESSAGE_DATA);
ACTIV_DECLARE_MESSAGE_TYPE(1,	MESSAGE_CLASS_UDP,		UDP_MESSAGE_SOCKET_OPEN);
ACTIV_DECLARE_MESSAGE_TYPE(2,	MESSAGE_CLASS_UDP,		UDP_MESSAGE_SOCKET_CLOSED);
ACTIV_DECLARE_MESSAGE_TYPE(3,	MESSAGE_CLASS_UDP,		UDP_MESSAGE_SHUTDOWN);
ACTIV_DECLARE_MESSAGE_TYPE(4,	MESSAGE_CLASS_UDP,		UDP_MESSAGE_FLUSH);
ACTIV_DECLARE_MESSAGE_TYPE(5,	MESSAGE_CLASS_UDP,		UDP_MESSAGE_SINGLE_MESSAGE);

} // namespace Activ

#endif // !defined (ACTIV_MESSAGE_TYPES_H)
