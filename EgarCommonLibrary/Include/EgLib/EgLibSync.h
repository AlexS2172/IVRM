//**************************************************************************************************************//
// abstract:	interface & implementation of synchronization wrapper classes
//
// content:		EgLib::CLock
//				EgLib::CAutoLock
//				EgLib::CNamedLock
//				EgLib::CAutoNamedLock
//				EgLib::CEventT<t_bManaged>
//				EgLib::CEvent
//				EgLib::CEventHandle
//				EgLib::CMutex<t_bManaged>
//				EgLib::CMutex
//				EgLib::CMutexHandle
//				EgLib::CUniqueInstance
//				EgLib::CInterlockedBool
//				EgLib::CSingleWriterMultipleReaderGuard (Single Writers/Multiple Reader Guard)
//
// copyright:	(c) 2003 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once
#include <atlbase.h>
//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//
#if !defined(__EGLIBDBG_H__)
	#error 'EgLibSync.h requires EgLibDbg.h to be included first'
#endif


//**************************************************************************************************************//
// header identifier
//**************************************************************************************************************//
#define __EGLIBSYNC_H__
//**************************************************************************************************************//
// namespace EgLib
//**************************************************************************************************************//
namespace EgLib {


	inline DWORD WaitWithEvents(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE, BOOL bWaitAll = FALSE )
	{
		DWORD dwStatus = -1;
		ATLASSERT(nCount > 0);
		DWORD dwStart = GetTickCount();
		DWORD dwTimeElapsed = 0L;
		DWORD dwWaitTime = dwMilliseconds;

		ATLTRACE(_T("WaitWithEvent Enter\n"));
		for(;;)
		{
			dwStatus = ::MsgWaitForMultipleObjects(nCount, pHandles, bWaitAll, dwWaitTime, QS_ALLINPUT);
			if(dwStatus != WAIT_OBJECT_0 + nCount) break;

			MSG msg;
			dwStatus = WAIT_TIMEOUT;
			while(dwStatus == WAIT_TIMEOUT && PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
			{TranslateMessage(&msg);
				DispatchMessage(&msg);
				dwStatus = ::WaitForMultipleObjects(nCount, pHandles, FALSE, 0);
				if (dwMilliseconds != INFINITE && dwStatus == WAIT_TIMEOUT)
				{
					dwTimeElapsed = GetTickCount() - dwStart;
					if(dwTimeElapsed >= dwMilliseconds)
						break;
				}

			}
			if (dwStatus != WAIT_TIMEOUT)
				break;

			if (dwMilliseconds != INFINITE)
			{
				dwTimeElapsed = GetTickCount() - dwStart;
				if(dwTimeElapsed >= dwMilliseconds)
					break;
				else
					dwWaitTime = dwMilliseconds-dwTimeElapsed;
			}
		}
		ATLTRACE(_T("WaitWithEvent Exit\n"));
		return dwStatus;		
	}
	inline DWORD WaitWithEvents(HANDLE Handle, DWORD dwMilliseconds = INFINITE)
	{
		HANDLE hEvents[] = {Handle};
		return WaitWithEvents(1, hEvents, dwMilliseconds);
	}

	class CWaitWithEventsObject
	{
	public:
		static DWORD Wait(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE)
		{
			return WaitWithEvents(nCount, pHandles,dwMilliseconds);
		}

		static DWORD Wait(HANDLE Handle, DWORD dwMilliseconds = INFINITE)
		{
			return WaitWithEvents(Handle, dwMilliseconds);
		}
	};

//**************************************************************************************************************//
// class CLock
//**************************************************************************************************************//
class CLock
{
public:

// Data members

	CRITICAL_SECTION m_cs;

// Constructor/Destructor

	CLock()
	{
		::InitializeCriticalSection(&m_cs);
	}

	~CLock()
	{
		::DeleteCriticalSection(&m_cs);
	}

// Operations

	void Lock()
	{
		::EnterCriticalSection(&m_cs);
	}

	void Unlock()
	{
		::LeaveCriticalSection(&m_cs);
	}

#if(_WIN32_WINNT >= 0x0400)
	BOOL TryToLock()
	{
		return ::TryEnterCriticalSection(&m_cs);
	}
#endif

private:
	//No copies do not implement
	CLock(const CLock& rhs);
	CLock& operator=(const CLock& rhs);
};



//**************************************************************************************************************//
// class CAutoLock
//**************************************************************************************************************//
class CAutoLock
{
public:

// Data members

	CLock& m_rLock;

	CAutoLock(CLock& rLock)
		: m_rLock(rLock)
	{
		m_rLock.Lock();
	}

	~CAutoLock()
	{
		m_rLock.Unlock();
	}

private:

	//No copies do not implement
	CAutoLock(const CAutoLock& rhs);
	CAutoLock& operator=(const CAutoLock& rhs);
};

//**************************************************************************************************************//
// class CAutoTryLock
//**************************************************************************************************************//
class CAutoTryLock
{
public:

	// Data members

	CLock& m_rLock;

	CAutoTryLock(CLock& rLock)
		: m_rLock(rLock)
	{
		m_bLocked = m_rLock.TryToLock();
	}
	BOOL IsLocked() {return m_bLocked;}

	~CAutoTryLock()
	{
		if(m_bLocked)
			m_rLock.Unlock();
	}

private:

	//No copies do not implement
	CAutoTryLock(const CAutoTryLock& rhs);
	CAutoTryLock& operator=(const CAutoTryLock& rhs);
	BOOL m_bLocked;
};

//************************************************************************
// CHandle used in CNamedLock not available under VC 6.0
//************************************************************************
#if _ATL_VER < 0x710

class CHandle
{
public:
	CHandle() throw();
	CHandle( CHandle& h ) throw();
	explicit CHandle( HANDLE h ) throw();
	~CHandle() throw();

	CHandle& operator=( CHandle& h ) throw();

	operator HANDLE() const throw();

	// Attach to an existing handle (takes ownership).
	void Attach( HANDLE h ) throw();
	// Detach the handle from the object (releases ownership).
	HANDLE Detach() throw();

	// Close the handle.
	void Close() throw();

public:
	HANDLE m_h;
};

inline CHandle::CHandle() throw() :
m_h( NULL )
{
}

inline CHandle::CHandle( CHandle& h ) throw() :
m_h( NULL )
{
	Attach( h.Detach() );
}

inline CHandle::CHandle( HANDLE h ) throw() :
m_h( h )
{
}

inline CHandle::~CHandle() throw()
{
	if( m_h != NULL )
	{
		Close();
	}
}

inline CHandle& CHandle::operator=( CHandle& h ) throw()
{
	if( this != &h )
	{
		if( m_h != NULL )
		{
			Close();
		}
		Attach( h.Detach() );
	}

	return( *this );
}

inline CHandle::operator HANDLE() const throw()
{
	return( m_h );
}

inline void CHandle::Attach( HANDLE h ) throw()
{
	ATLASSERT( m_h == NULL );
	m_h = h;  // Take ownership
}

inline HANDLE CHandle::Detach() throw()
{
	HANDLE h;

	h = m_h;  // Release ownership
	m_h = NULL;

	return( h );
}

inline void CHandle::Close() throw()
{
	if( m_h != NULL )
	{
		::CloseHandle( m_h );
		m_h = NULL;
	}
}

#endif

//**************************************************************************************************************//
// class CNamedLock
//**************************************************************************************************************//
class CNamedLock
{
private:

// Data members

	CHandle m_hMutex;
public:
// Constructor/Destructor

	CNamedLock(PCTSTR pszName)
		:m_hMutex(::CreateMutex(NULL, FALSE, pszName))
		,m_bLocked(FALSE)
		{
		}

	~CNamedLock(){}
// Operations

	BOOL Lock(DWORD dwTimeout = INFINITE)
	{
		EGLIBASSERT(m_hMutex != NULL);
		DWORD dw = ::WaitForSingleObject(m_hMutex, dwTimeout);
		m_bLocked = (WAIT_OBJECT_0 == dw)?TRUE:FALSE;

		return TRUE;
	}

	BOOL Unlock()
	{
		EGLIBASSERT(m_hMutex != NULL);
		m_bLocked = false;
		return ::ReleaseMutex(m_hMutex);
	}

	void SetLockName(PCTSTR pszName)
	{
		m_hMutex.Close();
		m_hMutex.Attach(::CreateMutex(NULL, FALSE, pszName));
	}

	BOOL IsLocked(){return m_bLocked;}

private:
	BOOL m_bLocked;

	//No copies do not implement
	CNamedLock(const CNamedLock& rhs);
	CNamedLock& operator=(const CNamedLock& rhs);
};

//**************************************************************************************************************//
// class CAutoNamedLock
//**************************************************************************************************************//
class CAutoNamedLock
{
public:

// Data members

	CNamedLock& m_rNamedLock;

// Constructor/Destructor

	CAutoNamedLock(CNamedLock& rNamedLock, DWORD dwTimeout = INFINITE)
		:	m_rNamedLock(rNamedLock)
	{
        m_rNamedLock.Lock(dwTimeout);
	}

	~CAutoNamedLock()
	{
		m_rNamedLock.Unlock();
	}

private:

	//No copies do not implement
	CAutoNamedLock(const CAutoNamedLock& rhs);
	CAutoNamedLock& operator=(const CAutoNamedLock& rhs);
};

//**************************************************************************************************************//
// 
//**************************************************************************************************************//
template <bool t_bManaged>
class CSemaphoreT
{
	HANDLE m_hSemaphore;

public:
	CSemaphoreT(HANDLE hSemaphore = NULL) 
		: m_hSemaphore(hSemaphore)
	{
	}

	virtual ~CSemaphoreT()
	{
		Close();
	}

	bool CreateSemaphore(LPSECURITY_ATTRIBUTES lpsaAttributes = NULL,
		LONG lInitialCount = 1, 
		LONG lMaxCount = 1, 
		LPCTSTR pstrName = NULL)
	{
		m_hSemaphore = ::CreateSemaphore(lpsaAttributes, lInitialCount, lMaxCount, pstrName);
		return m_hSemaphore != NULL;
	}

	BOOL Close()
	{
		if (!t_bManaged)
			return TRUE;

		BOOL bRet = false;
		if (m_hSemaphore != NULL)
		{
			bRet = (::CloseHandle(m_hSemaphore) == S_OK);
			m_hSemaphore = NULL;
		}

		return bRet;
	}

	// Implementation
	virtual DWORD Wait(DWORD dwTimeout = INFINITE)
	{
		return ::WaitForSingleObject(m_hSemaphore, dwTimeout);
	}

	virtual BOOL Unlock()
	{
		return ::ReleaseSemaphore(m_hSemaphore, 1, NULL);
	}

	virtual BOOL Unlock(LONG lCount, LPLONG lprevCount = NULL)
	{
		return ::ReleaseSemaphore(m_hSemaphore, lCount, lprevCount);
	}

	bool IsNull() const
	{
		return (NULL == m_hSemaphore);
	}

	void Attach(HANDLE hSemaphore)
	{
		if (t_bManaged && NULL != m_hSemaphore) 
			::CloseHandle(m_hSemaphore);
		m_hSemaphore = hSemaphore;
	}

	HANDLE Detach()
	{
		HANDLE hSemaphore = m_hSemaphore;
		m_hSemaphore = NULL;
		return hSemaphore;
	}

	CSemaphoreT<t_bManaged> operator=(HANDLE hSemaphore)
	{
		m_hSemaphore = hSemaphore;
		return *this;
	}
	operator HANDLE() const
	{
		return m_hSemaphore;
	}

private:
	//No copies do not implement
	CSemaphoreT(const CSemaphoreT& rhs);
	CSemaphoreT& operator=(const CSemaphoreT& rhs);
};

typedef CSemaphoreT<true> CSemaphore;
typedef CSemaphoreT<false> CSemaphoreHandle;

//**************************************************************************************************************//
// class CEventT
//**************************************************************************************************************//
template<bool t_bManaged>
class CEventT
{
public:

// Data members

	HANDLE m_hEvent;

// Constructor/Destructor

	CEventT(HANDLE hEvent = NULL)
	:	m_hEvent(hEvent)
	{
	}

	~CEventT()
	{
		if (t_bManaged && NULL != m_hEvent) CloseHandle();
	}

// Operators

	CEventT<t_bManaged> operator=(HANDLE hEvent)
	{
		m_hEvent = hEvent;
		return *this;
	}

	DWORD Wait(DWORD dwTimeout =INFINITE)
	{
		return ::WaitForSingleObject(m_hEvent, dwTimeout);
	}

	operator HANDLE() const
	{
		return m_hEvent;
	}

// Attributes

	bool IsNull() const
	{
		return (NULL == m_hEvent);
	}

// Operations

	void Attach(HANDLE hEvent)
	{
		if (t_bManaged && NULL != m_hEvent) ::CloseHandle(m_hEvent);
		m_hEvent = hEvent;
	}

	HANDLE Detach()
	{
		HANDLE hEvent = m_hEvent;
		m_hEvent = NULL;
		return hEvent;
	}

	HANDLE CreateEvent(
		PSECURITY_ATTRIBUTES pSa, 
		BOOL bManualReset, 
		BOOL bInitialState, 
		LPCTSTR pszName = NULL)
	{
		EGLIBASSERT(NULL == m_hEvent);
		m_hEvent = ::CreateEvent(pSa, bManualReset, bInitialState, pszName);
		return m_hEvent;
	}

	BOOL CloseHandle()
	{
		EGLIBASSERT(NULL != m_hEvent);
		BOOL bOk = ::CloseHandle(m_hEvent);
		if (TRUE == bOk) m_hEvent = NULL;
		return bOk;
	}

	BOOL PulseEvent()
	{
		EGLIBASSERT(NULL != m_hEvent);
		return ::PulseEvent(m_hEvent);
	}

	BOOL SetEvent()
	{
		EGLIBASSERT(NULL != m_hEvent);
		return ::SetEvent(m_hEvent);
	}

	BOOL ResetEvent()
	{
		EGLIBASSERT(NULL != m_hEvent);
		return ::ResetEvent(m_hEvent);
	}
};

typedef CEventT<true> CEvent;
typedef CEventT<false> CEventHandle;

//**************************************************************************************************************//
// class CMutexT
//**************************************************************************************************************//
template<bool t_bManaged>
class CMutexT
{
public:

// Data members

	HANDLE m_hMutex;

// Constructor/Destructor

	CMutexT(HANDLE hMutex = NULL)
		:	m_hMutex(hMutex)
	{
	}

	~CMutexT()
	{
		if (t_bManaged && NULL != m_hMutex) ::CloseHandle(m_hMutex);
	}

// Operators

	CEventT<t_bManaged> operator=(HANDLE hMutex)
	{
		m_hMutex = hMutex;
		return *this;
	}

	operator HANDLE() const
	{
		return m_hMutex;
	}

// Attributes

	bool IsNull() const
	{
		return (NULL == m_hMutex);
	}

// Operations

	void Attach(HANDLE hMutex)
	{
		if (t_bManaged && NULL != m_hMutex) ::CloseHandle(m_hMutex);
		m_hMutex = hMutex;
	}

	HANDLE Detach()
	{
		HANDLE hMutex;
		hMutex = m_hMutex;
		m_hMutex = NULL;
		return hMutex;
	}

	HANDLE CreateMutex(PSECURITY_ATTRIBUTES pSa, BOOL bInitialOwner, PCTSTR pszName)
	{
		EGLIBASSERT(NULL == m_hMutex);
		m_hMutex = ::CreateMutex(pSa, bInitialOwner, pszName);
        return m_hMutex;
	}

	HANDLE OpenMutex(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName)
	{
		EGLIBASSERT(NULL == m_hMutex);
		m_hMutex = ::OpenMutex(dwDesiredAccess, bInheritHandle, pszName);
		return m_hMutex;
	}

	BOOL ReleaseMutex()
	{
		EGLIBASSERT(NULL != m_hMutex);
		return ::ReleaseMutex(m_hMutex);
	}

	BOOL CloseHandle()
	{
		EGLIBASSERT(NULL != m_hMutex);
		BOOL bOk = ::CloseHandle(m_hMutex);
		if (TRUE == bOk) m_hMutex = NULL;
		return bOk;
	}

	DWORD Wait()
	{
		EGLIBASSERT(NULL != m_hMutex);
		return ::WaitForSingleObject(m_hMutex, INFINITE);
	}

private:
	//No copies do not implement
	CMutexT(const CMutexT& rhs);
	CMutexT& operator=(const CMutexT& rhs);
};

typedef CMutexT<true> CMutex;
typedef CMutexT<false> CMutexHandle;

//**************************************************************************************************************//
// class CUniqueInstance
//**************************************************************************************************************//
class CUniqueInstance
{
public:

// Data members

	HANDLE m_hMutex;

// Constructor/Destructor

	CUniqueInstance(PCTSTR pszName)
	{
		m_hMutex = ::CreateMutex(NULL, TRUE, pszName);
		EGLIBASSERT(NULL != m_hMutex);
	}

	~CUniqueInstance()
	{
		if (m_hMutex) ::CloseHandle(m_hMutex);
	}

// Attributes

	bool IsUnique()
	{
		return (NULL != m_hMutex && WAIT_OBJECT_0 == ::WaitForSingleObject(m_hMutex, 0));
	}

private:

	//No copies do not implement
	CUniqueInstance(const CUniqueInstance& rhs);
	CUniqueInstance& operator=(const CUniqueInstance& rhs);
};

//**************************************************************************************************************//
// class CInterlockedBool
//**************************************************************************************************************//
class CInterlockedBool
{
// Data members

private:

	mutable CLock	m_Lock;
    bool	m_bValue;

// Constructor/Destructor

public:

	CInterlockedBool(bool bValue = false)
		:	m_bValue(bValue)
	{
	}

	~CInterlockedBool()
	{
	}

// Operators

	bool operator==(bool bValue) const
	{
		CAutoLock Lock(m_Lock);
		return m_bValue == bValue;
	}

	bool operator!=(bool bValue) const
	{
		CAutoLock Lock(m_Lock);
		return m_bValue != bValue;
	}

	bool operator=(bool bValue)
	{
		CAutoLock Lock(m_Lock);
		m_bValue = bValue;
		return bValue;
	}

	operator bool() const
	{
		CAutoLock Lock(m_Lock);
		return m_bValue;
	}
};

//**************************************************************************************************************//
// class CSingleWriterMultipleReaderGuard (Single Writer Multiple Reader Guard)
//**************************************************************************************************************//
class CSingleWriterMultipleReaderGuard
{
	CLock	m_Lock;				// take single access to private members

	CSemaphore	m_hsemReaders;		// readers waiting for this semaphore if resource locked by writers
	CSemaphore	m_hsemWriters;		// writers waiting for this semaphore if resource locked by readers

	long	m_lWaitingReaders;	// # waiting readers
	long	m_lWaitingWriters;	// # waiting writers
	long	m_lActive;			// # threads work with resource
								//  0 - nobody 
								// >0 - # readers
								// -1 - one writer
public:
	// Constructor
	CSingleWriterMultipleReaderGuard() 
		: m_hsemReaders(NULL),
		  m_hsemWriters(NULL)
	{
		m_lWaitingReaders = m_lWaitingWriters = m_lActive = 0;
		m_hsemReaders.CreateSemaphore(NULL, 0, MAXLONG, NULL);
		m_hsemWriters.CreateSemaphore(NULL, 0, MAXLONG, NULL);
	}

	// Destructor
	~CSingleWriterMultipleReaderGuard()
	{
		#ifdef _DEBUG
			// somebody take ownership of resource
			if (m_lActive != 0)
				DebugBreak();
		#endif

		m_lWaitingReaders = m_lWaitingWriters = m_lActive = 0;
		m_hsemReaders.Close();
		m_hsemWriters.Close();
	}

	// 
	long GetWaitingReaders()
	{
		m_Lock.Lock();
		long l = m_lWaitingReaders;
		m_Lock.Unlock();
		return l;
	}

	long GetWaitingWriters()
	{
		m_Lock.Lock();
		long l = m_lWaitingWriters;
		m_Lock.Unlock();
		return l;
	}

	long GetActive()
	{
		//HACK: return -1
		// Этим хаком убиваем unsafe call в вызовах COrder::SetXXXX
		return -1;

		m_Lock.Lock();
		long l = m_lActive;
		m_Lock.Unlock();
		return l;
	}

	//- 
	void WaitToRead()
	{
		//HACK: Do implementation later
		return;

		// logic
		m_Lock.Lock();

		// some writer works with  resource OR 
		// some readers waiting for resource?
		bool bResourceWritePending = (m_lWaitingWriters || (m_lActive < 0));
		if (bResourceWritePending)
			m_lWaitingReaders++;	// THIS reader MUST WAITING too
		else
			m_lActive++;			// THIS reader CAN READ

		m_Lock.Unlock();

		if (bResourceWritePending)	// THIS thread MUST waiting
		{
			EGLIBASSERT(m_hsemReaders != NULL);
			m_hsemReaders.Wait();
		}
	}

	//-
	void WaitToWrite()
	{
		//HACK: Do implementation later
		return;

		// logic
		m_Lock.Lock();

		bool bResourceOwner = (m_lActive != 0);	// 0 - resource is free
		if (bResourceOwner)						// some thread works with resource?!
			m_lWaitingWriters++;				// yes, THIS writer MUST WAITING
		else
			m_lActive = -1;						// no, writer CAN WRITE

		m_Lock.Unlock();

		if (bResourceOwner)						// THIS thread MUST waiting
		{
			EGLIBASSERT(m_hsemWriters != NULL);
			m_hsemWriters.Wait();
		}
	}

	//-
	void Done()
	{
		//HACK: Do implementation later
		return;

		// logic
		m_Lock.Lock();

		if (m_lActive > 0)
			m_lActive--;	// ownership of resource controlled by readers
		else
			m_lActive++;	// ownership of resource controlled by writers

		CSemaphoreHandle SemHandle;
		long lCount = 1;

		if (m_lActive == 0)
		{
			if (m_lWaitingWriters > 0)
			{
				m_lActive = -1;
				m_lWaitingWriters--;
				//hSem = m_hsemWriters;
				SemHandle.Attach(m_hsemWriters);
			}
			else
			if (m_lWaitingReaders > 0)
			{
				m_lActive = m_lWaitingReaders;
				m_lWaitingReaders = 0;
				lCount = m_lActive;
				//hSem = m_hsemReaders;
				SemHandle.Attach(m_hsemReaders);
			}
			else
			{
				// nobody want take ownership of resource
			}
		}

		m_Lock.Unlock();

		if (!SemHandle.IsNull())
			SemHandle.Unlock(lCount);
	}

private:
	//No copies do not implement
	CSingleWriterMultipleReaderGuard(const CSingleWriterMultipleReaderGuard& rhs);
	CSingleWriterMultipleReaderGuard& operator=(const CSingleWriterMultipleReaderGuard& rhs);
};


class CSingleWriterMultipleReaderGuardAutoLock
{
public:

	// Data members
	CSingleWriterMultipleReaderGuard& m_rLock;

	typedef enum TypeLock {	LockRead, LockWrite };
	CSingleWriterMultipleReaderGuardAutoLock( CSingleWriterMultipleReaderGuard& rLock, TypeLock type )
		: m_rLock(rLock)
	{
		if(LockWrite == type)
			m_rLock.WaitToWrite();
		else
			m_rLock.WaitToRead();
	}

	~CSingleWriterMultipleReaderGuardAutoLock()
	{
		m_rLock.Done();
	}

private:

	//No copies do not implement
	CSingleWriterMultipleReaderGuardAutoLock(const CSingleWriterMultipleReaderGuardAutoLock& rhs);
	CSingleWriterMultipleReaderGuardAutoLock& operator=(const CSingleWriterMultipleReaderGuardAutoLock& rhs);
};

//**************************************************************************************************************//
// class CFairSingleWriterMultipleReaderGuard
//**************************************************************************************************************//
class CFairSingleWriterMultipleReaderGuard
{
	CLock		m_Lock;				// take single access to private members

	CSemaphore	m_AccessLock;
	CEvent		m_evtGate;

	long		m_lIsWriteLock;
	long		m_lWriterCount;
	long		m_lReaderCount;
	long		m_lReadersWaiting;

public:
// Constructor/Destructor
	CFairSingleWriterMultipleReaderGuard()
	{
		m_AccessLock.CreateSemaphore(NULL, 1, MAXLONG);
		//m_AccessLock.CreateMutex(NULL, FALSE, NULL);
		m_evtGate.CreateEvent(NULL, TRUE, FALSE);
		m_lIsWriteLock = m_lWriterCount = m_lReaderCount = m_lReadersWaiting = 0; 
	}

	~CFairSingleWriterMultipleReaderGuard()
	{
		//m_AccessLock.Close();
	}

	bool TryToRead()
	{
		bool bRet = false;
		CAutoLock lock(m_Lock);
		if (m_lWriterCount <=0)
		{
			// Do not need to wait writers
			m_lReaderCount++;
			bRet = true;
		}
		return bRet;

	}
	void WaitToRead()
	{
		m_Lock.Lock();

		// if there is at least one writer using the lock or waiting for it,
		// we need to wait for access
		if (m_lWriterCount > 0)
		{
			if (m_lReadersWaiting++ == 0)	// if We are the first reader in line
			{
				m_Lock.Unlock();

				m_AccessLock.Wait();			// get the access lock

				m_Lock.Lock();
				if (m_lReadersWaiting > 1)		// then if there are other readers
					m_evtGate.SetEvent();		// let them go
			}
			else 
			{
				m_Lock.Unlock();
				while(true)
				{
					if(WAIT_TIMEOUT != m_evtGate.Wait(200))	// otherwise wait until someone lets us go
						break;
					if(m_lReadersWaiting && !m_lWriterCount)
					{	// Reset semaphore
						m_AccessLock.Close();
						m_AccessLock.CreateSemaphore(NULL, 1, MAXLONG);
						m_evtGate.SetEvent();
					}
				}
				m_Lock.Lock();
			} 
			m_lReadersWaiting--;
		} // if 

		m_lReaderCount++;
		m_Lock.Unlock();
	}

	void WaitToWrite()
	{
		m_Lock.Lock();
		m_lWriterCount++;
		m_Lock.Unlock();
		m_AccessLock.Wait(2000);

		m_Lock.Lock();
		m_lIsWriteLock = 1;
		m_evtGate.ResetEvent();
		m_Lock.Unlock();
	}

	void Done()
	{
		CAutoLock lock(m_Lock);
		if (m_lIsWriteLock)
		{
			m_lIsWriteLock = 0;
			m_lWriterCount--;
			if(!m_lWriterCount)
				m_evtGate.SetEvent();

			m_AccessLock.Unlock();
		}
		else
		if (--m_lReaderCount == 0)
			m_AccessLock.Unlock();
	}

private:
	//No copies do not implement
	CFairSingleWriterMultipleReaderGuard(const CFairSingleWriterMultipleReaderGuard& rhs);
	CFairSingleWriterMultipleReaderGuard& operator=(const CFairSingleWriterMultipleReaderGuard& rhs);
};


//------------------------------------------------------------------
class CReaderAutoLock 
{
private:
	CFairSingleWriterMultipleReaderGuard& m_rLock;
public:
	CReaderAutoLock(CFairSingleWriterMultipleReaderGuard& rLock)
		: m_rLock(rLock)
	{
		m_rLock.WaitToRead();
	}
	~CReaderAutoLock()
	{
		m_rLock.Done();
	}
private:
	//No copies do not implement
	CReaderAutoLock(const CReaderAutoLock& rhs);
	CReaderAutoLock& operator=(const CReaderAutoLock& rhs);
};
//------------------------------------------------------------------
class CReaderAutoTryLock 
{
private:
	CFairSingleWriterMultipleReaderGuard& m_rLock;
public:
	CReaderAutoTryLock(CFairSingleWriterMultipleReaderGuard& rLock)
		: m_rLock(rLock)
	{
		m_bIsLocked = m_rLock.TryToRead();
	}
	~CReaderAutoTryLock()
	{
		if(m_bIsLocked)
			m_rLock.Done();
	}
	bool IsLocked(){ return m_bIsLocked; }

private:
	//No copies do not implement
	CReaderAutoTryLock(const CReaderAutoTryLock& rhs);
	CReaderAutoTryLock& operator=(const CReaderAutoTryLock& rhs);

	bool m_bIsLocked;
};

//------------------------------------------------------------------
class CWriterAutoLock 
{
private:
	CFairSingleWriterMultipleReaderGuard& m_rLock;
public:
	CWriterAutoLock(CFairSingleWriterMultipleReaderGuard& rLock)
		: m_rLock(rLock)
	{
		m_rLock.WaitToWrite();
	}
	~CWriterAutoLock()
	{
		m_rLock.Done();
	}
private:
	//No copies do not implement
	CWriterAutoLock(const CWriterAutoLock& rhs);
	CWriterAutoLock& operator=(const CWriterAutoLock& rhs);
};


} //namespace EgLib