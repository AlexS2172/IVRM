#include "StdAfx.h"
//#include "EodStepManager.h"
#include "EodLoadMktStructure.h"
#include "EodLoadMktStrUpdates.h"
#include "EodLoadUndIndexBeta.h"
#include "EodLoadAvgUndVolumes.h"
#include "EodLoadIvBands.h"
#include "EodLoadSkewKurtosis.h"
#include "EodLoadDefaultIv.h"
#include "EodLoadUndClosePrice.h"
#include "EodLoadOptClosePrice.h"
#include "EodCalcWeightedVega.h"
#include "EodCalcOptTheoClosePrice.h"
#include "EodLoadIndexFutures.h"
#include "EodCalcCommVola.h"

namespace EOD
{

	void CEodStepManager::Start(unsigned long nStepsMask)
	{
		Report(enSmEmpty, enRpMtStart, _T("Started"));
		Status(enStStartPending);
		CEodServer::s_pEodServer->Fire_OnEodStatusChanged(m_enStatus);

		EODSM::CSmSettingsMgrPtr spSettingsMgr(CEodServer::s_pEodServer->SettingsMgr());
		ATLASSERT(spSettingsMgr);
		CEodMktStructureMgrPtr spMktStrMgr(CEodServer::s_pEodServer->MktStructureMgr());
		ATLASSERT(spMktStrMgr);

		Report(enSmEmpty, enRpMtMessage, _T("Loading base data from Database..."));
		spMktStrMgr->LoadBaseData();
		Report(enSmEmpty, enRpMtMessage, _T("%d underlying loaded from Database..."), spMktStrMgr->UndColl()->size());

		// 	if((nStepsMask & enSmLoadDefaultIV) && (nStepsMask & enSmpOverwriteExistingIV) )
		// 	{
		// 		Report(enSmEmpty, enRpMtMessage, _T("Loading expirations without volatility curves from Database..."));
		// 		spMktStrMgr->LoadEmptyIvExpirations();
		// 	}

		if(	nStepsMask & enSmLoadMktStructure )
		{
			Report(enSmEmpty, enRpMtMessage, _T("Loading options roots data from Database..."));
			spMktStrMgr->LoadOptionRootsData();
		}

		if(	nStepsMask & enSmLoadOptClosePrice
			||	nStepsMask & enSmLoadUndClosePrice)
		{
			Report(enSmEmpty, enRpMtMessage, _T("Loading positions data from Database..."));
			spMktStrMgr->LoadPositionsData();
		}

		//	if(	/*
		//	nStepsMask & enSmLoadOptClosePrice
		//		||	nStepsMask & enSmLoadUndClosePrice
		//	
		//	||*/nStepsMask & enSmCalculateTheoPrice)
		//	{
		//		Report(enSmEmpty, enRpMtMessage, _T("Loading positions data from Database..."));
		//		spMktStrMgr->LoadPositionsData();
		//	}

		if(	nStepsMask & enSmCalculateVega )
		{
			Report(enSmEmpty, enRpMtMessage, _T("Loading expirations from Database for Weighted Vega calculations..."));
			spMktStrMgr->LoadExpiryDates();
		}

		_AllStepsMask(nStepsMask & enSmAllSteps);
		_CompletedStepsMask(enSmEmpty);

		_CreateStepsColl(nStepsMask);

		Status(enStStarted);
		Report(enSmEmpty, enRpMtMessage, _T("Eod Server successfully initialized."));

		CEodServer::s_pEodServer->Fire_OnEodStatusChanged(m_enStatus);
		_Progress(0L);
	}

	void CEodStepManager::Stop()
	{
		if(Status() == enStStopped)
		{
			Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return;
		}	

		Report(enSmEmpty, enRpMtMessage, _T("Forcedly stopping all EOD steps"));

		vector<HANDLE> vecHandles;

		for(CEodStepProcessorBaseColl::iterator iterStep = m_spStepsColl->begin();	iterStep != m_spStepsColl->end(); iterStep++)
		{
			CEodStepProcessorBasePtr spStep(iterStep->second);

			if(enSsInProgress == spStep->Status())
			{
				HANDLE hStep = spStep->GetStepHandle();
				vecHandles.push_back(hStep);
				spStep->Stop();
			}
		}
		if(!vecHandles.empty())
			::WaitForMultipleObjects(static_cast<DWORD>(vecHandles.size()),&vecHandles.front(),TRUE,INFINITE);

		m_spStepsColl->clear();

		_AllStepsMask(enSmEmpty);
		_CompletedStepsMask(enSmEmpty);
		Status(enStStopped);

		Report(enSmEmpty, enRpMtFinishWithError, _T("EOD Forcedly stopped"));
		CEodServer::s_pEodServer->Fire_OnEodStatusChanged(m_enStatus);
	}

	void CEodStepManager::RescheduleSteps(vector<HANDLE>& vecSteps)
	{
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

		if(Status() != enStStarted)
		{
			Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return;
		}

		// Check completed steps
		unsigned long nComplMask = 0L;
		for(CEodStepProcessorBaseColl::iterator iterStep = m_spStepsColl->begin();	iterStep != m_spStepsColl->end(); iterStep++)
		{
			CEodStepProcessorBasePtr spStep(iterStep->second);

			if(enSsInactive != spStep->Status() && enSsInProgress != spStep->Status())
			{
				nComplMask |= spStep->StepMask();		
			}
		}

		m_nCompletedStepsMask = nComplMask;

		if(m_nAllStepsMask != m_nCompletedStepsMask)
		{
			// start all postponed steps
			for(CEodStepProcessorBaseColl::iterator iterStep = m_spStepsColl->begin();	iterStep != m_spStepsColl->end(); iterStep++)
			{
				CEodStepProcessorBasePtr spStep(iterStep->second);

				CEodStepStatus enStatus = spStep->Status();
				if(enSsInactive == enStatus)
				{
					unsigned long nStartMask = spStep->GetStartMask() & m_nAllStepsMask;
					/*

					if( iterStep->second->StepMask() == enSmCalcCommVola && (m_nCompletedStepsMask & enSmLoadCommodity) )
					spStep->Start(NULL);

					if( iterStep->second->StepMask() != enSmCalcCommVola && ((nStartMask & m_nCompletedStepsMask) == nStartMask) )
					spStep->Start(NULL);

					if( iterStep->second->StepMask() == enSmCalcCommVola && (m_nCompletedStepsMask & enSmLoadMktStructure) )
					spStep->Start(NULL);

					if( iterStep->second->StepMask() != enSmCalcCommVola && ((nStartMask & m_nCompletedStepsMask) == nStartMask) )
					spStep->Start(NULL);
					*/

					if((nStartMask & m_nCompletedStepsMask) == nStartMask)
						spStep->Start(NULL);

				}

				HANDLE hStep = spStep->GetStepHandle();
				if(hStep && ( enStatus == enSsInProgress ||  enStatus == enSsInactive) )
					vecSteps.push_back(hStep);
			}
		}
		else
		{
			m_spStepsColl->clear();

			_AllStepsMask(enSmEmpty);
			_CompletedStepsMask(enSmEmpty);
			Status(enStStopped);

			CEodServer::s_pEodServer->Fire_OnEodStatusChanged(m_enStatus);
			Report(enSmEmpty, enRpMtFinishSuccess, _T("Completed"));
		}

		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	}

	void CEodStepManager::_CreateStepsColl(unsigned long nStepsMask)
	{
		ATLASSERT(m_spStepsColl);

		m_spStepsColl->clear();

		// 	if( nStepsMask & enSmLoadBeta )
		// 	{
		// 		CEodStepLoadUndIndexBetaPtr spLoadBeta(new CEodStepLoadUndIndexBeta);
		// 		m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadBeta, spLoadBeta));
		// 	}

		if(nStepsMask & enSmLoadAvgVolumes)
		{
			CEodStepLoadAvgUndVolumesPtr spLoadVolumes(new CEodStepLoadAvgUndVolumes);
			m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadAvgVolumes, spLoadVolumes));
		}

		if(nStepsMask & enSmLoadIVBands)
		{
			CEodStepLoadIvBandsPtr spLoadIvBands(new CEodStepLoadIvBands);
			m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadIVBands, spLoadIvBands));
		}

		if(nStepsMask & enSmLoadSkewKurtosis)
		{
			CEodStepLoadSkewKurtosisPtr spLoadSkewKurtosis(new CEodStepLoadSkewKurtosis);
			m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadSkewKurtosis, spLoadSkewKurtosis));
		}
		// 	if(nStepsMask & enSmLoadDefaultIV)
		// 	{
		// 		CEodStepLoadDefaultIvPtr spLoadDefaultIv(new CEodStepLoadDefaultIv);
		// 
		// 		bool bOwerwriteExistingIv = static_cast<bool>((nStepsMask & enSmpOverwriteExistingIV) != 0L);
		// 		spLoadDefaultIv->Init(bOwerwriteExistingIv);
		// 		m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadDefaultIV, spLoadDefaultIv));
		// 	}
		if(nStepsMask & enSmLoadOptClosePrice)
		{
			/*CEodStepLoadOptClosePricePtr spLoadOptClosePrice(new CEodStepLoadOptClosePrice);
			bool bLoadFromIv = static_cast<bool>((nStepsMask & enSmpOptLoadFromIV) != 0L);
			spLoadOptClosePrice->Init(bLoadFromIv);
			m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadOptClosePrice, spLoadOptClosePrice)); //fokiny*/
		}
		if(nStepsMask & enSmLoadUndClosePrice)
		{
			/*CEodStepLoadUndClosePricePtr spLoadUndClosePrice(new CEodStepLoadUndClosePrice);
			bool bLoadFromIv = static_cast<bool>((nStepsMask & enSmpUndLoadFromIV) != 0L);
			spLoadUndClosePrice->Init(bLoadFromIv);
			m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadUndClosePrice, spLoadUndClosePrice));*/
		}
		if(nStepsMask & enSmLoadMktStructure)
		{
			CEodStepLoadMktStructurePtr spLoadMktStr(new CEodStepLoadMktStructure);
			bool bCheckMktStrUpdates = ((nStepsMask & enSmpCheckMktStrUpdates) != 0L);
			bool bOnlyActive		 = ((nStepsMask & enSmpLoadOnlyActiveUnd) != 0L);
			bool bDeleteOldOptions   = ((nStepsMask & enSmpDeleteOldOptions) != 0L);		
			bool bSaveUndPrices		 = ((nStepsMask & enSmpMarketStructureUndLoadClosePrices)   != 0L);		
			bool bSaveOptPrices		 = ((nStepsMask & enSmpMarketStructureOptLoadClosePrices)   != 0L);		
			bool bFilterDividends    = ((nStepsMask & enSmpMarketStructureFilterWrongDividends) != 0L);		
			bool bOptionsLoad        = ((nStepsMask & enSmpMarketStructureOptionsLoad) != 0L);		

			long lLoadFuturesPrice   = -1;
			if(nStepsMask & enSmpLoadFutures)
			{
				lLoadFuturesPrice = 0; // None
				if(nStepsMask & enSmpLoadFuturesPrice)
					lLoadFuturesPrice = 1; //Close
				else
					if(nStepsMask & enSmpLoadFuturesLastPrice)
						lLoadFuturesPrice = 2; //Last

			}
			spLoadMktStr->Init(bCheckMktStrUpdates, bOnlyActive, bDeleteOldOptions, bSaveUndPrices, bSaveOptPrices, bFilterDividends, bOptionsLoad, lLoadFuturesPrice);
			m_spStepsColl->insert(CEodStepProcessorBasePair (enSmLoadMktStructure, spLoadMktStr));
		}	
		// 	if(nStepsMask & enSmLoadIndexFutures)
		// 	{
		// 		CEodStepLoadIndexFuturesPtr spLoadMktStr(new CEodStepLoadIndexFutures);
		// 		byte m_bLoadPrices = 0;
		// 		if(nStepsMask & enSmLoadIndexFuturesPrice)
		// 			m_bLoadPrices = 1;
		// 		else if(nStepsMask & enSmLoadIndexFuturesYestPrice)
		// 			m_bLoadPrices = 2;
		// 		spLoadMktStr->init(enSmLoadIndexFutures, m_bLoadPrices);
		// 		m_spStepsColl->insert(CEodStepProcessorBasePair(enSmLoadIndexFutures, spLoadMktStr));
		// 	}	
		// 	if(nStepsMask & enSmLoadCommodity)
		// 	{
		// 		CEodStepLoadIndexFuturesPtr spLoadMktStr(new CEodStepLoadIndexFutures);
		// 		byte m_bLoadPrices = 0;
		// 		if(nStepsMask & enSmLoadCommodityFutPrice)
		// 			m_bLoadPrices = 1;
		// 		else if(nStepsMask & enSmLoadCommodityYestPrice)
		// 			m_bLoadPrices = 2;
		// 		spLoadMktStr->init(enSmLoadCommodity, m_bLoadPrices);
		// 		m_spStepsColl->insert(CEodStepProcessorBasePair(enSmLoadCommodity, spLoadMktStr));
		// 	}	
		// 	if(nStepsMask & enSmLoadInterest)
		// 	{
		// 		CEodStepLoadIndexFuturesPtr spLoadMktStr(new CEodStepLoadIndexFutures);
		// 		byte m_bLoadPrices = 0;
		// 		if(nStepsMask & enSmLoadInterestFutPrice)
		// 			m_bLoadPrices = 1;
		// 		else if(nStepsMask & enSmLoadInterestFutYestPrice)
		// 			m_bLoadPrices = 2;
		// 		spLoadMktStr->init(enSmLoadInterest, m_bLoadPrices);
		// 		m_spStepsColl->insert(CEodStepProcessorBasePair(enSmLoadInterest, spLoadMktStr));
		// 	}
		// 	if(nStepsMask & enSmCalculateTheoPrice)
		// 	{
		// 		CEodStepCalcOptTheoClosePricePtr spTheoCalc(new CEodStepCalcOptTheoClosePrice);
		// 		m_spStepsColl->insert(CEodStepProcessorBasePair (enSmCalculateTheoPrice, spTheoCalc));
		// 	}	
		if(nStepsMask & enSmCalculateVega)
		{
			CEodStepCalcWeightedVegaPtr spWtdVegaCalc(new CEodStepCalcWeightedVega);
			m_spStepsColl->insert(CEodStepProcessorBasePair (enSmCalculateVega, spWtdVegaCalc));
		}
		// 	if(nStepsMask & enSmCalcCommVola)
		// 	{
		// 		CEodStepCalcCommVolaPtr spCommVolaCalc(new CEodStepCalcCommVola);
		// 		m_spStepsColl->insert(CEodStepProcessorBasePair(enSmCalcCommVola, spCommVolaCalc));
		// 	}
	}
	void CEodStepManager::IncProgress()
	{
		size_t nProgress = 0L;

		ATLASSERT(m_spStepsColl && m_spStepsColl->size());
		for(CEodStepProcessorBaseColl::iterator iterStep = m_spStepsColl->begin(); iterStep != m_spStepsColl->end(); iterStep++)
		{
			CEodStepProcessorBasePtr spStep(iterStep->second);
			ATLASSERT(spStep);

			if(spStep->Status() != enSsInProgress &&  spStep->Status() != enSsInactive)
				nProgress += 100L;
			else nProgress += spStep->GetProgress();
		}
		nProgress /= m_spStepsColl->size();

		CAutoLock lock(m_Lock);
		if(m_nProgress != nProgress)
		{
			m_nProgress = nProgress;
			Report(enSmEmpty, enRpMtProgress, _T("%d"), m_nProgress);
		}
	}

}
