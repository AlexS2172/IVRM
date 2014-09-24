// Futures.cpp : Implementation of CFutures
#include "stdafx.h"
#include "Futures.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CFutures::GetFutureResultsByRootSymbol(const _FutureRootParams& Params, bool bIsResultEx)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureResultsByRootSymbol Enter: %s"), _T(Params->FuturesRootSymbol));	

	if(IsStreamRunning())
	{
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureResultsByRootSymbol Exit because IsStreamRunning"));	
		return;
	}
		

	const_cast<_FutureRootParams&>(Params).CopyTo(m_vtParams);			
	m_bIsGotFutures = false;
	m_bIsResultEx = bIsResultEx;
	m_nStreamError = DBA_ERR_NO_ERROR;
	m_bIsRequestFuturesByRoot = true;

	
	_SetDbKey(Params->FuturesRootSymbol);
	_bstr_t bsBaseSymbol(Params->FuturesRootSymbol);
	if(bsBaseSymbol.length() < 2) // Sharky: try to resolve FT_CQ00001517
		bsBaseSymbol += L" ";
	m_szKeySymbol = bsBaseSymbol;

	m_streamFilter.ulPositionFunction = DBA_GET_APPROX;
	long nResult = GetNextStream(0);

	if(nResult == DBA_ERR_NO_ERROR)
	{		
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got Stream, wait stream terminate"));	
		WaitStreamTerminate();
		if(m_bTerminate)
			m_nStreamError = DBA_TERMINATED;

		nResult  = m_nStreamError;
	}

	if(nResult == DBA_ERR_NO_ERROR)
	{
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got Stream, no error"));	
		if(m_bIsGotFutures)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got futures"));	
			if(!m_bIsResultEx)
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing futures"));	
				_FutureResults future;
				future.Init();
				CFuturesResultResponsePtr pFuture = CFuturesResultResponsePtr(new CFuturesResultResponse(Params, future, true));
				PublicResponse(boost::shared_static_cast<CResponseBase>(pFuture));
			}
			else
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing futures"));	
				_FutureResultsEx future;
				future.Init();
				CFuturesResultExResponsePtr pFuture = CFuturesResultExResponsePtr(new CFuturesResultExResponse(Params, future, true));
				PublicResponse(boost::shared_static_cast<CResponseBase>(pFuture));
			}
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing, got no futures"));	
			CErrorResponse* pError = new CErrorResponse();
			pError->m_enRequestType = enRequestFutures;
			const_cast<_FutureRootParams&>(Params).CopyTo(pError->m_vtRequest);
			pError->m_bstrDescription = L"Future root has no futures";
			pError->m_Error = enNoDataAvailableForSymbol;
			CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
			PublicResponse(pErrorPtr);
		}
	}
	else
	{
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Error occured"));	
		CErrorResponse* pError = new CErrorResponse();
		pError->m_enRequestType = enRequestFutures;
		const_cast<_FutureRootParams&>(Params).CopyTo(pError->m_vtRequest);
		
		if(nResult == DBA_ERR_KEY_NOT_FOUND)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Future root has no futures"));	
			pError->m_bstrDescription = L"Future root has no futures";
			pError->m_Error = enNoDataAvailableForSymbol;
		}
		else if(nResult == DBA_TERMINATED)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Operation canceled by user"));	
			pError->m_bstrDescription = L"Operation was canceled by user";
			pError->m_Error = enInternalError;
		}
		else
		{
			
			_bstr_t bs =  "Couldn't get futures for future root. Error: ";
			bs += EtGetMessage(DBA_ERROR,nResult);
			TCHAR buffer[0x100] = {0};				
			_ltot(nResult,buffer,10);
			bs += " (";
			bs += buffer;
			bs += ")";
			pError->m_bstrDescription = bs;
			pError->m_Error = enProviderInternalError;
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Couldn't get futures: %s"), buffer);	
		}
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureResultsByRootSymbol Responsing: %s"), _T(Params->FuturesRootSymbol));	
		CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
		PublicResponse(pErrorPtr);
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureResultsByRootSymbol Exit: %s"), _T(Params->FuturesRootSymbol));	
	}
}
long CFutures::StreamData(long nStatus, long nFunc)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Stream data Enter"));	
	BOOL bRet = FALSE;
	try
	{

		if(nStatus == DBA_ERR_NO_ERROR && nFunc != DBA_TERMINATE_STREAM_FUNCTION)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Stream OK, checking header: %s and a KeySymbol: %s"), m_pDbaRec->future.header.item.symbol, m_szKeySymbol);	
			int nCmp = 0;
			if((nCmp = _strnicmp(m_szKeySymbol, (const char*)m_pDbaRec->future.header.item.symbol, strlen(m_szKeySymbol))) >= 0)
			{
				if(!nCmp)
				{
					if (m_pDbaRec->future.header.recordType == 1 /*&& !_strnicmp("INDEX", (const char*)m_pDbaRec->futClass.group, 5)*/)
					{
						char szName[MAX_TICKER + 1];
						ZeroMemory(szName,sizeof(szName));
						CreateSz(szName, (const char *)m_pDbaRec->futClass.item.symbol, MAX_TICKER);

						if(_CheckIsFutureSymbolValid(szName))
						{
							if(!_CheckIsFutureSymbolOur(m_szKeySymbol, szName))
							{
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Symbol is not ours: %s"), szName);	
								//bRet = TRUE;
							}
							else
							{
								m_bIsGotFutures = true;
								char szDesc[MAX_FUTURENAME + 1];
								ZeroMemory(szDesc,sizeof(szDesc));
								TrimRight(szDesc, (const char *)m_pDbaRec->futClass.contractName, MAX_FUTURENAME);
								
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Checking out %s, %s"), m_pDbaRec->futClass.contractName, szDesc);	

								if(m_bIsRequestFuturesByRoot)
								{
									EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Request by root"));	
									if(!m_bIsResultEx)
									{
										EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got results: %s"), szName);	
										_FutureResults results;
										results.Init();
										_bstr_t bsName(szName);
										results->Symbol = bsName;
										_bstr_t bsDesc(szDesc);
										results->Description = bsDesc;
										results->ContractSize = (long)(Price2Dbl(m_pDbaRec->futClass.contractSizeType, m_pDbaRec->futClass.contractSize) / m_pDbaRec->futClass.priceToDollars);
										results->OptionContractSize = 1;
										results->ExpirationDate = EgLib::vt_date(m_pDbaRec->futClass.expDate.century*100+m_pDbaRec->futClass.expDate.year, m_pDbaRec->futClass.expDate.month, m_pDbaRec->futClass.expDate.day);

										_FutureRootParams Params(m_vtParams);
										CFuturesResultResponsePtr pFuture = CFuturesResultResponsePtr(new CFuturesResultResponse(Params, results, false));
										if(pFuture)
										{
											EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing..."));	
											PublicResponse(boost::shared_static_cast<CResponseBase>(pFuture));
										}											
									}
									else
									{
										EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got Results (else)"));	
										_FutureResultsEx results;
										results.Init();
										_bstr_t bsName(szName);
										results->Symbol = bsName;
										_bstr_t bsDesc(szDesc);
										results->Description = bsDesc;
										results->ContractSize = (long)(Price2Dbl(m_pDbaRec->futClass.contractSizeType, m_pDbaRec->futClass.contractSize) / m_pDbaRec->futClass.priceToDollars);
										results->OptionContractSize = 1;
										results->ExpirationDate = EgLib::vt_date(m_pDbaRec->futClass.expDate.century*100+m_pDbaRec->futClass.expDate.year, m_pDbaRec->futClass.expDate.month, m_pDbaRec->futClass.expDate.day);
										results->ClosePrice = BAD_DOUBLE_VALUE;
										
										EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got Results (else) responsing: %s, %s"), szName, szDesc);	

										_FutureRootParams Params(m_vtParams);
										PublicFuture(Params, results);
									}
								}
							}
						}
					}
					else if(m_pDbaRec->future.header.recordType == 3)
					{

						char szFutName[MAX_TICKER + 1];
						ZeroMemory(szFutName,sizeof(szFutName));
						CreateSz(szFutName, (const char *)m_pDbaRec->futOption.item.symbol, MAX_TICKER);
						
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got results, header record type 3: %s"), szFutName);	
						if(_CheckIsFutureSymbolOur(m_szKeySymbol, szFutName))
						{
							char szName[MAX_TICKER + 1];
							ZeroMemory(szName,sizeof(szName));
							CreateSz(szName, (const char *)m_pDbaRec->futOption.optionSymbol.symbol, MAX_TICKER);

							CFutureOptionRootInfo optionRoot;
							optionRoot.m_sSymbol = szName;
							optionRoot.m_dtExpirationDate = EgLib::vt_date(m_pDbaRec->futOption.expDate.century*100+m_pDbaRec->futOption.expDate.year, m_pDbaRec->futOption.expDate.month, m_pDbaRec->futOption.expDate.day);
							optionRoot.m_nFutureLotSize = m_pDbaRec->futOption.contractSize;
							optionRoot.m_nOptionLotSize = m_pDbaRec->futOption.contract;
							m_clCurrentFuture.m_mapRoots.insert(futureoptionrootmap::value_type(szName, optionRoot));
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Future root is our, added to map: %s"), szName);	
						}
					}
				}
			}
			else{
				bRet = TRUE;
			}
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Stream fail"));	
			if(nStatus != DBA_ERR_EOF)
			{
				m_nStreamError = nStatus;
			}
			bRet = TRUE;
		}	
	}
	catch(...)
	{
		bRet = TRUE;
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Exception handled at StreamData"));	
	}
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Stream data Exit: %d"), (double)bRet);	
	return bRet;
}

void CFutures::GetFutureResultsBySymbol(const _FutureParams& Params, bool bIsResultEx)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureResultsBySymbol Enter"));	
	_SetDbKey(Params->FutureSymbol);

	m_bIsResultEx = bIsResultEx;
	long nResult = DBA_ERR_KEY_NOT_FOUND;
	try
	{
		nResult = GetEqual();
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Exception handled while getting equal"));		
		
	}
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Result: %d"), _T(nResult));	
	
	if(nResult == DBA_ERR_NO_ERROR)
	{		
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got no error"));	
		if(m_dbaRec.future.header.recordType == 1)
		{
			
			char szName[MAX_TICKER + 1];
			ZeroMemory(szName,sizeof(szName));
			CreateSz(szName, (const char *)m_dbaRec.futClass.item.symbol, MAX_TICKER);

			char szDesc[MAX_FUTURENAME + 1];
			ZeroMemory(szDesc,sizeof(szDesc));
			CreateSz(szDesc, (const char *)m_dbaRec.futClass.contractName, MAX_FUTURENAME);

			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got record type 1 for : %s, %s"), szName, szDesc);	
			if(!m_bIsResultEx)
			{
				
				_FutureResults results;
				results.Init();
				_bstr_t bsName(szName);
				results->Symbol = bsName;
				_bstr_t bsDesc(szDesc);
				results->Description =bsDesc;
				results->ContractSize = (long)(Price2Dbl(m_dbaRec.futClass.contractSizeType, m_dbaRec.futClass.contractSize) / m_dbaRec.futClass.priceToDollars);
				results->OptionContractSize = 1;
				results->ExpirationDate = EgLib::vt_date(m_dbaRec.futClass.expDate.century*100+m_dbaRec.futClass.expDate.year, m_dbaRec.futClass.expDate.month, m_dbaRec.futClass.expDate.day);

				CFutureResultResponsePtr pFuture = CFutureResultResponsePtr(new CFutureResultResponse(Params, results));
				if(pFuture)
				{
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing Results for %s, %s"), szName, szDesc);	
					PublicResponse(boost::shared_static_cast<CResponseBase>(pFuture));
				}
					
			}
			else
			{
				_FutureResultsEx results;
				results.Init();
				_bstr_t bsName(szName);
				results->Symbol = bsName;
				_bstr_t bsDesc(szDesc);
				results->Description =bsDesc;
				results->ContractSize = (long)(Price2Dbl(m_dbaRec.futClass.contractSizeType, m_dbaRec.futClass.contractSize) / m_dbaRec.futClass.priceToDollars);
				results->OptionContractSize = 1;
				results->ExpirationDate = EgLib::vt_date(m_dbaRec.futClass.expDate.century*100+m_dbaRec.futClass.expDate.year, m_dbaRec.futClass.expDate.month, m_dbaRec.futClass.expDate.day);
				results->ClosePrice = BAD_DOUBLE_VALUE;
				
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing Results for %s, %s"), szName, szDesc);	
				PublicFuture(Params, results);
			}
		}
		else 
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got error"));	
			CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
			if(pError)
			{
				pError->m_enRequestType = enRequestFuture;
				pError->m_bstrDescription = L"No such futures found";
				pError->m_Error = enSymbolNotSupported;
				
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing error - got no such futures"));	
				const_cast<_FutureParams&>(Params).CopyTo(pError->m_vtRequest);
				PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
			}
		}
	}
	else
	{
		CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
		if(pError)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got error"));	
			pError->m_enRequestType = enRequestFuture;
			if(nResult == DBA_ERR_KEY_NOT_FOUND)
			{
				pError->m_bstrDescription = L"No such futures found";
				pError->m_Error = enSymbolNotSupported;	
			}
			else
			{	
				_bstr_t bs =  "Couldn't get future info. Error: ";
				bs += EtGetMessage(DBA_ERROR,nResult);
				TCHAR buffer[0x100] = {0};				
				_ltot(nResult,buffer,10);
				bs += " (";
				bs += buffer;
				bs += ")";
				pError->m_bstrDescription = bs;
				pError->m_Error = enProviderInternalError;	
				
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Responsing error %s: "), buffer);	
			}
			const_cast<_FutureParams&>(Params).CopyTo(pError->m_vtRequest);

			PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
		}
	}
		
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureResultsBySymbol Exit: %s"), _T(Params->FutureSymbol));	
}

long CFutures::GetFutureInfo(_FuturesOptionParams& Params, bool bIsResultEx)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureInfo Enter"));	
	
	if(IsStreamRunning())
	{
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureInfo Exit because IsStreamRunning"));	
		return DBA_ERR_TERMINATE_STREAM;
	}
		
	long nResult = DBA_ERR_TERMINATE_STREAM;
	try
	{
		Params.CopyTo(m_vtParams);

		m_bIsResultEx = bIsResultEx;
		m_bIsRequestFuturesByRoot = false;
		m_nStreamError = DBA_ERR_NO_ERROR;
		_SetDbKey(Params->FutureSymbol);
		_bstr_t bsBaseSymbol(Params->FutureSymbol);
		m_szKeySymbol = bsBaseSymbol;
		m_clCurrentFuture.m_mapRoots.clear();
		m_clCurrentFuture.m_sSymbol = m_szKeySymbol;

		m_streamFilter.ulPositionFunction = DBA_GET_APPROX;
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetStream for: %s"), m_szKeySymbol);	

		nResult = GetNextStream(0);
		if(nResult == DBA_ERR_NO_ERROR)
		{		
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Got stream for: %s"), m_szKeySymbol);	
			WaitStreamTerminate();
			if(m_bTerminate)
			{
				nResult =  DBA_TERMINATED;
			}
			else
				nResult=  m_nStreamError;
		}
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetFutureInfo Exit with %d"), nResult);	
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Exception handled at GetFutureInfo"));	
	}
	return nResult;
}

void CFutures::_SetDbKey(BSTR bsSymbol)
{	
	//USES_CONVERSION;
//	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Enter: %s", W2T(bsSymbol));
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Enter");

	m_dbaKey.dbaKey.type = IT_FUTURE;
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.countryCode, "US", 2);
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.currencyCode, "USD", 3);
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.exchangeCode, "", 2);
	CreateSpaced((char *)m_dbaKey.dbaKey.symbol, bsSymbol, MAX_TICKER);
	EgLib::CEgLibTraceManager::Trace(LogSubs,   __FUNCTION__ , "Exit: Symbol %s, Country %s, Currency %s, Exchange %s", m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.countryCode, m_dbaKey.dbaKey.currencyCode , m_dbaKey.dbaKey.exchangeCode );
}

bool CFutures::_CheckIsFutureSymbolValid(const char* szSymbol)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Enter: %s" ,szSymbol);
	int nSize = strlen(szSymbol);	
	if(nSize < 3)
		return false;// invalid lenght
	if(!isdigit(szSymbol[nSize - 2]))
	{
		
		if (strchr("G", szSymbol[nSize - 1]))
		{
			if (!isdigit(szSymbol[nSize - 3]))
				return false;
			if(!strchr("FGHJKMNQUVXZ", szSymbol[nSize - 2]))
				return false;// invalid EXPIRATION MONTH CODE

			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Exit - valid (with 'G'): %s" , LPTSTR(szSymbol));

			return true;
		}
		
		
		return false;// invalid EXPIRATION MONTH CODE

	}
	if(!strchr("FGHJKMNQUVXZ", szSymbol[nSize - 1]))
		return false;// invalid EXPIRATION MONTH CODE
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Exit - valid: %s", szSymbol);
	return true;
}
bool CFutures::_CheckIsFutureSymbolOur(const char* szBaseSymbol, const char* szSymbol)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Enter Symbol: %s, BaseSymbol: %s ", LPTSTR(szSymbol), LPTSTR(szBaseSymbol));
	int nNewBaseSize = m_bIsRequestFuturesByRoot ? strlen(szSymbol) - 2 : strlen(szSymbol);
	int nBaseSize = strlen(szBaseSymbol);
	if(nBaseSize != nNewBaseSize)
	{
		//return false;
		
		if (((nNewBaseSize-nBaseSize) != 1)||(!strchr("G", szSymbol[nNewBaseSize - 1])))
			return false;
		if(_strnicmp(szBaseSymbol, szSymbol, nBaseSize))
			return false;
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Symbol is our (with 'G' symbol): %s, %s", LPTSTR(szSymbol), LPTSTR(szBaseSymbol));
		return true;		
	}
		
	if(_strnicmp(szBaseSymbol, szSymbol, nBaseSize))
		return false;
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Symbol is our: %s, %s", LPTSTR(szSymbol), LPTSTR(szBaseSymbol));
	return true;
}

void CFutures::CancelRequest(const CComVariant& Params)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CancelRequest Enter"));
	if(V_VT(&Params) == VT_ERROR)
	{
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CancelRequest Terminating Stream On Error"));
		TerminateStream();
	}
	else
	{
		_FutureRootParams param1(Params);
		_FutureRootParams param2(m_vtParams);
		_bstr_t bsSymbol1(param1->FuturesRootSymbol) ;
		_bstr_t bsSymbol2(param2->FuturesRootSymbol) ;

		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CancelRequest bsSymbol1: %s, Symbol2: %s"), _T(bsSymbol1), ", ", _T(bsSymbol2) );

		if(bsSymbol1 == bsSymbol2)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CancelRequest Terminating Stream"));	
			TerminateStream();
		}
	}
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CancelRequest Exit"));
}

void CFutures::CopyFutureFundamentalInfoToResult(_FuturesOptionResults& Result)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CopyFutureFundamentalInfoToResult Enter"));	
	_bstr_t bsSymbol(Result->Series);
	string sRoot(bsSymbol);
	sRoot = sRoot.substr(0, sRoot.length() - 6);
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CopyFutureFundamentalInfoToResult Symbol: %s"), Result->Series);	

	if(m_clCurrentFuture.m_mapRoots.find(sRoot) != m_clCurrentFuture.m_mapRoots.end())
	{
		const CFutureOptionRootInfo& clRoot = m_clCurrentFuture.m_mapRoots[sRoot];
		Result->ExpirationDate = clRoot.m_dtExpirationDate;
		Result->LotSize = clRoot.m_nOptionLotSize;
	
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CopyFutureFundamentalInfoToResult Found: %d, %d"), Result->ExpirationDate, Result->LotSize);	

	}
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CopyFutureFundamentalInfoToResult Exit: %s"),Result->Series);	
}

void CFutures::CopyFutureFundamentalInfoToResult(_FuturesOptionResultsEx& Result)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CopyFutureFundamentalInfoToResult Enter"));	
	_bstr_t bsSymbol(Result->Series);
	string sRoot(bsSymbol);
	sRoot = sRoot.substr(0, sRoot.length() - 6);
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CopyFutureFundamentalInfoToResult Symbol: %s "), _T(Result->Series));
	
	if(m_clCurrentFuture.m_mapRoots.find(sRoot) != m_clCurrentFuture.m_mapRoots.end())
	{
		const CFutureOptionRootInfo& clRoot = m_clCurrentFuture.m_mapRoots[sRoot];
		Result->ExpirationDate = clRoot.m_dtExpirationDate;
		Result->LotSize = clRoot.m_nOptionLotSize;
		//Result->ClosePrice = m_clCurrentFuture.m_dFutureClosePrice;
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , "Found: %d, %d", Result->LotSize, Result->ExpirationDate);	
	}
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("CopyFutureFundamentalInfoToResult Exit: %s"), _T(Result->Series));	
}
