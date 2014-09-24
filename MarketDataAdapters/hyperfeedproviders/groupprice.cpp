// GroupPrice.cpp : Implementation of CGroupPrice
#include "stdafx.h"
#include "GroupPrice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupPrice::CGroupPrice() :
m_bRegionals(false),
m_bComposites(false),
m_bIsGotOptions(false),
m_bIsGotError(false),
m_enRequestType(enGroupRequestAllNotify),
m_bTerminate(true),
m_hTerminateGroupOption(CreateEvent(NULL, FALSE, FALSE,NULL)),
m_UnsubscribeStrategy(this)
{
	m_optionsFilter.lpfnDbOptionsProc = _OptionsProcStatic;
	m_optionsFilter.pUser = this;
	m_optionsFilter.hThread = 0;
}


CGroupPrice::~CGroupPrice()
{
	UnsubscribeAll();
	_ASSERTE( m_mapGroupSubscript.empty() );
}

//derived from CPrice
long CGroupPrice::Attach()
{
#ifdef __PERFMON
	m_spGlobalPerfCounter = CHFServer::GetGlobalPerfMonitor();
#endif // __PERFMON
	m_UnsubscribeStrategy.Start();
	return CPrice::Attach() ;
}
long CGroupPrice::Detach()
{
#ifdef __PERFMON
	if(m_spGlobalPerfCounter!=NULL)
		m_spGlobalPerfCounter = NULL;
#endif // __PERFMON

	m_UnsubscribeStrategy.Stop();
	return CPrice::Detach();
}

void CGroupPrice::GetGroupPrice(const _QuoteUpdateParams& Params)
{
	try
	{
		_SetDbKey(Params);

		long nResult = _GetOptions(Params);
		if(DBA_ERR_NO_ERROR == nResult)
		{
			if(m_bIsGotOptions && !m_bIsGotError)
			{
				CDBARecordPtr pRec = CDBARecordPtr(new DBA_RECORD_3);
				if(pRec != NULL)
				{
					ZeroMemory(pRec.get(), sizeof(DBA_RECORD_3));
					PublicLastQuote(&Params, pRec);
				}
			}
			else if(!m_bIsGotOptions && !m_bIsGotError) 
			{
				CErrorResponsePtr pError(new CErrorResponse());
				pError->m_enRequestType = enRequestLastQuote;
				const_cast<_QuoteUpdateParams&>(Params).CopyTo(pError->m_vtRequest);			
				pError->m_bstrDescription = L"Couldn't get options prices for underlying. Unknown symbol.";
				pError->m_Error = enNoDataAvailableForSymbol;
				CResponseBasePtr pErrorPtr = boost::shared_dynamic_cast<CResponseBase>(pError);
				PublicResponse(pErrorPtr);
			}
		}
		else
		{
			if(DBA_TERMINATED != nResult)
			{
				CErrorResponsePtr pError(new CErrorResponse());

				pError->m_enRequestType = enRequestLastQuote;
				const_cast<_QuoteUpdateParams&>(Params).CopyTo(pError->m_vtRequest);			
				if(nResult == DBA_ERR_KEY_NOT_FOUND)
				{
					pError->m_bstrDescription = L"Couldn't get options prices for underlying. Unknown symbol.";
					pError->m_Error = enNoDataAvailableForSymbol;
				}
				else if(nResult == DBA_TERMINATED)
				{
					pError->m_bstrDescription = L"Operation canceled by user.";
					pError->m_Error = enInternalError;
				}
				else
				{
					_bstr_t bs =  L"Couldn't get options prices for underlying. Error: ";
					bs += EtGetMessage(DBA_ERROR,nResult);
					bs += L" (";
					bs += _bstr_t(nResult);
					bs += L")";
					pError->m_bstrDescription = bs;
					pError->m_Error = enProviderInternalError;
				}
				CResponseBasePtr pErrorPtr = boost::shared_dynamic_cast<CResponseBase>(pError);
				PublicResponse(pErrorPtr);
			}
		}
	}
	catch (...) 
	{
	}
	m_vtRequest.Clear();
}
void CGroupPrice::CancelGroupPrice(const CComVariant& Params)
{
	if(V_VT(&Params) == VT_ERROR || V_VT(&m_vtRequest) == VT_ERROR)
	{
		_CancelOptions();
	}
	else
	{
		_QuoteUpdateParams param1(Params);
		_QuoteUpdateParams param2(m_vtRequest);
		if(param1->Type == param2->Type)
		{
			_bstr_t bsSymbol1(param1->Symbol) ;
			_bstr_t bsSymbol2(param2->Symbol) ;
			if(bsSymbol1 == bsSymbol2) 
				_CancelOptions();
		}
	}

}

void CGroupPrice::SubscribeGroup(const _QuoteUpdateParams& Params, GroupRequestType enRequest)
{
	_SetDbKey(Params);
	m_NeedToUnsubscribe.clear();
	long nResult = _GetOptions(Params, enRequest);
	if(DBA_ERR_NO_ERROR == nResult || DBA_TERMINATED == nResult)
	{
		if(m_bIsGotOptions && !m_bIsGotError)
		{
			if(enGroupRequestSubscribe != enRequest && enGroupRequestAllNotify != enRequest)
			{
				CDBARecordPtr pRec = CDBARecordPtr(new DBA_RECORD_3);
				ZeroMemory(pRec.get(), sizeof(DBA_RECORD_3));
				PublicLastQuote(&Params, pRec);
			}
			else if(enGroupRequestAllNotify == enRequest)
			{
				CResponseBasePtr pResponse = 
					boost::shared_static_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(Params)));
				PublicResponse(pResponse);
			}
		}
		else if(!m_bIsGotOptions && !m_bIsGotError) 
		{
			CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
			if(pError)
			{
				pError->m_enRequestType = enSubscribeQuote;
				const_cast<_QuoteUpdateParams&>(Params).CopyTo(pError->m_vtRequest);			
				pError->m_bstrDescription = L"Couldn't subscribe underlings options. Unknown symbol.";
				pError->m_Error = enNoDataAvailableForSymbol;

				PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
			}
		}
		if(!m_NeedToUnsubscribe.empty())
		{
			boost::shared_ptr<CVectorKeys> pKeys = boost::shared_ptr<CVectorKeys>(new CVectorKeys());
			pKeys->insert(pKeys->end(), m_NeedToUnsubscribe.begin(), m_NeedToUnsubscribe.end());
			m_UnsubscribeStrategy.PushData(pKeys);
			m_NeedToUnsubscribe.clear();
		}

	}
	else
	{
		CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
		if(pError)
		{
			pError->m_enRequestType = enSubscribeQuote;
			const_cast<_QuoteUpdateParams&>(Params).CopyTo(pError->m_vtRequest);			
			if(nResult == DBA_ERR_KEY_NOT_FOUND)
			{
				pError->m_bstrDescription = L"Unable to subscribe underlings options.";
				pError->m_Error = enNoDataAvailableForSymbol;
			}
			else if(nResult == DBA_ERR_INTEREST)
			{
				pError->m_bstrDescription = L"HyperFeed server subscription list is full";
				pError->m_Error = enProviderInternalError;
			}
			else if(nResult == DBA_TERMINATED)
			{
				pError->m_bstrDescription = L"Operation canceled by user.";
				pError->m_Error = enInternalError;
			}
			else
			{
				_bstr_t bs =  "Unable to subscribe underlings options. Error: ";
				bs += EtGetMessage(DBA_ERROR,nResult);
				TCHAR buffer[0x100] = {0};				
				_ltot(nResult,buffer,10);
				bs += " (";
				bs += buffer;
				bs += ")";
				pError->m_bstrDescription = bs;
				pError->m_Error = enProviderInternalError;
			}
			PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
			EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t %s"), m_strUserName.c_str(), (LPCSTR)pError->m_bstrDescription);

		}
	}
	m_vtRequest.Clear();
}

void CGroupPrice::UnsubscribeGroup(const CComVariant& params)
{
	if(V_VT(&params) == VT_ERROR)
	{
		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Checking Subscription Group"), m_strUserName.c_str()); 
		UnsubscribeAll();
	}
	else
	{

		_QuoteUpdateParams Params(params);

		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Symbol: %s, Exchange: %s, Type: %d"), m_strUserName.c_str(), 
			Params->Symbol?CW2T(Params->Symbol):_T(""), 
			Params->Exchange?CW2T(Params->Exchange):_T(""), 
			Params->Type);


		if(V_VT(&m_vtRequest) != VT_ERROR)
		{
			_QuoteUpdateParams param2(m_vtRequest);
			_bstr_t bsSymbol1(Params->Symbol) ;
			_bstr_t bsSymbol2(param2->Symbol) ;
			if(bsSymbol1 == bsSymbol2 && Params->Type == param2->Type)
			{
				_CancelOptions();
			}
		}		
		_SetDbKey(Params);
		char szSymbol[MAX_LENGTH + 1];
		ZeroMemory(szSymbol,sizeof(szSymbol));
		if(CUtilities::KeyToString(&m_dbaKey.dbaKey, szSymbol, MAX_LENGTH))
		{
			CAutoLock lk(m_csGroupSubscript);
			grouptickermap::iterator itrGroup = m_mapGroupSubscript.find(szSymbol);
			if(itrGroup != m_mapGroupSubscript.end())
			{
				for(tickerset::iterator iter = itrGroup->second.begin(); iter !=  itrGroup->second.end(); iter++)
				{
					DBA_KEYS dbaKey;
					ZeroMemory(&dbaKey, sizeof(DBA_KEYS));
					DeleteSubscription(*iter);
					(*iter)->GetRawKey(&dbaKey.dbaKey);
					DecInterest(&dbaKey);		
				}
				m_mapGroupSubscript.erase(itrGroup);
			}
		}
	}
	m_vtRequest.Clear();
}

void CGroupPrice::UnsubscribeAll()
{
 
	EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Unsubscribe All"), m_strUserName.c_str()); 
	_CancelOptions();

	m_bTerminate = true;
	{
		CAutoLock lk(m_csGroupSubscript);
		m_mapGroupSubscript.clear();
	}
	try
	{
		CPrice::UnsubscribeAll();
	}
	catch( ... )
	{
		_ASSERTE ( !"CPrice is empty in CGroupPrice::UnsubscribeAll()" ) ;
	}
	ATLTRACE(_T("CGroupPrice::UnsubscribeAll Exit\n"));
}

long CGroupPrice::_GetOptions(const _QuoteUpdateParams& Params, GroupRequestType enRequest)
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("CGroupPrice::_GetOptions Enter %s"), _T(m_dbaKey.dbaKey.symbol) );
	long nStatus = 0;
	char szKey[MAX_TICKER*2+1]={0};

	try
	{
		ATLTRACE(_T("_GetOptions Enter\n"));
		
		_CancelOptions();

		char szSymbol[MAX_LENGTH + 1];
		ZeroMemory(szSymbol,sizeof(szSymbol));
		CUtilities::KeyToString(&m_dbaKey.dbaKey, szSymbol, MAX_LENGTH);
		m_bsUndSymbol = szSymbol;

		m_bIsGotOptions = false;
		m_bIsGotError = false;
		m_bTerminate = false;
		m_enRequestType = enRequest;
		const_cast<_QuoteUpdateParams&>(Params).CopyTo(m_vtRequest);

		int nRecLen = sizeof(OPTIONS_FILTER);
		int nKeyLen = sizeof(DBA_KEY);

		m_optionsFilter.flags.mask = 0;
		if (m_bComposites)
			m_optionsFilter.flags.mask += DBA_OFM_COMPOSITES;
		if (m_bRegionals)
			m_optionsFilter.flags.mask += DBA_OFM_REGIONALS;
		m_optionsFilter.flags.mask += DBA_OFM_CALLS;
		m_optionsFilter.flags.mask += DBA_OFM_PUTS;

		m_optionsFilter.months = -1;
		m_optionsFilter.count = -1;
		m_optionsFilter.nRecLen = sizeof(DBA_OPTIONS_FILTER_RECORD);
		ResetEvent(m_hTerminateGroupOption);

		CUtilities::KeyToString(&m_dbaKey.dbaKey, szKey, MAX_TICKER*2);

		bool bSubscribe = (m_enRequestType != enGroupRequestLastQuote);
		long nFunc = bSubscribe ? DBA_GET_OPTIONS | DBAX_INTEREST : DBA_GET_OPTIONS;
		nStatus = dba(&m_blk, nFunc, &m_dbaKey.dbaKey, &nKeyLen, &m_optionsFilter, &nRecLen, 0);
		if (nStatus == DBA_ERR_NO_ERROR)
		{
			EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("[%s]\t CGroupPrice::_GetOptions dba returns NO_ERROR"), m_strUserName.c_str());

			HANDLE hEvents[] = {m_hTerminateGroupOption, m_optionsFilter.hThread};
			DWORD dw = EgLib::WaitWithEvents(2, hEvents);
			if(m_bTerminate)
			{
				EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("[%s]\t nStatus = DBA_TERMINATED"), m_strUserName.c_str());
				nStatus = DBA_TERMINATED;
			}
			m_bTerminate = true;
			if(dw == (WAIT_OBJECT_0+1))
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t CGroupPrice::_GetOptions Succeeded for %s"), m_strUserName.c_str(), szKey );
			else
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t CGroupPrice::_GetOptions Terminate for %s"), m_strUserName.c_str(), szKey );
		}
		else
			EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__ , _T("[%s]\t Failed to get options for %s"), m_strUserName.c_str(), szKey );

		if(m_optionsFilter.hThread)
		{
			//::CloseHandle(m_optionsFilter.hThread);
			m_optionsFilter.hThread = 0;
		}
		ATLTRACE(_T("_GetOptions Exit\n"));
	}
	catch (...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("[%s]\t Unknown exception while %s processing"), m_strUserName.c_str(), szKey );
	}

	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("CGroupPrice::_GetOptions Exit %s"), _T(m_dbaKey.dbaKey.symbol) );
	return nStatus;
}

void CGroupPrice::_CancelOptions()
{
	if (m_optionsFilter.hThread != 0)
	{
		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Cancel Options"), m_strUserName.c_str()); 
		m_bTerminate = true;
		if(WAIT_TIMEOUT==EgLib::WaitWithEvents(1, &m_optionsFilter.hThread, 5000))
			TerminateThread(m_optionsFilter.hThread, -1);
		
		m_optionsFilter.hThread = NULL;
		m_bsUndSymbol = L"";

		if(!m_setExchanges.empty())
		{
			CAutoLock lock(m_csExchanges);
			m_setExchanges.clear();
		}

	}
	ATLTRACE(_T("_CancelOptions Exit\n"));
}

long CALLBACK CGroupPrice::_OptionsProcStatic(VOID *pblk, ULONG ulFunction, DBA_KEY *pDbaKey, int nKeyLen, DBA_OPTIONS_FILTER_RECORD *pDbaRec, int nRecLen, ULONG ulKey, DWORD dwStatus, void *p) 
{
	try
	{
		OPTIONS_FILTER* pFilter = (OPTIONS_FILTER*)p;
		ULONG ulFunc = ulFunction & DBAX_FUNCTION_MASK;

		CGroupPrice* pPrice = (CGroupPrice*)pFilter->pUser;
		_ASSERTE(pPrice);

		if(pPrice)
			return pPrice->_OptionsProc(ulFunction,pDbaKey, pDbaRec, dwStatus);
	}
	catch(_com_error& e)
	{
		_bstr_t bsErrorMessage = e.Description().length()? e.Description():L"";
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("Group Option Subscribe %d Error. %s"),  e.Error(),(LPCSTR)bsErrorMessage); 
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("Group Option Subscribe Unknown Error") ); 
	}
	
	return TRUE;
}

long CGroupPrice::_OptionsProc(ULONG ulFunction, DBA_KEY *pDbaKey, DBA_OPTIONS_FILTER_RECORD *pDbaRec, DWORD dwStatus) 
{
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION || m_bTerminate)
	{
		SetEvent(m_hTerminateGroupOption);
		return TRUE;
	}

	if (dwStatus == DBA_ERR_NO_ERROR)
	{
#ifdef __PERFMON
		if(m_spGlobalPerfCounter!=NULL)
			m_spGlobalPerfCounter->AddGroupRequestResponce();
		if(m_pPerfMon)
			m_pPerfMon->AddGroupRequestResponce();
#endif // __PERFMON


		if(_IsOurOptionExchange(pDbaKey->exchangeCode[0]))
		{
			_QuoteUpdateParams Params;
			ParamFromKey(*pDbaKey, enOPT, Params);

			CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString(pDbaKey));


			if(enGroupRequestLastQuote != m_enRequestType)
				EgLib::CEgLibTraceManager::Trace(LogSubsExt, __FUNCTION__, _T("[%s]\t Group Option Subscribe %s"), m_strUserName.c_str(), pHashedKey->GetKeyString().c_str()); 
			else
				EgLib::CEgLibTraceManager::Trace(LogSubsExt, __FUNCTION__, _T("[%s]\t Group Option Get %s"), m_strUserName.c_str(), pHashedKey->GetKeyString().c_str()); 

			//StoreClosePrice(pHashedKey, );
			//EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__, _T("[%s]\t Group Option Close Price Set %s"), m_strUserName.c_str(), pHashedKey->GetKeyString().c_str()); 

			m_bIsGotOptions = true;

			if(enGroupRequestLastQuote != m_enRequestType)
			{
				if(m_bsUndSymbol.length())
				{
					CAutoLock lk(m_csGroupSubscript);
					m_mapGroupSubscript[m_bsUndSymbol].insert(pHashedKey);
				}

				CSubscriptionInfoPtr pSubscription = AddSubscription(pHashedKey, enOPT);
				pSubscription->SetClosePrice(pDbaRec->priceRec.dbaRec.close.price);

				if(enGroupRequestAllNotify == m_enRequestType)
				{
					CSubscribedResponsePtr pResponse = CSubscribedResponsePtr(new CSubscribedResponse(Params));
					PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pResponse));
				}
				CDBARecordPtr pRecData =  CDBARecordPtr(new DBA_RECORD_3);
				memcpy(pRecData.get(), &pDbaRec->priceRec.dbaRec, sizeof(DBA_RECORD_3));

				pSubscription->AssignData(pRecData);
				PostIntoRealtimeMessageProcessor(pSubscription);

				pRecData = pSubscription->GetData();
				PublicLastQuote(&Params, pRecData);
			}
			
			if(enGroupRequestSubscribe != m_enRequestType && enGroupRequestAllNotify != m_enRequestType)
			{
				CDBARecordPtr pRec = CDBARecordPtr(new DBA_RECORD_3);
				if(pRec!=NULL)
				{
					memcpy(pRec.get(), &pDbaRec->priceRec.dbaRec, sizeof(DBA_RECORD_3));
					PublicLastQuote(&Params, pRec);
				}
			}
		}
		else
		{
			if(enGroupRequestLastQuote != m_enRequestType)
				m_NeedToUnsubscribe.push_back(CHahedKeyStringPtr(new CHahedKeyString(pDbaKey)));
		}
		return FALSE;
	}
	else 
	{
		CErrorResponsePtr pError(new CErrorResponse());
		pError->m_vtRequest = m_vtRequest;			
		if(enGroupRequestLastQuote != m_enRequestType)
		{
			pError->m_enRequestType = enSubscribeQuote;
			if(dwStatus == DBA_ERR_KEY_NOT_FOUND)
			{
				pError->m_bstrDescription = L"Unable to subscribe underlying options.";
				pError->m_Error = enNoDataAvailableForSymbol;
			}
			else if(dwStatus == DBA_ERR_INTEREST)
			{
				pError->m_bstrDescription = L"HyperFeed server subscription list is full";
				pError->m_Error = enProviderInternalError;
			}
			else
			{
				_bstr_t bs =  "Unable to get underlying options. Error: ";
				bs += EtGetMessage(DBA_ERROR,dwStatus);
				TCHAR buffer[0x100] = {0};				
				_ltot(dwStatus,buffer,10);
				bs += " (";
				bs += buffer;
				bs += ")";
				pError->m_bstrDescription = bs;
				pError->m_Error = enProviderInternalError;
			}
			EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t %s"), m_strUserName.c_str(), (LPCSTR)pError->m_bstrDescription);

		}
		else
		{
			pError->m_enRequestType = enRequestLastQuote;
			if(dwStatus == DBA_ERR_KEY_NOT_FOUND)
			{
				pError->m_bstrDescription = L"Couldn't get options prices for underlying. Unknown symbol.";
				pError->m_Error = enNoDataAvailableForSymbol;
			}
			else
			{
				_bstr_t bs =  "Couldn't get options prices for underlying. Error: ";
				bs += EtGetMessage(DBA_ERROR,dwStatus);
				TCHAR buffer[0x100] = {0};				
				_ltot(dwStatus,buffer,10);
				bs += " (";
				bs += buffer;
				bs += ")";
				pError->m_bstrDescription = bs;
				pError->m_Error = enProviderInternalError;
			}
		}
		CResponseBasePtr pErrorPtr = boost::shared_dynamic_cast<CResponseBase>(pError);
		PublicResponse(pErrorPtr);
		m_bIsGotError = true;
		EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t %s"), m_strUserName.c_str(), (LPCSTR)pError->m_bstrDescription);

	}

	return FALSE;
}

void CGroupPrice::_SetDbKey(const _QuoteUpdateParams& Params)
{
	switch(Params->Type) 
	{
	case enGrSTK:
	case enGrIDX:
		m_dbaKey.dbaKey.type = IT_EQUITY;
		break;
	}

	CreateSpacedUpper((char*)m_dbaKey.dbaKey.countryCode, "US", 2);
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.currencyCode, "USD", 3);
	CreateSpaced((char *)m_dbaKey.dbaKey.symbol, Params->Symbol, MAX_TICKER);
	m_dbaKey.dbaKey.exchangeCode[0] = ' ';
	m_dbaKey.dbaKey.exchangeCode[1] = ' ';

	_ParseOptionsExchange(Params->Exchange);
}

void CGroupPrice::_ParseOptionsExchange(BSTR Exchange)
{
	m_bComposites = false;
	m_bRegionals = false;
	m_csExchanges.Lock();
	m_setExchanges.clear();
	if(Exchange)
	{
		_bstr_t bsExchanges = Exchange;
		string strExchanges = (LPCSTR)bsExchanges;;
		//string strExchanges( CW2A(Exchange) );

		size_t prevPos = 0;
		size_t pos = string::npos;
		while((pos = strExchanges.find(',',prevPos)) != string::npos && pos < strExchanges.length())
		{
			string strExchange = strExchanges.substr(prevPos, pos - prevPos);
			if(strExchange.length() > 0)
				m_setExchanges.insert(GetHFOptionExchange(*strExchange.c_str()));
			else
				m_setExchanges.insert(' ');
			prevPos = pos + 1;
		}
		if(prevPos > 0 && prevPos < strExchanges.length())
		{
			string strExchange = strExchanges.substr(prevPos, strExchanges.length() - prevPos);
			if(strExchange.length() > 0)
				m_setExchanges.insert(GetHFOptionExchange(*strExchange.c_str()));
		}		
	}
	if(m_setExchanges.empty() || m_setExchanges.find(' ') != m_setExchanges.end())
		m_bComposites = true;

	if(!m_setExchanges.empty())
	{
		if(m_setExchanges.size() > 1 ||  *(m_setExchanges.begin()) != ' ')
			m_bRegionals = true;
	}

	m_csExchanges.Unlock();
}

bool CGroupPrice::_IsOurOptionExchange(char cExchange)
{
	bool IsOurExchange = false;
	try
	{
		CAutoLock lk(m_csExchanges);
		if(!m_setExchanges.size() && cExchange == ' ')
			IsOurExchange = true;
		else if(m_setExchanges.find(cExchange) != m_setExchanges.end())
			IsOurExchange = true;
	}catch (...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__, _T("[%s]\t _IsOurOptionExchange Unknown Error"), m_strUserName.c_str()); 
	}

	return IsOurExchange;
}


HRESULT CGroupPrice::OnStrategyInitialize()
{
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("Enter"));

	HRESULT dwResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(dwResult))
	{
		EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("Error while initializing Com %i"), dwResult);

	}
	EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("Leave"));
	return dwResult;

}
void    CGroupPrice::OnStrategyUninitialize()
{
	CoUninitialize();
}
void CGroupPrice::OnData(boost::shared_ptr<CVectorKeys>& Data)
{
	Sleep(500);
	CPrice::UnsubscribeAll(*Data.get());
}
