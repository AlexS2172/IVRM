// BstrPacker.h: interface for the CBstrPacker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSTRPACKER_H__608B8600_04CE_42DE_9E76_CB7C2CC4DB03__INCLUDED_)
#define AFX_BSTRPACKER_H__608B8600_04CE_42DE_9E76_CB7C2CC4DB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include <windef.h>
#include <vector>
using namespace std;


class CBstrPacker  
{
public:
	typedef vector<USHORT> _Data;

	CBstrPacker(ULONG nInitialSize = 0)
		: m_pdData(nInitialSize + 3) // reserve 4 bytes for length at start and 2 bytes for terminating 0
	{
		m_pdData[nInitialSize + 2] = 0; // place terminating zero to data
	}

	// add raw data
	void Add(LPBYTE pData, ULONG nSize)
	{
		_ASSERT(pData && nSize);

		// insert general data
		if(nSize > 1)
		{
			m_pdData.insert(m_pdData.end() - 1, (PUSHORT)pData, (PUSHORT)pData + nSize / sizeof(USHORT));
		}
		
		if(nSize % 2)
		{
			// insert last odd byte, aligned to USHORT
			m_pdData.insert(m_pdData.end() - 1, (USHORT)MAKEWORD(pData[nSize - 1], 0));
		}
	}

	// unpack raw data from BSTR
	void Unpack(BSTR pData)
	{
		_ASSERT(pData);
		
		Clear();

		ULONG nSize = ::SysStringLen(pData);
		m_pdData.insert(m_pdData.end() - 1, (PUSHORT)pData, (PUSHORT)pData + nSize);
	}

	// pointer to BSTR
	operator BSTR() 
	{
		ULONG nSize = Size();
		m_pdData[0] = LOWORD(nSize);
		m_pdData[1] = HIWORD(nSize);
		return m_pdData.begin() + 2;
	}

	// length of BSTR
	ULONG Length()
	{
		return m_pdData.size() - 3;
	}

	// pointer to raw data
	operator LPBYTE() 
	{
		return (LPBYTE)(m_pdData.begin() + 2);
	}

	// size of raw data in bytes
	ULONG Size()
	{
		return (m_pdData.size() - 3) * sizeof(USHORT);
	}

	// clear data
	void Clear()
	{
		m_pdData.clear();
		m_pdData.resize(3);
		m_pdData[2] = 0;
	}

private:
	_Data		m_pdData;
};

#endif // !defined(AFX_BSTRPACKER_H__608B8600_04CE_42DE_9E76_CB7C2CC4DB03__INCLUDED_)
