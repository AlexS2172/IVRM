// Futures.h : Declaration of the CFutures

#ifndef __FUTURES_H__
#define __FUTURES_H__

#include "Database.h"
#include "Stream.h"
#include "HyperFeedBase.h"

/////////////////////////////////////////////////////////////////////////////
// CFutures
struct CFutureOptionRootInfo
{
	string					m_sSymbol;
	DATE					m_dtExpirationDate;
	long					m_nFutureLotSize;
	long					m_nOptionLotSize;
};

typedef map<string, CFutureOptionRootInfo> futureoptionrootmap;

struct CFutureInfo
{
	string					m_sSymbol;
	double					m_dFutureClosePrice;
	futureoptionrootmap		m_mapRoots;
};

class CFutures : 
	public CDatabase<DBA_FUTURE>,
	public CStream,
	public CHyperFeedResponserBase
{
public:

	CFutures() : m_szKeySymbol(NULL), m_bIsResultEx(false),
		CStream(&m_blk, &m_dbaKey, &m_dbaRec) {};
protected:
	void GetFutureResultsByRootSymbol(const _FutureRootParams& Params, bool bIsResultEx);
	void GetFutureResultsBySymbol(const _FutureParams& Params, bool bIsResultEx);
	void CancelRequest(const CComVariant& Params = vtMissing);
	virtual long StreamData(long nStatus, long nFunc);
	long GetFutureInfo(_FuturesOptionParams& Params, bool bIsResultEx);
	void CopyFutureFundamentalInfoToResult(_FuturesOptionResults& Result);
	void CopyFutureFundamentalInfoToResult(_FuturesOptionResultsEx& Result);

private:
	void _SetDbKey(BSTR bsSymbol);
	bool _CheckIsFutureSymbolValid(const char* szSymbol);
	bool _CheckIsFutureSymbolOur(const char* szBaseSymbol, const char* szSymbol);
	virtual void PublicFuture(const _FutureRootParams& params, _FutureResultsEx& results) {};
	virtual void PublicFuture(const _FutureParams& params, _FutureResultsEx& results) {};

protected:
	CFutureInfo m_clCurrentFuture;
private:
	CComVariant m_vtParams;
	bool		m_bIsGotFutures;
	long		m_nStreamError;
	const char* m_szKeySymbol;
	bool		m_bIsRequestFuturesByRoot;
	bool		m_bIsResultEx;
};

#endif //__FUTURES_H__
