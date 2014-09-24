#ifndef __MSBASKETINDEX_H__
#define __MSBASKETINDEX_H__

#include "MsIndex.h"
#include "MsUnd.h"
#include "MsUndWeight.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsBasketIndex : public CMsIndex
{
public:
	CMsBasketIndex(void)
		: m_spUndWeight(new CMsUndWeightColl)
	{
	}

	~CMsBasketIndex(void)
	{
	}

	bool IsBasket()
	{
		return true;
	}

	CMsUndWeightCollPtr UndWeight()
	{
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Enter"));
		return m_spUndWeight;
	}

	CMsUndWeightPtr UndWeight(long nUndID)
	{
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Enter"));
		return GetCollectionItem<CMsUndWeightPtr>(m_spUndWeight, nUndID);
	}

	unsigned long BasketDivsCount() const
	{
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Enter"));
		return static_cast<unsigned long>(m_vecDivs.size());
	}

	bool InitBasketDivs(CMsUndByIDCollPtr pUndColl);

	void GetBasketDivs(REGULAR_DIVIDENDS* pDivs, unsigned long nMaxCount) const;

	void BasketDiv(unsigned long nIndex, REGULAR_DIVIDENDS& aDiv) const;

	virtual long FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
								long nToday, long nDTE, double& dYield);

protected:
	CMsUndWeightCollPtr			m_spUndWeight; // underlings weights by underlying ID


	RegularDividendVec			m_vecDivs;

};

typedef boost::shared_ptr<CMsBasketIndex>		CMsBasketIndexPtr;
typedef std::map<long, CMsBasketIndexPtr>		CMsBasketIndexColl;
typedef CMsBasketIndexColl::value_type			CMsBasketIndexPair;
typedef boost::shared_ptr<CMsBasketIndexColl>	CMsBasketIndexCollPtr;

};

#endif //__MSBASKETINDEX_H__