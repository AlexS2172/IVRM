/**
 *	@file	Application.h
 *
 *	@brief	Header file for base application class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_APPLICATION_H)
#define ACTIV_FRAMEWORK_APPLICATION_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/Component.h"
#include "ActivMiddleware/ActivFramework/DirectoryWatchApi.h"
#include "ActivMiddleware/ActivFramework/TimerServiceApi.h"
#include "ActivMiddleware/ActivFramework/LoggerServiceApi.h"
#include "ActivMiddleware/ActivFramework/RouterServiceApi.h"
#include "ActivMiddleware/ActivFramework/PubSubServiceApi.h"
#include "ActivMiddleware/ActivFramework/ServiceApi.h"
#include "ActivMiddleware/ActivBase/CtrlHandler.h"

#include "boost/scoped_ptr.hpp"

namespace Activ
{

/**
 *	@brief	Proxy class for ISystem interface.
 */
class SystemProxy : public ISystem
{
public:
	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API ~SystemProxy();

	// ISystem
	ACTIV_FRAMEWORK_API virtual IEndPoint *CreatePrivateEndPoint(const bool hasMutex, const IEndPoint::NotificationType notificationType);
	ACTIV_FRAMEWORK_API virtual ISyncEndPoint *CreateEndPoint(const std::string &name, const bool hasMutex, const IEndPoint::NotificationType notificationType);
	ACTIV_FRAMEWORK_API virtual ISyncEndPoint *OpenEndPoint(const std::string &name, Timeout &timeout = Timeout::GetInfinite());
	ACTIV_FRAMEWORK_API virtual ISyncEndPoint *OpenEndPoint(EndPointId endPointId, Timeout &timeout = Timeout::GetInfinite());
	ACTIV_FRAMEWORK_API virtual IHeap *CreateHeap(const std::string &name, const IHeap::BucketInfoVector &bucketInfoVector);
	ACTIV_FRAMEWORK_API virtual IHeap *OpenHeap(const std::string &name, Timeout &timeout = Timeout::GetInfinite());
	ACTIV_FRAMEWORK_API virtual IHeap *OpenHeap(const HeapId heapId, Timeout &timeout = Timeout::GetInfinite());
	ACTIV_FRAMEWORK_API virtual IProcess *OpenProcess(const std::string &name, Timeout &timeout = Timeout::GetInfinite());
	ACTIV_FRAMEWORK_API virtual IProcess *OpenProcess(const ProcessId processId, Timeout &timeout = Timeout::GetInfinite());
	ACTIV_FRAMEWORK_API virtual std::string GetSystemName() const;
	ACTIV_FRAMEWORK_API virtual IHeap *GetSystemHeap() const;
	ACTIV_FRAMEWORK_API virtual IProcess *GetProcess() const;
	ACTIV_FRAMEWORK_API virtual SystemInfo GetSystemInfo() const;
	ACTIV_FRAMEWORK_API virtual void EnumerateObjects(IProcessEnumerator &enumerator) const;
	ACTIV_FRAMEWORK_API virtual void EnumerateObjects(IEndPointEnumerator &enumerator) const;
	ACTIV_FRAMEWORK_API virtual void EnumerateObjects(IHeapEnumerator &enumerator) const;

private:
	/**
	 *	@brief	Create a system.
	 *
	 *	@param	settings settings.
	 *	@param	bucketInfoVector describing the size of the system heap to create.
	 *
	 *	@throw	Exception.
	 */
	SystemProxy(const ISystem::Settings &settings, const IHeap::BucketInfoVector &bucketInfoVector);

	/**
	 *	@brief	Open a system.
	 *
	 *	@param	settings settings.
	 *	@param	processName the name of this process, empty string gives a process name based on the os process id.
	 *	@param	timeout how long to wait for the system to exist, in ms.
	 *
	 *	@throw	Exception.
	 */
	SystemProxy(const ISystem::Settings &settings, const std::string &processName, Timeout &timeout = Timeout::GetInfinite());

	ISystem * const	m_pSystem;									///< The underlying system

	friend class Application;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Base application class.
 *
 *	The base application class provides the ISystem interface to create and open heaps, endpoints.
 *	It also provides always on router and timer functionality and creates a default endpoint which is given the
 *	name of the process.
 *
 *	Do not use the Application class directly. AgentApplication is provided for processes which are to
 *	create a shared memory system or standalone private memory application,
 *	and ThinApplication for those which link into an existing shared memory system.
 */
class Application : public SystemProxy, public ManagedEndPoint, public Component
{
public:
	/**
	 *	@brief	Settings.
	 */
	class Settings : public ISystem::Settings
	{
	public:
		/**
		 *	@brief	List of exception handler types.
		 *
		 *	Exception handler on available on Windows platform.
		 */
		enum ExceptionHandlerType
		{
			EXCEPTION_HANDLER_TYPE_NONE,						///< no exception handler
			EXCEPTION_HANDLER_TYPE_NORMAL,						///< don't dump entire address space
			EXCEPTION_HANDLER_TYPE_FULL_MEMORY					///< do dump address space
		};

		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_FRAMEWORK_API Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_FRAMEWORK_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		bool						m_enableCtrlHandler;		///< default: true
		ExceptionHandlerType		m_exceptionHandlerType;		///< default: EXCEPTION_HANDLER_TYPE_NONE
		ManagedEndPoint::Settings	m_managedEndPointSettings;	///< settings for default application endpoint
	};

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API virtual ~Application();

	/**
	 *	@brief	Initiate a shutdown.
	 */
	ACTIV_FRAMEWORK_API void Shutdown();

	/**
	 *	@brief	Get the monitor service heap.
	 *
	 *	The monitor service heap should be used for publishing monitoring messages.
	 *
	 *	@return	Heap used by the monitor service.
	 */
	ACTIV_FRAMEWORK_API IHeap *GetMonitorServiceHeap() const;

	/**
	 *	@brief	Get the name of the default ui service.
	 *
	 *	This name should be used when adding components to the ui service. See UiComponent.h.
	 *
	 *	@return	The name of the default ui service.
	 */
	ACTIV_FRAMEWORK_API static const std::string &GetUiServiceName();

	class Private;												///< forward reference of implementation details

	/**
	 *	@brief	Get private implementation.
	 *
	 *	@return	Const reference to private implementation object.
	 */
	const Private &GetPrivate() const;

protected:
	ACTIV_FRAMEWORK_API virtual StatusCode OnMessage(HeapMessage &message);

	/**
	 *	@brief	Application has received a ctrl event.
	 *
	 *	@param	eventType the type of ctrl event.
	 */
	ACTIV_FRAMEWORK_API virtual void OnCtrlEvent(const CtrlHandler::EventType eventType);

	/**
	 *	@brief	Shutdown has been invoked.
	 *
	 *	This callback is invoked for all processes when a system is shutdown (i.e. the Agent has called AgentApplication::Shutdown()),
	 *	or for a single application if it itself calls ThinApplication::Shutdown().
	 *
	 *	When overriding, call the base class method at the end of the override.
	 */
	ACTIV_FRAMEWORK_API virtual void OnShutdown();

private:
	/**
	 *	@brief	Create a system.
	 *
	 *	@param	settings settings for the application.
	 *	@param	bucketInfoVector info describing the size of the system heap to create.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_FRAMEWORK_API Application(const Settings &settings, const IHeap::BucketInfoVector &bucketInfoVector);

	/**
	 *	@brief	Open a system.
	 *
	 *	@param	settings settings for the application.
	 *	@param	processName the name of this process, empty string gives a process name based on the os process id.
	 *	@param	timeout how long to wait for the system to exist, in ms.
	 *
	 *	@throw	Exception.
	 */
	Application(const Settings &settings, const std::string &processName, Timeout &timeout = Timeout::GetInfinite());

	const Settings	m_settings;									///< settings for this application
	Private	*		m_pPrivate;									///< hidden implementation details

	friend class AgentApplication;
	friend class ThinApplication;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline const Application::Private &Application::GetPrivate() const
{
	return *m_pPrivate;
}

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_APPLICATION_H)
