// FuturesOptions.h : Declaration of the CFuturesOptions

#ifndef __FUTURESOPTIONS_H__
#define __FUTURESOPTIONS_H__

#include "Extended.h"
#include "Futures.h"
#include "Stream.h"
#include "HyperFeedBase.h"

#define BAD_FUTURE_CLOSE_PRICE_REPLACER		1000.

/////////////////////////////////////////////////////////////////////////////
// CFuturesOptions
class CFuturesOptions :					    
	virtual public CDatabase<DBA_EXTENDED>,//CExtended,
	public CStream
{
public:
	CFuturesOptions();
protected:
	void GetFutureOptionsResults(CFutureInfo& clFuture, bool bIsResultEx);
	void CancelRequest();
	virtual long StreamData(long nStatus, long nFunc);
private:
	void _SetDbKey(const char* szBaseSymbol);

	long _GetFutureOptionsResultsByRoot(const char* szBaseSymbol);
	bool _CheckIsFutureOptionSymbolValid(const char* szSymbol, bool& IsCall, double& dStrike);
	double _ConvertStrikePriceFromFuturesClosePrice(long nStrike);
	bool _CheckIsFutureOptionSymbolOur(const char* szSymbol);

	virtual void PublicGetFutureOptionError(ErrorNumberEnum nError, BSTR bsDescription){};
	virtual void PublicLastFutureOption(){};
	virtual void PublicFutureOption(_FuturesOptionResults& results) {};
	virtual void PublicFutureOption(_FuturesOptionResultsEx& results) {};

private:
	bool						m_bIsGotOptions;
	long						m_nStreamError;
	CFutureInfo*				m_pCurrentFuture;
	string						m_strCurrentRoot;
	bool						m_bIsResultsEx;
};

#endif //__FUTURESOPTIONS_H__
