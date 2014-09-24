#include "StdAfx.h"
#include "EodLoadIndexFutures.h"

namespace EOD
{

CEodStepLoadIndexFutures::~CEodStepLoadIndexFutures(void)
{
	if(m_hUndReceived)
	{
		::CloseHandle(m_hUndReceived);
		m_hUndReceived = NULL;
	}
	if(m_hAllOptReceived)
	{
		::CloseHandle(m_hAllOptReceived);
		m_hAllOptReceived = NULL;
	}
	if(m_hDisconnect)
	{
		::CloseHandle(m_hDisconnect);
		m_hDisconnect = NULL;
	}
	if(m_hAllFutReceived)
	{
		::CloseHandle(m_hAllFutReceived);
		m_hAllFutReceived = NULL;
	}
	if(m_hAllFutOptReceived)
	{
		::CloseHandle(m_hAllFutOptReceived);
		m_hAllFutOptReceived = NULL;
	}
}

void CEodStepLoadIndexFutures::init(EodStepsMaskEnum mask = enSmEmpty, int bLoadPrices = 0)
{
	m_spFuturesColl = EODMS::CMsFutureByIdCollPtr(new EODMS::CMsFutureByIdColl); 
	m_iMask = mask;
	m_bLoadPrices = bLoadPrices;
}

DWORD CEodStepLoadIndexFutures::Stop()
{
	DWORD dwRes = CThread::Stop();

	if(enEodLoadFuturePricesFromPp == m_enCurrentStage)
		CEodLoadFutClosePriceFromPp::CancelRequest();
	else if(enEodLoadFutureOptionPricesFromPp == m_enCurrentStage)
		CEodLoadFutOptClosePriceFromPp::CancelRequest();

	if(m_hUndReceived)
	{
		CloseHandle(m_hUndReceived);
		m_hUndReceived = NULL;
	}
	if(m_hAllOptReceived)
	{
		CloseHandle(m_hAllOptReceived);
		m_hAllOptReceived = NULL;
	}
	if(m_hDisconnect)
	{
		CloseHandle(m_hDisconnect);
		m_hDisconnect = NULL;
	}
	if(m_hAllFutReceived)
	{
		CloseHandle(m_hAllFutReceived);
		m_hAllFutReceived = NULL;
	}
	if(m_hAllFutOptReceived)
	{
		CloseHandle(m_hAllFutOptReceived);
		m_hAllFutOptReceived = NULL;
	}
	m_spFuturesColl.reset();
	EODPP::CPpStructureProvider::Disconnect();
	return dwRes;
}

EodStepsMaskEnum CEodStepLoadIndexFutures::StepMask()
{
	return m_iMask;
}

EodStepsMaskEnum CEodStepLoadIndexFutures::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepLoadIndexFutures::EodStepMainFunc()
{
// 	CString strStart(_T("Started."));
// 	//strStart += _T(" Load Index Futures.");
// 	Report(StepMask(),enRpMtStart, strStart);
// 
// 	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();
// 
// //	m_spUndColl = spMktStrMgr->UndByIdColl();
// 	switch(m_iMask){
// 	case enSmLoadIndexFutures:
// 		m_spFutRootsColl = spMktStrMgr->FuturesRoot();
// 		break;
// 	case enSmLoadCommodity:
// 		m_spFutRootsColl = spMktStrMgr->FuturesCommoditiesRoot();
// 		break;
// 	case enSmLoadInterest:
// 		m_spFutRootsColl = spMktStrMgr->FuturesInterestRoot();
// 		break;
// 	}
// 
// //	ATLASSERT(m_spUndColl);
// 
// /*	if(m_bOnlyActive)
// 		m_spUndColl = _GetActiveColl();
// */
// 	bool bIsStopped = false;
// /*	try
// 	{
// 		if(m_bCheckMktStrUpdates)
// 			CheckMktStrUpdates(m_bOnlyActive);
// 	}
// 	catch(EODCO::CEodStopException)
// 	{
// 		bIsStopped = true;
// 	}
// 	catch (exception& e)
// 	{
// 		m_bCheckMktStrUpdates = false;
// 		_SetAllNeedUpdate();
// 		Report(StepMask(),enRpMtWarning, _T("Can't check market structure updates on IVolatility: %s. All underlyings will be updated."), e.what());
// 	}
// 
// 	if(bIsStopped)
// 		throw EODCO::CEodStopException();
// */
// 	m_bUpdateFutures = m_spFutRootsColl && m_spFutRootsColl->size();
// 
// 	EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());
// 
// 	Connect(spSettingMgr);
// 
// 	m_spDB.Open(spSettingMgr->DbConnection().GetString(), spSettingMgr->DbConnectionTimeout(), spSettingMgr->DbCommandTimeout());
// 
// 	try
// 	{
// 		if( /*(m_spUndColl && m_spUndColl->size() ) || */m_bUpdateFutures)
// 		{
// 			/*EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());
// 
// 			Connect(spSettingMgr);
// 
// 			m_spDB.Open(spSettingMgr->DbConnection(), spSettingMgr->DbConnectionTimeout(), spSettingMgr->DbCommandTimeout());*/
// 
// 			_DeleteOldOrdersFromDb();
// 
// /*			if(m_spUndColl && m_spUndColl->size())
// 				_LoadUnderlyingsAndOptions(spSettingMgr);
// */
// 			if(m_bUpdateFutures)
// 			{
// 				m_hDisconnect  = CreateEvent(NULL,FALSE,FALSE,NULL);
// 
// 				_LoadFutures(spSettingMgr);
// 				
// 				_LoadFutureOptions(spSettingMgr);
// 			}
// 		}
// 	}
// 	catch(EODCO::CEodStopException)
// 	{
// 		_Uninitialize();
// 		bIsStopped = true;
// 	}
// 	catch (exception& e)
// 	{
// 		_Uninitialize();
// 		throw e;
// 	}
// 	if(bIsStopped)
// 		throw EODCO::CEodStopException();
// 
// 	if(0==m_bLoadPrices){								// 1 => 0 PriceProvider error with Advise()
// 		m_enCurrentStage = enEodLoadFuturePricesFromPp;
// 		CEodLoadFutClosePriceFromPp::SetType(1);
// 		try
// 		{
// 			CEodLoadFutClosePriceFromPp::LoadFromPp();
// 		}
// 		catch(EODCO::CEodStopException)
// 		{
// 			bIsStopped = true;
// 		}
// 		catch (exception& e)
// 		{
// 			Report(StepMask(),enRpMtError, _T("Loading future close price finished with error. %s"), e.what());
// 		}		
// 
// 		if(bIsStopped)
// 			throw EODCO::CEodStopException();
// 
// 		m_enCurrentStage = enEodLoadFutureOptionPricesFromPp;
// 		CEodLoadFutOptClosePriceFromPp::SetType(1);
// 		try
// 		{
// 			CEodLoadFutOptClosePriceFromPp::LoadFromPp();
// 		}
// 		catch(EODCO::CEodStopException)
// 		{
// 			bIsStopped = true;
// 		}
// 		catch (exception& e)
// 		{
// 			Report(StepMask(),enRpMtError, _T("Loading future option close price finished with error. %s"), e.what());
// 		}		
// 
// 		if(bIsStopped)
// 			throw EODCO::CEodStopException();
// 
// 	}
// 	else if(2==m_bLoadPrices){
// 		m_enCurrentStage = enEodLoadFuturePricesFromPp;
// 		CEodLoadFutClosePriceFromPp::SetType(2);
// 		try
// 		{
// 			CEodLoadFutClosePriceFromPp::LoadFromPp();
// 		}
// 		catch(EODCO::CEodStopException)
// 		{
// 			bIsStopped = true;
// 		}
// 		catch (exception& e)
// 		{
// 			Report(StepMask(),enRpMtError, _T("Loading future yest price finished with error. %s"), e.what());
// 		}		
// 
// 		if(bIsStopped)
// 			throw EODCO::CEodStopException();
// 
// 		m_enCurrentStage = enEodLoadFutureOptionPricesFromPp;
// 		CEodLoadFutOptClosePriceFromPp::SetType(2);
// 		try
// 		{
// 			CEodLoadFutOptClosePriceFromPp::LoadFromPp();
// 		}
// 		catch(EODCO::CEodStopException)
// 		{
// 			bIsStopped = true;
// 		}
// 		catch (exception& e)
// 		{
// 			Report(StepMask(),enRpMtError, _T("Loading future option yest price finished with error. %s"), e.what());
// 		}		
// 
// 		if(bIsStopped)
// 			throw EODCO::CEodStopException();
// 
// 		_Uninitialize();
// 
// 	}
// 
// 	m_nProgress = 100;
// 	Report(StepMask(),enRpMtProgress, _T("%d"), 100);
// 	CEodServer::s_pEodServer->StepsMgr()->IncProgress();
// 	
// 	Report(StepMask(),enRpMtFinishSuccess, _T("Finished."));
}

void CEodStepLoadIndexFutures::_RequestUnderlyingStructure()
{
	if(!m_spUndCurr->m_bNeedMsUpdate)
	{
		COleDateTime odtPrevUpdate(m_spUndCurr->m_dtLastMsUpdate);
		Report(StepMask(),enRpMtMessage, _T("'%s' market structure won't be updated (no changes from '%s')."), m_spUndCurr->m_sSymbol,odtPrevUpdate.Format(VAR_DATEVALUEONLY));
		_IncrementImportOptionsProgress();
		SetEvent(m_hAllOptReceived);
		return;
	}

	if(m_bCheckMktStrUpdates)
	{
		if(EODCO::DoubleEQZero(m_spUndCurr->m_dtLastMsUpdate) || m_spUndCurr->m_dtLastMsUpdate < 0.)
		{
			Report(StepMask(),enRpMtMessage, _T("'%s' market structure will be updated (first update of market structure)."), m_spUndCurr->m_sSymbol);
		}
		else if(EODCO::DoubleEQZero(m_spUndCurr->m_dtIvMsUpdate))
		{
			Report(StepMask(),enRpMtMessage, _T("'%s' market structure will be updated (symbol not found in IV DB)."), m_spUndCurr->m_sSymbol);
		}
		else if(m_spUndCurr->m_dtLastMsUpdate < m_spUndCurr->m_dtIvMsUpdate)
		{
			COleDateTime odtPrevUpdate(m_spUndCurr->m_dtLastMsUpdate);
			COleDateTime odtUpdate(m_spUndCurr->m_dtIvMsUpdate);
			Report(StepMask(),enRpMtMessage, _T("'%s' market structure will be updated (update date on '%s', prev.update on '%s' )"), m_spUndCurr->m_sSymbol, odtUpdate.Format(VAR_DATEVALUEONLY),odtPrevUpdate.Format(VAR_DATEVALUEONLY));
		}
	}

	RequestUnderlying(m_spUndCurr->m_sSymbol);
}

void CEodStepLoadIndexFutures::OnStock(struct PP::StockParams* pReq,  struct PP::StockResultsEx* pResult)
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), (LPCTSTR)_bstr_t(pReq->Stock));
	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	CString sSymbol = (LPCTSTR)_bstr_t(pReq->Stock);
	sSymbol.Trim().MakeUpper();
	sSymbol.Replace(_T(" "), _T(""));

	ATLASSERT(m_spUndCurr->m_sSymbol == sSymbol);
	ATLTRACE2("OnStock %s\n", (LPCSTR)sSymbol);

	if(m_spUndCurr->m_sSymbol == sSymbol)
	{
		try
		{

			m_spUndCurr->m_enContractType = (pResult->StockType == PP::enStock ? enStStock : enStIndex);

			CStoredProc<> spUndImport(m_spDB, _T("usp_EodUnderlying_Import"));
			spUndImport << m_spUndCurr->m_nID;
			spUndImport << (pResult->StockType == PP::enStock);
			spUndImport << _bstr_t(pResult->Description);

			if(pResult->DivFreq > 0L)
			{
				spUndImport << pResult->DivFreq;
				spUndImport << pResult->DivAmount;
				COleDateTime dtDiv(pResult->DivLastDate);
				spUndImport << dtDiv;
			}
			else
			{
				spUndImport << CDBNull();
				spUndImport << CDBNull();
				spUndImport << CDBNull();
			}

			if(!EODCO::IsBadValue(pResult->ClosePrice) )
				spUndImport << pResult->ClosePrice;
			else
				spUndImport << CDBNull();
			
			spUndImport << m_bLoadUndClosePrices;
			spUndImport.Execute();
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUnderlying_Import : %s, %s"), typeid(e).name(), e.what());
		}

		try
		{
			CStoredProc<> spStockSave(m_spDB, _T("usp_EodMsUnderlyingCache_Save"));
			spStockSave << m_spUndCurr->m_nID;
			spStockSave.Execute();
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodMsUnderlyingCache_Save : %s, %s"), typeid(e).name(), e.what());
		}
		SetEvent(m_hUndReceived);
	}
	else ATLASSERT(false);
}

void CEodStepLoadIndexFutures::OnOption(struct PP::OptionParams* pReq,  struct PP::OptionResultsEx* pResult, VARIANT_BOOL IsLastOption)
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), (LPCTSTR)_bstr_t(pResult->Series));

	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	CString sUndSymbol = (LPCTSTR)_bstr_t(pReq->UnderlyingSymbol);
	ATLTRACE2("OnOption %s\n", (LPCSTR)_bstr_t(pResult->Series));
	sUndSymbol.Trim().MakeUpper();
	sUndSymbol.Replace(_T(" "), _T(""));
	CString sOptSymbol = (LPCTSTR)_bstr_t(pResult->Series);
	sOptSymbol.Trim().MakeUpper();
	sOptSymbol.Replace(_T(" "), _T(""));

	if(m_spUndCurr->m_sSymbol == sUndSymbol)
	{
		if(IsLastOption)
		{
			m_spUndCurr->m_bMsUpdated = true;

			_FinishLoadUndOptions();

			_IncrementImportOptionsProgress();

			SetEvent(m_hAllOptReceived);
		}
		else if(sOptSymbol.GetLength() > 2)
		{
			CString sRootName = sOptSymbol.Left(sOptSymbol.GetLength() - 2);
			
			EODMS::CMsOptRootPtr spRoot(m_spUndCurr->OptRoot(sRootName));

			bool bNeedStoredInCache = false;
			
			if(!spRoot)
			{
				spRoot = EODMS::CMsOptRootPtr(new EODMS::CMsOptRoot);
				spRoot->m_sName = sRootName;
				spRoot->m_nLotSize = pResult->LotSize;

				try
				{
					CStoredProc<CDefaultRecordset> spRootImport(m_spDB, _T("usp_EodOptionRoot_Import"));
					spRootImport << m_spUndCurr->m_nID;
					spRootImport << _bstr_t(sRootName.GetString());
					spRootImport << pResult->LotSize;
					spRootImport.Open();
					spRoot->m_nID = spRootImport.GetResult();

					if(spRoot->m_nID < 0L)
					{
						bNeedStoredInCache = true;
						if(!spRootImport.IsEOF())
						{							
							CString sDescr =(LPCSTR) _bstr_t(spRootImport[_T("vcErrorMsg")]);
							Report(StepMask(),enRpMtWarning, _T("Can't insert option '%s' of underlying '%s' : %s"), sOptSymbol, sUndSymbol, sDescr);
						}
					}
				}
				catch (exception& e)
				{
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptionRoot_Save : %s, %s"), typeid(e).name(), e.what());
					Report(StepMask(),enRpMtError, _T("Can't insert option '%s' of underlying '%s' : %s"), sOptSymbol, sUndSymbol, e.what());
					bNeedStoredInCache = true;
				}

				if(spRoot->m_nID > 0L)
					m_spUndCurr->OptRoot()->insert(EODMS::CMsOptRootByNamePair(sRootName, spRoot));
			}

			vt_date dtExpiration(pResult->ExpirationDate);
			EgLib::vt_date dtTradingClose;
			EgLib::vt_date dtExpiryOV = GetDefaultValuationParams(	EgLib::vt_date(pResult->ExpirationDate),
																	m_spUndCurr->m_enContractType,
																	enStOption,
																	pResult->SettlementType,
																	pResult->ExpirationType,
																	dtTradingClose);
			if(!bNeedStoredInCache)
			{
				try
				{
					if(pResult->StrikePrice > 0.)
					{
						//long nExpiryID = EODMS::CMsContract::GetExpiryID((DATE)dtExpiration);
						COleDateTime dtFormat(dtExpiration);

						CString strContractName;
						strContractName.Format(_T("%s %s %s %.02f"), sUndSymbol, dtFormat.Format(_T("%B %Y")), 
							pResult->OptionType == PP::enCall ? _T("Call") : _T("Put"), pResult->StrikePrice );

						CStoredProc<> spOptionImport(m_spDB, _T("usp_EodOption_Import"));
						spOptionImport << spRoot->m_nID;
						spOptionImport << dtExpiration;
						spOptionImport << dtExpiryOV;
						spOptionImport << dtTradingClose;
						spOptionImport << pResult->StrikePrice;
						spOptionImport << (pResult->OptionType == PP::enCall);
						spOptionImport << _bstr_t (sOptSymbol.GetString());
						spOptionImport << _bstr_t (strContractName.GetString());

						long nBadOptionID = 0L;

						spOptionImport << dtExpiration;
						spOptionImport << CDBNull();
						spOptionImport << m_bLoadOptClosePrices;


						spOptionImport.Execute();

						long nRes = spOptionImport.GetResult();
						if(nRes < 0L)
							bNeedStoredInCache =true;

						if((nRes == -2L || nRes == -3L) && nBadOptionID)
						{
							CString badContract;
							try
							{
								CStoredProc<CDefaultRecordset> rsOptionGet(m_spDB, L"usp_EodOptionName_Get");
								rsOptionGet << nBadOptionID;
								rsOptionGet.Open();
								if(!rsOptionGet.IsEOF())
								{
									badContract = (LPCSTR)_bstr_t(rsOptionGet[L"vcContractName"]);
								}
							}
							catch (exception& e)
							{
								Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOption_Import : %s, %s"), typeid(e).name(), e.what());

							}
							if(nRes == -2L)				
								Report(StepMask(),enRpMtWarning, _T("Can't insert option '%s'(%s) because there exists option '%s'(%s) with position"), sOptSymbol, strContractName, sOptSymbol, badContract );
							else if(nRes == -3L)				
								Report(StepMask(),enRpMtWarning, _T("Can't insert option '%s'(%s) because there exists option '%s'(%s) with orders"), sOptSymbol, strContractName, sOptSymbol, badContract );
						}
						else if(nRes < 0L)
						{
							Report(StepMask(),enRpMtWarning, _T("Can't insert option '%s'(%s)"), sOptSymbol, strContractName );
						}	
					}
				}
				catch (exception& e)
				{
					bNeedStoredInCache = true;
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOption_Import : %s, %s"), typeid(e).name(), e.what());
					Report(StepMask(),enRpMtError, _T("%s"), e.what());
				}
			}

			if(bNeedStoredInCache)
			{
				try
				{
					if(pResult->StrikePrice > 0.)
					{
						CStoredProc<> spOptionSave(m_spDB, _T("usp_EodMsOptionCache_Save"));
						spOptionSave << m_spUndCurr->m_nID;
						spOptionSave << _bstr_t(sOptSymbol.GetString());
						spOptionSave << COleDateTime(pResult->ExpirationDate);
						spOptionSave << dtExpiryOV;
						spOptionSave << dtTradingClose;
						spOptionSave << pResult->StrikePrice;
						spOptionSave << (pResult->OptionType == PP::enCall);
						spOptionSave << pResult->LotSize;

						spOptionSave.Execute();
					}
				}
				catch (exception& e)
				{
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodMsOptionCache_Save : %s, %s"), typeid(e).name(), e.what());
				}
			}
		}
		else
			ATLASSERT(false);
	}
	else
		ATLASSERT(false);
}

void CEodStepLoadIndexFutures::OnGetStockError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::StockParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sUndSymbol = (LPCTSTR)_bstr_t(pReq->Stock);
	ATLASSERT(m_spUndCurr->m_sSymbol == sUndSymbol);

	Report(StepMask(), enRpMtWarning, _T("Underlying downloading error %d - %s for '%s'"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCTSTR)_bstr_t(pReq->Stock));
	SetEvent(m_hUndReceived);
}

void CEodStepLoadIndexFutures::OnGetOptionError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::OptionParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sUndSymbol = (LPCTSTR)_bstr_t(pReq->UnderlyingSymbol);

	ATLASSERT(m_spUndCurr->m_sSymbol == sUndSymbol);
	Report(StepMask(), enRpMtWarning, _T("Options downloading error %d - %s for '%s' underlying"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), sUndSymbol);

	if(m_spUndCurr)
		m_spUndCurr->m_bMsUpdated = false;

	_FinishLoadUndOptions();

	_IncrementImportOptionsProgress();

	SetEvent(m_hAllOptReceived);
}

void CEodStepLoadIndexFutures::OnGetFutureByRootError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::FutureRootParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sRootSymbol = (LPCTSTR)_bstr_t(pReq->FuturesRootSymbol);

	ATLASSERT(m_spFutRootCurr->m_sRootSymbol == sRootSymbol);
	Report(StepMask(), enRpMtWarning, _T("Futures downloading error %d - %s for '%s' futures root"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), sRootSymbol);

	_FinishLoadRootFutures();

	m_spFutRootCurr->m_bMsUpdated = false;

	IncrementImportFuturesProgress();

	SetEvent(m_hAllFutReceived);
}

void CEodStepLoadIndexFutures::OnFutureByRoot(struct PP::FutureRootParams* pReq,  struct PP::FutureResultsEx* pResult, VARIANT_BOOL IsLastFuture)
{
	if(_IsCancel() || IsStopping())
		return;
	_bstr_t bsSymbol(pResult->Symbol);
	CString sFutSymbol = (LPCTSTR)bsSymbol;
	sFutSymbol.Trim().MakeUpper();
	sFutSymbol.Replace(_T(" "), _T(""));

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), (LPCTSTR)sFutSymbol.GetString());
	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	CString sRootSymbol = (LPCTSTR)_bstr_t(pReq->FuturesRootSymbol);
	sRootSymbol.Trim().MakeUpper();
	sRootSymbol.Replace(_T(" "), _T(""));

	if(m_spFutRootCurr->m_sRootSymbol == sRootSymbol)
	{
		if(!IsLastFuture)
		{			
			EODMS::CMsFuturePtr spFut(new EODMS::CMsFuture);
			spFut->m_dtMaturity = /*EODMS::CMsContract::GetExpiryID(*/pResult->ExpirationDate/*)*/;
			spFut->m_sSymbol = sFutSymbol;
			COleDateTime dtExpiration(pResult->ExpirationDate);

			try
			{
				CStoredProc<> spFutImport(m_spDB, L"usp_EodFuture_Import");
				spFutImport << m_spFutRootCurr->m_nRootId;
				spFutImport << vt_date(spFut->m_dtMaturity);
				spFutImport << _bstr_t(sFutSymbol.GetString());
				spFutImport << _bstr_t(pResult->Description);
				spFutImport << pResult->ContractSize;
				spFutImport << pResult->OptionContractSize;
				
				long nIsActive = 0;
				spFutImport << &nIsActive;

				_bstr_t bsBadContractName;
				spFutImport.SetSize(255);
				spFutImport << & bsBadContractName;

				if(!EODCO::IsBadValue(pResult->ClosePrice))
					spFutImport << pResult->ClosePrice;
				else
					spFutImport << CDBNull();

				if(!EODCO::IsBadValue(pResult->MinimumTick))
					spFutImport << pResult->MinimumTick;
				else
					spFutImport << CDBNull();

				spFutImport << pResult->UnitOfMeasure;				
				spFutImport << _bstr_t(pResult->UnitOfMeasureCurrency);
				//spFutImport << dtExpiration;

				spFutImport.Execute();
				spFut->m_nID = spFutImport.GetResult();
				if(spFut->m_nID > 0L)
				{
					spFut->m_bIsActive = (nIsActive != 0);
					if(!IsExists(m_spFuturesColl,spFut->m_nID) && (spFut->m_bIsActive || !m_bOnlyActive))
					{
						m_spFuturesColl->insert(EODMS :: CMsFutureByIdPair(spFut->m_nID, spFut));
						spMktStrMgr->CommFxIrFutures()->insert(EODMS :: CMsFutureByIdPair(spFut->m_nID, spFut));
					}
				}
				else if(spFut->m_nID == -2L)
				{
					Report(StepMask(),enRpMtError, _T("Can't insert future '%s' of future root '%s' : future '%s' with open position and/or open options positions already exist in DB"), sFutSymbol, sRootSymbol, (LPCSTR)bsBadContractName);
				}
				else if(spFut->m_nID == -3L)
				{
					Report(StepMask(),enRpMtError, _T("Can't insert future '%s' of future root '%s' : future '%s' with orders and/or options orders already exist in DB"), sFutSymbol, sRootSymbol, (LPCSTR)bsBadContractName);
				}
				else
				{
					Report(StepMask(),enRpMtError, _T("Can't insert future '%s' of future root '%s'"), sFutSymbol, sRootSymbol);
				}
			}
			catch (exception& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFuture_Import : %s, %s"), typeid(e).name(), e.what());
				Report(StepMask(),enRpMtError, _T("Can't insert future '%s' of future root '%s' : %s"), sFutSymbol, sRootSymbol, e.what());
			}
		}
		else
		{
			_FinishLoadRootFutures();

			m_spFutRootCurr->m_bMsUpdated = true;

			IncrementImportFuturesProgress();
			SetEvent(m_hAllFutReceived);
		}
	}
	else
		ATLASSERT(false);

}
void CEodStepLoadIndexFutures::OnGetFuturesOptionError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::FuturesOptionParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sFutSymbol = (LPCTSTR)_bstr_t(pReq->FutureSymbol);

	ATLASSERT(m_spFutureCurr->m_sSymbol == sFutSymbol);
	Report(StepMask(), enRpMtWarning, _T("Future options downloading error %d - %s for '%s' future"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), sFutSymbol);

	_FinishLoadFutures();

	m_spFutureCurr->m_bMsUpdated = false;

	_IncrementImportFuturesOptionsProgress();

	SetEvent(m_hAllFutOptReceived);
}
void CEodStepLoadIndexFutures::OnFuturesOption(struct PP::FuturesOptionParams* pReq,  struct PP::FuturesOptionResultsEx* pResult, VARIANT_BOOL IsLastOption)
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), (LPCTSTR)_bstr_t(pResult->Series));

	CString sFutSymbol = (LPCTSTR)_bstr_t(pReq->FutureSymbol);
	sFutSymbol.Trim().MakeUpper();
	sFutSymbol.Replace(_T(" "), _T(""));
	CString sOptSymbol = (LPCTSTR)_bstr_t(pResult->Series);
	sOptSymbol.Trim().MakeUpper();
	//sOptSymbol.Replace(_T(" "), _T(""));

	if(m_spFutureCurr->m_sSymbol == sFutSymbol)
	{
		if(!IsLastOption)
		{

			EODMS::CMsFutureOptionPtr spFutOpt(new EODMS::CMsFutureOption);
			spFutOpt->m_bIsCall = (pResult->OptionType == PP::enCall);
			spFutOpt->m_dtExpiration = pResult->ExpirationDate;
			spFutOpt->m_dStrike = pResult->StrikePrice;
			spFutOpt->m_sSymbol = sOptSymbol;
			COleDateTime dtExpiration(pResult->ExpirationDate);
			EgLib::vt_date dtTradingClose;
			EgLib::vt_date dtExpiryOV = GetDefaultValuationParams(EgLib::vt_date(pResult->LastTradingDate), enStFuture, enStFutureOption, PP::enSTUndefined, 0, dtTradingClose);

			CString strContractName;
			strContractName.Format(_T("%s %s %s %.02f"), sFutSymbol, dtExpiration.Format(_T("%B %Y")), 
				pResult->OptionType == PP::enCall ? _T("Call") : _T("Put"), pResult->StrikePrice );

			try
			{				
				CStoredProc<CDefaultRecordset> spFutOptImport(m_spDB, L"usp_EodFutureOption_Import");
				spFutOptImport << m_spFutureCurr->m_nID;
				spFutOptImport << spFutOpt->m_dStrike;
				spFutOptImport << vt_date( /*EODMS::CMsContract::GetExpiryID(*/spFutOpt->m_dtExpiration);
				spFutOptImport << dtExpiryOV;	
				spFutOptImport << dtTradingClose;
				spFutOptImport << spFutOpt->m_bIsCall;
				spFutOptImport << _bstr_t(spFutOpt->m_sSymbol.GetString());
				spFutOptImport << _bstr_t(strContractName.GetString());

				_bstr_t bsBadContractName;
				spFutOptImport.SetSize(255);
				spFutOptImport << &bsBadContractName;

				if(!EODCO::IsBadValue(pResult->ClosePrice))
					spFutOptImport << pResult->ClosePrice;
				else
					spFutOptImport << CDBNull();


				spFutOptImport.Open();
				spFutOpt->m_nID = spFutOptImport.GetResult();
				if(spFutOpt->m_nID > 0L)
				{
					if(!IsExists(m_spFutureCurr->Opt(),sOptSymbol))
						m_spFutureCurr->Opt()->insert(EODMS::CMsFutureOptionPair (sOptSymbol, spFutOpt));
				}
				else if(spFutOpt->m_nID == -2L)
				{
					Report(StepMask(),enRpMtError, _T("Can't insert future option '%s' of future '%s' : future option '%s' with open position already exist in DB"), sOptSymbol, sFutSymbol, (LPCSTR)bsBadContractName);
				}
				else if(spFutOpt->m_nID == -3L)
				{
					Report(StepMask(),enRpMtError, _T("Can't insert future option '%s' of future '%s' : future option '%s' with orders already exist in DB"), sOptSymbol, sFutSymbol, (LPCSTR)bsBadContractName);
				}
				else
				{
					Report(StepMask(),enRpMtError, _T("Can't insert future option '%s' of future '%s'"), sOptSymbol, sFutSymbol);
				}

			}
			catch (exception& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFutureOption_Import : %s, %s"), typeid(e).name(), e.what());
				Report(StepMask(),enRpMtError, _T("Can't insert future option '%s' of future '%s' : %s"), sOptSymbol, sFutSymbol, e.what());
			}
		}
		else
		{
			_FinishLoadFutures();

			m_spFutureCurr->m_bMsUpdated = true;

			_IncrementImportFuturesOptionsProgress();
			SetEvent(m_hAllFutOptReceived);
		}
	}
	else
		ATLASSERT(false);

}

void CEodStepLoadIndexFutures::OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	//ATLASSERT(false);
}

void CEodStepLoadIndexFutures::OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	Report(StepMask(), enRpMtError, _T("Disconnect from Structure provider: %d %s"),enErrorNumber, (LPCTSTR)_bstr_t(bsDescription));
	SetEvent(m_hDisconnect);
}

void CEodStepLoadIndexFutures::Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	long nTry = 0L;
	while(true)
	{
		if(nTry)
		{
			DWORD dwRes = ::WaitForSingleObject(_StopEventHandle(), 1000 * pSettingsMgr->PpWaitInterval());
			if(dwRes == WAIT_OBJECT_0)
				throw EODCO::CEodStopException();
		}
		try
		{
			EODPP::CPpStructureProvider::Connect(pSettingsMgr);
			break;
		}
		catch (exception& e)
		{
			nTry++;
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Error connect to Structure provider: %s %s"), typeid(e).name(), e.what());

			if(nTry > pSettingsMgr->PpReconnectAttempt())
			{
				Report(StepMask(), enRpMtError, _T("Error connect to Structure provider: %s"), e.what());
				throw e;
			}
			else
				Report(StepMask(), enRpMtWarning, _T("Error connect to Structure provider: %s. Try to recconnect"), e.what());
		}
	}
}

EODMS::CMsUndByIDCollPtr CEodStepLoadIndexFutures::_GetActiveColl()
{
	EODMS::CMsUndByIDCollPtr spUndColl(new EODMS::CMsUndByIDColl);
	for(EODMS::CMsUndByIDColl::iterator iterUnd = m_spUndColl->begin(); iterUnd != m_spUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->m_bIsActive)
			spUndColl->insert(EODMS::CMsUndByIDPair(spUnd->m_nID, spUnd));
	}
	return spUndColl;
}

void CEodStepLoadIndexFutures::_SetAllNeedUpdate()
{
	for(EODMS::CMsUndByIDColl::iterator iterUnd = m_spUndColl->begin(); iterUnd != m_spUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		spUnd->m_bNeedMsUpdate = true;
		spUnd->m_bMsUpdated = false;
	}
}

void CEodStepLoadIndexFutures::_GetUpdatedUndCount(long& nTotalCount, long& nUpdatedCount)
{
	nTotalCount = 0L;
	nUpdatedCount = 0L;
	for(EODMS::CMsUndByIDColl::iterator iterUnd = m_spUndColl->begin(); iterUnd != m_spUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->m_bNeedMsUpdate)
			nTotalCount++;
		if(spUnd->m_bMsUpdated)
			nUpdatedCount++;
	}
}
void CEodStepLoadIndexFutures::_FinishLoadUndOptions()
{

	bool bOptDeleteError = false;
	if(m_bDeleteOldOptions)
	{
		try
		{
			CString strUndeletedOptions;
			CStoredProc<CDefaultRecordset> rsOldOptionsDel(m_spDB, _T("usp_EodExpiredOptions_Del"));
			rsOldOptionsDel << m_spUndCurr->m_nID;
			rsOldOptionsDel.Open();

			if(!rsOldOptionsDel.IsEOF())
				bOptDeleteError = true;

			while(!rsOldOptionsDel.IsEOF())
			{
				if(strUndeletedOptions.IsEmpty())
					strUndeletedOptions += _T("'");
				else
					strUndeletedOptions += _T(", '");
				CString strSymbol =(LPCSTR) _bstr_t(rsOldOptionsDel[_T("vcSymbol")]);
				strUndeletedOptions += strSymbol;
				strUndeletedOptions += _T("'");
				++rsOldOptionsDel;
			}

			if(!strUndeletedOptions.IsEmpty())
				Report(StepMask(), enRpMtWarning, _T("Can't delete the following old options of '%s' underlying because of existing positions: %s"), m_spUndCurr->m_sSymbol, strUndeletedOptions);

		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodExpiredOptions_Del : %s, %s"), typeid(e).name(), e.what());
		}
	}
	if(!m_bDeleteOldOptions || !bOptDeleteError)
	{
		try
		{
			CStoredProc<> spUndCheck(m_spDB, _T("usp_EodMsUnderlyingCache_Check"));
			spUndCheck << m_spUndCurr->m_nID;
			spUndCheck.Execute();
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodMsUnderlyingCache_Check : %s, %s"), typeid(e).name(), e.what());
			Report(StepMask(), enRpMtMessage, _T("'%s' underlying updated incompletely."), m_spUndCurr->m_sSymbol);
		}
	}
}

void CEodStepLoadIndexFutures::_DeleteOldOrdersFromDb()
{
	try
	{
		CStoredProc<> spDeleteOldOrders(m_spDB, _T("usp_FixOldOrder_Del"));
		spDeleteOldOrders.Execute();
	}
	catch (exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_FixOldOrder_Del : %s, %s"), typeid(e).name(), e.what());
		throw e;
	}
}

void CEodStepLoadIndexFutures::_InitMsLoadInDb()
{
	try
	{
		CStoredProc<> spMsLoadInit(m_spDB, _T("usp_EodMsLoad_Init"));
		spMsLoadInit.Execute();
	}
	catch (exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodMsLoad_Init : %s, %s"), typeid(e).name(), e.what());
		throw e;
	}
}

void CEodStepLoadIndexFutures::_CompleteMsLoadInDb(long& nUndNeedReUpdate)
{
	try
	{
		CString strUndeletedOptions;
		CStoredProc<CDefaultRecordset> rsMsLoadDone(m_spDB, _T("usp_EodMsLoad_Complete"));
		rsMsLoadDone.Open();

		while(!rsMsLoadDone.IsEOF())
		{
			nUndNeedReUpdate++;
			++rsMsLoadDone;
		}
	}
	catch (exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodMsLoad_Complete : %s, %s"), typeid(e).name(), e.what());
	}
}

void CEodStepLoadIndexFutures::_Uninitialize()
{
	m_spDB.Close();

	EODPP::CPpStructureProvider::Disconnect();

	if(m_hUndReceived)
	{
		CloseHandle(m_hUndReceived);
		m_hUndReceived = NULL;
	}
	if(m_hAllOptReceived)
	{
		CloseHandle(m_hAllOptReceived);
		m_hAllOptReceived = NULL;
	}
	if(m_hDisconnect)
	{
		CloseHandle(m_hDisconnect);
		m_hDisconnect = NULL;
	}
}

void CEodStepLoadIndexFutures::_LoadUnderlyingsAndOptions(EODSM::CSmSettingsMgrPtr pSettings)
{
	Report(StepMask(),enRpMtMessage, _T("Loading stocks, indexes and their options..."));

	_InitMsLoadInDb();

	EODMS::CMsUndByIDColl::iterator itNextUnd = m_spUndColl->begin();
	ATLASSERT(itNextUnd != m_spUndColl->end());

	HANDLE hStopEvent = GetStopEventHandle();

	m_hUndReceived = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hDisconnect  = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hAllOptReceived = CreateEvent(NULL,FALSE,FALSE,NULL);

	const HANDLE hEvents[] = {hStopEvent, m_hUndReceived, m_hAllOptReceived, m_hDisconnect};

	SetEvent(m_hAllOptReceived);

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE );

		if(dwRes == WAIT_OBJECT_0)
			throw EODCO::CEodStopException();

		else if(dwRes == WAIT_OBJECT_0 + 1)
		{
			RequestOptions(m_spUndCurr->m_sSymbol);
		}
		else if(dwRes == WAIT_OBJECT_0 + 2)
		{
			if(itNextUnd == m_spUndColl->end())
				break;

			m_spUndCurr  = itNextUnd->second;
			ATLASSERT(m_spUndCurr);
			itNextUnd++;
			_RequestUnderlyingStructure();
		}
		else if(dwRes == WAIT_OBJECT_0 + 3)
		{
			Connect(pSettings);
			itNextUnd--;
			SetEvent(m_hAllOptReceived);
		}
	}

	long nUndNeedReUpdate=0L;
	_CompleteMsLoadInDb(nUndNeedReUpdate);

	long nTotalCount,nUpdatedCount;

	_GetUpdatedUndCount(nTotalCount, nUpdatedCount);

	ATLASSERT(nUndNeedReUpdate <= nUpdatedCount);

	if(nUndNeedReUpdate)
		Report(StepMask(),enRpMtMessage, _T("%d underlying was updated of %d requested. %d underlying updated incompletely."), nUpdatedCount - nUndNeedReUpdate, nTotalCount, nUndNeedReUpdate );
	else
		Report(StepMask(),enRpMtMessage, _T("%d underlying was updated of %d requested."), nUpdatedCount, nTotalCount );
}

void CEodStepLoadIndexFutures::_LoadFutures(EODSM::CSmSettingsMgrPtr pSettings)
{
	Report(StepMask(),enRpMtMessage, _T("Loading futures..."));

	EODMS::CMsFuturesRootColl::iterator itNextRoot = m_spFutRootsColl->begin();
	ATLASSERT(itNextRoot != m_spFutRootsColl->end());

	HANDLE hStopEvent = GetStopEventHandle();

	m_hAllFutReceived = CreateEvent(NULL,FALSE,FALSE,NULL);

	const HANDLE hEvents[] = {hStopEvent, m_hAllFutReceived, m_hDisconnect};

	SetEvent(m_hAllFutReceived);

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE );

		if(dwRes == WAIT_OBJECT_0)
			throw EODCO::CEodStopException();
		else if(dwRes == WAIT_OBJECT_0 + 1)
		{
			if(itNextRoot == m_spFutRootsColl->end())
				break;
			m_spFutRootCurr  = itNextRoot->second;
			itNextRoot++;
			ATLASSERT(m_spFutRootCurr);
			RequestFutures(m_spFutRootCurr->m_sRootSymbol);
		}
		else if(dwRes == WAIT_OBJECT_0 + 2)
		{
			Connect(pSettings);
			itNextRoot--;
			SetEvent(m_hAllFutReceived);
		}
	}

	long nTotalCount,nUpdatedCount;

	_GetUpdatedFutRootCount(nTotalCount, nUpdatedCount);

	Report(StepMask(),enRpMtMessage, _T("%d futures roots was updated of %d requested."), nUpdatedCount, nTotalCount);
}

void CEodStepLoadIndexFutures::_LoadFutureOptions(EODSM::CSmSettingsMgrPtr pSettings)
{
	Report(StepMask(),enRpMtMessage, _T("Loading futures options..."));

	EODMS::CMsFutureByIdColl::iterator itNextFut = m_spFuturesColl->begin();
	ATLASSERT(itNextFut != m_spFuturesColl->end());

	HANDLE hStopEvent = GetStopEventHandle();

	m_hAllFutOptReceived = CreateEvent(NULL,FALSE,FALSE,NULL);

	const HANDLE hEvents[] = {hStopEvent, m_hAllFutOptReceived, m_hDisconnect};

	SetEvent(m_hAllFutOptReceived);

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE );

		if(dwRes == WAIT_OBJECT_0)
			throw EODCO::CEodStopException();
		else if(dwRes == WAIT_OBJECT_0 + 1)
		{
			if(itNextFut == m_spFuturesColl->end())
				break;
			m_spFutureCurr  = itNextFut->second;
			itNextFut++;
			ATLASSERT(m_spFutureCurr);

			Trace(EODCO::enTrLogDebug,__FUNCTION__, _T("Start loading options for %s futures"), (LPCTSTR)m_spFutureCurr->m_sSymbol);

			RequestFuturesOptions(m_spFutureCurr->m_sSymbol);
		}
		else if(dwRes == WAIT_OBJECT_0 + 2)
		{
			Connect(pSettings);
			itNextFut--;
			SetEvent(m_hAllFutOptReceived);
		}
	}
	long nTotalCount,nUpdatedCount;

	_GetUpdatedFutCount(nTotalCount, nUpdatedCount);

	Report(StepMask(),enRpMtMessage, _T("%d futures was updated of %d requested."), nUpdatedCount, nTotalCount);
}

void CEodStepLoadIndexFutures::_FinishLoadRootFutures()
{
	if(m_bDeleteOldOptions)
	{
		try
		{
			CString strUndeletedFutures;
			CStoredProc<CDefaultRecordset> rsOldFuturesDel(m_spDB, _T("usp_EodOldFutures_Del"));
			rsOldFuturesDel << m_spFutRootCurr->m_nRootId;
			rsOldFuturesDel.Open();

			while(!rsOldFuturesDel.IsEOF())
			{
				if(strUndeletedFutures.IsEmpty())
					strUndeletedFutures += _T("'");
				else
					strUndeletedFutures += _T(", '");
				_bstr_t strSymbol = rsOldFuturesDel[_T("vcSymbol")];
				strUndeletedFutures +=(LPCSTR) strSymbol;
				strUndeletedFutures += _T("'");
				++rsOldFuturesDel;
			}

			if(!strUndeletedFutures.IsEmpty())
				Report(StepMask(), enRpMtWarning, _T("Can't delete all futures of '%s' future root because of existing positions: %s"), m_spFutRootCurr->m_sRootSymbol, strUndeletedFutures);

		}
		catch(_com_error&)
		{ return ; }
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOldFutures_Del : %s, %s"), typeid(e).name(), e.what());
			Report(StepMask(), enRpMtWarning, _T("Can't delete all futures of '%s' future root because of error: %s"), m_spFutRootCurr->m_sRootSymbol, e.what());
		}
	}
}

void CEodStepLoadIndexFutures::_FinishLoadFutures()
{
	if(m_bDeleteOldOptions)
	{
		try
		{
			CString strUndeletedFuturesOpt;
			CStoredProc<CDefaultRecordset> rsOldFuturesOptDel(m_spDB, _T("usp_EodOldFutureOptions_Del"));
			rsOldFuturesOptDel << m_spFutureCurr->m_nID;
			rsOldFuturesOptDel.Open();

			while(!rsOldFuturesOptDel.IsEOF())
			{
				if(strUndeletedFuturesOpt.IsEmpty())
					strUndeletedFuturesOpt += _T("'");
				else
					strUndeletedFuturesOpt += _T(", '");
				_bstr_t strSymbol = rsOldFuturesOptDel[_T("vcSymbol")];
				strUndeletedFuturesOpt += (LPCSTR)strSymbol;
				strUndeletedFuturesOpt += _T("'");
				++rsOldFuturesOptDel;
			}

			if(!strUndeletedFuturesOpt.IsEmpty())
				Report(StepMask(), enRpMtWarning, _T("Can't delete the following old futures options of '%s' future because of existing positions: %s"), m_spFutureCurr->m_sSymbol, strUndeletedFuturesOpt);

		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOldFutureOptions_Del : %s, %s"), typeid(e).name(), e.what());
			Report(StepMask(), enRpMtWarning, _T("Can't delete the old futures options of '%s' future because of error: %s"), m_spFutureCurr->m_sSymbol, e.what());
		}
	}
}

void CEodStepLoadIndexFutures::_IncrementImportOptionsProgress()
{
	size_t nAllUnd = m_spUndColl->size();

	double dPersent = static_cast<double>(++m_nLoadedUnd) / nAllUnd;
	
	long nTotal = 100L - (m_bCheckMktStrUpdates ? 10L : 0L) - (m_bUpdateFutures ? 10L : 0L);

	long nProgress = static_cast<long>(nTotal * dPersent) + (m_bCheckMktStrUpdates ? 10L : 0L);

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}
void CEodStepLoadIndexFutures::IncrementImportFuturesProgress()
{
	size_t nAllFut = m_spFutRootsColl->size();

	double dPersent = static_cast<double>(++m_nLoadedFutRoots) / nAllFut;

	byte p = 25;
	if(!m_bLoadPrices)
		p*=2;

	long nProgress = /*90L + */static_cast<long>(dPersent * p);

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}
void CEodStepLoadIndexFutures::_IncrementImportFuturesOptionsProgress()
{
	size_t nAllFut = m_spFuturesColl->size();

	double dPersent = static_cast<double>(++m_nLoadedFutures) / nAllFut;

	byte p = 25;
	if(!m_bLoadPrices)
		p*=2;

	long nProgress = /*95*/p + static_cast<long>(dPersent * p);

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}

void CEodStepLoadIndexFutures::_IncrementImportFuturesProgress()
{
	long*m_nLoadedFuture;
	size_t nAllFut;
	int m_nStartProgress;
	if(m_enCurrentStage == enEodLoadFutureOptionPricesFromPp){
		m_nStartProgress = 75;
		nAllFut = m_lNumOptions;
		m_nLoadedFuture = &(CEodLoadFutOptClosePriceFromPp::m_nLoadedFuture);
	}
	else{
		m_nStartProgress = 50;
		nAllFut = m_spFutColl->size();
		m_nLoadedFuture = &(CEodLoadFutClosePriceFromPp::m_nLoadedFuture);
	}

	double dPersent = static_cast<double>(++*m_nLoadedFuture) / nAllFut;

	long nProgress = m_nStartProgress + static_cast<long>(dPersent * 25L);

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}

void CEodStepLoadIndexFutures::_GetUpdatedFutRootCount(long& nTotalCount, long& nUpdatedCount)
{
	nTotalCount = 0L;
	nUpdatedCount = 0L;
	for(EODMS::CMsFuturesRootColl::iterator iterFut = m_spFutRootsColl->begin(); iterFut != m_spFutRootsColl->end(); iterFut++)
	{
		EODMS::CMsFuturesRootPtr spRoot(iterFut->second);
		ATLASSERT(spRoot);

		nTotalCount++;
		if(spRoot->m_bMsUpdated)
			nUpdatedCount++;
	}
}

void CEodStepLoadIndexFutures::_GetUpdatedFutCount(long& nTotalCount, long& nUpdatedCount)
{
	nTotalCount = 0L;
	nUpdatedCount = 0L;
	for(EODMS::CMsFutureByIdColl::iterator iterFut = m_spFuturesColl->begin(); iterFut != m_spFuturesColl->end(); iterFut++)
	{
		EODMS::CMsFuturePtr spFut(iterFut->second);
		ATLASSERT(spFut);

		nTotalCount++;
		if(spFut->m_bMsUpdated)
			nUpdatedCount++;
	}
}

void CEodStepLoadIndexFutures::_PrepareFutureCollection(EODMS::CMsUndByIDCollPtr pUndColl)
{
	CEodLoadFutClosePriceFromPp::m_nLoadedFuture = 0L;
	m_spFutColl->clear();

	for(EODMS::CMsFutureByIdColl::iterator iterFut = m_spFuturesColl->begin(); iterFut != m_spFuturesColl->end(); iterFut++){
		m_spFutColl->insert(EODMS::CMsFuturePair(iterFut->second->m_sSymbol, iterFut->second));
		/*for(EODMS::CMsFutureOptionColl::iterator iterOpt = iterFut->second->Opt()->begin(); iterOpt != iterFut->second->Opt()->end(); iterOpt++){
			m_spFutColl->Add(iterOpt->first, iterOpt->second);
		}*/
	}

	/*for(EODMS::CMsFutureByIdColl::iterator iterFut = m_spFuturesColl->begin(); iterFut != m_spFuturesColl->end(); iterFut++)
		m_spFutColl->Add(iterFut->second->m_sSymbol, iterFut->second);*/
}
void CEodStepLoadIndexFutures::_PrepareIndexCollection(EODMS::CMsUndByIDCollPtr pUndColl)
{
	CEodLoadFutOptClosePriceFromPp::m_nLoadedFuture = 0L;

	/*for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{*/
	m_lNumOptions = 0;
	for(EODMS::CMsFutureByIdColl::iterator iterFut = m_spFuturesColl->begin(); iterFut != m_spFuturesColl->end(); iterFut++){
		EODMS::CMsIndexPtr spIndex(new EODMS::CMsIndex);
		if(spIndex)
		{
			spIndex->m_sSymbol = iterFut->second->m_sSymbol;
			spIndex->m_nID = iterFut->second->m_nID;
			EODMS::CMsFutureOptionCollPtr spOptColl(iterFut->second->Opt());
			ATLASSERT(spOptColl);
			for(EODMS::CMsFutureOptionColl::iterator iterOpt = spOptColl->begin(); iterOpt != spOptColl->end(); iterOpt++)
			{
				EODMS::CMsFutureOptionPtr spOpt(iterOpt->second);
				spIndex->FutureOptionWithPos()->insert(EODMS :: CMsFutureOptionPair (spOpt->m_sSymbol, spOpt));
				m_lNumOptions++;
			}
			if(spIndex->FutureOptionWithPos()->size())
				m_spIndexColl->insert(EODMS :: CMsIndexByIdPair (spIndex->m_nID, spIndex));
		}
	}
}
}