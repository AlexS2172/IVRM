#include "StdAfx.h"
#include "MsBasketIndex.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
long CMsBasketIndex::FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
									long nToday, long nDTE, double& dYield)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	long nBaskDivCount = static_cast<unsigned long>(m_vecDivs.size()), nRetCount = 0L, nDivCount = 0L;
	double dDivAmt = 0.;

	dYield = 0.;

	if(nBaskDivCount > 0L)
	{
		nDivCount = GetBasketDividendsCount(nToday, nDTE, &m_vecDivs.front(), nBaskDivCount);

		if(nDivCount > 0L)
		{
			vecDivDte.resize(nDivCount, 0.);
			vecDivAmt.resize(nDivCount, 0.);
			GetBasketDividends(nToday, nDTE, &m_vecDivs.front(), nBaskDivCount, &vecDivAmt.front(), &vecDivDte.front(), nDivCount, &nRetCount);
		}
	}

	if(nDivCount <= 0L)
		return CMsIndex::FillDivsForCalc(vecDivDte, vecDivAmt, nToday, nDTE, dYield);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
bool CMsBasketIndex::InitBasketDivs(CMsUndByIDCollPtr pUndColl)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	m_vecDivs.clear();

	ATLASSERT(m_spUndWeight != NULL);
	
	if(pUndColl != NULL && m_spUndWeight->size() > 0)
	{
		CMsUndByIDCollPtr spUndColl(pUndColl);

		for(CMsUndWeightColl::iterator itUndWeight = m_spUndWeight->begin();
			itUndWeight != m_spUndWeight->end(); itUndWeight++)
		{
			const CMsUndWeightPtr& spUndWeight = itUndWeight->second;
			if(spUndWeight->m_dWeight > 0.)
			{
				const CMsUndPtr spUnd(GetCollectionItem<CMsUndPtr>(spUndColl, spUndWeight->m_nUndID));
				if(spUnd)
				{
					if(enStStock == spUnd->Type())
					{
						const CMsStockPtr spStock(boost::shared_dynamic_cast<CMsStock>(spUnd));
						ATLASSERT(spStock);

						if(spStock)
						{
							if(spStock->IsValidDivs(spStock->m_bUseCustDivs))
							{
								REGULAR_DIVIDENDS aDiv;
								spStock->GetRegularDiv(spStock->m_bUseCustDivs, aDiv);
								aDiv.dAmount *= spUndWeight->m_dWeight;
								m_vecDivs.push_back(aDiv);
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

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsBasketIndex::GetBasketDivs(REGULAR_DIVIDENDS* pDivs, unsigned long nMaxCount) const
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
void CMsBasketIndex::BasketDiv(unsigned long nIndex, REGULAR_DIVIDENDS& aDiv) const
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	ATLASSERT(nIndex > 0 && nIndex < m_vecDivs.size());
	aDiv = m_vecDivs[nIndex];
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

};