#include "StdAfx.h"
#include "EodCalcWeightedVega.h"

namespace EOD
{
DWORD CEodStepCalcWeightedVega::Stop()
{
	return 	CThread::Stop();
}

EodStepsMaskEnum CEodStepCalcWeightedVega::StepMask()
{
	return enSmCalculateVega;
}

EodStepsMaskEnum CEodStepCalcWeightedVega::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepCalcWeightedVega::EodStepMainFunc()
{
	Report(StepMask(),enRpMtStart,_T("Started"));

	EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();

	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	EODMS::CMsExpDateCollPtr spExpDates(spMktStrMgr->ExpDate());

	ATLASSERT(spExpDates);

	m_spDB.Open(spSettingsMgr->DbConnection().GetString(), spSettingsMgr->DbConnectionTimeout(), spSettingsMgr->DbCommandTimeout());

	long nMaturity = _GetWtdVegaMaturity();

	if(EODCO::IsBadValue(nMaturity))
		throw logic_error(_T("Invalid maturity value"));

	size_t nAllExpDates = spExpDates->size();
	long nExpDates = 0L;

	for(EODMS::CMsExpDateColl::iterator iterExp = spExpDates->begin(); iterExp != spExpDates->end(); iterExp++)
	{
		if(IsStopping())
			throw EODCO::CEodStopException();

		EODMS::CMsExpDatePtr spExpDate(iterExp->second);
		ATLASSERT(spExpDate);
		_CalculateWtdVega(nMaturity, spExpDate);
		_SaveWtdVega(spExpDate);

		nExpDates++;

		size_t nProgress = 100 * nExpDates / nAllExpDates;

		if(m_nProgress != nProgress)
		{
			m_nProgress = nProgress;
			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
		}

	}
	m_spDB.Close();

	Report(StepMask(),enRpMtFinishSuccess, _T("Calculated Weighted vega for %d expiration dates"), spExpDates->size());
}

void CEodStepCalcWeightedVega::_CalculateWtdVega(long nMaturity, EODMS::CMsExpDatePtr pExpDate)
{			
	pExpDate->m_dWtdVega = floor(sqrt(static_cast<double>(nMaturity) / static_cast<double>(pExpDate->m_nDTE) ) * 10000.) / 10000.;
}

void CEodStepCalcWeightedVega::_SaveWtdVega(EODMS::CMsExpDatePtr pExpDate)
{			
	try
	{
		CStoredProc<> spVegaSave(m_spDB, L"usp_EodWeightedVega_Save");
		spVegaSave << COleDateTime(pExpDate->m_dtExpiration);
		spVegaSave << pExpDate->m_dWtdVega;
		spVegaSave.Execute();
	}
	catch (exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodWeightedVega_Save : %s, %s"), typeid(e).name(), e.what());
	}
}

long CEodStepCalcWeightedVega::_GetWtdVegaMaturity()
{
	try
	{
		CStoredProc<CClientRecordset> rsWtdVegaMaturity(m_spDB, L"usp_WtdVegaMaturity_Get");
		rsWtdVegaMaturity.Open();
		if(!rsWtdVegaMaturity.IsEOF())
		{
			return rsWtdVegaMaturity[_T("iWtdVegaMaturity")];				 
		}
	}
	catch (exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_WtdVegaMaturity_Get : %s, %s"), typeid(e).name(), e.what());
		throw e;
	}	
	return BAD_LONG_VALUE;
}

}
