#ifndef _POINTERSBUFFER_H__
#define _POINTERSBUFFER_H__

#include <DoubleBuffer.h>

template <typename _PointerType, long _GrowBy = 100L>
class CPointersBuffer : public CDoubleBuffer<_PointerType*,_GrowBy>
{
public:
	void ClearIn(bool bEraseSpace = false)
	{
		m_nSize[m_nInIdx] = 0;

		unsigned int nIdx = 0;		
		while(nIdx < m_vec[m_nInIdx].size())
		{
			_PointerType* pElem = m_vec[m_nInIdx][nIdx];
			if(pElem)
			{
				delete pElem;
				m_vec[m_nInIdx][nIdx] = NULL;
			}
			nIdx++;
		}
		if(bEraseSpace)
			m_vec[m_nInIdx].clear();
	}

	void ClearOut(bool bEraseSpace = false)
	{
		m_nSize[m_nOutIdx] = 0;

		unsigned int nIdx = 0;		
		while(nIdx < m_vec[m_nOutIdx].size())
		{
			_PointerType* pElem = m_vec[m_nOutIdx][nIdx];
			if(pElem)
			{
				delete pElem;
				m_vec[m_nOutIdx][nIdx] = NULL;
			}
			nIdx++;
		}
		if(bEraseSpace)
			m_vec[m_nOutIdx].clear();
	}	
};

#endif //_POINTERSBUFFER_H__