/**
 *	@file	HandleManager.h
 *
 *	@brief	Header file for the handle manager class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_HANDLE_MANAGER_H)
#define ACTIV_HANDLE_MANAGER_H

#include "ActivMiddleware/Activ.h"

#if (defined ACTIV_PLATFORM_POSIX)
#include <sys/poll.h>
#endif

namespace Activ
{

/**
 *	@brief	HandleManager class.
 *
 *	T should be a class that derives from HandleManager and provided a void OnEvent(size_t index) function,
 *	which will be invoked for each signalled handle during a Wait() call, eg:
 *
 *	class MyHandleManager : public HandleManager<MyHandleManager>
 *	{
 *	public:
 *		void OnEvent(const size_t index)
 *		{
 *		}
 *		...
 *	};
 */
template<typename T>
class HandleManager
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	HandleManager();
	
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pHandles pointer to an array of handles to manage.
	 *	@param	numberOfHandles size of @a pHandles.
	 */
	HandleManager(HANDLE *pHandles, const size_t numberOfHandles);
	
	/**
	 *	@brief	Initialize.
	 *
	 *	@param	pHandles pointer to an array of handles to manage.
	 *	@param	numberOfHandles size of @a pHandles.
	 */
	void Initialize(HANDLE *pHandles, const size_t numberOfHandles);
	
	/**
	 *	@brief	Wait for notification on handles.
	 *
	 *	For each handles, T::OnEvent(index) will be invoked.
	 *
	 *	@param	timeoutPeriod in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	...
	 */
	StatusCode Wait(const TimeoutPeriod timeoutPeriod);
	
private:
	T &			m_derived;
	size_t		m_numberOfHandles;
	
#if (defined ACTIV_PLATFORM_POSIX)

	typedef std::vector<pollfd> PollFdList;
	
	PollFdList	m_pollFdList;

#elif (defined ACTIV_PLATFORM_WINDOWS)

	typedef std::vector<HANDLE> HandleList;

	HandleList	m_handleList;

#else
#error Configure
#endif
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
HandleManager<T>::HandleManager() :
	m_derived(static_cast<T &>(*this))
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
HandleManager<T>::HandleManager(HANDLE *pHandles, const size_t numberOfHandles) :
	m_derived(static_cast<T &>(*this))
{
	Initialize(pHandles, numberOfHandles);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
void HandleManager<T>::Initialize(HANDLE *pHandles, const size_t numberOfHandles)
{
	m_numberOfHandles = numberOfHandles;
	
#if (defined ACTIV_PLATFORM_POSIX)
	
	m_pollFdList.clear();
	
	for (size_t i = 0; i < m_numberOfHandles; ++i)
	{
		pollfd pollFd;
		
		pollFd.fd = pHandles[i];
		pollFd.events = POLLIN;
		
		m_pollFdList.push_back(pollFd);
	}
	
#elif (defined ACTIV_PLATFORM_WINDOWS)

	m_handleList.assign(pHandles, pHandles + numberOfHandles);
	
#else
#error Configure
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode HandleManager<T>::Wait(const TimeoutPeriod timeoutPeriod)
{
#if (defined ACTIV_PLATFORM_POSIX)

	const int res = ::poll(&m_pollFdList[0], m_numberOfHandles, timeoutPeriod);
	
	if (0 == res)
		return STATUS_CODE_TIMEOUT;
		
	if (res < 0)
		return OsErrorToStatusCode(errno);
		
	for (size_t i = 0, numberSignalled = 0; i < m_numberOfHandles; ++i)
	{
		if (m_pollFdList[i].revents & POLLIN)
		{
			m_derived.OnEvent(i);
			
			if (++numberSignalled == static_cast<size_t>(res))
				break;
		}
	}
	
#elif (defined ACTIV_PLATFORM_WINDOWS)

	const DWORD res = ::WaitForMultipleObjects(static_cast<DWORD>(m_numberOfHandles), &m_handleList[0], FALSE, timeoutPeriod);

	if ((res >= WAIT_OBJECT_0) && (res < WAIT_OBJECT_0 + m_numberOfHandles))
		m_derived.OnEvent(res - WAIT_OBJECT_0);
	else if ((res >= WAIT_ABANDONED_0) && (res < WAIT_ABANDONED_0 + m_numberOfHandles))
		m_derived.OnEvent(res - WAIT_ABANDONED_0);
	else if (WAIT_TIMEOUT == res)
		return STATUS_CODE_TIMEOUT;
	else // (WAIT_FAILED == res)
		return OsErrorToStatusCode(::GetLastError());

#else
#error Configure
#endif

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // !defined (ACTIV_HANDLE_MANAGER_H)
