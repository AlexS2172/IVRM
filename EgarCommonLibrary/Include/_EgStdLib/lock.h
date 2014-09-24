#ifndef __EGSTDLIB_LOCK_H__
#define __EGSTDLIB_LOCK_H__

#include <EgStdLib\EgStdLibCommon.h>

namespace EgStd
{

class EGSTD_EXT_CLASS CLock
{
public:
	CLock()				
	{ 
		InitializeCriticalSection(&m_CS); 
	}
	
	virtual ~CLock()	
	{ 
		DeleteCriticalSection(&m_CS); 
	}

	void Lock()			
	{ 
		EnterCriticalSection(&m_CS); 
	}
	
	void Unlock()		
	{ 
		LeaveCriticalSection(&m_CS); 
	}

private:
	CRITICAL_SECTION m_CS;
};

class EGSTD_EXT_CLASS CAutoLock
{
public:

	CAutoLock(CLock* pLock)
	{
		m_pLock = pLock;
		m_pLock->Lock();
	}

	virtual ~CAutoLock() 
	{ 
		m_pLock->Unlock(); 
	}

private:
	CLock*		m_pLock;
};

}


#endif //__EGSTDLIB_LOCK_H__