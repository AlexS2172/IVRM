/**
 *	@file	Port.h
 *
 *	@brief	Header file for port class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_PORT_H)
#define ACTIV_SYSTEM_PORT_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/Address.h"
#include "ActivMiddleware/ActivSystem/ManagedEndPoint.h"
#include "ActivMiddleware/ActivBase/Atomic.h"

namespace Activ
{

class IHeap;
class RequestId;
class SyncRequestId;

/**
 *	@brief	Port class.
 */
class Port
{
public:
	static const TimeoutPeriod defaultTimeout = 30000;			///< default timeout for assorted synchronous calls

	ACTIV_SYSTEM_API static const std::string anonymousName;	///< use this if you don't want to name a port

	/**
	 *	@brief	Helper class to enable & disable a Port.
	 */
	class Enabler
	{
	public:
		/**
		 *	@brief	Constructor. Will enable the port.
		 *
		 *	@param	port reference to the port to enable.
		 */
		Enabler(Port &port);

		/**
		 *	@brief	Destructor. Will disable the port.
		 */
		~Enabler();

	private:
		Port	&m_port;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this object to a managed endpoint at the next available slot.
	 *
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	name name of the port, local to the endpoint.
	 *	@param	pHeap heap to use for messaging, or 0 for the system heap.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SYSTEM_API Port(ManagedEndPoint &managedEndPoint, const std::string &name, IHeap * const pHeap);

	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this object to a managed endpoint at the given slot.
	 *
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	name name of the port, local to the endpoint.
	 *	@param	portId the port id to attach to.
	 *	@param	pHeap heap to use for messaging, or 0 for the system heap.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SYSTEM_API Port(ManagedEndPoint &managedEndPoint, const std::string &name, const PortId portId, IHeap * const pHeap);

	/**
	 *	@brief	Destructor.
	 *
	 *	The destructor will assert if Disable() has not been called for this port if Enable() had previously been called.
	 *	See Disable() for more information.
	 */
	ACTIV_SYSTEM_API virtual ~Port();

	/**
	 *	@brief	Enable a port.
	 *
	 *	Enabling a port means that it can start to receive messages. This should be done only after the class derived from
	 *	Port has been fully constructed, so that messages won't be received by the Port before construction is complete.
	 *
	 *	If you have a derived Port class that will never be derived from itself, then it is safe to put Enable() in the
	 *	constructor for that class.
	 *
	 *	Note that since a Port cannot receive messages until Enable() has been called, no calls should be made before
	 *	an Enable() that may cause messages to arrive that you cannot miss. For instance, tcp connections should not
	 *	be initiated since a connect notification may be missed.
	 *
	 *	A Port can only be Enable()d once.
	 */
	ACTIV_SYSTEM_API void Enable();

	/**
	 *	@brief	Disable a port.
	 *
	 *	Disabling a port stops it from receiving any more messages and detaches it from it's ManagedEndPoint.
	 *	This should be done before any destruction of a derived Port class starts.
	 *
	 *	If you have a derived Port class that will never be derived from itself, then it is safe to put Disable() in the
	 *	destructor for that class.
	 */
	ACTIV_SYSTEM_API void Disable();

	/**
	 *	@brief	Get the managed endpoint this port is attached to.
	 *
	 *	@return	Reference to the port's managed endpoint.
	 */
	ACTIV_SYSTEM_API ManagedEndPoint &GetManagedEndPoint() const;

	/**
	 *	@brief	Get the name of the port.
	 *
	 *	@return	Reference to the port's name.
	 */
	ACTIV_SYSTEM_API const std::string &GetName() const;

	/**
	 *	@brief	Get the address of this port.
	 *
	 *	@return	The port's address.
	 */
	const Address &GetAddress() const;

	/**
	 *	@brief	Get the heap used for messaging.
	 *
	 *	@return	An IHeap interface.
	 */
	IHeap *GetHeap() const;

	/**
	 *	@brief	Set the heap used for messaging.
	 *
	 *	@param	pHeap new heap to use for messaging.
	 */
	ACTIV_SYSTEM_API void SetHeap(IHeap * const pHeap);

	/**
	 *	@brief	Find an endpoint & port by name.
	 *
	 *	@param	endPointName the name of the endpoint.
	 *	@param	portName the name of the port.
	 *	@param	portAddress referece to an Address object to receive the address of the discovered port.
	 *	@param	timeout how long to wait for the endpoint, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_NOT_FOUND.
	 *	@retval	STATUS_CODE_MISMATCH port found at the registered address that does not have the given name.
	 *	@retval	STATUS_CODE_TIMEOUT port not found in timeout period.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 */
	ACTIV_SYSTEM_API StatusCode FindPort(const std::string &endPointName, const std::string &portName, Address &portAddress,
										 const TimeoutPeriod timeout = defaultTimeout);

	/**
	 *	@brief	Post an asynchronous request.
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	requestId the message identification (to match up the response).
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to post.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostRequest(const Address &destinationAddress, const MessageType requestType, const RequestId &requestId,
											const MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Post an asynchronous request.
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	requestId the message identification (to match up the response).
	 *	@param	pRequest the request message buffer.
	 *	@param	requestLength the length of the request message buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostRequest(const Address &destinationAddress, const MessageType requestType, const RequestId &requestId,
											const void *pRequest = 0, const size_t requestLength = 0) const;

	/**
	 *	@brief	Post an asynchronous request.
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	requestId the message identification (to match up the response).
	 *	@param	request the HeapMessage containing the request body.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostRequest(const Address &destinationAddress, const MessageType requestType, 
											const RequestId &requestId, HeapMessage &request) const;

	/**
	 *	@brief	Send a synchronous request and wait for the response.
	 *
	 *	If this returns STATUS_CODE_SUCCESS, the value of response.GetStatusCode()
	 *	will contain the actual return status of the request (the return value from SendRequest() just
	 *	determines whether the actual message transmission was successful).
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to post.
	 *	@param	response HeapMessage to receive the response.
	 *	@param	timeout in milliseconds.
	 *
	 *	@retval	STATUS_CODE_SUCCESS a response has been received.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 *	@retval	STATUS_CODE_TIMEOUT no response received in the timeout period.
	 */
	ACTIV_SYSTEM_API StatusCode SendRequest(const Address &destinationAddress, const MessageType requestType,
											const MessageBuilder &messageBuilder,
											HeapMessage &response, const TimeoutPeriod timeout = defaultTimeout) const;

	/**
	 *	@brief	Send a synchronous request and wait for the response.
	 *
	 *	If this returns STATUS_CODE_SUCCESS, the value of response.GetStatusCode()
	 *	will contain the actual return status of the request (the return value from SendRequest() just
	 *	determines whether the actual message transmission was successful).
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	pRequest the request message buffer.
	 *	@param	requestLength the length of the request message buffer.
	 *	@param	response HeapMessage to receive the response.
	 *	@param	timeout in milliseconds.
	 *
	 *	@retval	STATUS_CODE_SUCCESS a response has been received.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 *	@retval	STATUS_CODE_TIMEOUT no response received in the timeout period.
	 */
	ACTIV_SYSTEM_API StatusCode SendRequest(const Address &destinationAddress, const MessageType requestType,
											const void *pRequest, const size_t requestLength,
											HeapMessage &response, const TimeoutPeriod timeout = defaultTimeout) const;

	/**
	 *	@brief	Send a synchronous request and wait for the response.
	 *
	 *	If this returns STATUS_CODE_SUCCESS, the value of request.GetStatusCode()
	 *	will contain the actual return status of the request (the return value from SendRequest() just
	 *	determines whether the actual message transmission was successful).
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	request the request message and container for the response. If request is an empty HeapMessage, a
	 *			request will be sent with no message body.
	 *	@param	timeout in milliseconds.
	 *
	 *	@retval	STATUS_CODE_SUCCESS a response has been received.
	 *	@retval	STATUS_CODE_HEAP_EMPTY if request was empty and no message could be allocated for the outgoing request.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 *	@retval	STATUS_CODE_TIMEOUT no response received in the timeout period.
	 */
	ACTIV_SYSTEM_API StatusCode SendRequest(const Address &destinationAddress, const MessageType requestType, 
											HeapMessage &request, const TimeoutPeriod timeout = defaultTimeout) const;

	/**
	 *	@brief	Send a synchronous request but do not wait for the response.
	 *
	 *	If this returns STATUS_CODE_SUCCESS, the response can be obtained by calling syncRequestId.GetResponse().
	 *	The actual return status of the request can then be obtained by GetStatusCode() on the message returned from
	 *	syncRequestId.GetResponse().
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	syncRequestId reference to a SyncRequestId object that can be used later to wait (or poll) for the response.
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to post.
	 *
	 *	@retval	STATUS_CODE_SUCCESS the request was sent successfully.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode SendRequest(const Address &destinationAddress, const MessageType requestType,
											SyncRequestId &syncRequestId, const MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Send a synchronous request but do not wait for the response.
	 *
	 *	If this returns STATUS_CODE_SUCCESS, the response can be obtained by calling syncRequestId.GetResponse().
	 *	The actual return status of the request can then be obtained by GetStatusCode() on the message returned from
	 *	syncRequestId.GetResponse().
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	syncRequestId reference to a SyncRequestId object that can be used later to wait (or poll) for the response.
	 *	@param	pRequest the request message buffer.
	 *	@param	requestLength the length of the request message buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS the request was sent successfully.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode SendRequest(const Address &destinationAddress, const MessageType requestType,
											SyncRequestId &syncRequestId, const void *pRequest, const size_t requestLength) const;

	/**
	 *	@brief	Send a synchronous request but do not wait for the response.
	 *
	 *	If this returns STATUS_CODE_SUCCESS, the response can be obtained by calling syncRequestId.GetResponse().
	 *	The actual return status of the request can then be obtained by GetStatusCode() on the message returned from
	 *	syncRequestId.GetResponse().
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	requestType the request type.
	 *	@param	syncRequestId reference to a SyncRequestId object that can be used later to wait (or poll) for the response.
	 *	@param	request the request message. If request is an empty HeapMessage, a
	 *			request will be sent with no message body.
	 *
	 *	@retval	STATUS_CODE_SUCCESS the request was sent successfully.
	 *	@retval	STATUS_CODE_HEAP_EMPTY if request was empty and no message could be allocated for the outgoing request.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode SendRequest(const Address &destinationAddress, const MessageType requestType, 
											SyncRequestId &syncRequestId, HeapMessage &request) const;

	/**
	 *	@brief	Post a response to a request.
	 *
	 *	@param	destinationAddress the target address (the source address of the request).
	 *	@param	requestType the request type.
	 *	@param	requestId the message identification (to match up the response).
	 *	@param	responseStatus the response status.
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to post.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostResponse(const Address &destinationAddress, const MessageType requestType, 
											 const RequestId &requestId, const StatusCode responseStatus, 
											 const MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Post a response to a request.
	 *
	 *	@param	destinationAddress the target address (the source address of the request).
	 *	@param	requestType the request type.
	 *	@param	requestId the message identification (to match up the response).
	 *	@param	responseStatus the response status.
	 *	@param	pResponse the response message buffer.
	 *	@param	responseLength the length of the response message buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostResponse(const Address &destinationAddress, const MessageType requestType, 
											 const RequestId &requestId, const StatusCode responseStatus, 
											 const void *pResponse = 0, const size_t responseLength = 0) const;

	/**
	 *	@brief	Post a response to a request. Use this call when the response message is a newly allocated
	 *			HeapMessage and is not the original request.
	 *
	 *	@param	destinationAddress the target address (the source address of the request).
	 *	@param	requestType the request type.
	 *	@param	requestId the message identification (to match up the response).
	 *	@param	responseStatus the response status.
	 *	@param	response the response message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostResponse(const Address &destinationAddress, const MessageType requestType, 
											 const RequestId &requestId, const StatusCode responseStatus, 
											 HeapMessage &response) const;

	/**
	 *	@brief	Post a response to a request where the request HeapMessage still has the original request id
	 *			and message type of the request. The contents of the request HeapMessage are overriden with
	 *			the provided MessageBuilder contents.
	 *
	 *	@param	responseStatus the response status.
	 *	@param	request the original request.
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to post.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostResponse(const StatusCode responseStatus, HeapMessage &request,
											 const MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Post a response to a request where the request HeapMessage still has the original request id
	 *			and message type of the request. The contents of the request HeapMessage are overriden with
	 *			the provided buffer.
	 *
	 *	@param	responseStatus the response status.
	 *	@param	request the original request.
	 *	@param	pResponse the response message buffer.
	 *	@param	responseLength the length of the response message buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostResponse(const StatusCode responseStatus, HeapMessage &request,
											 const void *pResponse, const size_t responseLength) const;

	/**
	 *	@brief	Post a response to a request where the request HeapMessage still has the original request id
	 *			and message type of the request. The provided response HeapMessage contains the response body.
	 *
	 *	@param	responseStatus the response status.
	 *	@param	request the original request.
	 *	@param	response a second HeapMessage containing the response body.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostResponse(const StatusCode responseStatus, HeapMessage &request,
											 HeapMessage &response) const;

	/**
	 *	@brief	Post a response to a request where the original request message has been filled in with the response.
	 *			All other members of the response message must be as received (request id, type, addresses).
	 *
	 *	@param	responseStatus the response status.
	 *	@param	request the original request message filled in with the response data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostResponse(const StatusCode responseStatus, HeapMessage &request) const;

	/**
	 *	@brief	Post an empty response to a request. The response will be sent to the source address of the request.
	 *
	 *	If replying to a request and the contents of the response are unimportant, it is preferable to use PostEmptyResponse()
	 *	as if the request was made remotely, there will be no serializing of message body over the network.
	 *
	 *	@param	responseStatus the response status.
	 *	@param	response the original request message. Any data in the message is truncated.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostEmptyResponse(const StatusCode responseStatus, HeapMessage &response) const;

	/**
	 *	@brief	Post an empty response to a request. Use this call when the response message is a newly allocated
	 *			HeapMessage and is not the original request.
	 *
	 *	If replying to a request and the contents of the response are unimportant, it is preferable to use PostEmptyResponse()
	 *	as if the request was made remotely, there will be no serializing of message body over the network.
	 *
	 *	@param	destinationAddress the target address (the source address of the original request).
	 *	@param	requestType the request type of the original request.
	 *	@param	requestId the message identification (to match up the response).
	 *	@param	responseStatus the response status.
	 *	@param	response the response message. Data will be truncated.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostEmptyResponse(const Address &destinationAddress, const MessageType requestType, 
												  const RequestId &requestId, const StatusCode responseStatus, 
												  HeapMessage &response) const;

	/**
	 *	@brief	Post a message.
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	messageType the message type.
	 *	@param	messageBuilder reference to a MessageBuilder containing the message body to post.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostMessage(const Address &destinationAddress, const MessageType messageType,
											const MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Post a message.
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	messageType the message type.
	 *	@param	pMessage the message buffer.
	 *	@param	messageLength the length of the message buffer.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_HEAP_EMPTY.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostMessage(const Address &destinationAddress, const MessageType messageType, 
											const void *pMessage = 0, const size_t messageLength = 0) const;

	/**
	 *	@brief	Post a message.
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	messageType the message type.
	 *	@param	message the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode PostMessage(const Address &destinationAddress, const MessageType messageType, 
											HeapMessage &message) const;

	/**
	 *	@brief	Forward a message to another address without changing the source address.
	 *
	 *	@param	destinationAddress the target address.
	 *	@param	message the message.
	 *	@param	newType the new message type, or default to keeping the message type unchanged.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 *	@retval	STATUS_CODE_DISABLED the endpoint is not accepting messages.
	 */
	ACTIV_SYSTEM_API StatusCode ForwardMessage(const Address &destinationAddress, HeapMessage &message,
											   const MessageType newType = INVALID_MESSAGE_TYPE) const;

protected:
	/**
	 *	@brief	Request callback.
	 *
	 *	If you override this method, please call the base class method for any unrecognized
	 *	request types. Note the request is automatically replied to with the returned status value
	 *	if the status is other than STATUS_CODE_SUCCESS or STATUS_CODE_THREAD_EXIT.
	 *
	 *	If an overriden OnRequest() throws an exception out of OnRequest(), the request is automatically replied to
	 *	if \a request still has an attached message.
	 *
	 *	If the exception is of type ActivException, the status of the response is the exception error code,
	 *	otherwise STATUS_CODE_UNHANDLED_EXCEPTION.
	 *
	 *	If this is not required behaviour, for example if the request has been replied to before an exception is
	 *	thrown, ensure the exception is not allowed out of the scope of the OnRequest() override, or that \a request
	 *	no longer has a message attached at the point the exception is thrown.
	 *
	 *	@param	request the request message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS the message was recognized.
	 *	@retval	STATUS_CODE_UNRECOGNIZED the message was not recognized.
	 *	@retval	STATUS_CODE_THREAD_EXIT received a thread-die message.
	 *	@retval	...
	 */
	ACTIV_SYSTEM_API virtual StatusCode OnRequest(HeapMessage &request);

	/**
	 *	@brief	Response callback.
	 *
	 *	If you override this method, please call the base class method for any unrecognized
	 *	request types.
	 *
	 *	@param	response the response message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS the message was recognized.
	 *	@retval	STATUS_CODE_UNRECOGNIZED the message was not recognized.
	 *	@retval	STATUS_CODE_THREAD_EXIT received a thread-die message.
	 *	@retval	...
	 */
	ACTIV_SYSTEM_API virtual StatusCode OnResponse(HeapMessage &response);

	/**
	 *	@brief	Message callback.
	 *
	 *	If you override this method, please call the base class method for any unrecognized
	 *	message types.
	 *
	 *	@param	message the message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS the message was recognized.
	 *	@retval	STATUS_CODE_UNRECOGNIZED the message was not recognized.
	 *	@retval	STATUS_CODE_THREAD_EXIT received a thread-die message.
	 *	@retval	...
	 */
	ACTIV_SYSTEM_API virtual StatusCode OnMessage(HeapMessage &message);

private:
	/**
	 *	@brief	Open and post a response message to an endpoint.
	 *
	 *	@param	response.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 */
	StatusCode PostResponse(HeapMessage &response) const;

	/**
	 *	@brief	Open and post a message to an endpoint.
	 *
	 *	@param	message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_DISABLED destination endpoint not accepting messages.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the destination endpoint does not exist.
	 */
	StatusCode Post(HeapMessage &message) const;

	/**
	 *	@brief	Open and send a message to an endpoint and get response.
	 *
	 *	@param	message message to send and container for response.
	 *	@param	timeout.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_DISABLED destination endpoint not accepting messages.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the target endpoint does not exist.
	 *	@retval	STATUS_CODE_TIMEOUT no response received in the timeout period.
	 */
	StatusCode Send(HeapMessage &message, const TimeoutPeriod timeout) const;

	/**
	 *	@brief	Open and send a message to an endpoint without waiting for the response.
	 *
	 *	@param	message message to send and container for response.
	 *	@param	syncRequestId reference to a SyncRequestId object that can be used later to wait (or poll) for the response.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_DISABLED destination endpoint not accepting messages.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT the target endpoint does not exist.
	 */
	StatusCode Send(HeapMessage &message, SyncRequestId &syncRequestId) const;

	/**
	 *	@brief	Process a find port request.
	 */
	void ProcessSystemRequestFindPort(HeapMessage &request) const;

	/**
	 *	@brief	Process a ping request.
	 */
	void ProcessSystemRequestPingPort(HeapMessage &request) const;

	/**
	 *	@brief	Process a token.
	 */
	void ProcessSystemRequestToken(HeapMessage &request) const;

	/**
	 *	@brief	Process a get port info list request.
	 */
	void ProcessSystemRequestGetPortInfoList(HeapMessage &request) const;

	/**
	 *	@brief	Process an endpoint startup message.
	 */
	void ProcessSystemMessageEndPointStartup(HeapMessage &message) const;

	ISystem * const			m_pSystem;							///< the owning system
	ManagedEndPoint &		m_managedEndPoint;					///< managed endpoint this port is attached to
	const std::string		m_name;								///< name of the port
	bool					m_hasEnabled;						///< true when Enable() has been invoked
	bool					m_isEnabled;						///< true when port can start to receive messages, after an Enable()
	const Address			m_address;							///< unique address of this port
	AtomicPointer<IHeap>	m_pHeap;							///< heap to use for messaging

	friend class ManagedEndPoint::Impl;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Address &Port::GetAddress() const
{
	return m_address;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IHeap *Port::GetHeap() const
{
	return m_pHeap;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline Port::Enabler::Enabler(Port &port) :
	m_port(port)
{
	m_port.Enable();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Port::Enabler::~Enabler()
{
	m_port.Disable();
}

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_PORT_H)
