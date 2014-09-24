#include "StdAfx.h"
#include "MsIndex.h"

namespace EODMS
{

void CMsIndex::ApplyRates(CMsRatePtr pRate)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	const CMsRatePtr spRate(pRate);
	ATLASSERT(spRate);

	if(spRate)
	{
		CMsUnd::ApplyRates(spRate);

		bool bUseMidRates = true;

		if(spRate->m_enRule == enMsRrRateBasedOnPosition
			&& !EODCO::IsBadValue(m_dPos) && EODCO::DoubleNEQZero(spRate->m_dPosThreshold))
		{
			double dPosThreshold = fabs(spRate->m_dPosThreshold);
			bUseMidRates = (fabs(m_dPos) < fabs(spRate->m_dPosThreshold));
		}

		long nToday = static_cast<long>(floor(COleDateTime::GetCurrentTime()));

		for(CMsFutureOptionColl::iterator itOpt = m_spFuturesOptWithPos->begin(); itOpt != m_spFuturesOptWithPos->end(); itOpt++)
		{
			CMsFutureOptionPtr& spOpt(itOpt->second);
			long nExpiration = static_cast<long>(floor(spOpt->m_dtExpiration));

			if(bUseMidRates)
				spOpt->m_dRate = (!m_bIsHTB ? spRate->NeutralRate(nToday, nExpiration)
				: spRate->NeutralHTBRate(nToday, nExpiration));
			else
			{
				if(m_dPos < 0.)
					spOpt->m_dRate = (!m_bIsHTB ? spRate->ShortRate(nToday, nExpiration)
					: spRate->HTBRate(nToday, nExpiration));
				else
					spOpt->m_dRate = spRate->LongRate(nToday, nExpiration);
			}
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
/////////////////////////////////////////////////////////////////////////////
//
long CMsIndex::FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
									long nToday, long nDTE, double& dYield)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	dYield = m_dYield;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return 0L;
}
/////////////////////////////////////////////////////////////////////////////
//
void CMsIndex::CalculateFutureOptionsTheoPrice(EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	for(CMsFutureColl::iterator itFuture = m_spFuturesWithPos->begin(); itFuture != m_spFuturesWithPos->end(); itFuture++)
	{
		const CMsFuturePtr& spFuture(itFuture->second);
		const CMsFutureOptionCollPtr& spOptColl = spFuture->Opt();

		for(CMsFutureOptionColl::iterator itOpt = spOptColl->begin(); itOpt != spOptColl->end(); itOpt++)
		{
			const CMsFutureOptionPtr& spOpt(itOpt->second);
			_CalcFutureOptionGreeks(spOpt, spFuture, pSettingsMgr);
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsIndex::_CalcFutureOptionGreeks(CMsFutureOptionPtr pOpt,  CMsFuturePtr pFuture, EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	const CMsFutureOptionPtr spOpt(pOpt);
	const CMsFuturePtr spFuture(pFuture);

	ATLASSERT(spOpt && spFuture);
	if(spOpt && spFuture)
	{
		GREEKS aGreeks;
		aGreeks.nMask = GT_THEOPRICE;
		long nOptID = spOpt->m_nID;

		if(!EODCO::IsBadValue(m_dPriceClose) && !EODCO::IsBadValue(spFuture->m_dPriceClose))
		{
			EodCalcModelTypeEnum enModel;
			CEodServer::s_pEodServer->get_EodCalcModel(&enModel);
			long nModel = static_cast<long>(enModel);

			long nIsAmerican = (m_bIsAmerican ? 1L : 0L);
			double dVola = 0.;
			long nRetCount = 0L, nDTE = 0L, nToday = 0L;
			nToday = static_cast<long>(floor(COleDateTime::GetCurrentTime()));

			nDTE = static_cast<long>(floor(spOpt->m_dtExpiration)) - nToday;

			dVola = m_spVolaSource->Vola(m_dPriceClose,spOpt->m_dtExpiration,spOpt->m_dStrike);

			nRetCount = CalcFutureOptionGreeks(spOpt->m_dRate, spFuture->m_dPriceClose, spOpt->m_dStrike,
					dVola, nDTE, spOpt->m_bIsCall, nIsAmerican, 
					100L, m_dSkew, m_dKurt, nModel, &aGreeks);

			spOpt->m_dPriceTheo = BAD_DOUBLE_VALUE;

			if(!EODCO::IsBadValue(aGreeks.dTheoPrice) && aGreeks.dTheoPrice > 0.)
				spOpt->m_dPriceTheo = aGreeks.dTheoPrice;
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

};