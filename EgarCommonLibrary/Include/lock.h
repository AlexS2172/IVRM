#ifndef LOCK_H
#define LOCK_H

class CLock
{
	CRITICAL_SECTION m_CS;

public:

	APIENTRY CLock()
	{
		InitializeCriticalSection(&m_CS);
	}

	virtual APIENTRY ~CLock()
	{
		DeleteCriticalSection(&m_CS);
	}

	void APIENTRY Lock()
	{
		EnterCriticalSection(&m_CS);
	}

	void APIENTRY Unlock()
	{
		LeaveCriticalSection(&m_CS);
	}
#if _WIN32_WINNT >= 0x0400
	BOOL APIENTRY TryLock()
	{
		return TryEnterCriticalSection(&m_CS);
	}
#endif
};

class CAutoLock
{
	CLock	* m_pLock;

public:

	APIENTRY CAutoLock(CLock * pLock)
	{
		m_pLock = pLock;
		m_pLock->Lock();
	}

	virtual APIENTRY ~CAutoLock()
	{
		m_pLock->Unlock();
	}
};

#endif