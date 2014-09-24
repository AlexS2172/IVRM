#pragma once
#include "CommonSPtr.h"
#include "Cache.h"

class CBorrowingRatesCache: public CCache
{
public:
	CBorrowingRatesCache(void);
	~CBorrowingRatesCache(void);

private:
	IBorrowingRatesCurvesPtr	m_spBorrowingRatesCurevs;
	
	IEtsRateCollPtr	LoadSingleCurve(long lCurveID);
	bool			AddCurve(IEtsRateCollPtr spCurve);

public:
	IEtsRateCollPtr	GetCurve(long lCurveID);
	void			Clear();
};

typedef boost::shared_ptr<CBorrowingRatesCache>	CBorrowingRatesCachePtr;
