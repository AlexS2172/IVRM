#include "stdafx.h"
#include <EgStdLib\ThreadPool.h>

namespace EgStd
{
///////////////////////////////////////////////////////////////////////////////////////
CThreadPool TPool;

///////////////////////////////////////////////////////////////////////////////////////
CThreadPool::CThreadPool()
{
    m_hStop = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}
///////////////////////////////////////////////////////////////////////////////////////
CThreadPool::~CThreadPool()
{
    HANDLE  hEvents[MAX_THREADS_IN_POOL] = {INVALID_HANDLE_VALUE};
    ATLASSERT(m_ThreadsVec.size() <= MAX_THREADS_IN_POOL);

    int nEvents = 0;    
    for(int i =0; i < static_cast<int>(m_ThreadsVec.size()); i++ )
    {
        if (m_ThreadsVec[i].hThread != INVALID_HANDLE_VALUE)
        {   
            hEvents[i] = m_ThreadsVec[i].hThread;
            nEvents++;
        }
    }

    ::SetEvent(m_hStop);
    
    if ( nEvents > 0)
    {    
        ::WaitForMultipleObjects(nEvents, hEvents, TRUE, STOP_TIMEOUT);

        for(int i =0; i < static_cast<int>(m_ThreadsVec.size()); i++ )
        {
            const THREAD_STATE& st = m_ThreadsVec[i];
            if (st.hThread != INVALID_HANDLE_VALUE)
            {   
                ::CloseHandle(st.hThread);
                ::CloseHandle(st.hEvent);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadPool::QueueUserWorkItem(PTHREAD_START_ROUTINE pfnCallback, 
        PVOID pvContext, ULONG dwFlags)
{    
    UINT ID;
    THREAD_PARAM TParam;

    TParam.lpp = pvContext;
    TParam.psr = pfnCallback;

    TPool.m_Lock.Lock();

    TPool.m_Queue.push(TParam);
    ATLTRACE(_T("TParam pushed, queue size is %d\n"), TPool.m_Queue.size() );

    if (!TPool.m_ThreadsVec.size())
    {    
        TPool.m_ThreadsVec.resize(MAX_THREADS_IN_POOL, THREAD_STATE());    
        ATLTRACE(_T("TThreadsVec resized, size is %d\n"), TPool.m_ThreadsVec.size());
    }

    BOOL bSucceeded = FALSE;

    for (int i =0; i< MAX_THREADS_IN_POOL; i++)
    {
        THREAD_STATE& TState = TPool.m_ThreadsVec[i];
        
        if ( TState.hThread == INVALID_HANDLE_VALUE )
        {
            TState.hEvent = ::CreateEvent(0, TRUE, TRUE, 0);
            TState.hThread = (HANDLE)_beginthreadex(NULL, 0, _Thread, TState.hEvent, 0, &ID);            

            if (TState.hThread == INVALID_HANDLE_VALUE)
            {
                CloseHandle(TState.hEvent);
                TState.hEvent = INVALID_HANDLE_VALUE;
                break;
            }

            bSucceeded = TRUE;
            break;
        }
        else if ( ::WaitForSingleObject(TState.hEvent, 0) == WAIT_TIMEOUT )
        {        
            ::SetEvent(TState.hEvent);
            bSucceeded = TRUE;
            break;
        }
    }
    
    TPool.m_Lock.Unlock();

    if (!bSucceeded)
        ::SetLastError(ERROR_MAX_THRDS_REACHED);

    return bSucceeded;
}

///////////////////////////////////////////////////////////////////////////////////////
UINT WINAPI CThreadPool::_Thread(LPVOID pParam)
{
    DWORD   dwRes = ERROR_SUCCESS;    
    HANDLE  hCurrentThread = ::GetCurrentThread();
    HANDLE  hEvent = reinterpret_cast<HANDLE>(pParam);
    HANDLE  hEvents[] = 
    { 
        TPool.m_hStop, 
        hEvent 
    };

    while(::WaitForMultipleObjects(2, hEvents, FALSE, INFINITE) != WAIT_OBJECT_0)
    {
        TPool.m_Lock.Lock();
        THREAD_PARAM TParam = TPool.m_Queue.front();
        TPool.m_Queue.pop();
        
        ATLTRACE(_T("TParam poped, queue size is %d\n"), TPool.m_Queue.size() );

        TPool.m_Lock.Unlock();        
        
        PTHREAD_START_ROUTINE pfnCallback = TParam.psr;
        pfnCallback(TParam.lpp);

        ::ResetEvent(hEvent);
    } 
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////

}