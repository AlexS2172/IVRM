#ifndef __MSINDEX_H__
#define __MSINDEX_H__

#include "MsUnd.h"
#include "MsFuturesRoot.h"
#include "MsFuture.h"
#include "MsFutureOption.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsIndex : public CMsUnd
{
public:
	CMsIndex(void)
		: m_dYield(0.)
		, m_bHasFutOrFutOptWithPos(false)
		, m_nFutOptClosePriceUpdated(0L)
		, m_spFuturesWithPos(new EODMS::CMsFutureColl)
		, m_spFuturesOptWithPos(new EODMS::CMsFutureOptionColl)
	{
	}

	~CMsIndex(void)
	{
	}

	SYMBOL_TYPE Type() const
	{
		return enStIndex;
	}

	virtual bool IsBasket()
	{
		return false;
	}

	virtual void ApplyRates(CMsRatePtr pRate);

	virtual long FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
								long nToday, long nDTE, double& dYield);

	EODMS::CMsFutureCollPtr FutureWithPos()
	{
		return m_spFuturesWithPos;
	}

	EODMS::CMsFuturePtr FutureWithPos(CString strSymbol)
	{
		return GetCollectionItem<EODMS::CMsFuturePtr>(m_spFuturesWithPos, strSymbol);
	}

	EODMS::CMsFutureOptionCollPtr FutureOptionWithPos()
	{
		return m_spFuturesOptWithPos;
	}

	EODMS::CMsFutureOptionPtr FutureOptionWithPos(CString strSymbol)
	{
		return GetCollectionItem<EODMS::CMsFutureOptionPtr>(m_spFuturesOptWithPos, strSymbol);
	}
	void CalculateFutureOptionsTheoPrice(EODSM::CSmSettingsMgrPtr pSettingsMgr);
private:
	void _CalcFutureOptionGreeks(CMsFutureOptionPtr pOpt,  CMsFuturePtr pFuture, EODSM::CSmSettingsMgrPtr pSettingsMgr);

public:
	double		m_dYield;
	bool		m_bHasFutOrFutOptWithPos;
	long		m_nFutOptClosePriceUpdated;

protected:
	CMsFutureCollPtr		m_spFuturesWithPos; // future with positions or/and option position
	CMsFutureOptionCollPtr	m_spFuturesOptWithPos; // references on future options with positions
};

typedef boost::shared_ptr<CMsIndex>		CMsIndexPtr;
typedef std::map<CString, CMsIndexPtr>	CMsIndexColl;
typedef CMsIndexColl::value_type		CMsIndexPair;

typedef boost::shared_ptr<CMsIndexColl>	CMsIndexCollPtr;
typedef std::map<long, CMsIndexPtr>		CMsIndexByIdColl;
typedef CMsIndexByIdColl::value_type	CMsIndexByIdPair;


typedef boost::shared_ptr<CMsIndexByIdColl>	CMsIndexByIdCollPtr;

};

#endif //__MSINDEX_H__