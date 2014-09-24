/**
 *	@file	ManagedEndPoint.h
 *
 *	@brief	Header file for managed endpoint class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_MANAGED_ENDPOINT_H)
#define ACTIV_SYSTEM_MANAGED_ENDPOINT_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/IEndPoint.h"
#include "ActivMiddleware/ActivBase/IniFileParser.h"

#include "boost/scoped_ptr.hpp"

#include <string>

namespace Activ
{

class ISystem;
class ISyncEndPoint;
class Address;
class HeapMessage;

/**
 *	@brief	Managed endpoint class.
 *
 *	A managed endpoint contains the underlying endpoint object and managed ports that are attached to the endpoint
 *	and threads that are processing messages from the endpoint.
 */
class ManagedEndPoint
{
public:
	typedef std::set<PortId> PortIdSet;							///< type of the set of port ids
	
	/**
	 *	@brief	Settings for the endpoint.
	 *
	 *	m_maxThreads the maximum number of threads that can be running the endpoint message loop at any one time.
	 *	m_notificationType the mechanism to use for notification of messages arriving on the endpoint.
	 *	m_initialPortListSize initial size of the port list, also used to grow the port list.
	 *	m_minFreePortIdListSize this number of port ids must be in the free list before a port can be resused.
	 *	m_reservedPortIdSet a set of PortIds that cannot by dynamically allocated by AttachPort().
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SYSTEM_API Settings();
		
		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_SYSTEM_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);
	
		size_t						m_maxThreads;				///< default: 1
		IEndPoint::NotificationType	m_notificationType;			///< default: NOTIFICATION_TYPE_DEFAULT
		PortId						m_initialPortListSize;		///< default: 64
		PortId						m_minFreePortIdListSize;	///< default: 32
		PortIdSet					m_reservedPortIdSet;		///< default: empty	
	};

	/**
	 *	@brief	Helper class to run an endpoint.
	 *
	 *	The behavior of the Run() method and the thread started by a call to StartThread() is to not
	 *	return until the endpoint has received die messages (from PostDiesToThreads()).
	 *
	 *	However, if you wish to integrate the endpoint message loop with another event loop
	 *	mechanism it may be necessary to poll the endpoint, or only block for a small timeout period.
	 *	This can be accomplished with the RunHelper class:
	 *
	 *	Application application;
	 *	ManagedEndPoint::Run applicationRun(application);
	 *
	 *	switch (applicationRun(10))
	 *	{
	 *		case STATUS_CODE_SUCCESS:
	 *			// message processed
	 *			break;
	 *
	 *		case STATUS_CODE_TIMEOUT:
	 *			// no messages on endpoint in the timeout period
	 *			break;
	 *
	 *		case STATUS_CODE_EXIT:
	 *			// endpoint has received dies
	 *			break;
	 *	}
	 */
	ACTIV_DECLARE_NESTED_CLASS(RunHelper)
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	Sets up the calling thread to run the endpoint.
		 *
		 *	@param	managedEndPoint the endpoint to run.
		 *
		 *	@throw	ActivException if too many threads are processing the endpoint already, out of memory.
		 */
		ACTIV_SYSTEM_API RunHelper(ManagedEndPoint &managedEndPoint);

		/**
		 *	@brief	Destructor.
		 *
		 *	Cleans up some thread local state.
		 */
		ACTIV_SYSTEM_API ~RunHelper();

		/**
		 *	@brief	Pump the endpoint message loop.
		 *
		 *	@param	timeout how long to wait for messages on the endpoint.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_TIMEOUT no message on the endpoint
		 *	@retval	STATUS_CODE_EXIT endpoint received a die message.
		 */
		ACTIV_SYSTEM_API StatusCode operator()(const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE);

	private:
		ManagedEndPoint &m_managedEndPoint;						///< hook back to the endpoint
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	This constructor creates an endpoint with the given name. No threads are launched.
	 *
	 *	@param	pSystem the system that will create the endpoint.
	 *	@param	name the name of the endpoint.
	 *	@param	settings settings for the endpoint.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SYSTEM_API ManagedEndPoint(ISystem * const pSystem, const std::string &name, const Settings &settings = Settings());

	/**
	 *	@brief	Destructor.
	 *
	 *	The destructor stops the endpoint from receiving any more messages, and clears any queued messages.
	 */
	ACTIV_SYSTEM_API virtual ~ManagedEndPoint();

	/**
	 *	@brief	Run the endpoint from the context of the calling thread.
	 *
	 *	This call will run return until the endpoint has received die messages from
	 *	PostDiesToThreads().
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FULL too many threads already running.
	 */
	ACTIV_SYSTEM_API StatusCode Run();

	/**
	 *	@brief	Launch a thread to process this endpoint.
	 *
	 *	The virtual method Process() is called from the new thread. The default behaviour of
	 *	Process() is to run the endpoint until dies are received (essentially Run() from the new thread).
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_FULL
	 */
	ACTIV_SYSTEM_API StatusCode StartThread();

	/**
	 *	@brief	Post shutdown messages to all the threads processing the endpoint.
	 */
	ACTIV_SYSTEM_API void PostDiesToThreads() const;

	/**
	 *	@brief	Wait for all threads to finish processing the endpoint to exit.
	 *
	 *	@param	timeout how long to wait, in ms.
	 *
 	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	STATUS_CODE_IN_USE called from within a thread processing the endpoint
	 */
	ACTIV_SYSTEM_API StatusCode WaitForThreadsToExit(const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get the underlying endpoint.
	 *
	 *	@return	The ISyncEndPoint interface of the endpoint.
	 */
	ACTIV_SYSTEM_API ISyncEndPoint *GetEndPoint() const;

	/**
	 *	@brief	Get the maximum number of threads that can process the endpoint at once.
	 *
	 *	@return	The maximum number of threads that can process the endpoint at once.
	 */
	ACTIV_SYSTEM_API size_t GetMaxThreads() const;

	/**
	 *	@brief	Find a port on the endpoint by name.
	 *
	 *	@param	portName name of the port to find.
	 *	@param	portAddress a reference to the found port.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_SYSTEM_API StatusCode FindPort(const std::string &portName, Address &portAddress) const;

	ACTIV_DECLARE_NESTED_CLASS(Impl);

protected:
	/**
	 *	@brief	Overridable endpoint processing function.
	 *
	 *	This function is called from the thread launched by a StartThread() call, it is *not* called
	 *	after using the Run function object.
	 */
	ACTIV_SYSTEM_API virtual void Process();

	/**
	 *	@brief	Ok to exit this thread?
	 *
	 *	Once a thread has received an exit message, CanThreadExit() will subsequently be invoked after processing
	 *	of any message by that thread to determine if the thread can actually exit. Default behaviour is
	 *	to return true.
	 *
	 *	@retval	true this thread can exit
	 *	@retval	false this thread cannot exit
	 */
	ACTIV_SYSTEM_API virtual bool CanThreadExit();

private:
	/**
	 *	@brief	Process a message and invoke the message handler of the destination port.
	 *
	 *	@param	heapMessage a message received on the endpoint.
	 *
	 *	@retval	STATUS_CODE_EXIT thread exit message received
	 *	@retval	...
	 */
	StatusCode ProcessMessage(HeapMessage &heapMessage);

	boost::scoped_ptr<Impl>	m_pImpl;							///< implementation details

	friend class Port;
};

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_MANAGED_ENDPOINT_H)
