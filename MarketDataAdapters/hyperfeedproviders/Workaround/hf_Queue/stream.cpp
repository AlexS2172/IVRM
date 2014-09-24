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

	m_hCompleteEvent.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
}

CStream::~CStream()
{
}

//////////////////////////////////////////////////////////////////////
// CStream member functions
long CALLBACK CStream::StreamThread(void* /*pblk*/, ULONG ulFunction, DBA_KEY* pDbaKey, int keyLen, DBA_RECORD* pDbaRec, int recLen, ULONG /*ulKey*/, DWORD dwStatus, void* p)
{
	DB_STREAM_FILTER* pFilter = reinterpret_cast<DB_STREAM_FILTER*>(p);
	CStream* pBody = reinterpret_cast<CStream*>(pFilter->pUser);

	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("StreamThread Enter: %s , %s, %s, %d"), pDbaKey->symbol, pDbaKey->currencyCode, pDbaKey->exchangeCode, pDbaKey->type  );	
	
	_ASSERTE(pBody);
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION || pBody->m_bTerminate)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Terminating"));	
		SetEvent(pBody->m_hCompleteEvent);
		return TRUE;
	}
	
	if (pDbaKey->type == 0)
		return -1;
	

	BOOL nRes = FALSE;
	try
	{
		ULONG ulFunc = ulFunction & DBAX_FUNCTION_MASK;
		DBA_KEYS* p1 = pBody->m_pDbaKey;
		DBA_RECORD* p2 = pBody->m_pDbaRec;
		pBody->m_pDbaKey = (DBA_KEYS*)pDbaKey;
		pBody->m_pDbaRec = pDbaRec;
		long nRes = pBody->StreamData(dwStatus, ulFunction);
		pBody->m_pDbaKey = p1;
		pBody->m_pDbaRec = p2;
	}
	catch(...)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("StreamThread Exception handled for %s , %s, %s, %d"), nRes, pDbaKey->symbol, pDbaKey->currencyCode, pDbaKey->exchangeCode, pDbaKey->type  );	
	}
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("StreamThread Exit with: %d for %s , %s, %s, %d"), nRes, pDbaKey->symbol, pDbaKey->currencyCode, pDbaKey->exchangeCode, pDbaKey->type  );	
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

	ResetEvent(m_hCompleteEvent);
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
			SetEvent(m_hCompleteEvent);
			::TerminateThread(m_streamFilter.hThread, -1);
		}
		::CloseHandle(m_streamFilter.hThread);
		m_streamFilter.hThread = 0;
	}
}

bool CStream::WaitStreamTerminate(long nTimeout /*= INFINITE*/)
{
	try
	{

		if(m_streamFilter.hThread && ::WaitForSingleObject(m_streamFilter.hThread, nTimeout) != WAIT_TIMEOUT)
		{
			m_streamFilter.hThread = 0;
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
	}
	return false;
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
