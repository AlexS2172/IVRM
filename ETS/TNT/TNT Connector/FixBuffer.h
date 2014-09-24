#pragma once
#include "Lock.h"
//#include "FixConst.h"

#define ALLOCATION_GRANULARITY		1024

/*-------------------------------------------------------------------------------
CFixBuffer class
--------------------------------------------------------------------------------*/

class CFixBuffer : public CLock
{
	void * m_pData;
	DWORD  m_dwLen;
	DWORD  m_dwSize;

protected:

	void   PrepareBuffer(DWORD dwDelta)
	{
		if (m_dwLen + dwDelta <= m_dwSize)
			return;

		if (dwDelta < ALLOCATION_GRANULARITY)
			dwDelta = ALLOCATION_GRANULARITY;

		m_dwSize += dwDelta;
		if (!m_pData)
		{
			m_pData = malloc(m_dwSize);
			_ASSERTE(m_pData);
		}
		else
		{
			void *pNewData = malloc(m_dwSize);
			_ASSERTE(pNewData);
			memcpy(pNewData, m_pData, m_dwLen);
			free(m_pData);
			m_pData = pNewData;
		}
	}

public:

	void Clear()
	{
		if (m_pData)
		{
			free(m_pData);
			m_pData = NULL;
		}

		m_dwLen = 0;
		m_dwSize = 0;
	}

	void Put(const void * pData, const DWORD dwLen)
	{
		PrepareBuffer(dwLen);
		memcpy((char*)m_pData + m_dwLen, pData, dwLen);
		m_dwLen += dwLen;
	}

	bool IsEmpty() const {return m_pData == NULL;}

	void ShiftLeft(const DWORD dwDelta)
	{
		if (m_dwLen > dwDelta)
		{
			m_dwLen -= dwDelta;

			if (m_dwLen)
				memcpy(m_pData, ((char*)m_pData) + dwDelta, m_dwLen);
		}
		else
		{        
			Clear();
		}
	}

public:

	CFixBuffer() : m_pData(NULL), m_dwLen(0), m_dwSize(0){}

	virtual ~CFixBuffer(){Clear();}

	CFixBuffer(const CFixBuffer& Origin) : m_pData(NULL), m_dwLen(0), m_dwSize(0)
	{
		Put(Origin.GetData(), Origin.GetLength());
	}

	CFixBuffer& operator=(const CFixBuffer& Origin) 
	{
		Clear();
		Put(Origin.GetData(), Origin.GetLength());
		return *this;
	}

	inline void * GetData() const {return m_pData;}
	inline DWORD  GetLength() const {return m_dwLen;}
};
