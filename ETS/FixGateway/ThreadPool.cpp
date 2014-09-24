#include "stdafx.h"
#include "ThreadPool.h"

namespace FXGCO
{
///////////////////////////////////////////////////////////////////////////////////////
CThreadPool g_TPool;

///////////////////////////////////////////////////////////////////////////////////////
CThreadPool::CThreadPool()
{
    m_hStop = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}
///////////////////////////////////////////////////////////////////////////////////////
CThreadPool::~CThreadPool()
{
    HANDLE  hEvents[c_nMaxThreadsInPool] = {INVALID_HANDLE_VALUE};
    ATLASSERT(m_ThreadsVec.size() <= c_nMaxThreadsInPool);

    int nEvents = 0;    
    for(size_t i =0; i < m_ThreadsVec.size(); i++ )
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
        ::WaitForMultipleObjects(nEvents, hEvents, TRUE, c_nStopTimeout);

        for(size_t i =0; i < m_ThreadsVec.size(); i++ )
        {
            const CThreadState& st = m_ThreadsVec[i];
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
    CThreadParam TParam;

    TParam.lpp = pvContext;
    TParam.psr = pfnCallback;

    g_TPool.m_Lock.Lock();

    g_TPool.m_Queue.push(TParam);
    ATLTRACE2(_T("TParam pushed, queue size is %d\n"), g_TPool.m_Queue.size() );

    if (!g_TPool.m_ThreadsVec.size())
    {    
        g_TPool.m_ThreadsVec.resize(c_nMaxThreadsInPool, CThreadState());    
        ATLTRACE2(_T("TThreadsVec resized, size is %d\n"), g_TPool.m_ThreadsVec.size());
    }

    BOOL bSucceeded = FALSE;

    for (int i =0; i< c_nMaxThreadsInPool; i++)
    {
        CThreadState& TState = g_TPool.m_ThreadsVec[i];
        
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
    
    g_TPool.m_Lock.Unlock();

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
        g_TPool.m_hStop, 
        hEvent 
    };

    while(::WaitForMultipleObjects(2, hEvents, FALSE, INFINITE) != WAIT_OBJECT_0)
    {
        g_TPool.m_Lock.Lock();
        CThreadParam TParam = g_TPool.m_Queue.front();
        g_TPool.m_Queue.pop();
        
        ATLTRACE2(_T("TParam poped, queue size is %d\n"), g_TPool.m_Queue.size() );

        g_TPool.m_Lock.Unlock();        
        
        PTHREAD_START_ROUTINE pfnCallback = TParam.psr;
        pfnCallback(TParam.lpp);

        ::ResetEvent(hEvent);
    } 
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////

}