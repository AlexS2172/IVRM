// GroupPrice.cpp : Implementation of CGroupPrice
#include "stdafx.h"
#include "GroupPrice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupPrice::CGroupPrice() :
m_bRegionals(false),
m_bComposites(false),
m_szUndSymbol(NULL),
m_bIsGotOptions(false),
m_bIsGotError(false),
m_enRequestType(enGroupRequestAllNotify),
m_bTerminate(true)
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

	return CPrice::Attach() ;
}
long CGroupPrice::Detach()
{

	return CPrice::Detach();
}

void CGroupPrice::GetGroupPrice(const _QuoteUpdateParams& Params)
{
	_SetDbKey(Params);

	long nResult = _GetOptions(Params);
	if(DBA_ERR_NO_ERROR == nResult)
	{
		if(m_bIsGotOptions && !m_bIsGotError)
		{
			DBA_RECORD_3 rec;
			ZeroMemory(&rec, sizeof(rec));
			PublicLastQuote(Params, rec);
		}
		else if(!m_bIsGotOptions && !m_bIsGotError) 
		{
			CErrorResponse* pError = new CErrorResponse();
			pError->m_enRequestType = enRequestLastQuote;
			const_cast<_QuoteUpdateParams&>(Params).CopyTo(pError->m_vtRequest);			
			pError->m_bstrDescription = L"Couldn't get options prices for underlying. Unknown symbol.";
			pError->m_Error = enNoDataAvailableForSymbol;
			CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
			PublicResponse(pErrorPtr);
		}
	}
	else
	{
		CErrorResponse* pError = new CErrorResponse();
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
			_bstr_t bs =  "Couldn't get options prices for underlying. Error: ";
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
		_bstr_t bsSymbol1(param1->Symbol) ;
		_bstr_t bsSymbol2(param2->Symbol) ;
		if(bsSymbol1 == bsSymbol2 && param1->Type == param2->Type)
		{
			_CancelOptions();
		}
	}

}

void CGroupPrice::SubscribeGroup(const _QuoteUpdateParams& Params, GroupRequestType enRequest)
{
	_SetDbKey(Params);
	
	long nResult = _GetOptions(Params, enRequest);
	if(DBA_ERR_NO_ERROR == nResult)
	{
		if(m_bIsGotOptions && !m_bIsGotError)
		{
			if(enGroupRequestSubscribe != enRequest && enGroupRequestAllNotify != enRequest)
			{
				DBA_RECORD_3 rec;
				ZeroMemory(&rec, sizeof(rec));
				PublicLastQuote(Params, rec);
			}
			else if(enGroupRequestAllNotify == enRequest)
			{
				CResponseBasePtr pResponse = 
					boost::shared_dynamic_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(Params)));
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
				pError->m_bstrDescription = L"Couldn't subscribe underlyings options. Unknown symbol.";
				pError->m_Error = enNoDataAvailableForSymbol;

				PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
			}
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
				pError->m_bstrDescription = L"Couldn't subscribe underlyings options.";
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
				_bstr_t bs =  "Couldn't underlyings options. Error: ";
				bs += EtGetMessage(DBA_ERROR,nResult);
				TCHAR buffer[0x100] = {0};				
				_ltot(nResult,buffer,10);
				bs += " (";
				bs += buffer;
				bs += ")";
				pError->m_bstrDescription = bs;
				pError->m_Error = enProviderInternalError;
			}
			PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
		}
	}
	m_vtRequest.Clear();
}

void CGroupPrice::UnsubscribeGroup(const CComVariant& params)
{
	if(V_VT(&params) == VT_ERROR)
	{
		UnsubscribeAll();
	}
	else
	{

		_QuoteUpdateParams Params(params);
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
		KeyToString(&m_dbaKey.dbaKey, szSymbol, MAX_LENGTH);

		m_csSubscript.Lock();
		for(tickerset::const_iterator iter = m_mapGroupSubscript[szSymbol].begin(); iter !=  m_mapGroupSubscript[szSymbol].end(); iter++)
		{
			ZeroMemory(&m_dbaKey.dbaKey, sizeof(DBA_KEY));
			::StringToKey(const_cast<char*>((*iter).c_str()), &m_dbaKey.dbaKey);
			m_mapSubscript.erase(*iter);
			DecInterest();		
		}
		m_mapGroupSubscript.erase(szSymbol);
		m_csSubscript.Unlock();
	}
	m_vtRequest.Clear();
}

void CGroupPrice::UnsubscribeAll()
{
	ATLTRACE(_T("CGroupPrice::UnsubscribeAll Enter\n"));
	_CancelOptions();
	m_csSubscript.Lock();
	if ( !m_mapGroupSubscript.empty() )
		m_mapGroupSubscript.clear();
	m_csSubscript.Unlock();
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
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("CGroupPrice::_GetOptions Enter %s"), _T(m_dbaKey.dbaKey.symbol) );
	long nStatus = 0;
	try
	{

		ATLTRACE(_T("_GetOptions Enter\n"));
		
		_CancelOptions();

		char szSymbol[MAX_LENGTH + 1];
		ZeroMemory(szSymbol,sizeof(szSymbol));
		KeyToString(&m_dbaKey.dbaKey, szSymbol, MAX_LENGTH);
		m_szUndSymbol = szSymbol;

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

		bool bSubscribe = (m_enRequestType != enGroupRequestLastQuote);
		long nFunc = bSubscribe ? DBA_GET_OPTIONS | DBAX_INTEREST : DBA_GET_OPTIONS;
		nStatus = dba(&m_blk, nFunc, &m_dbaKey.dbaKey, &nKeyLen, &m_optionsFilter, &nRecLen, 0);
		if (nStatus == DBA_ERR_NO_ERROR)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("CGroupPrice::_GetOptions dba returns NO_ERROR"));
			EgLib::WaitWithEvents(1, &m_optionsFilter.hThread);
			if(m_bTerminate)
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("CGroupPrice::_GetOptions nStatus = DBA_TERMINATED"));
				nStatus = DBA_TERMINATED;
			}
				
		}
		if(m_optionsFilter.hThread)
		{
			//::CloseHandle(m_optionsFilter.hThread);
			m_optionsFilter.hThread = 0;
		}
		ATLTRACE(_T("_GetOptions Exit\n"));
	}
	_CATCH_UNHANDLED_EXCEPTION;

	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("CGroupPrice::_GetOptions Exit %s"), _T(m_dbaKey.dbaKey.symbol) );
	return nStatus;
}

void CGroupPrice::_CancelOptions()
{
	ATLTRACE(_T("_CancelOptions Enter\n"));
	if (m_optionsFilter.hThread != 0)
	{
		m_bTerminate = true;
		EgLib::WaitWithEvents(1, &m_optionsFilter.hThread);
		m_szUndSymbol = NULL;
		m_csExchanges.Lock();
		m_setExchanges.clear();
		m_csExchanges.Unlock();
	}
	ATLTRACE(_T("_CancelOptions Exit\n"));
}

long CALLBACK CGroupPrice::_OptionsProcStatic(VOID *pblk, ULONG ulFunction, DBA_KEY *pDbaKey, int nKeyLen, DBA_OPTIONS_FILTER_RECORD *pDbaRec, int nRecLen, ULONG ulKey, DWORD dwStatus, void *p) 
{
	OPTIONS_FILTER* pFilter = (OPTIONS_FILTER*)p;
	ULONG ulFunc = ulFunction & DBAX_FUNCTION_MASK;

	CGroupPrice* pPrice = (CGroupPrice*)pFilter->pUser;
	_ASSERTE(pPrice);

	if(pPrice)
		return pPrice->_OptionsProc(ulFunction,pDbaKey, pDbaRec, dwStatus);
	
	return TRUE;
}

long CGroupPrice::_OptionsProc(ULONG ulFunction, DBA_KEY *pDbaKey, DBA_OPTIONS_FILTER_RECORD *pDbaRec, DWORD dwStatus) 
{
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION || m_bTerminate)
		return FALSE;

	if (dwStatus == DBA_ERR_NO_ERROR)
	{
		_QuoteUpdateParams Params;
		ParamFromKey(*pDbaKey, Params);

		if(_IsOurOptionExchange(pDbaKey->exchangeCode[0]))
		{
			char symbol[MAX_LENGTH + 1];
			ZeroMemory(&symbol, sizeof(symbol));
			KeyToString(pDbaKey, symbol, MAX_LENGTH);
			
			m_csClosePrice.Lock();
			m_mapClosePrices[symbol] = pDbaRec->priceRec.dbaRec.close.price;
			m_csClosePrice.Unlock();


			m_bIsGotOptions = true;

			if(enGroupRequestLastQuote != m_enRequestType)
			{
				m_csSubscript.Lock();
				m_mapGroupSubscript[m_szUndSymbol].insert(symbol);
				m_mapSubscript.insert(subscriptionmap::value_type(symbol, enOPT));
				m_csSubscript.Unlock();

				if(enGroupRequestAllNotify == m_enRequestType)
				{
					CSubscribedResponsePtr pResponse = CSubscribedResponsePtr(new CSubscribedResponse(Params));
					PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pResponse));
				}
				PublicQuoteUpdate(Params, pDbaRec->priceRec.dbaRec);
			}
			
			if(enGroupRequestSubscribe != m_enRequestType && enGroupRequestAllNotify != m_enRequestType)
			{
				PublicLastQuote(Params, pDbaRec->priceRec.dbaRec);
			}
		}
		else
		{
			memcpy(&m_dbaKey.dbaKey, pDbaKey, sizeof(DBA_KEY));
			DecInterest();
		}

		if(Params->Symbol)
		{
			SysFreeString(Params->Symbol);
			Params->Symbol = NULL;
		}
		if(Params->Exchange)
		{
			SysFreeString(Params->Exchange);
			Params->Exchange = NULL;
		}
		return FALSE;
	}
	else 
	{
		CErrorResponse* pError = new CErrorResponse();
		pError->m_vtRequest = m_vtRequest;			
		if(enGroupRequestLastQuote != m_enRequestType)
		{
			pError->m_enRequestType = enSubscribeQuote;
			if(dwStatus == DBA_ERR_KEY_NOT_FOUND)
			{
				pError->m_bstrDescription = L"Couldn't subscribe underlyings options.";
				pError->m_Error = enNoDataAvailableForSymbol;
			}
			else if(dwStatus == DBA_ERR_INTEREST)
			{
				pError->m_bstrDescription = L"HyperFeed server subscription list is full";
				pError->m_Error = enProviderInternalError;
			}
			else
			{
				_bstr_t bs =  "Could not underlyings options. Error: ";
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
		CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
		PublicResponse(pErrorPtr);
		m_bIsGotError = true;
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
		_bstr_t bsExch(Exchange);
		string strExchanges(bsExch);

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
	if(m_setExchanges.size() > 1 ||  *(m_setExchanges.begin()) != ' ')
		m_bRegionals = true;

	m_csExchanges.Unlock();
}

bool CGroupPrice::_IsOurOptionExchange(char cExchange)
{
	bool IsOurExchange = false;
	m_csExchanges.Lock();
	if(!m_setExchanges.size() && cExchange == ' ')
		IsOurExchange = true;
	else if(m_setExchanges.find(cExchange) != m_setExchanges.end())
		IsOurExchange = true;
	m_csExchanges.Unlock();
	return IsOurExchange;
}
