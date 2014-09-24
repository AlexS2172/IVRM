#ifndef ___THREAD_POOL_H___
#define ___THREAD_POOL_H___

#include <EgStdLib\EgStdLibCommon.h>
#include <EgStdLib\Lock.h>

/***********************************************************************************/
#define MAX_THREADS_IN_POOL    128
#define STOP_TIMEOUT           (DWORD)60*1000 //msec

/***********************************************************************************/
struct EGSTD_EXT_CLASS THREAD_PARAM
{
    PTHREAD_START_ROUTINE psr;
    PVOID lpp;    
};

typedef struct THREAD_PARAM* PTHREAD_PARAM;

/***********************************************************************************/
struct EGSTD_EXT_CLASS THREAD_STATE
{
    THREAD_STATE() : 
        hThread(INVALID_HANDLE_VALUE),
        hEvent(INVALID_HANDLE_VALUE) 
    {
    }    
    HANDLE hThread;
    HANDLE hEvent;
};

typedef THREAD_STATE* PTHREAD_STATE;

/********************************* CThreadPool class *******************************/
namespace EgStd
{
class EGSTD_EXT_CLASS CThreadPool
{
    typedef vector<THREAD_STATE> TThreadsVec;    
    typedef queue<THREAD_PARAM> TThreadParamsQueue;

public:
    CThreadPool();
    virtual ~CThreadPool();

    static BOOL QueueUserWorkItem(PTHREAD_START_ROUTINE pfnCallback, 
                                  PVOID pvContext, 
                                  ULONG dwFlags);
    
private:    
    static UINT WINAPI _Thread(LPVOID pParam);

    TThreadsVec         m_ThreadsVec;
    TThreadParamsQueue  m_Queue;
    CLock               m_Lock;
    HANDLE              m_hStop;
};

/***********************************************************************************/
extern CThreadPool TPool;

}

#endif //___THREAD_POOL_H___