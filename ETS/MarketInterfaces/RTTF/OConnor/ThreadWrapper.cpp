#include "stdafx.h"

#pragma hdrstop

#include "threadwrapper.h"

DWORD __stdcall CThreadWrapper::Start(void* pParam)
{
	m_pParam = pParam;

	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hStopEvent)
	{
		return GetLastError();
	}

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

UINT __stdcall CThreadWrapper::ThreadProc( void *pThisClass)
{
	return ((CThreadWrapper*)pThisClass)->Thread(((CThreadWrapper*)pThisClass)->m_pParam);
}

DWORD __stdcall CThreadWrapper::Stop()
{
	if (NULL == m_hStopEvent)
		return ERROR_SERVICE_NOT_ACTIVE;

	SetEvent(m_hStopEvent);

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	CloseHandle(m_hStopEvent);

	return 0;
}

BOOL __stdcall CThreadWrapper::IsStopping()
{
	return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hStopEvent, 0));
}

