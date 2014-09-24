// DoubleBuffer.h: interface for the CDoubleBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOUBLEBUFFER_H__E8CCE9A6_17F4_11D5_BEA9_00508B77148B__INCLUDED_)
#define AFX_DOUBLEBUFFER_H__E8CCE9A6_17F4_11D5_BEA9_00508B77148B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include <vector>
#include <set>


template< class _ValueType, long _GrowBy = 10L >
class CDoubleBuffer
{
public:
	//
	CDoubleBuffer()
	{
		m_nInIdx = 0;
		m_nOutIdx = 1;
		
		m_vec[0].reserve(_GrowBy);
		m_vec[1].reserve(_GrowBy);

		m_nSize[0] = 0;
		m_nSize[1] = 0;
	}
	
	//
	virtual ~CDoubleBuffer()
	{
		Reset(true);
	}

	//
	void Swap()
	{
		unsigned int nTemp = m_nInIdx;
		m_nInIdx = m_nOutIdx;
		m_nOutIdx = nTemp;
	}

	//
	void Reset(bool bEraseSpace = false)
	{
		LockIn();
		LockOut();

		ClearIn(bEraseSpace);
		ClearOut(bEraseSpace);

		UnlockOut();
		UnlockIn();
	}

	//
	void LockIn()
	{
		m_csIn.Lock();
	}

	//
	void UnlockIn()
	{
		m_csIn.Unlock();
	}

	//
	void AddIn(const _ValueType& Value)
	{
		unsigned int nCapacity = InCapacity();
		unsigned int nSize = m_nSize[m_nInIdx];
		if(m_nSize[m_nInIdx] > nCapacity)
		{
			m_vec[m_nInIdx].reserve(nCapacity + _GrowBy);
		}
		
		if(m_vec[m_nInIdx].size() > nSize)
			m_vec[m_nInIdx][nSize] = Value;
		else
			m_vec[m_nInIdx].push_back(Value);

		m_nSize[m_nInIdx]++;
	}

	//
	void ClearIn(bool bEraseSpace = false)
	{
		m_nSize[m_nInIdx] = 0;
		if(bEraseSpace)
			m_vec[m_nInIdx].clear();
	}

	//
	unsigned int InSize()
	{
		return m_nSize[m_nInIdx];
	}

	//
	unsigned int InCapacity()
	{
		return (unsigned int)m_vec[m_nInIdx].capacity();
	}

	//
	void LockOut()
	{
		m_csOut.Lock();
	}

	//
	void UnlockOut()
	{
		m_csOut.Unlock();
	}

	//
	const _ValueType& GetOut(unsigned int nIdx)
	{
		return m_vec[m_nOutIdx][nIdx];
	}

	//
	void ClearOut(bool bEraseSpace = false)
	{
		m_nSize[m_nOutIdx] = 0;
		if(bEraseSpace)
			m_vec[m_nOutIdx].clear();
	}

	//
	unsigned int OutSize()
	{
		return m_nSize[m_nOutIdx];
	}

	//
	unsigned int OutCapacity()
	{
		return m_vec[m_nOutIdx].capacity();
	}

protected:
	unsigned int					m_nInIdx;
	unsigned int					m_nOutIdx;

	CComAutoCriticalSection			m_csIn;
	CComAutoCriticalSection			m_csOut;

	std::vector<_ValueType>			m_vec[2];
	unsigned int					m_nSize[2];
};

/* old version
template< class _ValueType, long _GrowBy = 10L >
class CDoubleBuffer
{
public:
	//
	CDoubleBuffer()
	{
		m_nInIdx = 0;
		m_nOutIdx = 1;
		
		m_vec[0].reserve(_GrowBy);
		m_vec[1].reserve(_GrowBy);
	}
	
	//
	virtual ~CDoubleBuffer()
	{
	}

	//
	void Swap()
	{
		unsigned int nTemp = m_nInIdx;
		m_nInIdx = m_nOutIdx;
		m_nOutIdx = nTemp;
	}

	//
	void Reset(bool bEraseSpace = false)
	{
		LockIn();
		LockOut();

		ClearIn(bEraseSpace);
		ClearOut(bEraseSpace);

		UnlockOut();
		UnlockIn();
	}

	//
	void LockIn()
	{
		m_csIn.Lock();
	}

	//
	void UnlockIn()
	{
		m_csIn.Unlock();
	}

	//
	void AddIn(const _ValueType& Value)
	{
		unsigned int nCapacity = m_vec[m_nInIdx].capacity();
		if(m_vec[m_nInIdx].size() > nCapacity)
		{
			m_vec[m_nInIdx].reserve(nCapacity + _GrowBy);
		}
		m_vec[m_nInIdx].push_back(Value);
	}

	//
	void ClearIn()
	{
		m_vec[m_nInIdx].clear();
	}

	//
	unsigned int InSize()
	{
		return m_vec[m_nInIdx].size();
	}

	//
	unsigned int InCapacity()
	{
		return m_vec[m_nInIdx].capacity();
	}

	//
	void LockOut()
	{
		m_csOut.Lock();
	}

	//
	void UnlockOut()
	{
		m_csOut.Unlock();
	}

	//
	const _ValueType& GetOut(unsigned int nIdx)
	{
		return m_vec[m_nOutIdx][nIdx];
	}

	//
	void ClearOut()
	{
		m_vec[m_nOutIdx].clear();
	}

	//
	unsigned int OutSize()
	{
		return m_vec[m_nOutIdx].size();
	}

	//
	unsigned int OutCapacity()
	{
		return m_vec[m_nOutIdx].capacity();
	}

protected:
	unsigned int					m_nInIdx;
	unsigned int					m_nOutIdx;

	CComAutoCriticalSection			m_csIn;
	CComAutoCriticalSection			m_csOut;

	std::vector<_ValueType>			m_vec[2];
};
*/


//----------------------------------------------------------------------------------------------------------------------------------------

template< class _ValueType >
class CDoubleSet
{
public:
	typedef std::set<_ValueType> SETTYPE;
	typedef typename SETTYPE::const_iterator SETITERATOR;
	//
	CDoubleSet()
	{
		m_nInIdx = 0;
		m_nOutIdx = 1;
	}
	
	//
	virtual ~CDoubleSet()
	{
		Reset();
	}

	//
	void Swap()
	{
		unsigned int nTemp = m_nInIdx;
		m_nInIdx = m_nOutIdx;
		m_nOutIdx = nTemp;
	}

	//
	void Reset()
	{
		LockIn();
		LockOut();

		ClearIn();
		ClearOut();

		UnlockOut();
		UnlockIn();
	}

	//
	void LockIn()
	{
		m_csIn.Lock();
	}

	//
	void UnlockIn()
	{
		m_csIn.Unlock();
	}

	//
	void AddIn(const _ValueType& Value)
	{
		m_vec[m_nInIdx].insert(Value);
	}

	//
	void ClearIn(bool bEraseSpace = false)
	{
		m_vec[m_nInIdx].clear();
	}

	//
	unsigned int InSize()
	{
		return m_vec[m_nInIdx].size();
	}

	//
	void LockOut()
	{
		m_csOut.Lock();
	}

	//
	void UnlockOut()
	{
		m_csOut.Unlock();
	}

	//
	
	SETITERATOR GetOutStart()
	{
		return m_vec[m_nOutIdx].begin();
	}
	SETITERATOR GetOutEnd()
	{
		return m_vec[m_nOutIdx].end();
	}

	//
	void ClearOut()
	{
		m_vec[m_nOutIdx].clear();
	}

	//
	unsigned int OutSize()
	{
		return (unsigned int) m_vec[m_nOutIdx].size();
	}


protected:
	unsigned int					m_nInIdx;
	unsigned int					m_nOutIdx;

	CComAutoCriticalSection			m_csIn;
	CComAutoCriticalSection			m_csOut;

	SETTYPE							m_vec[2];
};




#endif // !defined(AFX_DOUBLEBUFFER_H__E8CCE9A6_17F4_11D5_BEA9_00508B77148B__INCLUDED_)
