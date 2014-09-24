#include "StdAfx.h"
#include "MsStock.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
long CMsStock::FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
									long nToday, long nDTE, double& dYield)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	long nDivDate = 0L, nDivFreq = 0L, nRetCount = 0L, nDivCount = 0L;
	double dDivAmt = 0.;

	dYield = 0.;

	if(m_bUseCustDivs)
	{
		nDivDate = m_nDivDateCust;
		dDivAmt = m_dDivAmtCust;
		nDivFreq = m_nDivFreqCust;
	}
	else
	{
		nDivDate = m_nDivDate;
		dDivAmt = m_dDivAmt;
		nDivFreq = m_nDivFreq;
	}

	if(nDivFreq != 0L)
	{
		nDivCount = GetDividendsCount(nToday, nDTE, nDivDate, nDivFreq);
		if(nDivCount > 0L)
		{
			vecDivDte.resize(nDivCount, 0.);
			vecDivAmt.resize(nDivCount, 0.);
			GetDividends2(nToday, nDTE, nDivDate, nDivFreq, dDivAmt, nDivCount, &vecDivAmt.front(), &vecDivDte.front(), &nRetCount);
		}
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return nDivCount;
}

/////////////////////////////////////////////////////////////////////////////
//
bool CMsStock::IsValidDivs(bool bCheckCust) const
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(bCheckCust)
	{
		return (m_nDivFreqCust == 1L || m_nDivFreqCust == 2L || m_nDivFreqCust == 4L || m_nDivFreqCust == 12L)
				&& m_dDivAmtCust > 0. && m_nDivDateCust > 0L;
	}
	else
	{
        return (m_nDivFreq == 1L || m_nDivFreq == 2L || m_nDivFreq == 4L || m_nDivFreq == 12L)
				&& m_dDivAmt > 0. && m_nDivDate > 0L;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsStock::GetRegularDiv(bool bCust, REGULAR_DIVIDENDS& aDiv) const
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(bCust)
	{
		aDiv.nLastDivDate = m_nDivDateCust;
		aDiv.nFrequency = m_nDivFreqCust;
		aDiv.dAmount = m_dDivAmtCust;
	}
	else
	{
		aDiv.nLastDivDate = m_nDivDate;
		aDiv.nFrequency = m_nDivFreq;
		aDiv.dAmount = m_dDivAmt;
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

}; //namespace EODMS
