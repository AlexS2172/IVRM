#include "StdAfx.h"
#include "EodLoadMktStrUpdates.h"

namespace EOD
{

DATE	GetTime(long lHour, long lMinute, long lSecond = 0)
{
	return	static_cast<double>(lHour)  * (1. / 24.)	+
			static_cast<double>(lMinute)* (1. / (24.* 60.))	+
			static_cast<double>(lSecond) * (1. / (24. * 60. * 60.));
}
//---------------------------------------------------------------------------------------------------------------
EgLib::vt_date CEodLoadMarketStructureUpdates::GetDefaultValuationParams(const EgLib::vt_date &dtExpiry,
																		 const SYMBOL_TYPE stBaseType,
																		 const SYMBOL_TYPE stOptionType,
																		 const PP::SettlementTypeEnum stType,
																		 long lExpirationType,
																		 EgLib::vt_date &dtTradingClose)
{
	DATE	dtExpiryOV = (DATE)dtExpiry;

	if (stBaseType == enStStock || stBaseType == enStIndex)
	{
		switch(lExpirationType)
		{
		case 0:
			dtExpiryOV--;
			if (stType == PP::enSTAM)	
				dtExpiryOV += GetTime(9, 30);
			else if (stType == PP::enSTPM)	
				dtExpiryOV += GetTime(16, 15);
			else // undefined
				dtExpiryOV += GetTime(16, 0);
			break;
		case 7:
			dtExpiryOV += GetTime(16, 0);
			break;
		case 6:
			if (stType == PP::enSTAM)	
				dtExpiryOV += GetTime(9, 30);
			else
				dtExpiryOV += GetTime(16, 15);
		    break;
		default:
			if (stType == PP::enSTAM)	
				dtExpiryOV += GetTime(9, 30);
			else
				dtExpiryOV += GetTime(16, 15);
		    break;
		}
		if (stBaseType == enStStock)
			dtTradingClose	=	EgLib::vt_date(1900, 1, 1, 16, 0);
		else
			dtTradingClose	=	EgLib::vt_date(1900, 1, 1, 16, 15);
	}
	else if (stBaseType == enStFuture) 
	{
		if (stOptionType == enStFutureOption)
			dtExpiryOV += GetTime(16, 15);
		dtTradingClose	=	EgLib::vt_date(1900, 1, 1, 16, 15);
	}
	else if (stBaseType == enStFutUnd)
	{
		if (stOptionType == enStFutureOption)
			dtExpiryOV += GetTime(14, 0);
		dtTradingClose	=	EgLib::vt_date(1900, 1, 1, 16, 0);
	}
	return dtExpiryOV;
}

//---------------------------------------------------------------------------------------------------------------
EgLib::vt_date	CEodLoadMarketStructureUpdates::GetDefaultValuationParams(  const EgLib::vt_date &dtDate,
																			const SYMBOL_TYPE stContractType,
																			const PP::SettlementTypeEnum stType,
																			long lExpirationType)
{
	DATE	dtExpiryOV = (DATE)dtDate;
	if (stContractType == enStOption){
		switch(lExpirationType)
		{
		case 1:
		case 2:
		case 4:
		case 5:
			if(stType == PP::enSTPM)	dtExpiryOV++;
			break;
		case 7:
			dtExpiryOV++;
			break;
		case 0:
		default:
			if(stType == PP::enSTAM)	dtExpiryOV--;
			break;
		}
	}
	else if (stContractType == enStFutureOption){
		dtExpiryOV++;
	}
	else if (stContractType == enStFuture){
		dtExpiryOV++;
	}

	return EgLib::vt_date(dtExpiryOV);
}
//---------------------------------------------------------------------------------------------------------------
void CEodLoadMarketStructureUpdates::CheckMktStrUpdates(bool bOnlyActive)
{
	Report(StepMask(),enRpMtMessage,_T("Check updates of underlyings market structure on IVolatility"));
	m_bOnlyActive = bOnlyActive;

	EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();

	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	EODMS::CMsUndCollPtr spAllUndColl = spMktStrMgr->UndColl();

	DATE dtUpdate = 0.;
	EODMS::CMsUndCollPtr spRequestUndColl = _PrepareDataForRequest(spAllUndColl,dtUpdate);

	if(spRequestUndColl->size())
	{
		COleDateTime odtUpdate(dtUpdate);

		Report(StepMask(),enRpMtMessage, _T("Request changes of underlyings market structures from '%s'"), odtUpdate.Format(VAR_DATEVALUEONLY));
		m_Request.Init(spSettingsMgr, spRequestUndColl, dtUpdate);

		RequestIvData(&m_Request);
	}

	Report(StepMask(),enRpMtMessage, _T("Finished checking of market structure updates. %d underlying will be updated"), _GetUndCountNeedUpdate(spAllUndColl));
}

EODMS::CMsUndCollPtr CEodLoadMarketStructureUpdates::_PrepareDataForRequest(EODMS::CMsUndCollPtr pUndColl, DATE& dtUpdate)
{
	EODMS::CMsUndCollPtr spUndColl(new EODMS::CMsUndColl);
	for(EODMS::CMsUndColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		if( (!m_bOnlyActive || spUnd->m_bIsActive) && spUnd->m_dtLastMsUpdate > 0.)
		{
			if(EODCO::DoubleEQZero(dtUpdate) || dtUpdate > spUnd->m_dtLastMsUpdate)
				dtUpdate = spUnd->m_dtLastMsUpdate;

			spUndColl->insert(EODMS::CMsUndColl::value_type(spUnd->m_sSymbol, spUnd));

			spUnd->m_bNeedMsUpdate = false;
		}
	}
	return spUndColl;
}

long CEodLoadMarketStructureUpdates::_GetUndCountNeedUpdate(EODMS::CMsUndCollPtr pUndColl)
{
	long nCount = 0L;
	for(EODMS::CMsUndColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		if((!m_bOnlyActive || spUnd->m_bIsActive) && spUnd->m_bNeedMsUpdate)
			nCount++;
	}
	return nCount;
}

void CEodLoadMarketStructureUpdates::OnIvData(EODIV::CIvMarketStructureUpdatesResultPtr pResult)
{	
	if(_IsCancel() || IsStopping())
		throw EODCO::CEodStopException();
	
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	EODMS::CMsUndCollPtr spUndColl = m_Request.Und();
	
	ATLASSERT(spUndColl);

	EODMS::CMsUndPtr spUnd(GetCollectionItem<EODMS::CMsUndPtr>(spUndColl, pResult->m_strUndSymbol));

	ATLASSERT(spUnd);
	
	if(spUnd)
	{

		spUnd->m_dtIvMsUpdate = pResult->m_dtLastUpdate;
	
		if(EODCO::DoubleEQZero(pResult->m_dtLastUpdate) || pResult->m_dtLastUpdate < 0. || spUnd->m_dtLastMsUpdate < spUnd->m_dtIvMsUpdate)
		{
			spUnd->m_bNeedMsUpdate = true;
		}

		_IncrementCheckMsProgress();
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
void CEodLoadMarketStructureUpdates::_IncrementCheckMsProgress()
{
	EODMS::CMsUndCollPtr spUndColl = m_Request.Und();

	ATLASSERT(spUndColl);

	size_t nAllUnd = spUndColl->size(); 
	size_t nProgress = 10 * m_Request.UndSend() / nAllUnd;

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}
}