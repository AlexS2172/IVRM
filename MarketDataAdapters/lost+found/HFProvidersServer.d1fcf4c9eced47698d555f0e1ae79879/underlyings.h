// Underlyings.h : Declaration of the CUnderlyings

#ifndef __UNDERLYINGS_H__
#define __UNDERLYINGS_H__

#include "SecurityProfile.h"
#include "HyperFeedBase.h"

/////////////////////////////////////////////////////////////////////////////
// CUnderlyings

class CUnderlyings : 
	virtual public CSecurityProfile,
	public CHyperFeedResponserBase
{
public:
	virtual ~CUnderlyings();

protected:
	void GetStockInfo(const _StockParams& Params);
	void GetStockResults(const _StockParams& Params);
	void GetStockResultsEx(const _StockParams& Params);
private:
	void _GetStock(const _StockParams& Params, bool IsResultInInfo, bool IsResultEx);
	void _SetDbKey(BSTR bsUnderlying);
	virtual void PublicStock(const _StockParams& params, _StockResultsEx& results) {};
};

#endif //__UNDERLYINGS_H__
