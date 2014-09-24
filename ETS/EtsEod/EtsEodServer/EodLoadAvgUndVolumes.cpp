#include "StdAfx.h"
#include "EodLoadAvgUndVolumes.h"

namespace EOD
{

DWORD CEodStepLoadAvgUndVolumes::Stop()
{
	CancelRequest();
	return CThread::Stop();
}

EodStepsMaskEnum CEodStepLoadAvgUndVolumes::StepMask()
{
	return enSmLoadAvgVolumes;
}

EodStepsMaskEnum CEodStepLoadAvgUndVolumes::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepLoadAvgUndVolumes::EodStepMainFunc()
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

	Report(StepMask(),enRpMtFinishSuccess, _T("Loaded average volumes for %d underlyings"),spUndColl->size());
}

void CEodStepLoadAvgUndVolumes::OnIvData(EODIV::CIvAvrgUndVolumesResultPtr pResult)
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
			CStoredProc<> spAvgVolumesSave(m_spDB, L"usp_EodUnderlyingAverageVolumes_Save");
			spAvgVolumesSave << spUnd->m_nID;
			spAvgVolumesSave << pResult->m_nVolume10Day;
			spAvgVolumesSave << pResult->m_nVolume5Expiration;
			spAvgVolumesSave.Execute();
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUnderlyingAverageVolumes_Save : %s, %s"), typeid(e).name(), e.what());
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
