#include "StdAfx.h"
#include "EodLoadDefaultIv.h"

namespace EOD
{

void CEodStepLoadDefaultIv::Init(bool bOverwriteExistingIv)
{
	m_bOverwriteExistingIv = bOverwriteExistingIv;
}

DWORD CEodStepLoadDefaultIv::Stop()
{
	CancelRequest();
	return 	CThread::Stop();
}

EodStepsMaskEnum CEodStepLoadDefaultIv::StepMask()
{
	return enSmEmpty;
}

EodStepsMaskEnum CEodStepLoadDefaultIv::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepLoadDefaultIv::EodStepMainFunc()
{
// 	Report(StepMask(),enRpMtStart,_T("Started"));
// 
// 	EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();
// 
// 	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();
// 
// 	EODMS::CMsUndCollPtr spUndColl = spMktStrMgr->UndColl();
// 
// 	m_spDB.Open(spSettingsMgr->DbConnection().GetString(), spSettingsMgr->DbConnectionTimeout(), spSettingsMgr->DbCommandTimeout());
// 
// 	if(!m_bOverwriteExistingIv)
// 		spUndColl = _GetUndWithEmtpyIvCurve(spUndColl);
// 
// 	if(spUndColl->size())
// 	{
// 		m_Request.Init(spSettingsMgr, spUndColl);
// 
// 		RequestIvData(&m_Request);
// 	}
// 
// 	m_spDB.Close();
// 
// 	Report(StepMask(),enRpMtFinishSuccess, _T("Loaded Default IV for %d underlyings"),spUndColl->size());
}

void CEodStepLoadDefaultIv::OnIvData(EODIV::CIvDefaultIvResultPtr pResult)
{	
// 	if(_IsCancel() || IsStopping())
// 		throw EODCO::CEodStopException();
// 
// 	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
// 
// 	EODMS::CMsUndCollPtr spUndColl = m_Request.Und();
// 
// 	ATLASSERT(spUndColl);
// 
// 	EODMS::CMsUndPtr spUnd(GetCollectionItem<EODMS::CMsUndPtr>(spUndColl, pResult->m_strUndSymbol));
// 
// 	ATLASSERT(spUnd);
// 
// 	if(spUnd && !EODCO::IsBadValue(pResult->m_dVolatility))
// 	{
// 		long nExpiryID = EODCO::ExpiryIdFromDate(pResult->m_dtExpiration);
// 		EODMS::CMsUndExpPtr spUndExp(spUnd->EmptyIvExp(nExpiryID));
// 
// 		if(m_bOverwriteExistingIv || (spUndExp && !spUndExp->m_bHasVolaPoints))
// 		{
// 			try
// 			{
// 				CStoredProc<> spIvPointSave(m_spDB, L"usp_EodDefaultIvPoint_Save");
// 				spIvPointSave << spUnd->m_nID;
// 
// 				double dStrike = floor(pResult->m_dStrike * 1000.) / 1000.;
// 				spIvPointSave << dStrike;
// 				spIvPointSave << nExpiryID;
// 				spIvPointSave << (pResult->m_dVolatility) * 100.;
// 				spIvPointSave.Execute();
// 			}
// 			catch (exception& e)
// 			{
// 				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodDefaultIvPoint_Save : %s, %s"), typeid(e).name(), e.what());
// 			}
// 		}
// 	}
// 
// 	size_t nAllUnd = spUndColl->size();
// 
// 	size_t nProgress = 100 * m_Request.UndSend() / nAllUnd;
// 
// 	if(m_nProgress != nProgress)
// 	{
// 		m_nProgress = nProgress;
// 		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
// 		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
// 	}
// 
// 	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

EODMS::CMsUndCollPtr CEodStepLoadDefaultIv::_GetUndWithEmtpyIvCurve(EODMS::CMsUndCollPtr pUndColl)
{
	EODMS::CMsUndCollPtr spNewUndColl(new EODMS::CMsUndColl);

	EODMS::CMsUndCollPtr spUndColl(pUndColl);
	ATLASSERT(spUndColl);

	for(EODMS::CMsUndColl::iterator iter = spUndColl->begin(); iter != spUndColl->end(); iter++)
	{
		EODMS::CMsUndPtr spUnd(iter->second);
		ATLASSERT(spUnd);

		EODMS::CMsUndExpCollPtr spUndExp(spUnd->EmptyIvExp());

		if(spUndExp && spUndExp->size())
			spNewUndColl->insert(EODMS :: CMsUndColl::value_type (spUnd->m_sSymbol, spUnd));
	}
	return spNewUndColl;
}

}
