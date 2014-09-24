#ifndef __MSSYNTHROOT_H__
#define __MSSYNTHROOT_H__

#include "MsOptRoot.h"
#include "MsUndWeight.h"
#include "MsUnd.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsSynthRoot : public CMsOptRoot
{
public:
	CMsSynthRoot(void)
		: m_dCashValue(0.)
		, m_dSkew(0.)
		, m_dKurt(0.)
		, m_bIsBasket(false)
		, m_dYield(0.)
		, m_spSynthRootComp(new CMsUndWeightColl)
	{
	}

	~CMsSynthRoot(void)
	{
	
	}

	bool IsSynth()
	{
		return true;
	}

	CMsUndWeightCollPtr SynthRootComp()
	{
		return m_spSynthRootComp;
	}

	CMsUndWeightPtr SynthRootComp(long nUndID)
	{
		return GetCollectionItem<CMsUndWeightPtr>(m_spSynthRootComp, nUndID);
	}

	unsigned long BasketDivsCount() const
	{
		return static_cast<unsigned long>(m_vecDivs.size());
	}

	bool InitBasketDivs(CMsUndByIDCollPtr pUndColl);

	void GetBasketDivs(REGULAR_DIVIDENDS* pDivs, unsigned long nMaxCount) const;

	void BasketDiv(unsigned long nIndex, REGULAR_DIVIDENDS& aDiv) const;

	long FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
										long nToday, long nDTE, double& dYield);

	double GetSynthUndClosePrice(CMsUndByIDCollPtr pUndColl);

public:
	double						m_dCashValue;
	double						m_dSkew;
	double						m_dKurt;
	bool						m_bIsBasket;
	double						m_dYield;

protected:
	CMsUndWeightCollPtr			m_spSynthRootComp; // components by underlying ID

	RegularDividendVec			m_vecDivs;

};

typedef boost::shared_ptr<CMsSynthRoot> CMsSynthRootPtr;

};

#endif //__MSSYNTHROOT_H__