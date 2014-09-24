#include "StdAfx.h"
#include "MsSynthRoot.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
long CMsSynthRoot::FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
									long nToday, long nDTE, double& dYield)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	long nBaskDivCount = static_cast<unsigned long>(m_vecDivs.size()), nRetCount = 0L, nDivCount = 0L;

	if(m_bIsBasket && nBaskDivCount > 0L)
	{
		dYield = m_dYield;

		nDivCount = GetBasketDividendsCount(nToday, nDTE, &m_vecDivs.front(), nBaskDivCount);

		if(nDivCount > 0L)
		{
			vecDivDte.resize(nDivCount, 0.);
			vecDivAmt.resize(nDivCount, 0.);
			GetBasketDividends(nToday, nDTE, &m_vecDivs.front(), nBaskDivCount, &vecDivAmt.front(), &vecDivDte.front(), nDivCount, &nRetCount);
			dYield = 0.;
		}
	}
	else
		dYield = 0.;

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return nDivCount;
}

/////////////////////////////////////////////////////////////////////////////
//
bool CMsSynthRoot::InitBasketDivs(CMsUndByIDCollPtr pUndColl)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	m_vecDivs.clear();
	
	ATLASSERT(m_spSynthRootComp != NULL);

	if(m_bIsBasket)
	{
		if(pUndColl != NULL && m_spSynthRootComp->size() > 0)
		{
			CMsUndByIDCollPtr spUndColl(pUndColl);

			for(CMsUndWeightColl::iterator itSR = m_spSynthRootComp->begin();
				itSR != m_spSynthRootComp->end(); itSR++)
			{
				const CMsUndWeightPtr& spSRComp = itSR->second;
				if(spSRComp->m_dWeight > 0.)
				{
					const CMsUndPtr spUnd(GetCollectionItem<CMsUndPtr>(spUndColl, spSRComp->m_nUndID));
					if(spUnd)
					{
						if(enStStock == spUnd->Type())
						{
							const CMsStockPtr spStock(boost::shared_dynamic_cast<EODMS::CMsStock>(spUnd));
							ATLASSERT(spStock);

							if(spStock)
							{
								if(spStock->IsValidDivs(spStock->m_bUseCustDivs))
								{
									REGULAR_DIVIDENDS aDiv;
									spStock->GetRegularDiv(spStock->m_bUseCustDivs, aDiv);
									aDiv.dAmount *= spSRComp->m_dWeight;
									m_vecDivs.push_back(aDiv);
								}
							}
						}
						else if(enStIndex == spUnd->Type())
						{
							const CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(spUnd));
							ATLASSERT(spIndex);

							if(spIndex)
							{
								if(spIndex->IsBasket())
								{
									const CMsBasketIndexPtr spBasketIndex(boost::shared_dynamic_cast<CMsBasketIndex>(spIndex));
									ATLASSERT(spBasketIndex);

									if(spBasketIndex)
									{
										REGULAR_DIVIDENDS aDiv;
										unsigned long nCount = spBasketIndex->BasketDivsCount();

										for(unsigned long i = 0; i < nCount; i++)
										{
											spBasketIndex->BasketDiv(i, aDiv);
											aDiv.dAmount *= spSRComp->m_dWeight;
											m_vecDivs.push_back(aDiv);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return false;
		}
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsSynthRoot::GetBasketDivs(REGULAR_DIVIDENDS* pDivs, unsigned long nMaxCount) const
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	ATLASSERT(pDivs != NULL);

	for(unsigned long i = 0; i < nMaxCount; i++)
	{
		pDivs[i] = m_vecDivs[i];
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsSynthRoot::BasketDiv(unsigned long nIndex, REGULAR_DIVIDENDS& aDiv) const
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	ATLASSERT(nIndex > 0 && nIndex < m_vecDivs.size());
	aDiv = m_vecDivs[nIndex];
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
/////////////////////////////////////////////////////////////////////////////
//

double CMsSynthRoot::GetSynthUndClosePrice(CMsUndByIDCollPtr pUndColl)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	double dUndClose = m_dCashValue;

	for(CMsUndWeightColl::iterator itComp = m_spSynthRootComp->begin(); itComp != m_spSynthRootComp->end(); itComp++)
	{
		const CMsUndWeightPtr& spComp(itComp->second);

		CMsUndPtr spUnd = GetCollectionItem<CMsUndPtr>(pUndColl, spComp->m_nUndID);
		ATLASSERT(spUnd);

		if(!spUnd) return BAD_DOUBLE_VALUE;

		if(!EODCO::IsBadValue(spUnd->m_dPriceClose))
			dUndClose += spUnd->m_dPriceClose * spComp->m_dWeight;
		else
			return BAD_DOUBLE_VALUE;
	}

	return dUndClose;
}

};