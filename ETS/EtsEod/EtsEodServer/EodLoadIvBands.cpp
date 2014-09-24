#include "StdAfx.h"
#include "EodLoadIvBands.h"

namespace EOD
{

DWORD CEodStepLoadIvBands::Stop()
{
	CancelRequest();
	return 	CThread::Stop();
}

EodStepsMaskEnum CEodStepLoadIvBands::StepMask()
{
	return enSmLoadIVBands;
}

EodStepsMaskEnum CEodStepLoadIvBands::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepLoadIvBands::EodStepMainFunc()
{
	Report(StepMask(),enRpMtStart,_T("Started"));

	EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();

	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	EODMS::CMsUndCollPtr spUndColl = spMktStrMgr->UndColl();

	m_spDB.Open(spSettingsMgr->DbConnection().GetString(), spSettingsMgr->DbConnectionTimeout(), spSettingsMgr->DbCommandTimeout());

	if(spUndColl->size())
	{
		try
		{
			CStoredProc<> spIvBandsInitSave(m_spDB, L"usp_EodVolaBands_Init_Save");
			spIvBandsInitSave.Execute();		
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodVolaBands_Init_Save : %s, %s"), typeid(e).name(), e.what());
			
			CString sDescription = _T("Exception in usp_EodVolaBands_Init_Save: ");
			sDescription += e.what();
			throw exception(sDescription);
		}

		m_Request.Init(spSettingsMgr, spUndColl);

		RequestIvData(&m_Request);
	}

	if(m_nUndInQuery)
		_SaveBuffer();

	try
	{
		CStoredProc<> spIvBandsCompleteSave(m_spDB, _T("usp_EodVolaBands_Complete_Save"));
		spIvBandsCompleteSave.Execute();		
	}
	catch (exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodVolaBands_Complete_Save : %s, %s"), typeid(e).name(), e.what());

		CString sDescription = _T("Exception in usp_EodVolaBands_Complete_Save: ");
		sDescription += e.what();
		throw exception(sDescription);
	}

	m_spDB.Close();

	Report(StepMask(),enRpMtFinishSuccess, _T("Loaded IV bands for %d underlyings"),spUndColl->size());
}

void CEodStepLoadIvBands::OnIvData(EODIV::CIvIvBandsResultPtr pResult)
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
		if(!EODCO::IsBadValue(pResult->m_dIVBandHigh) && !EODCO::IsBadValue(pResult->m_dIVBandLow))
		{
			CString sExecQuery;
			sExecQuery.Format(_T("exec usp_EodVolaBands_Save %d,%d,%f,%f,%f;"),spUnd->m_nID,pResult->m_nExpPeriod,
								(100. + static_cast<double>(pResult->m_nStrikeMoneyness)) / 100. ,
								pResult->m_dIVBandLow,
								pResult->m_dIVBandHigh);

			m_nUndInQuery++;
			m_sQueryBuffer += sExecQuery;

			if(m_nUndInQuery >= 200L)
				_SaveBuffer();
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

void CEodStepLoadIvBands::_SaveBuffer()
{
	try
	{
		CSQLQuery<> qIvBandsSave(m_spDB, m_sQueryBuffer.GetString());
		qIvBandsSave.Execute();
	}
	catch (exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodVolaBands_Save : %s, %s"), typeid(e).name(), e.what());
	}
	m_nUndInQuery = 0L;
	m_sQueryBuffer.Empty();
}

}
