#include "StdAfx.h"
#include "EodLoadSkewKurtosis.h"

namespace EOD
{

DWORD CEodStepLoadSkewKurtosis::Stop()
{
	CancelRequest();
	return 	CThread::Stop();
}

EodStepsMaskEnum CEodStepLoadSkewKurtosis::StepMask()
{
	return enSmLoadSkewKurtosis;
}

EodStepsMaskEnum CEodStepLoadSkewKurtosis::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepLoadSkewKurtosis::EodStepMainFunc()
{
	Report(StepMask(),enRpMtStart,_T("Started"));

	EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();

	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	EODMS::CMsUndCollPtr spUndColl = spMktStrMgr->UndColl();

	m_spDB.Open(spSettingsMgr->DbConnection().GetString(), spSettingsMgr->DbConnectionTimeout(), spSettingsMgr->DbCommandTimeout());

	if(spUndColl->size())
	{
		m_Request.Init(spSettingsMgr, spUndColl);

		RequestIvData(&m_Request);
	}

	m_spDB.Close();

	Report(StepMask(),enRpMtFinishSuccess, _T("Loaded Skew and Kurtosis for %d underlying"),spUndColl->size());
}

void CEodStepLoadSkewKurtosis::OnIvData(EODIV::CIvSkewKurtosisResultPtr pResult)
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
		try
		{
			CStoredProc<> spSkewSave(m_spDB, L"usp_EodUnderlyingSkewKurt_Save");
			spSkewSave << spUnd->m_nID;
			spSkewSave << pResult->m_dSkew;
			spSkewSave << pResult->m_dKurtosis;
			spSkewSave.Execute();
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUnderlyingSkewKurt_Save : %s, %s"), typeid(e).name(), e.what());
		}

		size_t nAllUnd = spUndColl->size();

		size_t nProgress = 100 * m_Request.UndSend() / nAllUnd;

		if(m_nProgress != nProgress)
		{
			m_nProgress = nProgress;
			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
		}
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

}