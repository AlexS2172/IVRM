#include "StdAfx.h"
#include "EodLoadUndClosePrice.h"

namespace EOD
{

void CEodStepLoadUndClosePrice::Init(bool bLoadFromIv)
{
	if(bLoadFromIv)
		m_enCurrentStage = enEodCpLoadFromIv;
	else
		m_enCurrentStage = enEodCpLoadFromPp;
}

EodStepsMaskEnum CEodStepLoadUndClosePrice::StepMask()
{
	return enSmLoadUndClosePrice;
}

EodStepsMaskEnum CEodStepLoadUndClosePrice::GetStartMask()
{
	return enSmEmpty;
}

DWORD CEodStepLoadUndClosePrice::Stop()
{
	if(Status() == enSsInProgress)
	{
		if(enEodCpLoadFromIv == m_enCurrentStage)
			CEodLoadUndClosePriceFromIv::CancelRequest();		
		DWORD dwRes = CThread::Stop();

		if(enEodCpLoadFromPp == m_enCurrentStage)
			CEodLoadUndClosePriceFromPp::CancelRequest();
		else if(enEodCpLoadFutureFromPp == m_enCurrentStage)
			CEodLoadFutClosePriceFromPp::CancelRequest();
		return dwRes;
	}
	else return 0;
}

void CEodStepLoadUndClosePrice::EodStepMainFunc()
{
	EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

	m_spDB.Open(spSettingMgr->DbConnection().GetString(), spSettingMgr->DbConnectionTimeout(), spSettingMgr->DbCommandTimeout());

//	try
//	{
//		CStoredProc<> spLoadPriceInit(m_spDB, _T("usp_EodUndClosePrice_Init_Save"));
//		spLoadPriceInit.Execute();
//	}
//	catch (exception& e)
//	{
//		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUndClosePrice_Init_Save : %s, %s"), typeid(e).name(), e.what());
//		throw e;
//	}

	Report(StepMask(),enRpMtStart, _T("Start loading underlying close price."));

	bool bIsStopped = false;
	bool bIsWithError = false;

	if(enEodCpLoadFromIv == m_enCurrentStage)
	{
		try
		{
			LoadFromIv();
		}
		catch(EODCO::CEodStopException)
		{
			bIsStopped = true;
		}
		catch (exception& e)
		{
			Report(StepMask(),enRpMtWarning, _T("Can't download all prices from IVolatility: %s. Proceed loading from Price Provider."),e.what());
			m_enCurrentStage = enEodCpLoadFromPp;
		}
	}

	if(bIsStopped)
		throw EODCO::CEodStopException();

	if(enEodCpLoadFromPp == m_enCurrentStage)
	{
		try
		{
			CEodLoadUndClosePriceFromPp::LoadFromPp();
		}
		catch(EODCO::CEodStopException)
		{
			bIsStopped = true;
		}
		catch (exception& e)
		{
			Report(StepMask(),enRpMtError, _T("Loading underlying close price from Price Provider finished with error: %s "), e.what());
			bIsWithError = true;
		}		
	}
	if(bIsStopped)
		throw EODCO::CEodStopException();

	m_enCurrentStage = enEodCpLoadFutureFromPp;
	try
	{
		CEodLoadFutClosePriceFromPp::LoadFromPp();
	}
	catch(EODCO::CEodStopException)
	{
		bIsStopped = true;
	}
	catch (exception& e)
	{
		Report(StepMask(),enRpMtError, _T("Loading futures close price finished with error. %s"), e.what());
	}		

	if(bIsStopped)
		throw EODCO::CEodStopException();

	if(!bIsWithError)
		Report(StepMask(),enRpMtFinishSuccess, _T("Finished"));
	else
		Report(StepMask(),enRpMtFinishWithError, _T("Finished with error"));

//	try
//	{
//		CStoredProc<> spLoadPriceInit(m_spDB, L"usp_EodUndClosePrice_Complete_Save");
//		spLoadPriceInit.Execute();
//	}
//	catch (exception& e)
//	{
//		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUndClosePrice_Complete_Save : %s, %s"), typeid(e).name(), e.what());
//		throw e;
//	}

	m_spDB.Close();
}

}