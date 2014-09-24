/**
 *	@file	Component.h
 *
 *	@brief	Header file for component class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_COMPONENT_H)
#define ACTIV_FRAMEWORK_COMPONENT_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/ServiceTypes.h"
#include "ActivMiddleware/ActivFramework/TimeHelper.h"

namespace Activ
{

class Application;

/**
 *	@brief	Component class.
 *
 *	Extends Port by adding callbacks for various base service notifications.
 */
class Component : public Port
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this object to a managed endpoint at the next available slot.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	name name of the port, local to the endpoint.
	 *	@param	pHeap heap to use for messaging, or 0 for the system heap.
	 *	@param	componentId the component id.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_FRAMEWORK_API Component(Application &application, ManagedEndPoint &managedEndPoint,
								  const std::string &name = anonymousName, IHeap * const pHeap = 0, 
								  const std::string &componentId = defaultComponentId);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API ~Component();

	/**
	 *	@brief	Get a reference to the application object.
	 *
	 *	@return	Reference to the application object.
	 */
	ACTIV_FRAMEWORK_API Application &GetApplication() const;

	ACTIV_FRAMEWORK_API static const std::string defaultComponentId;	///< the default component id

protected:
	ACTIV_FRAMEWORK_API virtual StatusCode OnRequest(HeapMessage &request);
	ACTIV_FRAMEWORK_API virtual StatusCode OnMessage(HeapMessage &message);

	/**
	 *	@brief	Timer callback.
	 *
	 *	When overriding, please call the base class version for any unrecognized timer.
	 *
	 *	@param	timerAddress the timer address.
	 *	@param	time the date/time of the timer.
	 *
	 *	@return	The return value is not used by the framework, but may be used by an application to pass values between
	 *			parent and sub class versions of this function. Returns STATUS_CODE_UNRECOGNIZED by default.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnTimer(const Address &timerAddress, const BaseTime &time);

	/**
	 *	@brief	A circuit has been established.
	 *
	 *	When overriding, please call the base class version for any unrecognized circuit.
	 *
	 *	This is called for a successful asynchronous connection. connectMessage.GetSourceAddress() is the circuit
	 *	that has established.
	 *
	 *	The message may contain response data from an user data that was sent in the circuit establishment, if
	 *	the underlying protocol supports it.
	 *
	 *	For example, if this is the result of an asynchronous OpenService() call, ValidateOpenServiceResponse()
	 *	should be called.
	 *
	 *	@param	connectMessage.
	 *
	 *	@return	The return value is not used by the framework, but may be used by an application to pass values between
	 *			parent and sub class versions of this function. Returns STATUS_CODE_UNRECOGNIZED by default.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnConnect(HeapMessage &connectMessage);

	/**
	 *	@brief	A circuit has failed to connect.
	 *
	 *	When overriding, please call the base class version for any unrecognized circuit.
	 *
	 *	The callback is for informational purposes only. If an asynchronous connection attempts fails, the
	 *	OnConnectFailed() callback will be invoked but the router service will automatically attempt to connect again
	 *	on a lazy basis until the circuit is Disconnect()ed. No OnConnectFailed() is called for a synchronous connect
	 *	(the connect call will just timeout after the specified period with an error status code).
	 *
	 *	@param	connectFailMessage the circuit that has failed to connect is connectFailMessage.GetSourceAddress(), a reason
	 *			for the failed connect is connectFailMessage.GetStatusCode().
	 *
	 *	@return	The return value is not used by the framework, but may be used by an application to pass values between
	 *			parent and sub class versions of this function. Returns STATUS_CODE_UNRECOGNIZED by default.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnConnectFailed(HeapMessage &connectFailMessage);

	/**
	 *	@brief	A client is attempting to connect.
	 *
	 *	The source address of \a acceptMessage contains the address of the circuit that is attempting to connect.
	 *	This address should be used to communicate with the client.
	 *
	 *	The ip address embedded in acceptMessage (accessible with acceptMessage.GetIpAddress()) contains the ip address
	 *	of the client attempting to connect.
	 *
	 *	The acceptMessage must be replied to, using PostResponse(statusCode, acceptMessage). Passing
	 *	STATUS_CODE_SUCCESS allows the client access, any other other value refuses the client.
	 *
	 *	The acceptMessage itself can be forwarded to another component instead of being replied to immediately.
	 *	That component will then have OnAccept() called, from which it can PostResponse(). The established circuit
	 *	will then be to that component and not the original receiver of the OnAccept().
	 *
	 *	If acceptMessage is set to another HeapMessage from a different heap, the router will use the new heap
	 *	for received messages on the connection, rather than the original heap. NB the contents of acceptMessage
	 *	must be copied to the new HeapMessage.
	 *
	 *	The default Component::OnAccept() behaviour is to call PostResponse(STATUS_CODE_SUCCESS, acceptMessage) to
	 *	accept the client.
	 *
	 *	@param	acceptMessage.
	 *
	 *	@return	The return value is not used by the framework and is provided as a helper for overriding.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnAccept(HeapMessage &acceptMessage);

	/**
	 *	@brief	A circuit has broken under our feet.
	 *
	 *	When overriding, please call the base class version for any unrecognized circuit.
	 *
	 *	@param	breakMessage the address of the circuit that is now not connected is breakMessage.GetSourceAddress().
	 *			A reason for the break can be found in breakMessage.GetStatusCode().
	 *
	 *	@return	The return value is not used by the framework, but may be used by an application to pass values between
	 *			parent and sub class versions of this function. Returns STATUS_CODE_UNRECOGNIZED by default.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnBreak(HeapMessage &breakMessage);

	/**
	 *	@brief	A circuit has been closed after an asynchronous Disconnect() call.
	 *
	 *	When overriding, please call the base class version for any unrecognized circuit.
	 *
	 *	This is called for a successful asynchronous disconnect completion.
	 *	disconnectMessage.GetSourceAddress() is the circuit that has been established.
	 *
	 *	The message may contain response data from any user data that was sent in the disconnect, if
	 *	the underlying protocol supports it.
	 *
	 *	@param	disconnectMessage.
	 *
	 *	@return	The return value is not used by the framework, but may be used by an application to pass values between
	 *			parent and sub class versions of this function. Returns STATUS_CODE_UNRECOGNIZED by default.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnDisconnect(HeapMessage &disconnectMessage);

	/**
	 *	@brief	Raw data received.
	 *
	 *	When overriding, please call the base class version if the data is from an unknown address.
	 *
	 *	@param	heapMessage heap message containing the received data.
	 *
	 *	@return	The return value is not used by the framework, but may be used by an application to pass values between
	 *			parent and sub class versions of this function. Returns STATUS_CODE_UNRECOGNIZED by default.
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnData(HeapMessage &heapMessage);

private:
	/**
	 *	@brief	Process a timer message.
	 */
	void ProcessOnTimerMessage(HeapMessage &message);

	Application	&m_application;									///< application object this component is using
};

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_COMPONENT_H)
