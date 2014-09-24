#include "StdAfx.h"
#include "EodCalcCommVola.h"

namespace EOD
{

	DWORD CEodStepCalcCommVola::Stop()
	{
		return 	CThread::Stop();
	}

	EodStepsMaskEnum CEodStepCalcCommVola::StepMask()
	{
		return enSmEmpty;
	}

	EodStepsMaskEnum CEodStepCalcCommVola::GetStartMask()
	{
		return enSmEmpty; // ?
	}
	
	long CEodStepCalcCommVola::SaveContractVola(CDBConnection * pDB, long nContractID, double dStrike, DATE dtExpiry, double dVola )
	{
		long lRet = -1; 
// 		try
// 		{
// 
// 			long nExpiryID = EODCO::ExpiryIdFromDate(dtExpiry);
// 			CStoredProc<> spIvPointSave(*pDB, _T("usp_EodDefaultIvPoint_Save"));
// 			spIvPointSave << nContractID;
// 			double dStrikeRounded = floor(dStrike* 1000.) / 1000.;
// 			spIvPointSave << dStrikeRounded;
// 			spIvPointSave << nExpiryID;
// 			spIvPointSave << (dVola) * 100.;
// 			spIvPointSave.Execute();
// 		}
// 		catch (exception& e)
// 		{
// 			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodDefaultIvPoint_Save : %s, %s"), typeid(e).name(), e.what());
// 		}
// 		catch (...)
// 		{
// 			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Unhandled exception in usp_EodDefaultIvPoint_Save"));
// 		}
		return lRet;
	}

	void CEodStepCalcCommVola::EodStepMainFunc()
	{
// 		Report(StepMask(),enRpMtStart,_T("Started"));
// 
// 		
// 		EODSM::CSmSettingsMgrPtr spSettingsMgr = CEodServer::s_pEodServer->SettingsMgr();
// 		EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();
// 		//EOD::CEodStepLoadIndexFuturesPtr spIndexFuturesMgr = CEodServer::s_pEodServer->IndexFuturesMgr();
// 		m_spDB.Open(spSettingsMgr->DbConnection().GetString(), spSettingsMgr->DbConnectionTimeout(), spSettingsMgr->DbCommandTimeout());
// 		
// 		size_t lFuturesCount = 0;
// 		size_t lFutCoounter = 0;
// 		size_t nProgress = 0;
// 		size_t lFuturesCalculated = 0;
// 		long bIsCall = 0;
// 		spMktStrMgr->_CommodityFuturesAdjustRates();
// 		EODMS::CMsFutureByIdCollPtr m_spFutures =  spMktStrMgr->CommFxIrFutures();
// 		//EODMS::CMsFutureByIdCollPtr m_spFutures =  spIndexFuturesMgr->CommodityFutures();
// 		EODMS::CMsFutureOptionCollPtr pFutOpts;
// 		EODMS::CMsFuturePtr spFutureTmp;
// 		EODMS::CMsFuturePtr spFuture;
// 		EODMS::CMsFutureOptionCollPtr spOptColl;
// 		CString strOptSymbol;
// 		double dFutureClosePrice = 0;
// 		long nID = 0;
// 		double dStrike = 0; 
// 		double dVola = 0; 
// 		double dOptionClosePrice = 0;
// 		long nModel = 0;
// 		long nFlag = 0;
// 		double dRate = 0;
// 		long nFutureID = 0;
// 		long nTotalVolaPointsSaved = 0;
// 		long nVolaPointsSaved = 0;
// 
// 		EodCalcModelTypeEnum enModel;
// 		CEodServer::s_pEodServer->get_EodCalcModel(&enModel);
// 
// 		/*vector<HANDLE> vecHandles;
// 		for(CEodStepProcessorBaseColl::iterator iterStep = m_spStepsColl->begin();	iterStep != m_spStepsColl->end(); iterStep++)
// 		{
// 			CEodStepProcessorBasePtr spStep(iterStep->second);
// 
// 			if(enSsInProgress == spStep->Status())
// 			{
// 				HANDLE hStep = spStep->GetStepHandle();
// 				if (spStep->StepMask == enSmLoadCommodity)
// 				{
// 					vecHandles.push_back(hStep);
// 					spStep->SuspendThread(); 
// 				}
// 			}
// 		}
// 		::WaitForMultipleObjects(static_cast<DWORD>(vecHandles.size()),&vecHandles.front(),TRUE,INFINITE);
// 		spStep->ResumeThread();*/
// 
// 		if (m_spFutures != NULL)
// 		{
// 			lFuturesCount = m_spFutures->size();
// 			if (lFuturesCount > 0)
// 			{
// 				double dFutureClosePrice = 0;
// 				for(EODMS::CMsFutureByIdColl::iterator iterUnd = m_spFutures->begin(); iterUnd != m_spFutures->end(); iterUnd++)
// 				{
// 					lFutCoounter++;
// 					//EODMS::CMsFuturePtr spFuture(iterUnd->second);
// 					//spFutureTmp = iterUnd->second;
// 					nFutureID = iterUnd->first;
// 
// 
// 
// 					//spFuture = m_spFutures->Item(nFutureID);
// 					spFuture = GetCollectionItem<EODMS :: CMsFuturePtr>(m_spFutures, nFutureID);
// 					if(spFuture != NULL)
// 					{
// 						dFutureClosePrice =  spFuture->m_dPriceClose;
// 
// 						if (dFutureClosePrice < 0)
// 						{
// 							dFutureClosePrice = _GetContractClosePrice(spFuture->m_nID, &m_spDB); // ??????
// 						}
// 
// 						if (dFutureClosePrice >= 0)
// 						{
// 							spOptColl = spFuture->Opt();
// 							if (spOptColl != NULL )
// 							{
// 								if (spOptColl->size() > 0)
// 								{
// 									nVolaPointsSaved = _FutureOptsCalcFutureOptionVola(nFutureID, spOptColl, dFutureClosePrice, &m_spDB, spMktStrMgr, enModel, spFuture->m_nIsAmerican );
// 									nTotalVolaPointsSaved += nVolaPointsSaved;
// 
// 									if (nVolaPointsSaved > 0)
// 									{
// 										lFuturesCalculated++;
// 										Report(StepMask(), enRpMtMessage, _T("%s %d vola points calculated..."),spFuture->m_sSymbol,  nVolaPointsSaved );
// 									}
// 									else
// 									{
// 										Report(StepMask(), enRpMtMessage, _T("%s 0 points saved"),	spFuture->m_sSymbol);
// 									}
// 
// 								}
// 								else
// 								{
// 									Report(StepMask(), enRpMtMessage, _T("%s no future options, nothing calculated"),spFuture->m_sSymbol);
// 								}
// 								
// 							}
// 						}
// 						else
// 						{
// 							Report(StepMask(), enRpMtMessage, _T("%s no close price, nothing calculated"),spFuture->m_sSymbol);
// 						}
// 
// 					}
// 					//nProgress =(long)((lFutCoounter/lFuturesCount)*100);
// 					//Report(StepMask(),enRpMtProgress, _T("%d"),  nProgress);
// 					_IncrementProgress(lFuturesCount, lFutCoounter);
// 				} // for
// 
// 
// 
// 
// 
// 			} // } //if (lFuturesCount > 0)
// 
// 		} // if (m_spFutures != NULL)
// 
// 		
// 		if ((m_spFutures == NULL)||(lFuturesCount <= 0))
// 		{
// 			m_nProgress = 100L;
// 			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
// 			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
// 			Report(StepMask(),enRpMtFinishSuccess, _T("There are no positions exists in DB"));
// 		
// 		}
// 		else
// 		{
// 			m_nProgress = 100L;
// 			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
// 			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
// 			Report(StepMask(),enRpMtFinishSuccess, _T("Saved %d volatility points for %d futures"), nTotalVolaPointsSaved, lFuturesCalculated);
// 		}
// 		
// 		m_spDB.Close();
// 
	}
	

	
	void CEodStepCalcCommVola::_IncrementProgress(size_t nAllUnderlyings, size_t& nUndCount)
	{
		size_t nProgress = 100 * ++nUndCount / nAllUnderlyings;
		
		if(m_nProgress != nProgress)
		{
			m_nProgress = nProgress;
			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
		}
		
	}


	double CEodStepCalcCommVola::_GetContractClosePrice(long nContractID, CDBConnection * pDB)
	{
		double dClosePrice = -1;
		try
		{
			CStoredProc<CDefaultRecordset> rsPriceGet(*pDB , _T("usp_EodContractClosePrice_Get"));
			rsPriceGet << nContractID;
			rsPriceGet.Open();
			if(!rsPriceGet.IsEOF())
			{
				dClosePrice = rsPriceGet[_T("PriceClose")];
			}
			rsPriceGet.Close();
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in GetContractClosePrice : %s, %s"), typeid(e).name(), e.what());

		}
		catch (...)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Unhandled exception in GetContractClosePrice"));

		}

		return dClosePrice;
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	long CEodStepCalcCommVola::_FutureOptsCalcFutureOptionVola(long nFutureID, EODMS::CMsFutureOptionCollPtr pFutOpts, double dFutureClosePrice, CDBConnection * pDB, EOD::CEodMktStructureMgrPtr pMktStrMngrPtr, EodCalcModelTypeEnum enModel, long nIsAmerican)
	{
		double dFirstClosePrice = 0;
		double dSecondClosePrice = 0;
		double dVola = 0;
		double dFirstVola = 0;
		double dSecondVola = 0;
		double dFirstStrike = 0;
		double dFirstExp = 0;
		double dSecondStrike = 0;
		double dSecondExp = 0;
		long lFirstIsCall = 0;
		long lSecondIsCall = 0;
		bool bFirstIsUsed = false;
		bool bSecondIsUsed = false;
		
		long nModel = 0;
		long nVolaPointsSaved = 0;
		
		bool bGotPair = false;

		DATE dtTemp = 0.;
		dtTemp = vt_date::GetCurrentDate();
		LONG nToday = static_cast<LONG>(dtTemp);

		if (pFutOpts != NULL)
		{
			for(EODMS::CMsFutureOptionColl::iterator iterFutOpts = pFutOpts->begin(); iterFutOpts != pFutOpts->end(); iterFutOpts++)
			{
				EODMS::CMsFutureOptionPtr spFutOpt(iterFutOpts->second);
				if(spFutOpt != NULL)
				{
					
					bFirstIsUsed = spFutOpt->m_bUsed;
					if (!bFirstIsUsed)
					{
						dFirstClosePrice = spFutOpt->m_dPriceClose;
						lFirstIsCall = spFutOpt->m_bIsCall;
						dFirstStrike = spFutOpt->m_dStrike;
						dFirstExp = spFutOpt->m_dtExpiration;

						spFutOpt->m_bUsed = true;
						// Calc First Item Vola
						if (dFirstClosePrice < 0)
						{
							dFirstClosePrice = _GetContractClosePrice(spFutOpt->m_nID, pDB);
						}
						if (dFirstClosePrice >= 0)
						{
							dFirstVola = _FutOptCalcFutureOptionVolatility(spFutOpt->m_dRate, dFutureClosePrice, dFirstClosePrice, dFirstStrike, ((long)dFirstExp - nToday), lFirstIsCall, nIsAmerican, (long)enModel);
						}
						else
						{
							Report(StepMask(), enRpMtMessage, _T("%s no close price, nothing calculated"),spFutOpt->m_sSymbol);
						}


						bGotPair = false;
						// find pair
						for(EODMS::CMsFutureOptionColl::iterator iterPuts = pFutOpts->begin(); iterPuts != pFutOpts->end(); iterPuts++)
						{
							EODMS::CMsFutureOptionPtr spPut(boost::shared_dynamic_cast<EODMS::CMsFutureOption>(iterPuts->second));
							if(spPut != NULL)
							{
								lSecondIsCall = spPut->m_bIsCall;
								dSecondStrike = spPut->m_dStrike;
								dSecondExp = spPut->m_dtExpiration;
								dSecondClosePrice = spPut->m_dPriceClose;
								bSecondIsUsed = spPut->m_bUsed;
								if ((!bSecondIsUsed)&&((lSecondIsCall != lFirstIsCall)&&(dFirstStrike == dSecondStrike)&&(dFirstExp == dSecondExp)))
								{
									// Got pair
									bGotPair = true;
									spPut->m_bUsed = true;


									// Calc second vola
									if (dSecondClosePrice < 0)
									{
										dSecondClosePrice = _GetContractClosePrice(spPut->m_nID, pDB);
									}
									
									if (dSecondClosePrice >= 0)
									{
										dSecondVola = _FutOptCalcFutureOptionVolatility(spPut->m_dRate, dFutureClosePrice, dSecondClosePrice, dSecondStrike, ((long)dSecondExp - nToday), lSecondIsCall, nIsAmerican, (long)enModel);
									}
									else
									{
										Report(StepMask(), enRpMtMessage, _T("%s no close price, nothing calculated"),	spPut->m_sSymbol);
									}
									dVola = -1;

									//Store vola for OTM one
									if (lFirstIsCall == 1)
									{
										if ((dFirstVola > 0)&&((dFirstStrike > dFutureClosePrice)||(dSecondVola <= 0)))
										{
											dVola = dFirstVola;
										}

										if ((dSecondVola > 0)&&((dSecondStrike < dFutureClosePrice)||(dFirstVola <= 0)))
										{
											dVola = dSecondVola;
										}
									} // first is call, second is put
									else
									{
										if ((dSecondVola > 0)&&((dSecondStrike > dFutureClosePrice)||(dFirstVola <= 0)))
										{
											dVola = dFirstVola;
										}

										if ((dFirstVola > 0)&&((dFirstStrike < dFutureClosePrice)||(dSecondVola <= 0)))
										{
											dVola = dSecondVola;
										}


									}

									if (dSecondStrike == dFutureClosePrice)
									{
										if ((dFirstVola <= 0)||(dSecondVola <= 0))
										{
											if (dFirstVola > 0)
												dVola = dFirstVola;

											if (dSecondVola > 0)
												dVola = dSecondVola;

										}
										else
										{
											dVola = (dFirstVola + dSecondVola)/2;
										}


									}

									// store found vola
									if (dVola > 0)
									{
										SaveContractVola(pDB, nFutureID, dFirstStrike, dFirstExp, dVola);
										nVolaPointsSaved++;

									}
									break;
								} //if (lIsNewCall == 0)
							}
						} //for 

						if (!bGotPair)
						{
							// для "Call"  не найден соответствующий "Put"...  или - наоборот...
							if (dFirstVola > 0)
							{
								SaveContractVola(pDB, nFutureID, dFirstStrike, dFirstExp, dFirstVola);
								nVolaPointsSaved++;

							}
						}


					}///if (!bFirstIsUsed)

				} // if(spFutOpt != NULL)
			} //for

		}
		return nVolaPointsSaved;
	}

	
	double CEodStepCalcCommVola::_FutOptCalcFutureOptionVolatility(double dRate, double dFuturePrice, double dOptionPrice, double dStrike, DATE nDTE, long nIsCall, long nIsAmerican, long nModel)
	{
		double dRes = -1;
		long nFlag = 0;
		dRes = CalcFutureOptionVolatility(dRate, dFuturePrice, dOptionPrice, dStrike, (long)nDTE, nIsCall, nIsAmerican, 100,  0, 0, nModel,  &nFlag );
		if (nFlag != 0)
			dRes = -1;

		return dRes;
	}

}

