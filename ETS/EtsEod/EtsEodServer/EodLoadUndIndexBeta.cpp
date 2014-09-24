#include "StdAfx.h"
#include "EodLoadUndIndexBeta.h"

namespace EOD
{

DWORD CEodStepLoadUndIndexBeta::Stop()
{
	CancelRequest();
	return CThread::Stop();
}

EodStepsMaskEnum CEodStepLoadUndIndexBeta::StepMask()
{
	return enSmEmpty;
}

EodStepsMaskEnum CEodStepLoadUndIndexBeta::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepLoadUndIndexBeta::EodStepMainFunc()
{
// 	Report(StepMask(),enRpMtStart,_T("Started"));
// 
// 	EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();
// 
// 	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();
// 
// 	EODMS::CMsUndCollPtr spUndColl = spMktStrMgr->UndColl();
// 
// 	EODMS::CMsIndexCollPtr spIndexColl(_GetIndexColl(spUndColl));
// 	
// 	m_spDB.Open(spSettingsMgr->DbConnection().GetString(), spSettingsMgr->DbConnectionTimeout(), spSettingsMgr->DbCommandTimeout());
// 
// 	try
// 	{
// 		CStoredProc<> spBetaClear(m_spDB, _T("usp_EodIndexUnderlyingBeta_Clear"));
// 		spBetaClear.Execute();
// 	}
// 	catch (exception& e)
// 	{
// 		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodIndexUnderlyingBeta_Clear : %s, %s"), typeid(e).name(), e.what());
// 		throw e;
// 	}
// 
// 	if(spIndexColl->size())
// 	{
// 		m_Request.Init(spSettingsMgr, spUndColl, spIndexColl);
// 
// 		RequestIvData(&m_Request);
// 	}
// 	
// 	m_spDB.Close();
// 
// 	Report(StepMask(),enRpMtFinishSuccess, _T("Loaded beta between %d underlying and %d indices"),spUndColl->size(), spIndexColl->size());
}

EODMS::CMsIndexCollPtr CEodStepLoadUndIndexBeta::_GetIndexColl(EODMS::CMsUndCollPtr pUndColl)
{
	EODMS::CMsIndexCollPtr spIndexColl(new EODMS::CMsIndexColl);

// 	EODMS::CMsIndexPtr spIndex = boost::shared_dynamic_cast<EODMS::CMsIndex>(GetCollectionItem<EODMS::CMsUndPtr>(pUndColl, _T("DJX")));
// 
// 	if(spIndex)
// 		spIndexColl->insert(EODMS :: CMsIndexColl::value_type (spIndex->m_sSymbol, spIndex));
// 
// 	spIndex = boost::shared_dynamic_cast<EODMS::CMsIndex>(GetCollectionItem<EODMS::CMsUndPtr>(pUndColl, _T("NDX")));
// 
// 	if(spIndex)
// 		spIndexColl->insert(EODMS :: CMsIndexColl::value_type (spIndex->m_sSymbol, spIndex));
// 
// 	spIndex = boost::shared_dynamic_cast<EODMS::CMsIndex>(GetCollectionItem<EODMS::CMsUndPtr>(pUndColl, _T("OEX")));
// 
// 	if(spIndex)
// 		spIndexColl->insert(EODMS :: CMsIndexColl::value_type (spIndex->m_sSymbol, spIndex));
// 
// 	spIndex = boost::shared_dynamic_cast<EODMS::CMsIndex>(GetCollectionItem<EODMS::CMsUndPtr>(pUndColl, _T("SPX")));
// 
// 	if(spIndex)
// 		spIndexColl->insert(EODMS :: CMsIndexColl::value_type (spIndex->m_sSymbol, spIndex));
// 
	return spIndexColl;
}

void CEodStepLoadUndIndexBeta::OnIvData(EODIV::CIvUndIndexBetaResultPtr pResult)
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
// 	EODMS::CMsIndexCollPtr spIndexColl = m_Request.Indexes();
// 
// 	ATLASSERT(spIndexColl);
// 
// 	EODMS::CMsIndexPtr spIndex(GetCollectionItem<EODMS :: CMsIndexPtr>(spIndexColl, pResult->m_strIndexSymbol));
// 
// 	ATLASSERT(spIndex);
// 
// 	if(spUnd && spIndex)
// 	{
// 		try
// 		{
// 			CStoredProc<> spBetaSave(m_spDB, L"usp_EodUnderlyingIndexBeta_Save");
// 			spBetaSave << spUnd->m_nID;
// 			spBetaSave << spIndex->m_nID;
// 			spBetaSave << pResult->m_dBeta;
// 			spBetaSave.Execute();
// 		}
// 		catch (exception& e)
// 		{
// 			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUnderlyingIndexBeta_Save : %s, %s"), typeid(e).name(), e.what());
// 		}
// 
// 		size_t nAllUnd = spUndColl->size();
// 
// 		size_t nProgress = 100 * m_Request.UndSend() / nAllUnd;
// 
// 		if(m_nProgress != nProgress)
// 		{
// 			m_nProgress = nProgress;
// 			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
// 			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
// 		}
// 	}
// 
// 	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

}