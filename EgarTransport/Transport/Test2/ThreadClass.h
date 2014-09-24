#ifndef __THREAD_CLASS_H__
#define __THREAD_CLASS_H__

#include <windows.h>
#include "Lock.h"

class CThreadClass
{
public:

	CThreadClass():m_hThread(NULL), m_hStopEvent(NULL){}

	virtual bool Initialize()
	{
		if(m_hStopEvent)
			return true;

		m_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		if (m_hStopEvent == NULL)
			return false;

		return true;
	};
	
	virtual void Uninitialize()
	{
		if (m_hThread)
			Stop();

		if(m_hStopEvent)
		{
			CloseHandle(m_hStopEvent);
			m_hStopEvent = NULL;
		}
	};	

	virtual bool Start()
	{
		if (m_hStopEvent == NULL)
			return false;

		if (m_hThread)
			return true;
	
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThreadClass::ThreadFunction, (void*)this, 0, NULL);

		if (m_hThread == NULL)
			return false;

		return true;
	}

	virtual bool Stop()
	{
		if (!m_hThread || !m_hStopEvent)
			return true;
	
		SetEvent(m_hStopEvent);

		if (WaitForSingleObject(m_hThread, 100000 /*100 sec*/) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hThread, 0);			
		}

		CloseHandle(m_hThread);
		m_hThread = NULL;
	
		return true;
	}

	virtual void Do() = 0;

protected:

	static unsigned int __stdcall ThreadFunction(void* pVoid)
	{
		CThreadClass*		pThis = (CThreadClass*)pVoid;
		pThis->Do();
		return 0;
	}

	HANDLE			m_hThread;
	HANDLE			m_hStopEvent;
	CLock	m_Lock;
};

#endif //__THREAD_CLASS_H__