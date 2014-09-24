// Stream.cpp: implementation of the CStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stream.h"
#include <comutil.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStream::CStream(void* const pBlk, DBA_KEYS* pDbaKey, DBA_RECORD* pDbaRec) : 
	m_pBlk(pBlk),
	m_pDbaKey(pDbaKey),
	m_pDbaRec(pDbaRec),
	m_bTerminate(true)
{
	m_streamFilter.lpfnDbProc = StreamThread; // Call Back routine
	m_streamFilter.hThread = 0;
	m_streamFilter.nRecLen = sizeof (DBA_RECORD);
}

CStream::~CStream()
{
}

//////////////////////////////////////////////////////////////////////
// CStream member functions
long CALLBACK CStream::StreamThread(void* /*pblk*/, ULONG ulFunction, DBA_KEY* pDbaKey, int keyLen, DBA_RECORD* pDbaRec, int recLen, ULONG /*ulKey*/, DWORD dwStatus, void* p)
{
	DB_STREAM_FILTER* pFilter = (DB_STREAM_FILTER*)p;
	ULONG ulFunc = ulFunction & DBAX_FUNCTION_MASK;
	CStream* pBody = (CStream*)pFilter->pUser;

	if (pBody->m_bTerminate || ulFunction == DBA_TERMINATE_STREAM_FUNCTION)
	{
		return TRUE; // last call
	}

	DBA_KEYS* p1 = pBody->m_pDbaKey;
	DBA_RECORD* p2 = pBody->m_pDbaRec;
	pBody->m_pDbaKey = (DBA_KEYS*)pDbaKey;
	pBody->m_pDbaRec = pDbaRec;
	long nRes = pBody->StreamData(dwStatus, ulFunction);
	pBody->m_pDbaKey = p1;
	pBody->m_pDbaRec = p2;

	return nRes;
}

long CStream::CreateStream(short nKeyNum)
{
	m_streamFilter.pUser = this; // object pointer
	m_streamFilter.nUserStatus = FALSE; // Initialize my status flags
	m_streamFilter.nCreateFlags = 0; // Thread Creation Flags
	m_streamFilter.nCount = -1;
	m_streamFilter.nProcStatus = 0;

	TerminateStream();

	m_bTerminate = false;
	int nKeyLen = sizeof (DBA_KEYS);
	int nRecLen = sizeof (DB_STREAM_FILTER);
	return dba(m_pBlk, m_nFunc, m_pDbaKey, &nKeyLen, &m_streamFilter, &nRecLen, nKeyNum);
}

void CStream::TerminateStream()
{
	if (m_streamFilter.hThread != 0)
	{
		m_bTerminate = true;
		dba(m_pBlk, DBA_TERMINATE_STREAM_FUNCTION, 0, 0, 0, 0, 0);
		if (::WaitForSingleObject(m_streamFilter.hThread, HFC_TIMEOUT) == WAIT_TIMEOUT)
		{
			_ASSERTE(FALSE);
			::TerminateThread(m_streamFilter.hThread, -1);
		}
		::CloseHandle(m_streamFilter.hThread);
		m_streamFilter.hThread = 0;
	}
}

bool CStream::WaitStreamTerminate(long nTimeout /*= INFINITE*/)
{
	if (::WaitForSingleObject(m_streamFilter.hThread, nTimeout) != WAIT_TIMEOUT)
	{
		::CloseHandle(m_streamFilter.hThread);
		m_streamFilter.hThread = 0;
		return true;
	}
	else
	{
		return false;
	}
}

long CStream::StreamData(long nStatus, long /*nFunc*/)
{
	return -1;
}
long CStream::GetNextStream(short nKeyNum)
{
	long nResult = -1;
	if (!IsStreamRunning())
	{
		m_nFunc = DBA_GET_NEXT_STREAM;
		nResult = CreateStream(nKeyNum);
	}
	return nResult;
}
