// Futures.cpp : Implementation of CFutures
#include "stdafx.h"
#include "Futures.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CFutures::GetFutureResultsByRootSymbol(const _FutureRootParams& Params, bool bIsResultEx)
{
	if(IsStreamRunning())
		return;

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
		WaitStreamTerminate();
		if(m_bTerminate)
			m_nStreamError = DBA_TERMINATED;

		nResult  = m_nStreamError;
	}

	if(nResult == DBA_ERR_NO_ERROR)
	{
		if(m_bIsGotFutures)
		{
			if(!m_bIsResultEx)
			{
				_FutureResults future;
				future.Init();
				CFuturesResultResponsePtr pFuture = CFuturesResultResponsePtr(new CFuturesResultResponse(Params, future, true));
				PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pFuture));
			}
			else
			{
				_FutureResultsEx future;
				future.Init();
				CFuturesResultExResponsePtr pFuture = CFuturesResultExResponsePtr(new CFuturesResultExResponse(Params, future, true));
				PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pFuture));
			}
		}
		else
		{
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
		CErrorResponse* pError = new CErrorResponse();
		pError->m_enRequestType = enRequestFutures;
		const_cast<_FutureRootParams&>(Params).CopyTo(pError->m_vtRequest);
		
		if(nResult == DBA_ERR_KEY_NOT_FOUND)
		{
			pError->m_bstrDescription = L"Future root has no futures";
			pError->m_Error = enNoDataAvailableForSymbol;
		}
		else if(nResult == DBA_TERMINATED)
		{
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
		}
		CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
		PublicResponse(pErrorPtr);
	}
}
long CFutures::StreamData(long nStatus, long nFunc)
{
	BOOL bRet = FALSE;
	if(nStatus == DBA_ERR_NO_ERROR && nFunc != DBA_TERMINATE_STREAM_FUNCTION)
	{
		int nCmp = 0;
		if((nCmp = _strnicmp(m_szKeySymbol, (const char*)m_pDbaRec->future.header.item.symbol, strlen(m_szKeySymbol))) >= 0)
		{
			if(!nCmp)
			{
				if(m_pDbaRec->future.header.recordType == 1 &&
					!_strnicmp("INDEX", (const char*)m_pDbaRec->futClass.group, 5))
				{
					char szName[MAX_TICKER + 1];
					ZeroMemory(szName,sizeof(szName));
					CreateSz(szName, (const char *)m_pDbaRec->futClass.item.symbol, MAX_TICKER);

					if(_CheckIsFutureSymbolValid(szName))
					{
						if(!_CheckIsFutureSymbolOur(m_szKeySymbol, szName))
						{
							bRet = TRUE;
						}
						else
						{
							m_bIsGotFutures = true;
							char szDesc[MAX_FUTURENAME + 1];
							ZeroMemory(szDesc,sizeof(szDesc));
							TrimRight(szDesc, (const char *)m_pDbaRec->futClass.contractName, MAX_FUTURENAME);

							if(m_bIsRequestFuturesByRoot)
							{
								if(!m_bIsResultEx)
								{
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
										PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pFuture));
								}
								else
								{
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
					}
				}
			}
		}
		else bRet = TRUE;
	}
	else
	{
		if(nStatus != DBA_ERR_EOF)
		{
			m_nStreamError = nStatus;
		}
		bRet = TRUE;
	}	
	return bRet;
}

void CFutures::GetFutureResultsBySymbol(const _FutureParams& Params, bool bIsResultEx)
{
	_SetDbKey(Params->FutureSymbol);

	m_bIsResultEx = bIsResultEx;
	long nResult = GetEqual();

	if(nResult == DBA_ERR_NO_ERROR)
	{		
		if(m_dbaRec.future.header.recordType == 1)
		{
			char szName[MAX_TICKER + 1];
			ZeroMemory(szName,sizeof(szName));
			CreateSz(szName, (const char *)m_dbaRec.futClass.item.symbol, MAX_TICKER);

			char szDesc[MAX_FUTURENAME + 1];
			ZeroMemory(szDesc,sizeof(szDesc));
			CreateSz(szDesc, (const char *)m_dbaRec.futClass.contractName, MAX_FUTURENAME);

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
					PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pFuture));
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

				PublicFuture(Params, results);
			}
		}
		else 
		{
			CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
			if(pError)
			{
				pError->m_enRequestType = enRequestFuture;
				pError->m_bstrDescription = L"No such futures found";
				pError->m_Error = enSymbolNotSupported;

				const_cast<_FutureParams&>(Params).CopyTo(pError->m_vtRequest);
				PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
			}
		}
	}
	else
	{
		CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
		if(pError)
		{
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
			}
			const_cast<_FutureParams&>(Params).CopyTo(pError->m_vtRequest);
			PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
		}
	}
}

long CFutures::GetFutureInfo(_FuturesOptionParams& Params, bool bIsResultEx)
{
	if(IsStreamRunning())
		return DBA_ERR_TERMINATE_STREAM;

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
	long nResult = GetNextStream(0);
	if(nResult == DBA_ERR_NO_ERROR)
	{		
		WaitStreamTerminate();
		if(m_bTerminate)
		{
			return DBA_TERMINATED;
		}
		return m_nStreamError;
	}
	return nResult;
}

void CFutures::_SetDbKey(BSTR bsSymbol)
{	
	m_dbaKey.dbaKey.type = IT_FUTURE;
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.countryCode, "US", 2);
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.currencyCode, "USD", 3);
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.exchangeCode, "", 2);
	CreateSpaced((char *)m_dbaKey.dbaKey.symbol, bsSymbol, MAX_TICKER);
}

bool CFutures::_CheckIsFutureSymbolValid(const char* szSymbol)
{
	int nSize = strlen(szSymbol);	
	if(nSize < 3)
		return false;// invalid lenght
	if(!isdigit(szSymbol[nSize - 2]))
		return false;// invalid YEAR CODE
	if(!strchr("FGHJKMNQUVXZ", szSymbol[nSize - 1]))
		return false;// invalid EXPIRATION MONTH CODE
	return true;
}
bool CFutures::_CheckIsFutureSymbolOur(const char* szBaseSymbol, const char* szSymbol)
{
	int nNewBaseSize = m_bIsRequestFuturesByRoot ? strlen(szSymbol) - 2 : strlen(szSymbol);
	int nBaseSize = strlen(szBaseSymbol);
	if(nBaseSize != nNewBaseSize)
		return false;
	if(_strnicmp(szBaseSymbol, szSymbol, nBaseSize))
		return false;
	return true;
}

void CFutures::CancelRequest(const CComVariant& Params)
{
	if(V_VT(&Params) == VT_ERROR)
	{
		TerminateStream();
	}
	else
	{
		_FutureRootParams param1(Params);
		_FutureRootParams param2(m_vtParams);
		_bstr_t bsSymbol1(param1->FuturesRootSymbol) ;
		_bstr_t bsSymbol2(param2->FuturesRootSymbol) ;
		if(bsSymbol1 == bsSymbol2)
		{
			TerminateStream();
		}
	}
}

void CFutures::CopyFutureFundamentalInfoToResult(_FuturesOptionResults& Result)
{
	_bstr_t bsSymbol(Result->Series);
	string sRoot(bsSymbol);
	sRoot = sRoot.substr(0, sRoot.length() - 6);

	if(m_clCurrentFuture.m_mapRoots.find(sRoot) != m_clCurrentFuture.m_mapRoots.end())
	{
		const CFutureOptionRootInfo& clRoot = m_clCurrentFuture.m_mapRoots[sRoot];
		Result->ExpirationDate = clRoot.m_dtExpirationDate;
		Result->LotSize = clRoot.m_nOptionLotSize;
	}
}

void CFutures::CopyFutureFundamentalInfoToResult(_FuturesOptionResultsEx& Result)
{
	_bstr_t bsSymbol(Result->Series);
	string sRoot(bsSymbol);
	sRoot = sRoot.substr(0, sRoot.length() - 6);

	if(m_clCurrentFuture.m_mapRoots.find(sRoot) != m_clCurrentFuture.m_mapRoots.end())
	{
		const CFutureOptionRootInfo& clRoot = m_clCurrentFuture.m_mapRoots[sRoot];
		Result->ExpirationDate = clRoot.m_dtExpirationDate;
		Result->LotSize = clRoot.m_nOptionLotSize;
		//Result->ClosePrice = m_clCurrentFuture.m_dFutureClosePrice;
	}
}
