#include "StdAfx.h"
#include "MsUnd.h"
#include <math.h>

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
void CMsUnd::ApplyRates(CMsRatePtr pRate)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	const CMsRatePtr spRate(pRate);
	ATLASSERT(spRate);

	if(spRate)
	{
		bool bUseMidRates = true;

		if(spRate->m_enRule == enMsRrRateBasedOnPosition
			&& !EODCO::IsBadValue(m_dPos) && EODCO::DoubleNEQZero(spRate->m_dPosThreshold))
		{
			double dPosThreshold = fabs(spRate->m_dPosThreshold);
			bUseMidRates = (fabs(m_dPos) < fabs(spRate->m_dPosThreshold));
		}

		long nToday = static_cast<long>(floor(COleDateTime::GetCurrentTime()));

		for(CMsOptionColl::iterator itOpt = m_spOpt->begin(); itOpt != m_spOpt->end(); itOpt++)
		{
			CMsOptionPtr& spOpt(itOpt->second);
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
void CMsUnd::MakeGroupPriceRequestKey(EODPP::CPpPriceRequestKey& aKey)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	MakePriceRequestKey(aKey);

	switch(Type())
	{
	case enStStock:
		aKey.m_enType = PP::enGrSTK;
		break;
	case enStIndex:
		aKey.m_enType = PP::enGrIDX;
		break;
	};
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsUnd::MakeOptionKey(CMsOptRootPtr pRoot, CMsOptionPtr pOpt, CMsOptionKey& aKey)
{
	aKey.m_sRoot = pRoot->m_sName;
	aKey.m_bIsCall = pOpt->m_bIsCall;
	aKey.m_dStrike = pOpt->m_dStrike;
	aKey.m_nExpiryID = GetExpiryID(pOpt-> m_dtExpiration);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsUnd::CalculateAllOptionsTheoPrice(CMsUndByIDCollPtr pUndColl, EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	for(CMsOptRootColl::iterator itOptRoot = m_spOptRootWithPos->begin(); itOptRoot != m_spOptRootWithPos->end(); itOptRoot++)
	{
		const CMsOptRootPtr& spOptRoot(itOptRoot->second);
		const CMsOptionByIDCollPtr& spOptColl = spOptRoot->Opt();

		for(CMsOptionByIDColl::iterator itOpt = spOptColl->begin(); itOpt != spOptColl->end(); itOpt++)
		{
			const CMsOptionPtr& spOpt(itOpt->second);
			_CalcOptionGreeks(spOpt, spOptRoot, pUndColl, pSettingsMgr);
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsUnd::_CalcOptionGreeks(CMsOptionPtr pOpt,  CMsOptRootPtr pOptRoot, CMsUndByIDCollPtr pUndColl, EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	const CMsOptionPtr spOpt(pOpt);
	const CMsOptRootPtr spOptRoot(pOptRoot);

	ATLASSERT(spOpt && spOptRoot);
	if(spOpt && spOptRoot)
	{
		GREEKS aGreeks;
		aGreeks.nMask = GT_THEOPRICE;
		long nOptID = spOpt->m_nID;
		double dUndClose;

		CMsSynthRootPtr spSynthRoot;
		if(!spOptRoot->IsSynth())
		{
			dUndClose = m_dPriceClose;
		}
		else
		{
			spSynthRoot = boost::shared_dynamic_cast<CMsSynthRoot>(spOptRoot);
			ATLASSERT(spSynthRoot);
			dUndClose = spSynthRoot->GetSynthUndClosePrice(pUndColl);
		}

		if(!EODCO::IsBadValue(dUndClose))
		{
			double dOptClose = spOpt->m_dPriceClose;

			EodCalcModelTypeEnum enModel;
			CEodServer::s_pEodServer->get_EodCalcModel(&enModel);
			long nModel = static_cast<long>(enModel);

			long nIsAmerican = (m_bIsAmerican ? 1L : 0L);
			double dYield = 0., dVola = 0.;
			double  dSkew = 0., dKurt = 0., dRate = 0.;
			long nDivCount = 0L, nRetCount = 0L, nDTE = 0L, nToday = 0L;
			double* pdDivDte = NULL;
			double* pdDivAmt = NULL;
			EODCO::EsDoubleVec vecDivDte, vecDivAmt;

			nToday = static_cast<long>(floor(COleDateTime::GetCurrentTime()));

			nDTE = static_cast<long>(floor(spOpt->m_dtExpiration)) - nToday;

			if(!spOptRoot->IsSynth())
			{
				dSkew = m_dSkew;
				dKurt = m_dKurt;
				nDivCount = FillDivsForCalc(vecDivDte, vecDivAmt, nToday, nDTE, dYield);
			}
			else
			{
				dSkew = spSynthRoot->m_dSkew;
				dKurt = spSynthRoot->m_dKurt;
				nDivCount = spSynthRoot->FillDivsForCalc(vecDivDte, vecDivAmt, nToday, nDTE, dYield);
			}

			if(nDivCount > 0L)
			{
				pdDivDte = &vecDivDte.front();
				pdDivAmt = &vecDivAmt.front();
			}

			dRate = spOpt->m_dRate;

			dVola = m_spVolaSource->Vola(dUndClose,spOpt->m_dtExpiration,spOpt->m_dStrike);

			nRetCount = CalcGreeksMM2(dRate, dYield, BAD_DOUBLE_VALUE, dUndClose, spOpt->m_dStrike,
					dVola, nDTE, spOpt->m_bIsCall, nIsAmerican, nDivCount, pdDivAmt, pdDivDte,
					100L, dSkew, dKurt, nModel, &aGreeks);

			spOpt->m_dPriceTheo = BAD_DOUBLE_VALUE;

			if(!EODCO::IsBadValue(aGreeks.dTheoPrice) && aGreeks.dTheoPrice > 0.)
				spOpt->m_dPriceTheo = aGreeks.dTheoPrice;
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

};
