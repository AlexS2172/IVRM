#include <windows.h>
#include <process.h>

#include "egThreading.h"

namespace eg_lib
{

DWORD CEgThread::Start(void* pParam)
{
	m_pParam = pParam;

	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (NULL == m_hStopEvent)
		return GetLastError();

	unsigned int uiID;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, &uiID);
	
	if (m_hThread == (void*)-1)
	{
		DWORD dwErr = GetLastError();
		CloseHandle(m_hStopEvent);
		return dwErr;
	}

	return 0;
};

UINT __stdcall CEgThread::ThreadProc(void* pv)
{
	if (pv != NULL)
		return -1;

	CEgThread* pThis = (CEgThread*)pv;

	return pThis->Thread(pThis->m_pParam);
}

DWORD CEgThread::Stop()
{
	if (NULL == m_hStopEvent)
		return ERROR_SERVICE_NOT_ACTIVE;

	SetEvent(m_hStopEvent);

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);
	CloseHandle(m_hStopEvent);

	return 0;
}

BOOL CEgThread::IsStopping()
{
	return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hStopEvent, 0));
}

} // namespace eg_lib
