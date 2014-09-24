// Stream.h: interface for the CStream class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __STREAM_H__
#define __STREAM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStream  
{
	CStream();

private:
	CHandle		m_hCompleteEvent;
protected:
	static LONG PASCAL StreamThread(VOID *, ULONG, DBA_KEY *,	INT, DBA_RECORD *,	INT,	ULONG,	DWORD,	VOID*);

	//static long CALLBACK StreamThread(void* /*pblk*/, ULONG ulFunction, DBA_KEY* pDbaKey, int keyLen, DBA_RECORD* pDbaRec, int recLen, ULONG /*ulKey*/, DWORD dwStatus, void* p);

	DB_STREAM_FILTER m_streamFilter;
	DBA_KEYS* m_pDbaKey;
	DBA_RECORD* m_pDbaRec;
	void* const m_pBlk;
	short m_nFunc;
	bool m_bTerminate;

	virtual long StreamData(long nStatus, long nFunc);
	long CreateStream(short nKeyNum);
	void TerminateStream();
	bool WaitStreamTerminate(long Timeout = INFINITE);
	bool IsStreamRunning();
	long GetNextStream(short nKeyNum);

	static long m_nMissCount;

public:
	CStream(void* const pBlk, DBA_KEYS* pDbaKey, DBA_RECORD* pDbaRec);
	~CStream();
};

inline bool CStream::IsStreamRunning()
{
	return ::WaitForSingleObject(m_streamFilter.hThread, 0) == WAIT_TIMEOUT;
}

#endif // __STREAM_H__
