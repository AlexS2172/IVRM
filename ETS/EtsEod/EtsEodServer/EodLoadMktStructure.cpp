#include "StdAfx.h"
#include "EodLoadMktStructure.h"

namespace EOD
{

void CEodStepLoadMktStructure::Init(
									bool bCheckMktStrUpdates,
									bool bOnlyActive, 
									bool bDeleteOldOptions,  
									bool bLoadUndClosePrices, 
									bool bLoadOptClosePrices, 
									bool bFilterDividends, 
									bool bOptionsLoad,
									long lLoadFuturesPrice
									)
{
	m_bCheckMktStrUpdates	= bCheckMktStrUpdates;
	m_bOnlyActive			= bOnlyActive;
	m_bDeleteOldOptions		= bDeleteOldOptions;
	m_bLoadUndClosePrices	= bLoadUndClosePrices;
	m_bLoadOptClosePrices	= bLoadOptClosePrices;
	m_bFilterDividends		= bFilterDividends;
	m_bOptionsLoad          = bOptionsLoad; 
	m_lLoadFuturesPrice     = lLoadFuturesPrice;
	if(!m_bOptionsLoad)
		m_bLoadOptClosePrices = false;

	m_spFuturesColl = EODMS::CMsFutureByIdCollPtr (new EODMS::CMsFutureByIdColl); 
}

CEodStepLoadMktStructure::~CEodStepLoadMktStructure(void)
{
	m_hUndReceived.Close();
	m_hAllOptReceived.Close();
	m_hDisconnect.Close();
	m_hAllFutReceived.Close();
	m_hAllFutOptReceived.Close();
}

DWORD CEodStepLoadMktStructure::Stop()
{
	DWORD dwRes = CThread::Stop();

	m_hUndReceived.Close();
	m_hAllOptReceived.Close();
	m_hDisconnect.Close();
	m_hAllFutReceived.Close();
	m_hAllFutOptReceived.Close();

	m_spFuturesColl.reset();
	Disconnect();
	return dwRes;
}

EodStepsMaskEnum CEodStepLoadMktStructure::StepMask()
{
	return enSmLoadMktStructure;
}

EodStepsMaskEnum CEodStepLoadMktStructure::GetStartMask()
{
	return enSmEmpty;
}

void CEodStepLoadMktStructure::EodStepMainFunc()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CString strStart(_T("Started."));
	if(m_bCheckMktStrUpdates)
		strStart += _T(" Market structure updates will be checked.");
	if(m_bOnlyActive)
		strStart += _T(" Load only active underlyings.");
	if(m_bDeleteOldOptions)
		strStart += _T(" Options & futures which are not on provider today will be deleted.");
	Report(StepMask(), enRpMtStart, (LPCSTR)strStart);

	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	m_spUndColl = spMktStrMgr->UndColl();
	m_spFutRootsColl = spMktStrMgr->FuturesRoot();

	ATLASSERT(m_spUndColl);

	if(m_bOnlyActive)
		m_spUndColl = _GetActiveColl();

	bool bIsStopped = false;
	try
	{
		if(m_bCheckMktStrUpdates)
			CheckMktStrUpdates(m_bOnlyActive);
	}
	catch(EODCO::CEodStopException)
	{
		bIsStopped = true;
	}
	catch (exception& e)
	{
		m_bCheckMktStrUpdates = false;
		_SetAllNeedUpdate();
		Report(StepMask(), enRpMtWarning, 
			   _T("Can't check market structure updates on IVolatility: %s. All underlyings will be updated."), e.what());
	}

	if(bIsStopped)
		throw EODCO::CEodStopException();

	m_bUpdateFutures = m_lLoadFuturesPrice != -1 && m_spFutRootsColl && m_spFutRootsColl->size();

	try
	{
		if( (m_spUndColl && m_spUndColl->size() ) || m_bUpdateFutures)
		{
			EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

			_Connect(spSettingMgr);

			m_spDB.Open(spSettingMgr->DbConnection().GetString(), spSettingMgr->DbConnectionTimeout(), spSettingMgr->DbCommandTimeout());

			_DeleteOldOrdersFromDb();

			if(m_spUndColl && m_spUndColl->size())
			{
				_LoadUnderlyingsAndOptions(spSettingMgr);
			}

			if(m_bUpdateFutures)
			{
				_LoadFutures(spSettingMgr);
				//Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("_LoadFutures is out"));

				_LoadFutureOptions(spSettingMgr);
				//Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("_LoadFutureOptions is out"));

			}
			_Uninitialize();
			//Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("_Uninitialize is out "));

		}
	}
	catch(EODCO::CEodStopException)
	{
		_Uninitialize();
		bIsStopped = true;
	}
	catch (exception& e)
	{
		_Uninitialize();
		throw e;
	}
	if(bIsStopped)
		throw EODCO::CEodStopException();

	Report(StepMask(), enRpMtFinishSuccess, _T("Finished."));
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodStepLoadMktStructure::_RequestUnderlyingStructure()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(!m_spUndCurr->m_bNeedMsUpdate)
	{
		COleDateTime odtPrevUpdate(m_spUndCurr->m_dtLastMsUpdate);
		Report(StepMask(), enRpMtMessage, 
			_T("'%s' market structure won't be updated (no changes from '%s')."), (LPCSTR)m_spUndCurr->m_sSymbol, (LPCSTR)odtPrevUpdate.Format(VAR_DATEVALUEONLY));
		_IncrementImportOptionsProgress();
		SetEvent(m_hAllOptReceived);
		return;
	}

	if(m_bCheckMktStrUpdates)
	{
		if(EODCO::DoubleEQZero(m_spUndCurr->m_dtLastMsUpdate) || m_spUndCurr->m_dtLastMsUpdate < 0.)
		{
			Report(StepMask(), enRpMtMessage, 
				_T("'%s' market structure will be updated (first update of market structure)."), (LPCSTR)m_spUndCurr->m_sSymbol);
		}
		else if(EODCO::DoubleEQZero(m_spUndCurr->m_dtIvMsUpdate))
		{
			Report(StepMask(), enRpMtMessage, 
				_T("'%s' market structure will be updated (symbol not found in IV DB)."), (LPCSTR)m_spUndCurr->m_sSymbol);
		}
		else if(m_spUndCurr->m_dtLastMsUpdate < m_spUndCurr->m_dtIvMsUpdate)
		{
			COleDateTime odtPrevUpdate(m_spUndCurr->m_dtLastMsUpdate);
			COleDateTime odtUpdate(m_spUndCurr->m_dtIvMsUpdate);
			Report(StepMask(), enRpMtMessage, 
				_T("'%s' market structure will be updated (update date on '%s', prev.update on '%s' )"), (LPCSTR)m_spUndCurr->m_sSymbol, (LPCSTR)odtUpdate.Format(VAR_DATEVALUEONLY), (LPCSTR)odtPrevUpdate.Format(VAR_DATEVALUEONLY));
		}
	}

	RequestUnderlying(m_spUndCurr->m_sSymbol);
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodStepLoadMktStructure::OnStock(struct PP::StockParams* pReq,  struct PP::StockResultsEx* pResult)
{
	if(_IsCancel() || IsStopping())
		return;
	_bstr_t bsStock(pReq->Stock);
	Trace(EODCO::enTrLogLevel5,__FUNCTION__,_T("%s"), (LPCTSTR)bsStock);
	m_nLoadedOptions  = 0 ;
	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	CString sSymbol = (LPCTSTR)_bstr_t(pReq->Stock);
	sSymbol.Trim().MakeUpper();
	sSymbol.Replace(_T(" "), _T(""));

	ATLASSERT(m_spUndCurr->m_sSymbol == sSymbol);
	ATLTRACE2("OnStock %s\n", (LPCSTR)sSymbol);


	if(m_spUndCurr->m_sSymbol == sSymbol)
	{
		bool bSuccess = false;
		try
		{
			m_spUndCurr->m_enContractType = (pResult->StockType == PP::enStock ? enStStock : enStIndex);

			CStoredProc<> spUndImport(m_spDB, _T("usp_EodUnderlying_Import"));
			spUndImport << m_spUndCurr->m_nID;
			spUndImport << (pResult->StockType == PP::enStock);
			spUndImport << _bstr_t(pResult->Description);

			if( m_bFilterDividends && pResult->DivFreq > 0L)
			{
				vt_date dtDiv(pResult->DivLastDate);
				vt_date dtLastAcceptableDay;
				std::string strFreq;
				switch(pResult->DivFreq) 
				{
				case 1://Annualy
					dtLastAcceptableDay = vt_date::GetCurrentDate(true);
					dtLastAcceptableDay -= vt_date_span(365L, 0L);
					strFreq= _T("Annualy");
					break;
				case 2://semiannualy
					dtLastAcceptableDay = vt_date::GetCurrentDate(true);
					dtLastAcceptableDay -= vt_date_span(183L, 0L);
					strFreq= _T("Semiannualy");
					break;
				case 4://quarterly
					dtLastAcceptableDay =  vt_date::GetCurrentDate(true);
					dtLastAcceptableDay -= vt_date_span(91L, 0L);
					strFreq= _T("Quarterly");
					break;
				case 12:
					dtLastAcceptableDay =  vt_date::GetCurrentDate(true); 
					dtLastAcceptableDay -= vt_date_span(31L, 0L);
					strFreq= _T("Monthly");
					break;
				default:
					dtLastAcceptableDay = 0;
					strFreq= _T("Unknown");

				}

                if(dtDiv < dtLastAcceptableDay)
				{

					COleDateTime st(dtDiv);
					Trace(EODCO::enTrLogWarning,__FUNCTION__,
						_T("The dividends provided for %s were filtered out. The Next Div was at %s. Expected frequency: %s"),(LPCSTR)bsStock, (LPCSTR)st.Format(VAR_DATEVALUEONLY),strFreq.c_str());
					Report(StepMask(), enRpMtWarning, 
						_T("The dividends provided for %s were filtered out. The Next Div was at %s. Expected frequency: %s"),(LPCSTR)bsStock, (LPCSTR)st.Format(VAR_DATEVALUEONLY),strFreq.c_str());

					spUndImport << CDBNull();
					spUndImport << CDBNull();
					spUndImport << CDBNull();
				}
				else
				{
					spUndImport << pResult->DivFreq;
					spUndImport << pResult->DivAmount;
					vt_date dtDiv(pResult->DivLastDate);
					spUndImport << dtDiv;
				}
			}
			else
			{
				if(pResult->DivFreq > 0L)
				{
					spUndImport << pResult->DivFreq;
					spUndImport << pResult->DivAmount;
					vt_date dtDiv(pResult->DivLastDate);
					spUndImport << dtDiv;
				}
				else
				{
					spUndImport << CDBNull();
					spUndImport << CDBNull();
					spUndImport << CDBNull();
				}
			}

			if(!EODCO::IsBadValue(pResult->ClosePrice) )
				spUndImport << pResult->ClosePrice;
			else
				spUndImport << CDBNull();
			
			spUndImport << m_bLoadUndClosePrices;
			spUndImport << _bstr_t(pResult->Exchange);
			spUndImport.Execute();
			bSuccess = true;
		}
		catch (exception& e)
		{

			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUnderlying_Import : %s, %s"), typeid(e).name(), e.what());
		}
		catch (_com_error& err)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUnderlying_Import : %s"), err.Description());
		}
		catch(...)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUnderlying_Import"));
		}

		if(!bSuccess)
		{
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
		}
		if(m_bOptionsLoad)
			SetEvent(m_hUndReceived);
		else
		{
			_IncrementImportOptionsProgress();
			SetEvent(m_hAllOptReceived);
		}
	}
	else ATLASSERT(false);
}

void CEodStepLoadMktStructure::OnOption(struct PP::OptionParams* pReq,  struct PP::OptionResultsEx* pResult, VARIANT_BOOL IsLastOption)
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s Strike %f"), (LPCTSTR)_bstr_t(pResult->Series), pResult->StrikePrice);

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
		if(sOptSymbol.GetLength() > 2)
		{
			CString sRootName = sOptSymbol.Left(sOptSymbol.GetLength() - 2);
			++m_nLoadedOptions;
			
			EODMS::CMsOptRootPtr spRoot(m_spUndCurr->OptRoot(sRootName));

			bool bNeedStoredInCache = false;
			
			if(!spRoot)
			{
				spRoot = EODMS::CMsOptRootPtr(new EODMS::CMsOptRoot);
				spRoot->m_sName = sRootName;
				spRoot->m_nLotSize = pResult->LotSize;
               
				try
				{
					Trace(EODCO::enTrLogLevel6,__FUNCTION__,_T("Import Option Root %s for %s"), sRootName.GetString(), sUndSymbol.GetString());
					
					CStoredProc<CClientRecordset> spRootImport(m_spDB, L"usp_EodOptionRoot_Import");
					spRootImport << m_spUndCurr->m_nID;
					spRootImport << _bstr_t(sRootName.GetString());
					spRootImport << pResult->LotSize;

					spRootImport.Open();
					spRoot->m_nID = spRootImport.GetResult();

					if(spRoot->m_nID < 0L)
					{
						bNeedStoredInCache = true;
						if(!spRootImport.GetRecordCount())
						{
							CString sDescr = (LPCSTR)((_bstr_t)spRootImport[L"vcErrorMsg"]);
							Report(StepMask(), enRpMtWarning,
								    _T("Can't insert option '%s' of underlying '%s' : %s"), (LPCTSTR)sOptSymbol, (LPCTSTR)sUndSymbol, (LPCTSTR)sDescr);
						}
						else
						{
							Report(StepMask(),enRpMtWarning, 
								   _T("Can't insert option '%s' of underlying '%s' : Unknown reason"), (LPCTSTR)sOptSymbol, (LPCTSTR)sUndSymbol);

						}
					}					
				}
				catch (exception& e)
				{
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptionRoot_Save : %s, %s"), typeid(e).name(), e.what());
					Report(StepMask(), enRpMtError, 
						_T("Can't insert option '%s' of underlying '%s' : %s"), (LPCSTR)sOptSymbol, (LPCSTR)sUndSymbol, e.what());
					bNeedStoredInCache = true;
				}

				if(spRoot->m_nID > 0L)
					m_spUndCurr->OptRoot()->insert(EODMS::CMsOptRootByNameColl::value_type(sRootName, spRoot));
			}

			COleDateTime dtExpiration(pResult->ExpirationDate);

			EgLib::vt_date dtTradingClose = EgLib::vt_date(1900, 1, 1);
			EgLib::vt_date dtExpiryOV = GetDefaultValuationParams(	EgLib::vt_date(pResult->ExpirationDate),
																	m_spUndCurr->m_enContractType,
																	enStOption,
																	pResult->SettlementType,
																	pResult->ExpirationType,
																	dtTradingClose);
			/*EgLib::vt_date	dtExpiryOV = GetDefaultValuationParams(	EgLib::vt_date(pResult->ExpirationDate),
																	enStOption,
																	pResult->SettlementType,
																	pResult->ExpirationType);*/
			
			if(!bNeedStoredInCache)
			{
				try
				{
					if(pResult->StrikePrice > 0.)
					{
						long nExpiryID = EODMS::CMsContract::GetExpiryID(dtExpiration);

						CString strContractName;
						strContractName.Format(_T("%s %s %s %.02f"), sUndSymbol, dtExpiration.Format(_T("%B,%d %Y")), 
							pResult->OptionType == PP::enCall ? _T("Call") : _T("Put"), pResult->StrikePrice );

						CStoredProc<> spOptionImport(m_spDB, L"usp_EodOption_Import");
						spOptionImport << spRoot->m_nID;
						spOptionImport << vt_date( dtExpiration )/*nExpiryID*/;
						spOptionImport << dtExpiryOV;
						spOptionImport << dtTradingClose;
						spOptionImport << pResult->StrikePrice;
						spOptionImport << (pResult->OptionType == PP::enCall);
						spOptionImport << _bstr_t(sOptSymbol.GetString());
						spOptionImport << _bstr_t(strContractName.GetString());

						long nBadOptionID = 0L;
						spOptionImport << &nBadOptionID;
						double dClosePrice = BAD_DOUBLE_VALUE;
						if(!EODCO::IsBadValue(pResult->CloseBid) || !EODCO::IsBadValue(pResult->CloseAsk))
							dClosePrice =(( !EODCO::IsBadValue(pResult->CloseBid)?pResult->CloseBid:0.0 )+ (!EODCO::IsBadValue(pResult->CloseAsk)?pResult->CloseAsk:0.0))/2;

						if(!EODCO::IsBadValue(dClosePrice))
							spOptionImport << dClosePrice;
						else
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
								CStoredProc<CClientRecordset> rsOptionGet(m_spDB, L"usp_EodOptionName_Get");
								rsOptionGet << nBadOptionID;
								rsOptionGet.Open();
								if(!rsOptionGet.IsEOF())
									badContract = (LPCSTR)((_bstr_t)rsOptionGet[L"vcContractName"]);
							}
							catch (exception& e)
							{
								Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOption_Import : %s, %s"), typeid(e).name(), e.what());
							}
							if(nRes == -2L)				
								Report(StepMask(),enRpMtWarning, 
								       _T("Can't insert option '%s'(%s) because there exists option '%s'(%s) with position"), (LPCSTR)sOptSymbol, (LPCSTR)strContractName, (LPCSTR)sOptSymbol, (LPCSTR)badContract );
							else if(nRes == -3L)				
								Report(StepMask(),enRpMtWarning, 
								      _T("Can't insert option '%s'(%s) because there exists option '%s'(%s) with orders"), (LPCSTR)sOptSymbol, (LPCSTR)strContractName, (LPCSTR)sOptSymbol, (LPCSTR)badContract );
						}
						else if(nRes < 0L)
						{
							Report(StepMask(),enRpMtWarning, _T("Can't insert option '%s'(%s) %d"), (LPCSTR)sOptSymbol, (LPCSTR)strContractName,  nRes);
						}	
					}
				}
				catch (exception& e)
				{
					bNeedStoredInCache = true;
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOption_Import : %s, %s"), typeid(e).name(), e.what());
					Report(StepMask(),enRpMtError, _T("%s"), e.what());
				}
				catch (_com_error& err)
				{
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOption_Import : %s"), (LPCSTR)err.Description());
				}
				catch(...)
				{
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOption_Import"));
				}
			}

			if(bNeedStoredInCache)
			{
				try
				{
					if(pResult->StrikePrice > 0.)
					{
						CStoredProc<> spOptionSave(m_spDB, L"usp_EodMsOptionCache_Save");
						spOptionSave << m_spUndCurr->m_nID;
						spOptionSave << _bstr_t(sOptSymbol.GetString());
						spOptionSave << vt_date(pResult->ExpirationDate);
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
		else if(!IsLastOption)
			{
				CString strWhat;
				strWhat.Format(_T("Incorrect %s symbol length"),(LPCSTR)sUndSymbol);
				Trace(EODCO::enTrLogWarning,__FUNCTION__,strWhat);
				Report(StepMask(),enRpMtWarning, _T("%s"), (LPCTSTR)strWhat);
				ATLASSERT(false);
			}

		if(IsLastOption)
		{
			Report(StepMask(), enRpMtMessage,_T("Loaded %d options for %s"), m_nLoadedOptions, sUndSymbol.GetString());
			Trace(EODCO::enTrLogLevel6,__FUNCTION__,_T("Loaded %d options for %s"), m_nLoadedOptions, sUndSymbol.GetString());
			m_nLoadedOptions = 0;
			m_spUndCurr->m_bMsUpdated = true;
			_FinishLoadUndOptions();
			_IncrementImportOptionsProgress();
			SetEvent(m_hAllOptReceived);
		}
	}
    else
	{
		CString strWhat;
		strWhat.Format(_T("%s is not equal to %s"),(LPCSTR) m_spUndCurr->m_sSymbol, (LPCSTR)sUndSymbol);
		Trace(EODCO::enTrLogWarning,__FUNCTION__,strWhat);
		Report(StepMask(),enRpMtWarning, _T("%s"), (LPCSTR)strWhat);
		ATLASSERT(false);
	}
}
       

//void CEodStepLoadMktStructure::OnOption(struct PP::OptionParams* pReq,  struct PP::OptionResultsEx* pResult, VARIANT_BOOL IsLastOption)
//{
//	if(_IsCancel() || IsStopping())
//		return;
//	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s Strike %f"), (LPCTSTR)_bstr_t(pResult->Series), pResult->StrikePrice);
//
//	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();
//
//	CString sUndSymbol = (LPCTSTR)_bstr_t(pReq->UnderlyingSymbol);
//	ATLTRACE2("OnOption %s\n", (LPCSTR)_bstr_t(pResult->Series));
//	sUndSymbol.Trim().MakeUpper();
//	sUndSymbol.Replace(_T(" "), _T(""));
//	CString sOptSymbol = (LPCTSTR)_bstr_t(pResult->Series);
//	sOptSymbol.Trim().MakeUpper();
//	sOptSymbol.Replace(_T(" "), _T(""));
//
//	if(m_spUndCurr->m_sSymbol == sUndSymbol)
//	{
//		if(sOptSymbol.GetLength() > 2)
//		{
//			CString strMessage;
//			CString sRootName = sOptSymbol.Left(sOptSymbol.GetLength() - 2);
//			COleDateTime dtExpiration(pResult->ExpirationDate);
//			CString strContractName;
//			strContractName.Format(_T("%s %s %s %.02f"), sUndSymbol, dtExpiration.Format(_T("%B %Y")), 
//										pResult->OptionType == PP::enCall ? _T("Call") : _T("Put"), pResult->StrikePrice );
//
//			m_mapRoots[sRootName] = pResult->LotSize;
//
//			if(!EODCO::IsBadValue(pResult->ClosePrice) && m_bLoadOptClosePrices)
//				strMessage.Format("<Option Symbol=\"%s\" IsCall=\"%d\" Strike=\"%.2f\" Expiry=\"%s\" ContractName=\"%s\" ClosePrice=\"%.2f\" />"	
//				,sOptSymbol.GetString()
//				, pResult->OptionType==PP::enCall?1:0
//				,pResult->StrikePrice
//				,(LPCSTR)_bstr_t(vt_date(pResult->ExpirationDate).GetVtDate())
//				,(LPCSTR) strContractName
//				,pResult->ClosePrice
//				);
//			else
//				strMessage.Format("<Option Symbol=\"%s\" IsCall=\"%d\" Strike=\"%.2f\" Expiry=\"%s\" ContractName=\"%s\" />"	
//				,sOptSymbol.GetString()
//				,pResult->OptionType==PP::enCall? 1:0
//				,pResult->StrikePrice
//				,(LPCSTR)_bstr_t(vt_date(pResult->ExpirationDate).GetVtDate())
//				,(LPCSTR) strContractName
//				);
//			m_sOptionsUpdate += strMessage.GetString();
//
//		}
//		else if(!IsLastOption)
//		{
//			CString strWhat;
//			strWhat.Format(_T("Incorrect %s symbol length"),(LPCSTR)sUndSymbol);
//			Trace(EODCO::enTrLogWarning,__FUNCTION__,strWhat);
//			Report(StepMask(),enRpMtWarning, _T("%s"), strWhat);
//			ATLASSERT(false);
//		}
//		if(IsLastOption)
//		{
//			std::string sUpdate = "<Import>";
//			if(m_sOptionsUpdate.length())
//				sUpdate+=  m_sOptionsUpdate;
//				sUpdate += "</Import>";
//
//			std::string sUpdateRoots = "<Roots>";
//
//			for(std::map<CString, long>::iterator itrRoots = m_mapRoots.begin(); itrRoots != m_mapRoots.end(); ++itrRoots)
//			{
//				 CString strRoot;
//				 strRoot.Format("<OptRoot Symbol=\"%s\" LotSize=\"%d\" /> ", (LPCSTR)itrRoots->first, itrRoots->second);
//				 sUpdateRoots += strRoot;
//			}
//
//			sUpdateRoots += "</Roots>";
//				m_sOptionsUpdate = "";
//				m_mapRoots.clear();
//			try
//			{
//				CStoredProc<> spOptionSave(m_spDB, L"usp_EODOption_Bulk_Import");
//				spOptionSave << m_spUndCurr->m_nID;         			
//				spOptionSave << sUpdateRoots;
//				spOptionSave << sUpdate;
//				spOptionSave.Execute();
//			}
//			catch(_com_error& e)
//			{
//				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_Option_Bulk_Import : %s"), (LPCSTR)e.Description());
//				Report(StepMask(),enRpMtWarning, _T("%s"), (LPCSTR)e.Description());
//
//			}
//
//			m_spUndCurr->m_bMsUpdated = true;
//			_FinishLoadUndOptions();
//			_IncrementImportOptionsProgress();
//			SetEvent(m_hAllOptReceived);
//		}
//	}
//	else
//	{
//		CString strWhat;
//		strWhat.Format(_T("%s is not equal to %s"),(LPCSTR) m_spUndCurr->m_sSymbol, (LPCSTR)sUndSymbol);
//		Trace(EODCO::enTrLogWarning,__FUNCTION__,strWhat);
//		Report(StepMask(),enRpMtWarning, _T("%s"), strWhat);
//		ATLASSERT(false);
//	}
//}

void CEodStepLoadMktStructure::OnGetStockError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::StockParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sUndSymbol = (LPCTSTR)_bstr_t(pReq->Stock);
	ATLASSERT(m_spUndCurr->m_sSymbol == sUndSymbol);

	Report(StepMask(), enRpMtWarning, 
		_T("Underlying uploading error %d - %s for '%s'"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCTSTR)_bstr_t(pReq->Stock));
	SetEvent(m_hUndReceived);
}

void CEodStepLoadMktStructure::OnGetOptionError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::OptionParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sUndSymbol = (LPCTSTR)_bstr_t(pReq->UnderlyingSymbol);

	ATLASSERT(m_spUndCurr->m_sSymbol == sUndSymbol);
	Report(StepMask(), enRpMtWarning, 
		_T("Options uploading error %d - %s for '%s' underlying"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCSTR)sUndSymbol);

	if(m_spUndCurr)
		m_spUndCurr->m_bMsUpdated = false;

	_FinishLoadUndOptions();

	_IncrementImportOptionsProgress();

	SetEvent(m_hAllOptReceived);
}

void CEodStepLoadMktStructure::OnGetFutureByRootError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::FutureRootParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sRootSymbol = (LPCTSTR)_bstr_t(pReq->FuturesRootSymbol);

	ATLASSERT(m_spFutRootCurr->m_sRootSymbol == sRootSymbol);
	Report(StepMask(), enRpMtWarning, 
		_T("Futures uploading error %d - %s for '%s' futures root"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCSTR)sRootSymbol);

	_FinishLoadRootFutures();

	m_spFutRootCurr->m_bMsUpdated = false;

	_IncrementImportFuturesProgress();

	SetEvent(m_hAllFutReceived);
}

void CEodStepLoadMktStructure::OnFutureByRoot(struct PP::FutureRootParams* pReq,  struct PP::FutureResultsEx* pResult, VARIANT_BOOL IsLastFuture)
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), (LPCTSTR)_bstr_t(pResult->Symbol));
	m_nLoadedOptions  = 0 ;

	CString sRootSymbol = (LPCTSTR)_bstr_t(pReq->FuturesRootSymbol);
	sRootSymbol.Trim().MakeUpper();
	sRootSymbol.Replace(_T(" "), _T(""));
	CString sFutSymbol = (LPCTSTR)_bstr_t(pResult->Symbol);
	sFutSymbol.Trim().MakeUpper();
	sFutSymbol.Replace(_T(" "), _T(""));

	if(m_spFutRootCurr->m_sRootSymbol == sRootSymbol)
	{
		if(sFutSymbol.GetLength())
		{			
			EODMS::CMsFuturePtr spFut(new EODMS::CMsFuture);
			spFut->m_dtMaturity = /*EODMS::CMsContract::GetExpiryID(*/pResult->ExpirationDate/*)*/;
			spFut->m_sSymbol = sFutSymbol;
			EgLib::vt_date dtExpiration(pResult->ExpirationDate);

			try
			{
				CStoredProc<CClientRecordset> spFutImport(m_spDB, L"usp_EodFuture_Import");
				spFutImport << m_spFutRootCurr->m_nRootId;
				spFutImport << vt_date(spFut->m_dtMaturity);
				//spFutImport << GetDefaultValuationParams(pResult->LastTradingDate, enStFuture, PP::enSTUndefined, 0);
				spFutImport << _bstr_t(sFutSymbol);
				spFutImport << _bstr_t(pResult->Description);
				spFutImport << pResult->ContractSize;
				spFutImport << pResult->OptionContractSize;

				long nIsActive = 0;
				spFutImport << &nIsActive;

				_bstr_t bsBadContractName;
				spFutImport.SetSize(255);
				spFutImport << &bsBadContractName;

				switch(m_lLoadFuturesPrice)
				{
				case 0:
					spFutImport << CDBNull();
					break;
				case 1:
					if(!EODCO::IsBadValue(pResult->ClosePrice))
						spFutImport << pResult->ClosePrice;
					else
						spFutImport << CDBNull();
					break;
				case 2:
					if(!EODCO::IsBadValue(pResult->Bid) || !EODCO::IsBadValue(pResult->Ask))
						spFutImport << ((pResult->Bid!=BAD_DOUBLE_VALUE?pResult->Bid:0.0) + (pResult->Ask!=BAD_DOUBLE_VALUE?pResult->Ask:0.0))/2;
					else
						spFutImport << CDBNull();
					break;
				}

				if(!EODCO::IsBadValue(pResult->MinimumTick))
					spFutImport << pResult->MinimumTick;
				else
					spFutImport << CDBNull();

				spFutImport << pResult->UnitOfMeasure;				
				spFutImport << _bstr_t(pResult->UnitOfMeasureCurrency);
				//spFutImport << dtExpiration;

				spFutImport.Open();
				spFut->m_nID = spFutImport.GetResult();
				if(spFut->m_nID > 0L)
				{
					spFut->m_bIsActive = (nIsActive != 0);
					if(!IsExists(m_spFuturesColl, spFut->m_nID) && (spFut->m_bIsActive || !m_bOnlyActive))
						m_spFuturesColl->insert(EODMS::CMsFutureByIdColl::value_type(spFut->m_nID, spFut));
				}
				else if(spFut->m_nID == -2L)
				{
					Report(StepMask(),enRpMtError, 
						_T("Can't insert future '%s' of future root '%s' : future '%s' with open position and/or open options positions already exist in DB"), (LPCSTR)sFutSymbol, (LPCSTR)sRootSymbol, (LPCSTR)bsBadContractName);
				}
				else if(spFut->m_nID == -3L)
				{
					Report(StepMask(),enRpMtError, 
						_T("Can't insert future '%s' of future root '%s' : future '%s' with orders and/or options orders already exist in DB"), (LPCSTR)sFutSymbol, (LPCSTR)sRootSymbol, (LPCSTR)bsBadContractName);
				}
				else
				{
					Report(StepMask(),enRpMtError, 
						_T("Can't insert future '%s' of future root '%s'"), (LPCSTR)sFutSymbol, (LPCSTR)sRootSymbol);
				}
			}
			catch (exception& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFuture_Import : %s, %s"), typeid(e).name(), e.what());
				Report(StepMask(),enRpMtError, _T("Can't insert future '%s' of future root '%s' : %s"), (LPCSTR)sFutSymbol, (LPCSTR)sRootSymbol, e.what());
			}
			catch (_com_error& err)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFuture_Import : %s"), err.Description());
			}
			catch(...)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFuture_Import"));
			}
		}

		if(IsLastFuture)
		{
			_FinishLoadRootFutures();

			m_spFutRootCurr->m_bMsUpdated = true;

			_IncrementImportFuturesProgress();
			SetEvent(m_hAllFutReceived);
		}
	}
	else
		ATLASSERT(false);


}
void CEodStepLoadMktStructure::OnGetFuturesOptionError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, struct PP::FuturesOptionParams* pReq )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));

	CString sFutSymbol = (LPCTSTR)_bstr_t(pReq->FutureSymbol);

	ATLASSERT(m_spFutureCurr->m_sSymbol == sFutSymbol);
	Report(StepMask(), enRpMtWarning, 
		_T("Future options uploading error %d - %s for '%s' future"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCSTR)sFutSymbol);

	_FinishLoadFutures();

	m_spFutureCurr->m_bMsUpdated = false;

	_IncrementImportFuturesOptionsProgress();

	SetEvent(m_hAllFutOptReceived);
}
void CEodStepLoadMktStructure::OnFuturesOption(struct PP::FuturesOptionParams* pReq,  struct PP::FuturesOptionResultsEx* pResult, VARIANT_BOOL IsLastOption)
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

		EODMS::CMsFutureOptionPtr spFutOpt(new EODMS::CMsFutureOption);
		spFutOpt->m_bIsCall = (pResult->OptionType == PP::enCall);
		spFutOpt->m_dtExpiration = pResult->ExpirationDate;
		spFutOpt->m_dStrike = pResult->StrikePrice;
		spFutOpt->m_sSymbol = sOptSymbol;
		EgLib::vt_date dtExpiration(pResult->ExpirationDate);
		EgLib::vt_date dtTradingClose = EgLib::vt_date(1900, 1, 1);
		EgLib::vt_date dtExpiryOV = GetDefaultValuationParams(	EgLib::vt_date(pResult->LastTradingDate),
																enStFuture,
																enStFutureOption,
																PP::enSTUndefined,
																0,
																dtTradingClose);
		/*EgLib::vt_date	dtExpiryOV = GetDefaultValuationParams(	EgLib::vt_date(pResult->LastTradingDate),
																enStFutureOption,
																PP::enSTUndefined,
																0);*/
		++m_nLoadedOptions;

		CString strContractName;
		strContractName.Format(_T("%s %s %s %.02f"), sFutSymbol, dtExpiration.Format(_T("%B %Y")), 
			pResult->OptionType == PP::enCall ? _T("Call") : _T("Put"), pResult->StrikePrice );

		try
		{				
			CStoredProc<CClientRecordset> spFutOptImport(m_spDB, _T("usp_EodFutureOption_Import"));
			spFutOptImport << m_spFutureCurr->m_nID;
			spFutOptImport << spFutOpt->m_dStrike;
			spFutOptImport << vt_date(/*EODMS::CMsContract::GetExpiryID(*/spFutOpt->m_dtExpiration);
			spFutOptImport << dtExpiryOV;	
			spFutOptImport << dtTradingClose;
			spFutOptImport << spFutOpt->m_bIsCall;
			spFutOptImport << _bstr_t(spFutOpt->m_sSymbol.GetString());
			spFutOptImport << _bstr_t(strContractName.GetString());

			_bstr_t bsBadContractName;
			spFutOptImport.SetSize(255);
			spFutOptImport << &bsBadContractName;

			switch(m_lLoadFuturesPrice)
			{
			case 0:
				spFutOptImport << CDBNull();
				break;
			case 1:
				if(!EODCO::IsBadValue(pResult->CloseBid) || !EODCO::IsBadValue(pResult->CloseAsk))
					spFutOptImport << ((pResult->CloseBid!=BAD_DOUBLE_VALUE?pResult->CloseBid:0.0) + (pResult->CloseAsk!=BAD_DOUBLE_VALUE?pResult->CloseAsk:0.0))/2;
				else
					spFutOptImport << CDBNull();
				break;
			case 2:
				if(!EODCO::IsBadValue(pResult->Bid) || !EODCO::IsBadValue(pResult->Ask))
					spFutOptImport << ((pResult->Bid!=BAD_DOUBLE_VALUE?pResult->Bid:0.0) + (pResult->Ask!=BAD_DOUBLE_VALUE?pResult->Ask:0.0))/2;
				else
					spFutOptImport << CDBNull();
				break;
			}

			spFutOptImport.Open();
			spFutOpt->m_nID = spFutOptImport.GetResult();
			if(spFutOpt->m_nID > 0L)
			{
				if(!IsExists(m_spFutureCurr->Opt(),sOptSymbol))
					m_spFutureCurr->Opt()->insert(EODMS::CMsFutureOptionColl::value_type(sOptSymbol, spFutOpt));
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
		catch (_com_error& err)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFutureOption_Import : %s"), err.Description());
		}
		catch(...)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFutureOption_Import"));
		}
		if(IsLastOption)
		{
			_FinishLoadFutures();

			Report(StepMask(), enRpMtMessage,_T("Loaded %d  Options for %s"), m_nLoadedOptions, sFutSymbol.GetString());
			Trace(EODCO::enTrLogLevel6,__FUNCTION__,_T("Loaded %d Options for %s"), m_nLoadedOptions, sFutSymbol.GetString());
			m_nLoadedOptions = 0;


			m_spFutureCurr->m_bMsUpdated = true;

			_IncrementImportFuturesOptionsProgress();
			SetEvent(m_hAllFutOptReceived);
		}
	}
	else
		ATLASSERT(false);
}

void CEodStepLoadMktStructure::OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	//ATLASSERT(false);
}

void CEodStepLoadMktStructure::OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(_IsCancel() || IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	Report(StepMask(), enRpMtError, 
		_T("Disconnect from Structure provider: %d %s"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription));
	SetEvent(m_hDisconnect);
}

void CEodStepLoadMktStructure::_Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	long nTry = 0L;
	bool bIsStopped = false;
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
				Report(StepMask(), enRpMtError,
					_T("Error connect to Structure provider: %s"), e.what());
				throw e;
			}
			else
				Report(StepMask(), enRpMtWarning,
				_T("Error connect to Structure provider: %s. Try to recconnect"), e.what());
		}
		catch (EODCO::CEodStopException())
		{
			bIsStopped = true;
			break;
		}
	}
	if (bIsStopped)
		throw EODCO::CEodStopException();
}

EODMS::CMsUndCollPtr CEodStepLoadMktStructure::_GetActiveColl()
{
	EODMS::CMsUndCollPtr spUndColl(new EODMS::CMsUndColl);
	for(EODMS::CMsUndColl::iterator iterUnd = m_spUndColl->begin(); iterUnd != m_spUndColl->end(); ++iterUnd)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->m_bIsActive)
			spUndColl->insert(EODMS::CMsUndColl::value_type(iterUnd->first, spUnd));
	}
	return spUndColl;
}

void CEodStepLoadMktStructure::_SetAllNeedUpdate()
{
	for(EODMS::CMsUndColl::iterator iterUnd = m_spUndColl->begin(); iterUnd != m_spUndColl->end(); ++iterUnd)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		spUnd->m_bNeedMsUpdate = true;
		spUnd->m_bMsUpdated = false;
	}
}

void CEodStepLoadMktStructure::_GetUpdatedUndCount(long& nTotalCount, long& nUpdatedCount)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	nTotalCount = 0L;
	nUpdatedCount = 0L;
	for(EODMS::CMsUndColl::iterator iterUnd = m_spUndColl->begin(); iterUnd != m_spUndColl->end(); ++iterUnd)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->m_bNeedMsUpdate)
			nTotalCount++;
		if(spUnd->m_bMsUpdated)
			nUpdatedCount++;
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Complete"));
}
void CEodStepLoadMktStructure::_FinishLoadUndOptions()
{

	bool bOptDeleteError = false;
	if(m_bDeleteOldOptions)
	{
		try
		{
			_bstr_t bsUndeletedOptions;
			CStoredProc<CClientRecordset> rsOldOptionsDel(m_spDB, _T("usp_EodExpiredOptions_Del"));
			rsOldOptionsDel << m_spUndCurr->m_nID;
			rsOldOptionsDel.Open();

			if(!rsOldOptionsDel.IsEOF())
				bOptDeleteError = true;

			while(!rsOldOptionsDel.IsEOF())
			{
				if(!bsUndeletedOptions.length())
					bsUndeletedOptions += L"'";
				else
					bsUndeletedOptions += L", '";

				_bstr_t bsSymbol = rsOldOptionsDel[L"vcSymbol"];
				bsUndeletedOptions += bsSymbol;
				bsUndeletedOptions += L"'";
				++rsOldOptionsDel;
			}

			if(bsUndeletedOptions.length())
				Report(StepMask(), enRpMtWarning,
				_T("Can't delete the following old options of '%s' underlying because of existing positions: %s"), (LPCSTR)m_spUndCurr->m_sSymbol, (LPCSTR)bsUndeletedOptions);
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodExpiredOptions_Del : %s, %s"), typeid(e).name(), e.what());
		}
		catch (_com_error& err)
		{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodExpiredOptions_Del : %s"), (LPCSTR)err.Description());
		}
	}
	if(!m_bDeleteOldOptions || !bOptDeleteError)
	{
		try
		{
			CStoredProc<> spUndCheck(m_spDB, L"usp_EodMsUnderlyingCache_Check");
			spUndCheck << m_spUndCurr->m_nID;
			spUndCheck.Execute();
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodMsUnderlyingCache_Check : %s, %s"), typeid(e).name(), e.what());
			Report(StepMask(), enRpMtMessage,
				_T("'%s' underlying updated incompletely."), (LPCSTR)m_spUndCurr->m_sSymbol);
		}
	}
}

void CEodStepLoadMktStructure::_DeleteOldOrdersFromDb()
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

void CEodStepLoadMktStructure::_InitMsLoadInDb()
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

void CEodStepLoadMktStructure::_CompleteMsLoadInDb(long& nUndNeedReUpdate)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		CString strUndeletedOptions;
		CStoredProc<CClientRecordset> rsMsLoadDone(m_spDB, _T("usp_EodMsLoad_Complete"));
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
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Complete"));
}

void CEodStepLoadMktStructure::_Uninitialize()
{
	try
	{
		m_spDB.Close();
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,(LPCSTR)e.Description());
	}

	Disconnect();

	m_hUndReceived.Close();
	m_hAllOptReceived.Close();
	m_hDisconnect.Close() ;
}

void CEodStepLoadMktStructure::_LoadUnderlyingsAndOptions(EODSM::CSmSettingsMgrPtr pSettings)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(m_bOptionsLoad)
		Report(StepMask(),enRpMtMessage, _T("Loading stocks, indexes and they options..."));
	else
		Report(StepMask(),enRpMtMessage, _T("Loading stocks, indexes but no options..."));

	_InitMsLoadInDb();

	EODMS::CMsUndColl::iterator itNextUnd = m_spUndColl->begin();
	ATLASSERT(itNextUnd != m_spUndColl->end());

	HANDLE hStopEvent = GetStopEventHandle();

	m_hUndReceived.Attach(CreateEvent(NULL,FALSE,FALSE,NULL));
	m_hDisconnect.Attach(CreateEvent(NULL,FALSE,FALSE,NULL));
	m_hAllOptReceived.Attach(CreateEvent(NULL,FALSE,FALSE,NULL));

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
			///m_spUndCurr = m_spUndColl->find("XSP")->second;
			ATLASSERT(m_spUndCurr);
			itNextUnd++;
			_RequestUnderlyingStructure();
		}
		else if(dwRes == WAIT_OBJECT_0 + 3)
		{
			_Connect(pSettings);
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
		Report(StepMask(),enRpMtMessage, 
		       _T("%d underlying was updated of %d requested. %d underlying updated incompletely."), nUpdatedCount - nUndNeedReUpdate, nTotalCount, nUndNeedReUpdate );
	else
		Report(StepMask(),enRpMtMessage, _T("%d underlying was updated of %d requested."), nUpdatedCount, nTotalCount );
Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("complete"));
}


void CEodStepLoadMktStructure::_LoadFutures(EODSM::CSmSettingsMgrPtr pSettings)
{
	Report(StepMask(),enRpMtMessage, _T("Loading futures..."));

	EODMS::CMsFuturesRootColl::iterator itNextRoot = m_spFutRootsColl->begin();
	ATLASSERT(itNextRoot != m_spFutRootsColl->end());

	HANDLE hStopEvent = GetStopEventHandle();

	m_hAllFutReceived.Attach(CreateEvent(NULL,FALSE,FALSE,NULL));
	if(m_hDisconnect == NULL)
		m_hDisconnect.Attach(CreateEvent(NULL,FALSE,FALSE,NULL));


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
			_Connect(pSettings);
			itNextRoot--;
			SetEvent(m_hAllFutReceived);
		}
	}

	long nTotalCount,nUpdatedCount;

	_GetUpdatedFutRootCount(nTotalCount, nUpdatedCount);

	Report(StepMask(),enRpMtMessage, _T("%d futures roots was updated of %d requested."), nUpdatedCount, nTotalCount);
}

void CEodStepLoadMktStructure::_LoadFutureOptions(EODSM::CSmSettingsMgrPtr pSettings)
{
	Report(StepMask(),enRpMtMessage, _T("Loading futures options..."));

	EODMS::CMsFutureByIdColl::iterator itNextFut = m_spFuturesColl->begin();
	//ATLASSERT(itNextFut != m_spFuturesColl->end());

	HANDLE hStopEvent = GetStopEventHandle();

	m_hAllFutOptReceived.Attach(CreateEvent(NULL,FALSE,FALSE,NULL));

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
			_Connect(pSettings);
			itNextFut--;
			SetEvent(m_hAllFutOptReceived);
		}
	}
	long nTotalCount,nUpdatedCount;

	_GetUpdatedFutCount(nTotalCount, nUpdatedCount);

	Report(StepMask(),enRpMtMessage, _T("%d futures was updated of %d requested."), nUpdatedCount, nTotalCount);
}

void CEodStepLoadMktStructure::_FinishLoadRootFutures()
{
	if(m_bDeleteOldOptions)
	{
		try
		{
			_bstr_t bsUndeletedFutures;
			CStoredProc<CClientRecordset> rsOldFuturesDel(m_spDB, _T("usp_EodOldFutures_Del"));
			rsOldFuturesDel << m_spFutRootCurr->m_nRootId;
			rsOldFuturesDel.Open();

			while(!rsOldFuturesDel.IsEOF())
			{
				if(!bsUndeletedFutures.length())
					bsUndeletedFutures += L"'";
				else
					bsUndeletedFutures += L", '";
				_bstr_t bsSymbol = rsOldFuturesDel[L"vcSymbol"];
				bsUndeletedFutures += bsSymbol;
				bsUndeletedFutures += L"'";
				++rsOldFuturesDel;
			}

			if(bsUndeletedFutures.length())
				Report(StepMask(), enRpMtWarning, 
				  _T("Can't delete all futures of '%s' future root because of existing positions: %s"), (LPCSTR)m_spFutRootCurr->m_sRootSymbol, (LPCSTR)bsUndeletedFutures);

		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOldFutures_Del : %s, %s"), typeid(e).name(), e.what());
		}
	}
}

void CEodStepLoadMktStructure::_FinishLoadFutures()
{
	if(m_bDeleteOldOptions)
	{
		try
		{
			_bstr_t bsUndeletedFuturesOpt;
			CStoredProc<CClientRecordset> rsOldFuturesOptDel(m_spDB, L"usp_EodOldFutureOptions_Del");
			rsOldFuturesOptDel << m_spFutureCurr->m_nID;
			rsOldFuturesOptDel.Open();

			while(!rsOldFuturesOptDel.IsEOF())
			{
				if(!bsUndeletedFuturesOpt.length())
					bsUndeletedFuturesOpt += L"'";
				else
					bsUndeletedFuturesOpt += L", '";
				_bstr_t bsSymbol =rsOldFuturesOptDel[L"vcSymbol"];
				
				bsUndeletedFuturesOpt += bsSymbol;
				bsUndeletedFuturesOpt += L"'";
				++rsOldFuturesOptDel;
			}

			if(bsUndeletedFuturesOpt.length())
				Report(StepMask(), enRpMtWarning, 
				  _T("Can't delete the following old futures options of '%s' future because of existing positions: %s"), (LPCSTR)m_spFutRootCurr->m_sRootSymbol, (LPCSTR)bsUndeletedFuturesOpt);
		}
		catch (exception& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOldFutureOptions_Del : %s, %s"), typeid(e).name(), e.what());
		}
	}
}

void CEodStepLoadMktStructure::_IncrementImportOptionsProgress()
{
	size_t nAllUnd = m_spUndColl->size();

	double dPersent = static_cast<double>(++m_nLoadedUnd) / nAllUnd;
	
	size_t nTotal = 100L - (m_bCheckMktStrUpdates ? 10L : 0L) - (m_bUpdateFutures ? 10L : 0L);

	size_t nProgress = static_cast<long>(nTotal * dPersent) + (m_bCheckMktStrUpdates ? 10L : 0L);

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}
void CEodStepLoadMktStructure::_IncrementImportFuturesProgress()
{
	size_t nAllFut = m_spFutRootsColl->size();

	double dPersent = static_cast<double>(++m_nLoadedFutRoots) / nAllFut;

	size_t nProgress = 90L + static_cast<long>(dPersent * 5L);

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}
void CEodStepLoadMktStructure::_IncrementImportFuturesOptionsProgress()
{
	size_t nAllFut = m_spFuturesColl->size();

	double dPersent = static_cast<double>(++m_nLoadedFutures) / nAllFut;

	long nProgress = 95L + static_cast<long>(dPersent * 5L);

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}

void CEodStepLoadMktStructure::_GetUpdatedFutRootCount(long& nTotalCount, long& nUpdatedCount)
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

void CEodStepLoadMktStructure::_GetUpdatedFutCount(long& nTotalCount, long& nUpdatedCount)
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

}