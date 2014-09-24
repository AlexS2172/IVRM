#ifndef ___THREAD_POOL_H___
#define ___THREAD_POOL_H___

/********************************* CThreadPool class *******************************/
namespace FXGCO
{
/***********************************************************************************/
const long c_nMaxThreadsInPool = 128L;
const long c_nStopTimeout = 60000L;

/***********************************************************************************/
struct CThreadParam
{
	PTHREAD_START_ROUTINE psr;
	PVOID lpp;    
};

/***********************************************************************************/
struct CThreadState
{
	CThreadState() : hThread(INVALID_HANDLE_VALUE), hEvent(INVALID_HANDLE_VALUE){}    
	
	HANDLE hThread;
	HANDLE hEvent;
};

/***********************************************************************************/
class  CThreadPool
{
    typedef vector<CThreadState> TThreadsVec;    
    typedef queue<CThreadParam> TThreadParamsQueue;

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
extern CThreadPool g_TPool;

}

#endif //___THREAD_POOL_H___