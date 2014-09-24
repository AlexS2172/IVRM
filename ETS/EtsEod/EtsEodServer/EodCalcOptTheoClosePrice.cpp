#include "StdAfx.h"
#include "EodCalcOptTheoClosePrice.h"

namespace EOD
{

DWORD CEodStepCalcOptTheoClosePrice::Stop()
{
	return 	CThread::Stop();
}

EodStepsMaskEnum CEodStepCalcOptTheoClosePrice::StepMask()
{
	return enSmEmpty;
}

EodStepsMaskEnum CEodStepCalcOptTheoClosePrice::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepCalcOptTheoClosePrice::EodStepMainFunc()
{
// 	Report(StepMask(),enRpMtStart,_T("Started"));
// 
// 	EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();
// 
// 	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();
// 
// 	EODMS::CMsUndByIDCollPtr spAllUndColl(spMktStrMgr->UndByIdColl());
// 
// 	ATLASSERT(spAllUndColl);
// 
// 	EODMS::CMsUndByIDCollPtr spCalcUndColl(_PrepareUndCollection(spAllUndColl));
// 
// 
// 	if(spCalcUndColl && spCalcUndColl->size())
// 	{
// 		m_spDB.Open(spSettingsMgr->DbConnection().GetString(), spSettingsMgr->DbConnectionTimeout(), spSettingsMgr->DbCommandTimeout()); //fokiny
// 
// 		try
// 		{
// 			CStoredProc<> spSavePriceInit(m_spDB, L"usp_EodOptTheoPrice_Init_Save");
// 			spSavePriceInit.Execute();
// 		}
// 		catch (exception& e)
// 		{
// 			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptTheoPrice_Init_Save : %s, %s"), typeid(e).name(), e.what());
// 			throw e;
// 		}
// 		
// 		size_t nUnd = 0;
// 		size_t nAllUnd = spCalcUndColl->size();
// 
// 		EODVP::CVpVolaProviderPtr spVolaProvider(new EODVP::CVpVolaProvider);
// 		ATLASSERT(spVolaProvider);
// 		spVolaProvider->Connect(spSettingsMgr);
// 
// 		for(EODMS::CMsUndByIDColl::iterator iterUnd = spCalcUndColl->begin(); iterUnd != spCalcUndColl->end(); iterUnd++)
// 		{
// 			if(IsStopping())
// 				throw EODCO::CEodStopException();
// 
// 			EODMS::CMsUndPtr spUnd(iterUnd->second);
// 			ATLASSERT(spUnd);
// 
// 			spUnd->SetVolaSource(spVolaProvider->UndVolaSource(spUnd->m_sSymbol, spUnd->Type()));
// 
// 			if(spUnd->Opt()->size())
// 				spUnd->CalculateAllOptionsTheoPrice(spAllUndColl, spSettingsMgr);
// 
// 			EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(iterUnd->second));
// 			if(spIndex && spIndex->FutureOptionWithPos()->size())
// 				spIndex->CalculateFutureOptionsTheoPrice(spSettingsMgr);
// 
// 			spUnd->SetVolaSource(EODVP::CVpUndVolaSourcePtr());
// 
// 			if(spUnd->Opt()->size())
// 				_SaveOptionClosePrice(spUnd);
// 
// 			if(spIndex && spIndex->FutureOptionWithPos()->size())
// 				_SaveFutureOptionClosePrice(spIndex);
// 
// 			_IncrementProgress(nAllUnd, nUnd);
// 		}
// 
// 		try
// 		{
// 			CStoredProc<> spLoadPriceDone(m_spDB, L"usp_EodOptTheoPrice_Complete_Save");
// 			spLoadPriceDone.Execute();
// 		}
// 		catch (exception& e)
// 		{
// 			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptTheoPrice_Complete_Save : %s, %s"), typeid(e).name(), e.what());
// 			throw e;
// 		}
// 
// 		m_spDB.Close();
// 
// 		Report(StepMask(),enRpMtFinishSuccess, _T("Calculated theo close price for %d options & %d future options of %d underlying"), _GetOptionsCount(spCalcUndColl), _GetFutureOptionsCount(spCalcUndColl), spCalcUndColl->size());
// 	}
// 	else
// 	{
// 		m_nProgress = 100L;
// 		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
// 		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
// 		Report(StepMask(),enRpMtFinishSuccess, _T("There are no positions exists in DB"));
// 	}
// 
}

EODMS::CMsUndByIDCollPtr CEodStepCalcOptTheoClosePrice::_PrepareUndCollection(EODMS::CMsUndByIDCollPtr pUndColl)
{
 	EODMS::CMsUndByIDCollPtr spUndColl(new EODMS::CMsUndByIDColl);
// 	for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
// 	{
// 		EODMS::CMsUndPtr spUnd(iterUnd->second);
// 		ATLASSERT(spUnd);
// 
// 		bool bHaveFutureOptionPos = false;
// 		EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(iterUnd->second));
// 		if(spIndex)
// 			bHaveFutureOptionPos = (spIndex->FutureOptionWithPos()->size() != 0L);
// 
// 		if(spUnd->Opt()->size() || bHaveFutureOptionPos)
// 		{
// 			spUndColl->insert(EODMS :: CMsUndByIDPair(spUnd->m_nID, spUnd));
// 		}
// 	}
	return spUndColl;
}

size_t CEodStepCalcOptTheoClosePrice::_GetOptionsCount(EODMS::CMsUndByIDCollPtr pUndColl)
{
	size_t nOptCount = 0L;
// 	for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
// 	{
// 		EODMS::CMsUndPtr spUnd(iterUnd->second);
// 		ATLASSERT(spUnd);
// 
// 		nOptCount += spUnd->Opt()->size();
// 	}
	return static_cast<long>(nOptCount);
}

size_t CEodStepCalcOptTheoClosePrice::_GetFutureOptionsCount(EODMS::CMsUndByIDCollPtr pUndColl)
{
	size_t nOptCount = 0L;
// 	for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
// 	{
// 		EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(iterUnd->second));
// 		if(spIndex)
// 			nOptCount += spIndex->FutureOptionWithPos()->size();
// 	}
	return nOptCount;
}

void CEodStepCalcOptTheoClosePrice::_SaveOptionClosePrice(EODMS::CMsUndPtr pUnd)
{
// 	for(EODMS::CMsOptRootColl::iterator itOptRoot = pUnd->OptRootWithPos()->begin(); itOptRoot != pUnd->OptRootWithPos()->end(); itOptRoot++)
// 	{
// 		const EODMS::CMsOptRootPtr& spOptRoot(itOptRoot->second);
// 		const EODMS::CMsOptionByIDCollPtr& spOptColl = spOptRoot->Opt();
// 
// 		for(EODMS::CMsOptionByIDColl::iterator itOpt = spOptColl->begin(); itOpt != spOptColl->end(); itOpt++)
// 		{
// 			const EODMS::CMsOptionPtr& spOpt(itOpt->second);
// 			if(!EODCO::IsBadValue(spOpt->m_dPriceTheo))
// 			{
// 				try
// 				{
// 					CStoredProc<> spPriceSave(m_spDB, L"usp_EodOptTheoPrice_Save");
// 					spPriceSave << spOpt->m_nID;
// 					spPriceSave << spOpt->m_dPriceTheo;
// 					spPriceSave.Execute();
// 				}
// 				catch (exception& e)
// 				{
// 					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptTheoPrice_Save : %s, %s"), typeid(e).name(), e.what());
// 					throw e;
// 				}
// 			}
// 		}
// 	}
}

void CEodStepCalcOptTheoClosePrice::_SaveFutureOptionClosePrice(EODMS::CMsIndexPtr pIndex)
{
// 	for(EODMS::CMsFutureOptionColl::iterator itOpt = pIndex->FutureOptionWithPos()->begin(); itOpt != pIndex->FutureOptionWithPos()->end(); itOpt++)
// 	{
// 		const EODMS::CMsFutureOptionPtr& spOpt(itOpt->second);
// 		if(!EODCO::IsBadValue(spOpt->m_dPriceTheo))
// 		{
// 			try
// 			{
// 				CStoredProc<> spPriceSave(m_spDB, L"usp_EodOptTheoPrice_Save");
// 				spPriceSave << spOpt->m_nID;
// 				spPriceSave << spOpt->m_dPriceTheo;
// 				spPriceSave.Execute();
// 			}
// 			catch (exception& e)
// 			{
// 				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptTheoPrice_Save : %s, %s"), typeid(e).name(), e.what());
// 				throw e;
// 			}
// 		}
// 	}
}
void CEodStepCalcOptTheoClosePrice::_IncrementProgress(size_t nAllUnderlyings, size_t& nUndCount)
{
// 	size_t nProgress = 100 * ++nUndCount / nAllUnderlyings;
// 
// 	if(m_nProgress != nProgress)
// 	{
// 		m_nProgress = nProgress;
// 		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
// 		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
// 	}
}

}
