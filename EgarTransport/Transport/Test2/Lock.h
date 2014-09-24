#ifndef LOCK_H
#define LOCK_H

class CLock
{
	CRITICAL_SECTION m_CS;

public:

	WINAPI CLock()
	{
		InitializeCriticalSection(&m_CS);
	}

	virtual WINAPI ~CLock()
	{
		DeleteCriticalSection(&m_CS);
	}

	void WINAPI Lock()
	{
		EnterCriticalSection(&m_CS);
	}

	void WINAPI Unlock()
	{
		LeaveCriticalSection(&m_CS);
	}

	BOOL WINAPI TryLock()
	{
		return TryEnterCriticalSection(&m_CS);
	}
};

class CReentry
{
	CLock	* m_pLock;

public:

	WINAPI CReentry(CLock * pLock)
	{
		m_pLock = pLock;
		m_pLock->Lock();
	}

	virtual WINAPI ~CReentry()
	{
		m_pLock->Unlock();
	}
};

#endif