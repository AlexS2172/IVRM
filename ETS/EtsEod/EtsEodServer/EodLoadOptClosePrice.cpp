#include "StdAfx.h"
#include "EodLoadOptClosePrice.h"

namespace EOD
{

void CEodStepLoadOptClosePrice::Init(bool bLoadFromIv)
{
	if(bLoadFromIv)
		m_enCurrentStage = enEodCpLoadFromIv;
	else
		m_enCurrentStage = enEodCpLoadFromPp;
}

EodStepsMaskEnum CEodStepLoadOptClosePrice::StepMask()
{
	return enSmLoadOptClosePrice;
}

EodStepsMaskEnum CEodStepLoadOptClosePrice::GetStartMask()
{
	return enSmEmpty;
}

DWORD CEodStepLoadOptClosePrice::Stop()
{
	if(Status() == enSsInProgress)
	{
		if(enEodCpLoadFromIv == m_enCurrentStage)
			CEodLoadOptClosePriceFromIv::CancelRequest();
		DWORD dwRes = CThread::Stop();

		if(enEodCpLoadFromPp == m_enCurrentStage)
			CEodLoadOptClosePriceFromPp::CancelRequest();
		else if(enEodCpLoadFutureFromPp == m_enCurrentStage)
			CEodLoadFutOptClosePriceFromPp::CancelRequest();
		return dwRes;
	}
	else return 0;
}

void CEodStepLoadOptClosePrice::EodStepMainFunc()
{
	EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

	m_spDB.Open(spSettingMgr->DbConnection().GetString(), spSettingMgr->DbConnectionTimeout(), spSettingMgr->DbCommandTimeout());

//	try
//	{
//		CStoredProc<> spLoadPriceInit(m_spDB, _T("usp_EodOptClosePrice_Init_Save"));
//		spLoadPriceInit.Execute();
//	}
//	catch (exception& e)
//	{
//		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptClosePrice_Init_Save : %s, %s"), typeid(e).name(), e.what());
//		throw e;
//	}

	Report(StepMask(),enRpMtStart, _T("Start loading options close price."));

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
			CEodLoadOptClosePriceFromPp::LoadFromPp();
		}
		catch(EODCO::CEodStopException)
		{
			bIsStopped = true;
		}
		catch (exception& e)
		{
			Report(StepMask(),enRpMtError, _T("Loading options close price from Price Provider finished with error."), e.what());
			bIsWithError = true;
		}		
	}

	if(bIsStopped)
		throw EODCO::CEodStopException();

	m_enCurrentStage = enEodCpLoadFutureFromPp;
	try
	{
		CEodLoadFutOptClosePriceFromPp::LoadFromPp();
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
//		CStoredProc<> spLoadPriceInit(m_spDB, _T("usp_EodOptClosePrice_Complete_Save"));
//		spLoadPriceInit.Execute();
//	}
//	catch (exception& e)
//	{
//		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptClosePrice_Complete_Save : %s, %s"), typeid(e).name(), e.what());
//		throw e;
//	}

	m_spDB.Close();
}

}