#ifndef __EGSTDLIB_THREAD_H__
#define __EGSTDLIB_THREAD_H__

#include <EgStdLib\EgStdLibCommon.h>
#include <EgStdLib\Lock.h>

namespace EgStd
{

class EGSTD_EXT_CLASS CThread
{
public:
	CThread() : m_hThread(NULL),m_hStopEvent(NULL)
	{
	}
	DWORD Start(void* pParam);
	DWORD Stop(DWORD dwWait = INFINITE);
	
	UINT  virtual Thread(void* pParam) = 0;

private:
	static UINT __stdcall ThreadProc(void* pv);

protected:
	const HANDLE	GetThreadHandle() const 
	{
		return m_hThread;
	}
	
	const HANDLE	GetStopEventHandle() const 
	{
		return m_hStopEvent;
	}
	
	BOOL			IsStopping();

protected:
	void*		m_pParam;

private:
	HANDLE		m_hThread;
	HANDLE		m_hStopEvent;
	CLock		m_LockControl;
	
};

} // namespace EgStd

#endif //__EG_THREADING_H__