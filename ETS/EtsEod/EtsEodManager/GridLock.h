#pragma once

class CGridLock
{
private:
	friend class CGridAutoLock;

public:
	CGridLock(IVSFlexGridPtr spFg)
		: m_nLockCount(0L)
	{
		ASSERT(spFg != NULL);
		m_spFg = spFg;
	}

	virtual ~CGridLock(void)
	{
		m_spFg->PutRedraw(flexRDBuffered);
	}

private:
	void _Lock()
	{
		if(0L == m_nLockCount)
			m_spFg->PutRedraw(flexRDNone);
		m_nLockCount++;
	}

	void _Unlock()
	{
		ASSERT(m_nLockCount);
		m_nLockCount--;
		if(0L == m_nLockCount)
			m_spFg->PutRedraw(flexRDBuffered);
	}

private:
	IVSFlexGridPtr		m_spFg;
	long				m_nLockCount;
};

class CGridAutoLock
{
public:
	CGridAutoLock(CGridLock* pLock)
		: m_pLock(pLock)
	{
		ASSERT(m_pLock);
		m_pLock->_Lock();
	}

	virtual ~CGridAutoLock()
	{
		if(m_pLock)
			m_pLock->_Unlock();
	}

private:
	CGridLock*		m_pLock;
};