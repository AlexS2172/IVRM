/**
 *	@file	TimerService.h
 *
 *	@brief	Header file for the timer service class.
 *
 *	$Log: $
 */

#if !defined(ACTIV_TIMER_SERVICE_H)
#define ACTIV_TIMER_SERVICE_H

#include "ActivMiddleware/ActivBaseServices/External.h"

#include <set>

// Default heap size
#define ACTIV_DEFAULT_TIMER_SERVICE_BUCKET_INFO		{ { 18, 2500 } }

namespace Activ
{

class TimerPort;
class TimerServiceCurrentTime;

/**
 *	@brief	Timer service class.
 */
class TimerService : private ManagedEndPoint, public Component
{
public:
	/**
	 *	@brief	Settings.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_BASE_SERVICES_API Settings();

		/**
		 *	@brief	Default destructor.
		 */
		ACTIV_BASE_SERVICES_API ~Settings();

		/**
		 *	@brief	Read settings from an ini file section.
		 *
		 *	@param	sectionIterator iterator to an ini file section.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	...
		 */
		ACTIV_BASE_SERVICES_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

		/**
		 *	@brief	Heap size for timer service, default above.
		 */
		IHeap::BucketInfoVector	m_heapBucketInfoVector;

		/**
		 *	@brief	For TimerServiceApi::AdjustSystemTime(), threshold in milliseconds at which to set
		 *			absolute time rather than adjust gradually. Default 60000.
		 */
		uint32_t				m_adjustSystemTimeThreshold;
	};

	/**
	 *	@brief	Timer service ui component.
	 */
	class UiComponent : public Activ::UiComponent
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	timerService the timer service to have the ui.
		 *	@param	uiServiceName name of the ui service.
		 *
		 *	@throw	Exception.
		 */
		ACTIV_BASE_SERVICES_API UiComponent(TimerService &timerService, const std::string &uiServiceName);

	private:
		virtual void Run(UiUser &uiUser);

		TimerService	&m_timerService;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	The constructor creates an endpoint and heap in the system and runs a single thread.
	 *
	 *	@param	application the application the service is to run in.
	 *	@param	serviceName the name of the timer service.
	 *	@param	settings settings for the timer service.
	 *
	 *	@throw	Exception
	 */
	ACTIV_BASE_SERVICES_API TimerService(Application &application, const std::string &serviceName, const Settings &settings);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_SERVICES_API ~TimerService();

protected:
	ACTIV_BASE_SERVICES_API virtual StatusCode OnRequest(HeapMessage &request);
	ACTIV_BASE_SERVICES_API virtual StatusCode OnMessage(HeapMessage &message);

	// ManagedEndPoint overrides
	ACTIV_BASE_SERVICES_API virtual void Process();

private:
	virtual bool CanThreadExit();

	/**
	 *	@brief	Process a set timer request.
	 *
	 *	@param	request HeapMessage containing the serialized request.
	 */
	void ProcessSetTimerRequest(HeapMessage &request);

	/**
	 *	@brief	Process get current time object request.
	 *
	 *	@param	request HeapMessage containing the serialized request.
	 */
	void ProcessGetCurrentTimeObjectRequest(HeapMessage &request);

	/**
	 *	@brief	Process adjust system time request.
	 *
	 *	@param	request HeapMessage containing the serialized request.
	 */
	void ProcessAdjustSystemTimeRequest(HeapMessage &request);

	/**
	 *	@brief	Process a cancel outstanding timer message.
	 *
	 *	@param	message HeapMessage containing the serialized message.
	 */
	void ProcessCancelTimerMessage(HeapMessage &message);

	/**
	 *	@brief	Process a endpoint setmessage.
	 *
	 *	@param	message HeapMessage containing the serialized message.
	 *	@param	type string for log purposes.
	 */
	void ProcessSystemEndPointIdSetMessage(HeapMessage &message, const char * const type);

	/**
	 *	@brief	Process a tick from the endpoint thread.
	 */
	void ProcessTick();

	/**
	 *	@brief	Set current time in shared object.
	 *
	 *	@param	utcTime current utc time.
	 *	@param	localTime current local time.
	 */
	void SetCurrentTime(const UtcTime &utcTime, const LocalTime &localTime);

	/**
	 *	@brief	Add timer port to service.
	 *
	 *	@param	pTimerPort the port to add.
	 */
	void AddTimerPort(TimerPort * const pTimerPort);

	/**
	 *	@brief	Remove timer port from service.
	 *
	 *	@param	pTimerPort the port to remove.
	 */
	void RemoveTimerPort(TimerPort * const pTimerPort);

	/**
	 *	@brief	Display current time.
	 *
	 *	@param	uiIo.
	 */
	void UiDisplayCurrentTime(const UiIo &uiIo);

	/**
	 *	@brief	Display current timers.
	 *
	 *	@param	uiIo.
	 */
	void UiDisplayTimers(const UiIo &uiIo);

	typedef std::set<TimerPort *> TimerPortSet;						///< set of timers
	class SharedCurrentTime;

	static const size_t DATA_BUFFER_SIZE = 64;						///< size of data buffer used for building/validating messages
	static const TimeoutPeriod TICK_TIMEOUT = 50;					///< timeout on endpoint get

	const Settings							m_settings;				///< settings

	TimerPortSet							m_timerPortSet;
	std::auto_ptr<SharedCurrentTime>		m_pSharedCurrentTime;	///< current time in shared memory
	std::auto_ptr<TimerServiceCurrentTime>	m_pPrivateCurrentTime;	///< current time in private memory
	TimerServiceCurrentTime *				m_pCurrentTime;			///< pointer to current time (private or shared)

	friend class TimerPort;
	friend class TimerManagedEndPoint;
};

} // namespace Activ

#endif // !defined(ACTIV_TIMER_SERVICE_H)
