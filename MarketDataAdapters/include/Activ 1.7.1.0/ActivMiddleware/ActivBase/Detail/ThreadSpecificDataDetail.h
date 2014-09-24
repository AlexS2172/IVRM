/**
 *	@file	ThreadSpecificDataDetail.h
 *
 *	@brief	Implementation details thread for specific data storage class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_THREAD_SPECIFIC_DATA_DETAIL_H)
#define ACTIV_BASE_THREAD_SPECIFIC_DATA_DETAIL_H

#include "ActivMiddleware/ActivBase/OsException.h"
#include "ActivMiddleware/ActivBase/StatusCodeException.h"

namespace Activ
{

#if (defined ACTIV_PLATFORM_WINDOWS)

typedef void (*TsdDestructorFn)(void *);

ACTIV_BASE_API void ThreadSpecificDataCleanupAddKey(const DWORD key, TsdDestructorFn pTsdDestructorFn);
ACTIV_BASE_API void ThreadSpecificDataCleanupRemoveKey(const DWORD key);

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
inline ThreadSpecificDataBase<TYPE, DERIVED>::ThreadSpecificDataBase() :
	m_index(::TlsAlloc())
{
	if (TLS_OUT_OF_INDEXES == m_index)
		ACTIV_THROW(OsException, ::GetLastError());

	ThreadSpecificDataCleanupAddKey(m_index, &StaticDestructor);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
inline ThreadSpecificDataBase<TYPE, DERIVED>::~ThreadSpecificDataBase()
{
	void * const pData = ::TlsGetValue(m_index);
	
	if (0 != pData)
		StaticDestructor(pData);
	
	ThreadSpecificDataCleanupRemoveKey(m_index);
	(void) ::TlsFree(m_index);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
inline TYPE *ThreadSpecificDataBase<TYPE, DERIVED>::Get() const
{
	PerThreadInfo * const pPerThreadInfo = reinterpret_cast<PerThreadInfo *>(::TlsGetValue(m_index));
	
	if (0 == pPerThreadInfo)
		return 0;
	
	return pPerThreadInfo->m_pValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
inline void ThreadSpecificDataBase<TYPE, DERIVED>::Set(TYPE * const pValue)
{
	PerThreadInfo *pPerThreadInfo = reinterpret_cast<PerThreadInfo *>(::TlsGetValue(m_index));
	
	if (0 == pPerThreadInfo)
	{
		pPerThreadInfo = new PerThreadInfo(*this);
		(void) ::TlsSetValue(m_index, reinterpret_cast<void *>(pPerThreadInfo));
	}
	
	pPerThreadInfo->m_pValue = pValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#elif (defined ACTIV_PLATFORM_POSIX)

template <typename TYPE, typename DERIVED>
inline ThreadSpecificDataBase<TYPE, DERIVED>::ThreadSpecificDataBase()
{
	int res = ::pthread_key_create(&m_key, &StaticDestructor);
	
	if (0 != res)
		ACTIV_THROW(OsException, res);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
inline ThreadSpecificDataBase<TYPE, DERIVED>::~ThreadSpecificDataBase()
{
	void * const pData = ::pthread_getspecific(m_key);
	
	if (0 != pData)
		StaticDestructor(pData);
	
	(void) ::pthread_key_delete(m_key);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
inline TYPE *ThreadSpecificDataBase<TYPE, DERIVED>::Get() const
{
	PerThreadInfo * const pPerThreadInfo = reinterpret_cast<PerThreadInfo *>(::pthread_getspecific(m_key));
	
	if (0 == pPerThreadInfo)
		return 0;
	
	return pPerThreadInfo->m_pValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
inline void ThreadSpecificDataBase<TYPE, DERIVED>::Set(TYPE * const pValue)
{
	PerThreadInfo *pPerThreadInfo = reinterpret_cast<PerThreadInfo *>(::pthread_getspecific(m_key));
	
	if (0 == pPerThreadInfo)
	{
		pPerThreadInfo = new PerThreadInfo(*this);
		(void) ::pthread_setspecific(m_key, reinterpret_cast<void *>(pPerThreadInfo));
	}
	
	pPerThreadInfo->m_pValue = pValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#else
#error Configure for this platform
#endif
	
// ---------------------------------------------------------------------------------------------------------------------------------

template <typename TYPE, typename DERIVED>
void ThreadSpecificDataBase<TYPE, DERIVED>::StaticDestructor(void *pData)
{
	if (0 != pData)
	{
		PerThreadInfo * const pPerThreadInfo = reinterpret_cast<PerThreadInfo *>(pData);
		DERIVED &derived = static_cast<DERIVED &>(pPerThreadInfo->m_threadSpecificData);
	
		if (0 != pPerThreadInfo->m_pValue)
			derived.OnThreadExit(pPerThreadInfo->m_pValue);

		delete pPerThreadInfo;
	}
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_THREAD_SPECIFIC_DATA_DETAIL_H)
