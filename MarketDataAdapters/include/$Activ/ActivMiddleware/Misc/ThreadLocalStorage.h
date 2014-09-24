/**
 *	@file	ThreadLocalStorage.h
 *
 *	@brief	Header file for thread local storage class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MISC_THREAD_LOCAL_STORAGE_H)
#define ACTIV_MISC_THREAD_LOCAL_STORAGE_H

#include "ActivMiddleware/ActivBase/OsException.h"

namespace Activ
{

/**
 *	@brief	Thread local storage helper.
 *
 *	Stores pointers of type T*.
 */
template <typename T>
class ThreadLocalStorage
{
public:
	/**
	 *	@brief	Constructor. Allocate thread local storage.
	 *
	 *	@throw	Exception.
	 */
	ThreadLocalStorage();

	/**
	 *	@brief	Destructor. Free the local storage.
	 */
	~ThreadLocalStorage();
	
	/**
	 *	@brief	Get the current value of thread local storage for this thread.
	 *
	 *	@return	Current value.
	 */
	T *Get() const;

	/**
	 *	@brief	Set the current value of thread local storage for this thread.
	 *
	 *	@param	p value to set.
	 */
	void Set(T * const p);

private:
#if defined (ACTIV_PLATFORM_WINDOWS)
	const DWORD		m_index;
#elif defined (ACTIV_PLATFORM_POSIX)
	pthread_key_t	m_key;
#else
#error Write me for this platform
#endif
};

// ---------------------------------------------------------------------------------------------------------------------------------

#if defined (ACTIV_PLATFORM_WINDOWS)

template <typename T>
ThreadLocalStorage<T>::ThreadLocalStorage() :
	m_index(::TlsAlloc())
{
	if (TLS_OUT_OF_INDEXES == m_index)
		ACTIV_THROW(OsException, ::GetLastError());
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
ThreadLocalStorage<T>::~ThreadLocalStorage()
{
	(void) ::TlsFree(m_index);
}


// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
T *ThreadLocalStorage<T>::Get() const
{
	return reinterpret_cast<T *>(::TlsGetValue(m_index));
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void ThreadLocalStorage<T>::Set(T * const p)
{
	(void) ::TlsSetValue(m_index, (void *)p);
}

// ---------------------------------------------------------------------------------------------------------------------------------

#elif defined (ACTIV_PLATFORM_POSIX)

template <typename T>
ThreadLocalStorage<T>::ThreadLocalStorage()
{
	int res = ::pthread_key_create(&m_key, NULL);
	
	if (0 != res)
		ACTIV_THROW(OsException, res);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
ThreadLocalStorage<T>::~ThreadLocalStorage()
{
	(void) ::pthread_key_delete(m_key);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
T *ThreadLocalStorage<T>::Get() const
{
	return reinterpret_cast<T *>(::pthread_getspecific(m_key));
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void ThreadLocalStorage<T>::Set(T * const p)
{
	(void) ::pthread_setspecific(m_key, (void *)p);
}

#else
#error Write me
#endif

} // namespace Activ

#endif // !defined (ACTIV_MISC_THREAD_LOCAL_STORAGE_H)
