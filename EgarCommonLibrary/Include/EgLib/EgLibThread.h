//**************************************************************************************************************//
// abstract:	interface & implementation of thread wrapper class
//
// content:		EgLib::CThread
//
// created:		27-Mar-2003 13:18 by Suchkov Dmitry
// copyright:	(c) 2003 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//
#if !defined(__EGLIBDBG_H__)
	#error 'EgLibThread.h requires EgLibDbg.h to be included first'
#endif

#if !defined(_MT)
	#error 'EgLibThread.h requires multithread model to be defined'
#endif

#include <TCHAR.H>
//**************************************************************************************************************//
// header identifier
//**************************************************************************************************************//
#define __EGLIBTHREAD_H__

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include <process.h>

//**************************************************************************************************************//
// namespace EgLib
//**************************************************************************************************************//
namespace EgLib {

//**************************************************************************************************************//
// Thread Pool Helpers
//**************************************************************************************************************//

	template < typename _baseAdaptor >
	class CEgTaskBase
	{
	public:
		CEgTaskBase(_baseAdaptor* pAdaptor)
		      :m_pAdaptor(pAdaptor)
			  ,m_bHasToBeDeleted(true){};

		  CEgTaskBase(const CEgTaskBase<_baseAdaptor>& cp)
		      :m_pAdaptor(cp.m_pAdaptor) 
			  ,m_bHasToBeDeleted(cp.m_bHasToBeDeleted){};

		  virtual ~CEgTaskBase(void){};
		  virtual void DoTask(LPVOID, LPOVERLAPPED) = 0;  

		  bool IsDelete(){return m_bHasToBeDeleted;}
		  void SetDelete(bool bMode=true){m_bHasToBeDeleted = bMode;}

	private:
		CEgTaskBase(){};
	protected:
		_baseAdaptor*	   m_pAdaptor;
		bool               m_bHasToBeDeleted;
	};

	//-------------------------------------------------------------------------------------------------------
	template <typename _baseAdaptor>
	class CEgThreadPool
	{
	public:
		typedef DWORD_PTR RequestType;

		CEgThreadPool(void):m_hEventStop(INVALID_HANDLE_VALUE){};
		virtual ~CEgThreadPool(void){};

		virtual BOOL Initialize(void *pvParam)
		{
			m_hEventStop     = reinterpret_cast<HANDLE>(pvParam);
			return SUCCEEDED(::CoInitializeEx(NULL, COINIT_MULTITHREADED))?TRUE:FALSE;
		}

		void  SetStopEvent(HANDLE hEvent){m_hEventStop = hEvent;}
		virtual void Terminate(void* /*pvParam*/)
		{
			::CoUninitialize();
		}

		void Execute(RequestType dw, void *pvParam, OVERLAPPED* pOverlapped) throw()
		{
			ATLASSERT(pvParam != NULL);
			CEgTaskBase<_baseAdaptor>* pTask = reinterpret_cast<CEgTaskBase<_baseAdaptor>*>(dw);
			try
			{
				pTask->SetDelete();
				if(m_hEventStop==INVALID_HANDLE_VALUE || ::WaitForSingleObject(m_hEventStop,0)==WAIT_TIMEOUT)
					pTask->DoTask(pvParam, pOverlapped);
				else
					ATLTRACE(_T("!!!! Ignoring pool data"));

			}
			catch (...){ATLASSERT(FALSE);}
			if(pTask->IsDelete())
				delete pTask;
		}
	private:
		HANDLE        m_hEventStop; 
	};

//**************************************************************************************************************//
// class CThread
//**************************************************************************************************************//
class CThread
{
// Data members

private:

	UINT   m_uiThreadID;
	HANDLE m_hThread;
	HANDLE m_hEventStop;

// Constructor/Destructor

public:

	CThread()
	:	m_hThread(NULL),
		m_hEventStop(NULL),
		m_uiThreadID(0)
	{
	}

	~CThread()
	{
#if defined(_DEBUG)
		if (IsRunning()) 
		{
			::MessageBox(NULL, _T("Thread is running."), _T("Logic error"), MB_ICONERROR);
			__asm int 3;
		}
#endif //defined(_DEBUG)
		if (NULL != m_hThread) CloseThreadHandle();
		if (NULL != m_hEventStop) ::CloseHandle(m_hEventStop);
	}

// Attributes

	bool IsRunning() const
	{
		DWORD dwExitCode;
		return (NULL != m_hThread && ::GetExitCodeThread(m_hThread, &dwExitCode) && STILL_ACTIVE == dwExitCode);
	}

	bool IsStopping() const
	{
		return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hEventStop, 0));
	}

	HANDLE GetStopEventHandle() const
	{
		return m_hEventStop;
	}

	HANDLE GetThreadHandle() const
	{
		return m_hThread;
	}

	UINT GetThreadID() const
	{
		return m_uiThreadID;
	}
// Operations

	DWORD Start(int iThreadPriority = THREAD_PRIORITY_NORMAL, PSECURITY_ATTRIBUTES pSa = NULL, DWORD cbStack = 0, DWORD dwFlags = 0, PUINT pThreadId = NULL)
	{
		if (IsRunning()) return ERROR_SERVICE_ALREADY_RUNNING;

		if (NULL != m_hEventStop) 
		{
			CloseHandle(m_hEventStop);
			m_hEventStop = NULL;
		}
		m_hEventStop = ::CreateEvent(pSa, TRUE, FALSE, NULL);
     
		if (NULL == m_hEventStop) return ::GetLastError();
		if (NULL != m_hThread && FALSE == CloseThreadHandle()) return ::GetLastError();

		m_hThread = (HANDLE)_beginthreadex(pSa, cbStack, ThreadProc, this, dwFlags, &m_uiThreadID);
		if (NULL == m_hThread) return ::GetLastError();
		if(pThreadId)
			*pThreadId = m_uiThreadID;
		if(iThreadPriority != THREAD_PRIORITY_NORMAL)
			SetPriority(iThreadPriority);

		return ERROR_SUCCESS;
	}

	DWORD Stop(bool bWait = true, long lWaitTimeout = INFINITE)
	{
		if (!IsRunning()) return ERROR_SERVICE_NOT_ACTIVE;
		if (IsStopping()) return S_OK;

		while (DWORD dwLocks = ResumeThread() > 0) if (-1 == dwLocks) return ::GetLastError();
		
		::SetEvent(m_hEventStop);
		if(bWait)
		{
			DWORD dwResult =  ::WaitForSingleObject(m_hThread, lWaitTimeout);
			if (WAIT_OBJECT_0 != dwResult && WAIT_TIMEOUT != dwResult ) return ::GetLastError();
			if( WAIT_TIMEOUT == dwResult ) 		
				::TerminateThread(m_hThread, -1);

			if (FALSE == CloseThreadHandle()) return ::GetLastError();
		}
		return ERROR_SUCCESS;
	}

	DWORD SuspendThread()
	{
		EGLIBASSERT(NULL != m_hThread);
		return ::SuspendThread(m_hThread);
	}

	DWORD ResumeThread()
	{
		EGLIBASSERT(NULL != m_hThread);
		return ::ResumeThread(m_hThread);
	}

	BOOL CloseThreadHandle()
	{
		EGLIBASSERT(NULL != m_hThread);
		BOOL bRet = ::CloseHandle(m_hThread);
		m_hThread = NULL;
		return TRUE;
	}

	int GetPriority() 
	{
		return ::GetThreadPriority(m_hThread);
	}
	void SetPriority(int nPriority)
	{
		::SetThreadPriority(m_hThread, nPriority);
	}

// Overrides

protected:

	virtual UINT Run() = 0;
	
// Implementation

private:

	static UINT __stdcall ThreadProc(PVOID pThis)
	{
		return static_cast<CThread*>(pThis)->Run();
	}

	//No copies do not implement
	CThread(const CThread& rhs);
	CThread& operator=(const CThread& rhs);
};

} //namespace EgLib
