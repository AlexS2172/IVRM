// Price.cpp : Implementation of CPrice
#include "stdafx.h"
#include "Price.h"
#include <io.h>
#include <fcntl.h>

CHFServer CPrice::m_hfServer;
#define   MAX_REQUESTS     1000
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
	#ifdef _DEBUG
		static void WriteUnsubsLineLN(LPTSTR lpParam)
		{
			int fh;
			fh = _open( "d:\\UnSubsLog.txt", _O_WRONLY );
			_lseek( fh, 0L, SEEK_END );
			_write(fh, lpParam, strlen(lpParam) );
			_write(fh, _T("\n"), sizeof(_T("\n")) );
			_close(fh);
		}
	

		static void WriteUnsubsLine(LPTSTR lpParam)
		{
			int fh;
			fh = _open( "d:\\UnSubsLog.txt", _O_WRONLY );
			_lseek( fh, 0L, SEEK_END );
			_write(fh, lpParam, strlen(lpParam) );
			//_write(fh, _T("\n"), sizeof(_T("\n")) );
			_close(fh);
		}

		static void WriteUnsubsTime()
		{
			SYSTEMTIME st;
			GetSystemTime(&st);
			char chTmp[255];
			char chTmpDate[255];
			ZeroMemory(chTmp, sizeof(chTmp));
			ZeroMemory(chTmpDate, sizeof(chTmpDate));
			GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, &st, NULL,chTmp, 255);
			GetDateFormat(LOCALE_SYSTEM_DEFAULT, DATE_SHORTDATE, &st, NULL,chTmpDate, 255);
			int fh;
			fh = _open( "d:\\UnSubsLog.txt", _O_WRONLY );
			_lseek( fh, 0L, SEEK_END );
			_write(fh,_T("\n"), sizeof(_T("\n")) );
			_write(fh, chTmpDate, strlen(chTmpDate) );
			_write(fh, _T(" "), strlen(_T(" ")));
			_write(fh, chTmp, strlen(chTmp) );
			_write(fh, _T(" "), strlen(_T(" ")));
			//_write(fh, L"\n", sizeof(L"\n") );
			_close(fh);
		}

		static void WriteSubsTime()
		{
			SYSTEMTIME st;
			GetSystemTime(&st);
			char chTmp[255];
			char chTmpDate[255];
			ZeroMemory(chTmp, sizeof(chTmp));
			ZeroMemory(chTmpDate, sizeof(chTmpDate));
			GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, &st, NULL,chTmp, 255);
			GetDateFormat(LOCALE_SYSTEM_DEFAULT, DATE_SHORTDATE, &st, NULL,chTmpDate, 255);
			int fh;
			fh = _open( "d:\\SubsLog.txt", _O_WRONLY );
			_lseek( fh, 0L, SEEK_END );
			_write(fh, _T("\n"), sizeof(_T("\n")) );
			_write(fh, chTmpDate, strlen(chTmpDate) );
			_write(fh, _T(" "), strlen(_T(" ")));
			_write(fh, chTmp, strlen(chTmp) );
			_write(fh, _T(" "), strlen(_T(" ")));
			//_write(fh, L"\n", sizeof(L"\n") );
			_close(fh);
		}

	#endif

CPrice::~CPrice()
{
	UnsubscribeAll() ;
}
long CPrice::Attach()
{
	m_bWasDisconnected = false;
	m_bWasConnectedOnce = false;

	m_bMultipleRequests      = false;
	m_bMultipleSubscription  = false;

	m_bUnsubscribeAll        = false;
	m_bCancelLastQuote       = false;
	m_hDataReady.Attach(::CreateEvent(NULL, FALSE,FALSE,NULL));
	
	if(long nResult = m_hfServer.Initialize() != DBA_ERR_NO_ERROR)
		return nResult;
	m_hfServer.Subscribe(this);
	return CExtended::Attach();
}
long CPrice::Detach()
{
	UnsubscribeAll();
	m_hfServer.Unsubscribe(this);
	m_hfServer.Uninitialize();
	_ASSERTE(m_mapSubscript.size() == 0);
	return CExtended::Detach();
}

bool CPrice::_GetMultipleRecords(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey, bool bFireError)
{
	bool bRet = true;
	try
	{
		long nResult = GetMultiple(pFilrer, pKey);
		if(nResult && !m_bCancelLastQuote)
		{
			bRet                = false;
			m_bMultipleRequests = false;
			if(bFireError)
			{
				CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
				pError->m_enRequestType  = enRequestLastQuote;
				pError->m_vtRequest      = vtMissing;

				pError->m_bstrDescription = L"Could not use Multiple requests feature: " + _bstr_t(_variant_t(nResult));
				pError->m_Error = enVersionIsNoLongerSupport;
				PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));		
			}
			std::for_each(m_MultipleRequests.begin(),
						m_MultipleRequests.end(),
						boost::bind(&CPrice::GetPrice, 
														this, 
														boost::bind(&CMuttipleRequests::value_type::second, _1), 
														bFireError,
														true));

			EgLib::CAutoLock lock(m_csMultipleRequests);
			m_MultipleRequests.clear();
		}
		pFilrer->count=0;
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return bRet;
}

long CPrice::GetOneClosePrice(const CRequestBasePtr& pParams, double& dClosePrice)
{
	dClosePrice = BAD_DOUBLE_VALUE;
	long nResult = 0;
	const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
	if(!Params)
		return nResult;

	if(m_bCancelLastQuote)
		return ERROR_SUCCESS;

	_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
	nResult = GetEqual();
	if(nResult == DBA_ERR_NO_ERROR)
	{		
		dClosePrice = SetDoubleValue(Price2Dbl(m_dbaRec.extended.priceType, m_dbaRec.extended.close.price));
	}
	return nResult;
}

long  CPrice::GetPrice(const CRequestBasePtr& pParams, bool bFireError, bool bInternal)
{
	long nResult = 0;
	const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
	if(!Params)
		return nResult;

	switch(Params->m_recParams->Type)
	{
	case enMStart:
		{
			m_bCancelLastQuote = false;
			EgLib::CAutoLock lock(m_csMultipleRequests);
			m_MultipleRequests.clear();
			m_bMultipleRequests = true;

		} break;
	case enMStop:
		{
			MULTIGE_FILTER		multiGEFilter;
			m_bCancelLastQuote = false;
			ZeroMemory(&multiGEFilter, sizeof(multiGEFilter));
			multiGEFilter.nRecLen				= sizeof(DBA_RECORD);
			multiGEFilter.count				= 0;
			multiGEFilter.flags				= 0;
			multiGEFilter.pUser             = this;
			multiGEFilter.nUserInfo         = bFireError;  
			multiGEFilter.lpfnDbMultiGEProc	= _MultipleRequests;
			if(!m_MultipleRequests.empty())
			{
				m_pLastQuoteKey = boost::shared_array<DBA_MAXKEY>(new DBA_MAXKEY[m_MultipleRequests.size()]);
				::ZeroMemory(m_pLastQuoteKey.get(), sizeof(DBA_MAXKEY)*m_MultipleRequests.size());
				multiGEFilter.count=0;
				for(CMuttipleRequests::iterator iter = m_MultipleRequests.begin(); iter != m_MultipleRequests.end(); ++iter)
				{
					if(m_bCancelLastQuote)
					{
						EgLib::CAutoLock lock(m_csMultipleRequests);
						m_MultipleRequests.clear();
						multiGEFilter.count=0;
						break;
					}
					char buffer[2048];
					ZeroMemory(buffer, sizeof(buffer));
					strncpy(buffer, iter->first.c_str(), sizeof(buffer)-1);
					::StringToKey(buffer, &m_pLastQuoteKey[multiGEFilter.count].dbaKey);
					multiGEFilter.count++;
					if(multiGEFilter.count > MAX_REQUESTS && !m_bCancelLastQuote)
					{
						_GetMultipleRecords(&multiGEFilter, m_pLastQuoteKey.get(), bFireError);
						::ZeroMemory(m_pLastQuoteKey.get(), sizeof(DBA_MAXKEY)*m_MultipleRequests.size());
						multiGEFilter.count = 0;
					}

					if(m_MultipleRequests.empty())
						break;
				}
				if(multiGEFilter.count && !m_bCancelLastQuote)
					_GetMultipleRecords(&multiGEFilter, m_pLastQuoteKey.get(), bFireError);
			}

			m_bMultipleRequests = false;
			EgLib::CAutoLock lock(m_csMultipleRequests);
			m_MultipleRequests.clear();
		} break;
	default:
		{
			if(m_bMultipleRequests)
			{
				m_bCancelLastQuote = false;
				DBA_MAXKEY maxKey;
				CHAR szBuffer[256];
				::ZeroMemory(&maxKey, sizeof(maxKey));
				::ZeroMemory(szBuffer, sizeof(szBuffer));

				_SetDbKey(Params->m_recParams, maxKey.dbaKey);

				if(KeyToString(&maxKey.dbaKey, szBuffer, sizeof(szBuffer)))
				{
					EgLib::CAutoLock lock(m_csMultipleRequests);
					m_MultipleRequests[szBuffer] = Params;
				}
			}
			else
			{
				if(m_bCancelLastQuote && bInternal)
					return ERROR_SUCCESS;
				else
					m_bCancelLastQuote = false;

				_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
				nResult = GetEqual();
				if(nResult == DBA_ERR_NO_ERROR)
				{		
					char szSymbol[MAX_LENGTH + 1];
					ZeroMemory(szSymbol,sizeof(szSymbol));
					KeyToString(&m_dbaRec.extended.item, szSymbol, MAX_LENGTH);
					{
						EgLib::CAutoLock lock(m_csClosePrice);
						m_mapClosePrices[szSymbol] =  m_dbaRec.extended.close.price;
					}
					PublicLastQuote(Params->m_recParams, m_dbaRec.extended);
				}
				else if(bFireError)
				{	
					CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
					pError->m_enRequestType = enRequestLastQuote;
					const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			
					if(nResult == DBA_ERR_KEY_NOT_FOUND)
					{
						pError->m_bstrDescription = L"Could not get the last quote data";
						pError->m_Error = enNoDataAvailableForSymbol;
					}
					else
					{
						_bstr_t bs =  "Could not get the last quote data. Error: ";
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
		}break;
	}

	return nResult;
}

bool CPrice::_SubscribeMultipleRecords(MULTIGE_FILTER* pFilter, DBA_MAXKEY* pKey, bool bNotify)
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("SubscribeMultiple Records Enter"));
	#ifdef _DEBUG
		WriteSubsTime();
	#endif
	bool bRet = true;
	try
	{
		long nResult = AddInterstMultiple(pFilter, pKey);
		if(nResult && !m_bUnsubscribeAll)
		{
			bRet = false;
			m_bMultipleSubscription = false;

			std::for_each(m_MultipleSubscriptions.begin(),
						m_MultipleSubscriptions.end(),
						boost::bind(&CPrice::Subscribe, 
													this, 
													boost::bind(&CMuttipleRequests::value_type::second, _1), 
													bNotify,
													true));

			EgLib::CAutoLock lock(m_csMultipleSubscriptions);
			m_MultipleSubscriptions.clear();
		}
		pFilter->count = 0;
	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("SubscribeMultiple Records Exit"));
	return bRet;
}

void CPrice::Subscribe(const CRequestBasePtr& pParams, bool bNotify, bool bInternal)
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Subscribe Enter..."));
	try
	{
		long nResult = 0;
		const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
		switch(Params->m_recParams->Type)
		{
		case enMStart:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Subs enMStart"));
				m_bUnsubscribeAll = false;
				EgLib::CAutoLock lock(m_csMultipleSubscriptions);
				m_MultipleSubscriptions.clear();
				m_bMultipleSubscription = true;

			}break;
		case enMStop:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Subs enMStop"));
				m_bUnsubscribeAll = false;
				MULTIGE_FILTER		multiGEFilter;
				ZeroMemory(&multiGEFilter, sizeof(multiGEFilter));
				multiGEFilter.nRecLen			= sizeof(DBA_RECORD);
				multiGEFilter.count				= 0;
				multiGEFilter.flags				= 0;
				multiGEFilter.pUser             = this;
				multiGEFilter.nUserInfo         = bNotify;
				multiGEFilter.lpfnDbMultiGEProc	= _MultipleSubscriptions;

				if(!m_MultipleSubscriptions.empty())
				{
					m_pKeySubscriptionKey = boost::shared_array<DBA_MAXKEY>(new DBA_MAXKEY[m_MultipleSubscriptions.size()]);
					::ZeroMemory(m_pKeySubscriptionKey.get(), sizeof(DBA_MAXKEY)*m_MultipleSubscriptions.size());

					multiGEFilter.count=0;
					for(CMuttipleRequests::iterator iter = m_MultipleSubscriptions.begin(); iter != m_MultipleSubscriptions.end(); ++iter)
					{
						if(m_bUnsubscribeAll)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("enMStop, Subs - UnsubsAll"));
							m_MultipleSubscriptions.clear();
							multiGEFilter.count=0;
							break;
						}

						char buffer[2048];
						ZeroMemory(buffer, sizeof(buffer));
						strncpy(buffer,iter->first.c_str(), sizeof(buffer)-1);
						::StringToKey(buffer, &m_pKeySubscriptionKey[multiGEFilter.count].dbaKey);
						multiGEFilter.count++;
						
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("enMStop, multiGEFilter iterating: %s "), _T(buffer));

						if(multiGEFilter.count > MAX_REQUESTS && !m_bUnsubscribeAll)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("enMStop, MaxRequest reached"));
							_SubscribeMultipleRecords(&multiGEFilter, m_pKeySubscriptionKey.get(), bNotify);
							::ZeroMemory(m_pKeySubscriptionKey.get(), sizeof(DBA_MAXKEY)*m_MultipleSubscriptions.size());
							multiGEFilter.count = 0;
						}
						if(m_MultipleSubscriptions.empty())
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("enMStop, m_MultipleSubscriptions.empty..."));						
							break;
						}
					}
					if(!m_bUnsubscribeAll  && multiGEFilter.count)
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("enMStop, Subscribing Multiple Records"));
						_SubscribeMultipleRecords(&multiGEFilter, m_pKeySubscriptionKey.get(), bNotify);
					}
				}
				m_bMultipleSubscription = false;

				EgLib::CAutoLock lock(m_csMultipleSubscriptions);
				m_MultipleSubscriptions.clear();
			}break;
		default:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Subs default"));
				if(m_bMultipleSubscription)
				{
					m_bUnsubscribeAll = false;
					DBA_MAXKEY maxKey;
					CHAR szBuffer[256];
					::ZeroMemory(&maxKey, sizeof(maxKey));
					::ZeroMemory(szBuffer, sizeof(szBuffer));

					_SetDbKey(Params->m_recParams, maxKey.dbaKey);
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Multiple"));
	
					if(KeyToString(&maxKey.dbaKey, szBuffer, sizeof(szBuffer)))
					{
						EgLib::CAutoLock lock(m_csMultipleSubscriptions);
						m_MultipleSubscriptions[szBuffer] = Params;
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Multiple, m_MultipleSubscriptions filling: %s"), _T(szBuffer));
					}
				}
				else
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple"));
					if(bInternal && m_bUnsubscribeAll)
					 {
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, UnsubsAll or Internal"));
						return;
					 }
						
					else
						m_bUnsubscribeAll = false;

					_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, Add Interest"));
					long nResult = AddInterest(bNotify);
					if(nResult == DBA_ERR_NO_ERROR)
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, Add Interest, NO_ERROR"));
						char szSymbol[MAX_TICKER + 1];
						ZeroMemory(szSymbol,sizeof(szSymbol));
						KeyToString(&m_dbaRec.extended.item, szSymbol, MAX_TICKER);
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T(szSymbol));

						{
							EgLib::CAutoLock lock(m_csSubscript);
							m_mapSubscript.insert(subscriptionmap::value_type(szSymbol, Params->m_recParams->Type));
						}
						{
							EgLib::CAutoLock lock(m_csClosePrice);
							m_mapClosePrices[szSymbol] = m_dbaRec.extended.close.price;
						}

						if(bNotify)
						{
							CResponseBasePtr pResponse = boost::shared_dynamic_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(Params->m_recParams)));
							PublicResponse(pResponse);
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, Notify"));
						}
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, Public Quote Update"));
						PublicQuoteUpdate(Params->m_recParams, m_dbaRec.extended);
					}
					else
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, Error occured"));
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enSubscribeQuote;

						CComVariant vtReq;
						const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			

						if(nResult == DBA_ERR_KEY_NOT_FOUND)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, DBA_ERR_KEY_NOT_FOUND"));
							pError->m_bstrDescription = L"Could not subscribe symbol";
							pError->m_Error = enNoDataAvailableForSymbol;
						}
						else if(nResult == DBA_ERR_INTEREST)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, DBA_ERR_INTEREST"));
							pError->m_bstrDescription = L"HyperFeed server subscription list is full";
							pError->m_Error = enProviderInternalError;
						}
						else
						{
							
							_bstr_t bs =  "Could not subscribe symbol. Error: ";
							bs += EtGetMessage(DBA_ERROR,nResult);
							TCHAR buffer[0x100] = {0};				
							_ltot(nResult,buffer,10);
							bs += " (";
							bs += buffer;
							bs += ")";
							pError->m_bstrDescription = bs;
							pError->m_Error = enProviderInternalError;
							
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, Unknown Error with %s"), _T(buffer) );
						}
						
						PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
						
					}
				}
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Subscribe Exit"));
}

void CPrice::Unsubscribe(const CComVariant& params)
{
	try
	{
		ATLTRACE(_T("CPrice::Unsubscribe Enter\n"));
		#ifdef _DEBUG
			WriteUnsubsTime();
			WriteUnsubsLineLN(_T("CPrice::Unsubscribe Enter\n"));
		#endif
		if(V_VT(&params) == VT_ERROR)
		{
			UnsubscribeAll();
		}
		else
		{
			_QuoteUpdateParams Params(params);
			_SetDbKey(Params, m_dbaKey.dbaKey);
			if(DBA_ERR_NO_ERROR == DecInterest())
			{
				char szSymbol[MAX_TICKER + 1];
				ZeroMemory(szSymbol,sizeof(szSymbol));
				KeyToString(&m_dbaKey.dbaKey, szSymbol, MAX_TICKER);

				EgLib::CAutoLock lock(m_csSubscript);
				m_mapSubscript.erase(szSymbol);
			}
		}
		ATLTRACE(_T("CPrice::Unsubscribe Exit\n"));
		#ifdef _DEBUG
			WriteUnsubsLineLN(_T("CPrice::Unsubscribe Exit\n"));
		#endif
	}
_CATCH_UNHANDLED_EXCEPTION;

}

long CPrice::GetMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetMultiple Enter %s"), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		lRet =  dba(&m_blk, DBA_MULTI_GET_EQUAL,	pKey, &nKeyLen,	pFilrer, &nRecLen, 0);
		if(lRet)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetMultiple Return %d"), lRet);
		//	return lRet;
		}
		else
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetMultiple Waiting infinite %s"), _T(pKey->dbaKey.symbol) );
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("DBA_MULTI_GET_EQUAL Exit %s"), _T(pKey->dbaKey.symbol) );
	return lRet;
}

long CPrice::AddInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("AddInterstMultiple Enter %s"), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		lRet =  dba(&m_blk, DBA_MULTI_GET_EQUAL | DBAX_INTEREST/*| DBA_ADD_INTEREST*/, pKey, &nKeyLen,	pFilrer, &nRecLen, 0);
		if(lRet)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("AddInterstMultiple Return %d"), lRet);
			//return lRet;
		}
		else
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("AddInterstMultiple Infinite Wait %s"), _T(pKey->dbaKey.symbol));
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
		}

	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("AddInterstMultiple Exit %s"), _T(pKey->dbaKey.symbol));
	return lRet;
}

long CPrice::DecInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("DecInterstMultiple Enter %s"), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		ATLTRACE(_T("DecInterstMultiple_CP1\n"));
		lRet =  dba(&m_blk, DBA_MULTI_DEC_INTEREST, pKey, &nKeyLen,	pFilrer, &nRecLen, 0);

		ATLTRACE(_T("DecInterstMultiple_CP2\n"));
		if(lRet)
		{
			//	return lRet;
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("DecInterstMultiple Return %d "), lRet);
		}
		else
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("DecInterstMultiple Infinite Wait %s"), _T(pKey->dbaKey.symbol));
			ATLTRACE(_T("DecInterstMultiple_CP3\n"));
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	ATLTRACE(_T("DecInterstMultiple_CP4\n"));
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("DecInterstMultiple Exit %s"), _T(pKey->dbaKey.symbol));
	return lRet;
	
//	return lRet;
}

long CPrice::AddInterest(bool bNotify)
{
	long lRet = 0;
	try
	{
		lRet = dbax(&m_blk, bNotify ? DBA_GET_EQUAL | DBA_ADD_INTEREST : DBA_ADD_INTEREST, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return lRet;
}

long CPrice::DecInterest()
{
	
	long lRet = 0;
	try
	{
		lRet = dbax(&m_blk, DBA_DEC_INTEREST, &m_dbaKey, &m_dbaRec, 0);
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return lRet;

}

void CPrice::UnsubscribeAll()
{		 
	ATLTRACE(_T("CPrice::UnsubscribeAll Enter\n"));

	#ifdef _DEBUG
		WriteUnsubsLineLN(_T("CPrice::UnsubscribeAll Enter\n"));
	#endif
	m_bUnsubscribeAll = true;
	try
	{
		MULTIGE_FILTER		multiGEFilter;
		ZeroMemory(&multiGEFilter, sizeof(multiGEFilter));
		multiGEFilter.nRecLen				= sizeof(DBA_RECORD);
		multiGEFilter.count				= 0;
		multiGEFilter.flags				= 0;
		multiGEFilter.pUser             = this;
		multiGEFilter.lpfnDbMultiGEProc	= _MultipleDecInterest; //_MultipleRequests;

		if(!m_mapSubscript.empty())
		{
			m_pUnsubscribeKey = boost::shared_array<DBA_MAXKEY>(new DBA_MAXKEY[m_mapSubscript.size()]);
			::ZeroMemory(m_pUnsubscribeKey.get(), sizeof(DBA_MAXKEY)*m_mapSubscript.size());

			multiGEFilter.count=0;
			for(subscriptionmap::const_iterator iter = m_mapSubscript.begin(); iter !=  m_mapSubscript.end(); iter++)
			{
				char buffer[2048];
				ZeroMemory(buffer, sizeof(buffer));
				strncpy(buffer,iter->first.c_str(), sizeof(buffer)-1);
				::StringToKey(buffer, &m_pUnsubscribeKey[multiGEFilter.count].dbaKey);
				ATLTRACE(buffer);
				ATLTRACE("\n");
				#ifdef _DEBUG
					WriteUnsubsLine(_T("CPrice::Unsubscribe subscriptionmap iterating "));
					WriteUnsubsLineLN(buffer);
				#endif
				multiGEFilter.count++;
				if(multiGEFilter.count > MAX_REQUESTS)
				{
					ATLTRACE("if(multiGEFilter.count > MAX_REQUESTS)\n");
					#ifdef _DEBUG
						WriteUnsubsLineLN(_T("CPrice::Unsubscribe DecInterestMultiple call"));
					#endif
					if(DecInterstMultiple(&multiGEFilter, m_pUnsubscribeKey.get()))
					{
						for(subscriptionmap::const_iterator iter = m_mapSubscript.begin(); iter !=  m_mapSubscript.end(); iter++)
						{
							ATLTRACE("subscriptionmap iterating\n");
							#ifdef _DEBUG
								WriteUnsubsLineLN(_T("CPrice::Unsubscribe DecInterestMultiple=false subsmap iterating"));
							#endif
							ZeroMemory(&m_dbaKey.dbaKey, sizeof(DBA_KEY));
							::StringToKey(const_cast<char*>((iter->first).c_str()), &m_dbaKey.dbaKey);
							DecInterest();		
						}
						ATLTRACE("clear\n");
						m_mapSubscript.clear();
						multiGEFilter.count = 0;
						#ifdef _DEBUG
							WriteUnsubsLineLN(_T("CPrice::Unsubscribe DecInterestMultiple=false subsmap iterating DONE"));
						#endif
						break;
					}
					ATLTRACE("Zero m_pUnsubscribeKey\n");
					::ZeroMemory(m_pUnsubscribeKey.get(), sizeof(DBA_MAXKEY)*m_mapSubscript.size());
					multiGEFilter.count = 0;
				}
			}
			if(multiGEFilter.count)
			{
				ATLTRACE("DecInterstMultiple\n");
				#ifdef _DEBUG
					WriteUnsubsLineLN(_T("CPrice::Unsubscribe if(multiGEFilter.count) DecInterestMultiple call"));
				#endif
				if(DecInterstMultiple(&multiGEFilter, m_pUnsubscribeKey.get()))
				{
					ATLTRACE(_T("Iterate subscription map\n"));
					for(subscriptionmap::const_iterator iter = m_mapSubscript.begin(); iter !=  m_mapSubscript.end(); iter++)
					{
						ZeroMemory(&m_dbaKey.dbaKey, sizeof(DBA_KEY));
						::StringToKey(const_cast<char*>((iter->first).c_str()), &m_dbaKey.dbaKey);
						
						ATLTRACE("DecInterst\n");
						#ifdef _DEBUG
							WriteUnsubsLineLN(_T("CPrice::Unsubscribe if(multiGEFilter.count) subsmap iterating "));
						#endif
						DecInterest();		
					}
					m_mapSubscript.clear();
					multiGEFilter.count=0;
				}
			}
			#ifdef _DEBUG
				WriteUnsubsLineLN(_T("CPrice::Unsubscribe clear subs"));
			#endif
			ATLTRACE("ClearSubscript\n");
			m_mapSubscript.clear();
		}
/*
		DBA_MAXKEY		dbaKey[MAX_KEYS];
		::ZeroMemory(dbaKey, sizeof(dbaKey));

		EgLib::CAutoLock lock(m_csSubscript);
		for(subscriptionmap::const_iterator iter = m_mapSubscript.begin(); iter !=  m_mapSubscript.end(); iter++)
		{
			if(multiGEFilter.count < MAX_KEYS)
			{
				char buffer[2048];
				ZeroMemory(buffer, sizeof(buffer));
				strncpy(buffer,iter->first.c_str(), sizeof(buffer)-1);
				::StringToKey(buffer, &dbaKey[multiGEFilter.count].dbaKey);
				multiGEFilter.count++;
			}
			else
			{
				if(DecInterstMultiple(&multiGEFilter, dbaKey))
				{
					for(subscriptionmap::const_iterator iter = m_mapSubscript.begin(); iter !=  m_mapSubscript.end(); iter++)
					{
						ZeroMemory(&m_dbaKey.dbaKey, sizeof(DBA_KEY));
						::StringToKey(const_cast<char*>((iter->first).c_str()), &m_dbaKey.dbaKey);
						DecInterest();		
					}
					m_mapSubscript.clear();
					multiGEFilter.count=0;
					break;
				}
				::ZeroMemory(dbaKey, sizeof(dbaKey));
				multiGEFilter.count=0;
				--iter;
			}
		}
		if(multiGEFilter.count)
		{
			if(DecInterstMultiple(&multiGEFilter, dbaKey))
			{
				for(subscriptionmap::const_iterator iter = m_mapSubscript.begin(); iter !=  m_mapSubscript.end(); iter++)
				{
					ZeroMemory(&m_dbaKey.dbaKey, sizeof(DBA_KEY));
					::StringToKey(const_cast<char*>((iter->first).c_str()), &m_dbaKey.dbaKey);
					DecInterest();		
				}
			}
		}
*/
		m_mapSubscript.clear();
	}
_CATCH_UNHANDLED_EXCEPTION;
	ATLTRACE(_T("CPrice::UnsubscribeAll Exit\n"));
	#ifdef _DEBUG
		WriteUnsubsLineLN(_T("CPrice::UnsubscribeAll Exit"));
	#endif
}
void CPrice::ProcessMessage(const char* symb, DBA_RECORD* pRec)
{
	try
	{
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("ProccessMessages Enter %s"), _T(symb));
		m_dbaRec.extended = pRec->extended;
		DBA_KEYS* pDbaKey = (DBA_KEYS*)&m_dbaRec.extended.item;	 
		_QuoteUpdateParams Params;
		ParamFromKey(pDbaKey->dbaKey, Params);

		// fix hyperfeed bug with incorrect type for index quotes...
		{
			EgLib::CAutoLock lock(m_csSubscript);
			if(m_mapSubscript.find(symb) != m_mapSubscript.end())
				Params->Type = m_mapSubscript[symb];
		}
		{
			EgLib::CAutoLock lock(m_csClosePrice);
			m_dbaRec.extended.close.price = m_mapClosePrices[symb];
		}

		PublicQuoteUpdate(Params, m_dbaRec.extended);
		
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
	}
_CATCH_UNHANDLED_EXCEPTION;
EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("ProccessMessages Exit %s"), _T(symb));
}

void CPrice::NotificationMessage(ErrorNumberEnum enType, const std::string& sMessage)
{
	try
	{
		_QuoteUpdateParams empty;
				  		empty.Init();

		CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
		pError->m_enRequestType = enNoRequest;
		empty.CopyTo(pError->m_vtRequest);			
		pError->m_bstrDescription = sMessage.c_str();
		pError->m_Error = enType;
		PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
		long nRes;
		if ( enType == enConnectionWasDisconnected )
		{
			m_bWasDisconnected = true;
		}

		if (enType == enProviderConnected)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("Was disconnected...."));
			
			if (( m_bWasDisconnected) && (m_bWasConnectedOnce))
			{
				m_bWasConnectedOnce = true;
				EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("ReSubscribe after Reconnect..."));
				m_bWasDisconnected = false;
				nRes = CPrice::Detach();
				if(nRes != DBA_ERR_NO_ERROR)
				{
					HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
					EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("Couldn't detach after reconnect..."));
				}
				nRes = CPrice::Attach();
				if(nRes != DBA_ERR_NO_ERROR)
				{
					HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
					EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("Couldn't attach after reconnect..."));
				}
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::RealtimeMessage(ETI_TABLE* etiPtr)
{
	try
	{
		if (etiPtr == 0)
		{
			_bstr_t bs =  "Server fatal status. ETI Status: ";		
			bs += EtGetMessage(DBA_ERROR,m_hfServer.GetStatus());
			TCHAR buffer[0x100] = {0};				
			_ltot(m_hfServer.GetStatus(),buffer,10);
			bs += " (";
			bs += buffer;
			bs += ")";
			_QuoteUpdateParams empty;
			empty.Init();
			CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
			pError->m_enRequestType = enSubscribeQuote;
			empty.CopyTo(pError->m_vtRequest);			
			pError->m_bstrDescription = bs;
			pError->m_Error = enProviderInternalError;
			PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
		}
		else
		{
			char symb[MAX_TICKER + 1];
			ZeroMemory(symb,sizeof(symb));
			DBA_RECORD* pRec = 0;
			if (etiPtr->regionalPtr != 0 &&
				etiPtr->regionalPtr->standard.item.type != 0xffff &&
				KeyToString(&etiPtr->regionalPtr->standard.item, symb, MAX_TICKER) &&
				_IsSubscribed(symb))
			{
				ProcessMessage(symb, etiPtr->regionalPtr);
			}

			if (etiPtr->compositePtr != 0 &&
				etiPtr->compositePtr->standard.item.type != 0xffff &&
				KeyToString(&etiPtr->compositePtr->standard.item, symb, MAX_TICKER) &&
				_IsSubscribed(symb))
			{
				ProcessMessage(symb, etiPtr->compositePtr);
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::CopyExtendedToInfo(const DBA_RECORD_3& rec, _QuoteUpdateInfo& result)
{
	try
	{
		result.Init();

		result->BidPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.bid.price));
		result->AskPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.ask.price));

		result->BidSize = InitByLong(rec.bid.size);
		result->AskSize = InitByLong(rec.ask.size);
		result->SeqNum = 0;

		result->Currency = SysAllocString(L"USD");
		result->LastPrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.lastSale.price));
		result->UpdateDateTime = EgLib::vt_date::GetCurrentDate();

		result->OpenInterest = InitByLong(rec.openInterest);
		result->Volume = InitByLong(rec.volume);

		result->OpenPrice  = SetDoubleValue(Price2Dbl( rec.priceType, rec.open.price));
		result->ClosePrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.close.price));

		result->HighPrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.high.price));
		result->LowPrice  = SetDoubleValue(Price2Dbl(rec.priceType, rec.low.price));

		if(!IsInvalidDoubleValue(result->ClosePrice) && !IsInvalidDoubleValue(result->LastPrice))
			result->NetChange = result->LastPrice - result->ClosePrice;
		else
			result->NetChange = BAD_DOUBLE_VALUE;

		result->Status = GetQuoteStatus(rec.quoteIndicator);

		result->Skipped = 0;
		result->TotalRequests = 0;
		result->WaitTime = 0;
	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::CopyExtendedToResult(const DBA_RECORD_3& rec, _QuoteUpdateResult& result)
{
	try
	{
		result.Init();

		result->BidPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.bid.price));
		result->AskPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.ask.price));

		result->BidSize = InitByLong(rec.bid.size);
		result->AskSize = InitByLong(rec.ask.size);

		result->Currency = SysAllocString(L"USD");
		result->UpdateDateTime = EgLib::vt_date::GetCurrentDate();

		result->OpenInterest = InitByLong(rec.openInterest);
		result->Volume = InitByLong(rec.volume);
	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::CopyExtendedToIvResult(const DBA_RECORD_3& rec, _StockForIvResult& Result)
{
	Result->Bid = Price2Dbl(rec.priceType, rec.bid.price);
	Result->Ask = Price2Dbl(rec.priceType, rec.ask.price);
	Result->Open = Price2Dbl(rec.priceType, rec.open.price);
	Result->High = Price2Dbl(rec.priceType, rec.high.price);
	Result->Low = Price2Dbl(rec.priceType, rec.low.price);
	Result->Close = Price2Dbl(rec.priceType, rec.close.price);
	Result->Volume = rec.volume;
}

void CPrice::_SetDbKey(const _QuoteUpdateParams& Params, DBA_KEY& dbaKey)
{
	::ZeroMemory(&dbaKey, sizeof(DBA_KEY));

	char cExchange = ' ';
	if(Params->Exchange)
	{
		_bstr_t bsExchange(Params->Exchange);
		const char* szExch = bsExchange;
		if(strlen(szExch) == 1)
			cExchange = *szExch;
	}
	CreateSpacedUpper((char*)dbaKey.countryCode, "US", 2);
	CreateSpacedUpper((char*)dbaKey.currencyCode, "USD", 3);
	CreateSpaced((char *)dbaKey.symbol, Params->Symbol, MAX_TICKER);

	dbaKey.exchangeCode[1] = ' ';
	switch(Params->Type) 
	{
	case enSTK:
		dbaKey.type = IT_EQUITY;
		if(cExchange != ' ')
			cExchange = GetHFStockExchange(cExchange);
		dbaKey.exchangeCode[0] = cExchange;
		break;
	case enIDX:
		if(dbaKey.symbol[0] == '$')
		{
			dbaKey.type = IT_INDEX;
			RemoveBucksFromName((char*)dbaKey.symbol);
		}
		else
			dbaKey.type = IT_EQUITY;

		if(cExchange != ' ')
			cExchange = GetHFStockExchange(cExchange);
		dbaKey.exchangeCode[0] = cExchange;
		break;
	case enOPT:
		dbaKey.type = IT_EQUITY_OPTION;
		if(cExchange != ' ')
			cExchange = GetHFOptionExchange(cExchange);
		dbaKey.exchangeCode[0] = cExchange;
		_ConvertOptionNameInKey(dbaKey);
		break;
	case enFUT:
		dbaKey.type = IT_FUTURE;
		if(cExchange != ' ')
			AddFutureExchangeSymbol(dbaKey, cExchange);
		dbaKey.exchangeCode[0] = ' ';
		break;
	case enFOP:
		dbaKey.type = IT_FUTURE_OPTION;
		if(cExchange != ' ')
			_AddFutureOptionExchangeSymbol(dbaKey, cExchange);
		dbaKey.exchangeCode[0] = ' ';
		break;
	}
}

void CPrice::AddFutureExchangeSymbol(DBA_KEY& key, char cExchange)
{
	if (key.type == IT_FUTURE)
	{
		int i = MAX_TICKER - 1;
		while (*(key.symbol + i) == ' ')
			--i;
		if (++i < MAX_TICKER)
			*(key.symbol + i) = cExchange;
	}
}

void CPrice::_AddFutureOptionExchangeSymbol(DBA_KEY& key, char cExchange)
{
	if (key.type == IT_FUTURE_OPTION)
	{
		int i = MAX_TICKER - 1;
		while (*(key.symbol + i) == ' ' || isdigit(*(key.symbol + i)))
			--i;
		if (++i < MAX_TICKER)
		{
			for(int j = MAX_TICKER - 1; j > i; j--)
				*(key.symbol + j) =  *(key.symbol + j - 1);
			*(key.symbol + i) =  cExchange;
		}
	}
}




void CPrice::CopyExtendedToInfo(const DBA_RECORD_3& rec, QuoteUpdateInfo* result)
{
	result->BidPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.bid.price));
	result->AskPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.ask.price));

	result->BidSize = InitByLong(rec.bid.size);
	result->AskSize = InitByLong(rec.ask.size);
	result->SeqNum = 0;

	result->Currency = SysAllocString(L"USD");
	result->LastPrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.lastSale.price));
	result->UpdateDateTime = EgLib::vt_date::GetCurrentDate();

	result->OpenInterest = InitByLong(rec.openInterest);
	result->Volume = InitByLong(rec.volume);

	result->OpenPrice  = SetDoubleValue(Price2Dbl( rec.priceType, rec.open.price));
	result->ClosePrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.close.price));

	result->HighPrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.high.price));
	result->LowPrice  = SetDoubleValue(Price2Dbl(rec.priceType, rec.low.price));

	if(!IsInvalidDoubleValue(result->ClosePrice) && !IsInvalidDoubleValue(result->LastPrice))
		result->NetChange = result->LastPrice - result->ClosePrice;
	else
		result->NetChange = BAD_DOUBLE_VALUE;
	result->Status = GetQuoteStatus(rec.quoteIndicator);

	result->Skipped = 0;
	result->TotalRequests = 0;
	result->WaitTime = 0;
}

/******************************************************************************
/		Convertion:
/	'ABC'	->	'A  BC'
/	'ABCD'	->	'AB CD'
******************************************************************************/
void CPrice::_ConvertOptionNameInKey(DBA_KEY& key)
{
	int len = MAX_TICKER;
	while (*(key.symbol + len - 1) == ' ')
	{
		--len;
	}
	if(len == 4)
	{
		*(key.symbol + 4) =  *(key.symbol + 3);
		*(key.symbol + 3) =  *(key.symbol + 2);
		*(key.symbol + 2) =  ' ';
	}
	else if(len == 3)
	{
		*(key.symbol + 4) =  *(key.symbol + 2);
		*(key.symbol + 3) =  *(key.symbol + 1);
		*(key.symbol + 2) =  ' ';
		*(key.symbol + 1) =  ' ';
	}
}

void CPrice::ParamFromKey(const DBA_KEY& key, _QuoteUpdateParams& Params)
{
	Params.Init();
	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)key.symbol, MAX_TICKER);

	switch(key.type) 
	{
	case IT_EQUITY:
		Params->Type = enSTK;
		break;
	case IT_INDEX:
		Params->Type = enIDX;
		AddBucksInName(szName);
		break;
	case IT_EQUITY_OPTION:
		Params->Type = enOPT;
		RemoveSpaceFromName(szName);
		break;
	case IT_FUTURE:
		Params->Type = enFUT;
		break;
	case IT_FUTURE_OPTION:
		Params->Type = enFOP;
		break;
	}

	if(Params->Type != enFUT && Params->Type != enFOP)
	{
		if(Params->Type == enOPT)
			_GetOptionExchangeFromKey(key, &(Params->Exchange));
		else
			_GetStockExchangeFromKey(key, &(Params->Exchange));
	}
	else if(Params->Type == enFUT)
	{
		_GetFutureExchangeFromName(szName, &(Params->Exchange));
	}
	else if(Params->Type == enFOP)
	{
		_GetFutureOptionExchangeFromName(szName, &(Params->Exchange));
	}

	Params->Symbol = _bstr_t(szName).copy();
}



void CPrice::_GetOptionExchangeFromKey(const DBA_KEY& key, BSTR* Exchange)
{
	if (key.type == IT_EQUITY_OPTION)
	{
		char szExch[3];
		ZeroMemory(szExch,sizeof(szExch));
		CreateSz(szExch, (const char*)key.exchangeCode, 2);
		
		*szExch = GetETSOptionExchange(*szExch);
		*Exchange = _bstr_t(szExch).copy();
	}
}
void CPrice::_GetStockExchangeFromKey(const DBA_KEY& key, BSTR* Exchange)
{
	if (key.type == IT_EQUITY || key.type == IT_INDEX )
	{
		char szExch[3];
		ZeroMemory(szExch,sizeof(szExch));
		CreateSz(szExch, (const char*)key.exchangeCode, 2);
		
		*szExch = GetETSStockExchange(*szExch);
		*Exchange = _bstr_t(szExch).copy();
	}
}

void CPrice::_GetFutureExchangeFromName(char* szSymbol, BSTR* Exchange)
{
	int i = strlen(szSymbol);
	if(i > 3 && !isdigit(*(szSymbol + i - 2)) && isdigit(*(szSymbol + i - 3)))
	{
		char szExch[2];
		ZeroMemory(szExch,sizeof(szExch));

		*szExch = *(szSymbol + i - 1);
		*(szSymbol + i - 1) = 0;
		
		*Exchange = _bstr_t(szExch).copy();
	}
}

void CPrice::_GetFutureOptionExchangeFromName(char* szSymbol, BSTR* Exchange)
{
	int nLen = strlen(szSymbol);
	int i = nLen - 1;
	while(i >= 0 && (isdigit(*(szSymbol + i)) || *(szSymbol + i) == ' '))
		--i;
	if(i > 3 && !isdigit(*(szSymbol + i - 2)) && isdigit(*(szSymbol + i - 3)))
	{
		char szExch[2];
		ZeroMemory(szExch,sizeof(szExch));
		*szExch = *(szSymbol + i);
		*Exchange = _bstr_t(szExch).copy();

		for(int j = i; j < nLen; j++)
		{
			*(szSymbol + j) = *(szSymbol + j + 1);
		}
	}
}

long CPrice::_MultipleRequests(VOID* pblk,	// pointer to param block
									   ULONG			ulFunc,	// function
									   VOID*			pKey,	// pointer to key
									   INT				nKeyLen,// length of key struct
									   DBA_RECORD*		pRec,	// pointer to return record
									   INT				nRecLen,// length of return record
									   ULONG			ulKey,	// key number
									   DWORD			dwStatus,//Status of the DBA callback
									   VOID*			pVoid)//ptr to Stream Filter struct
{
	ULONG 			ulFunction  = ulFunc & DBAX_FUNCTION_MASK;
	MULTIGE_FILTER* pFilter = (MULTIGE_FILTER*)pVoid;
	//--------------------------------------
	// No more data to process, so all done 
	//--------------------------------------
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION)
		return TRUE;

	CPrice* pPrice = reinterpret_cast<CPrice*>(pFilter->pUser);
	if(!pPrice || pPrice->m_bCancelLastQuote)
		return TRUE;

	bool bNotify = pFilter->nUserInfo!=0;

	char buffer[2048];
	ZeroMemory(buffer, sizeof(buffer));
	KeyToString((DBA_KEY *)pKey, buffer, 2047);

	EgLib::CAutoLock lock(pPrice->m_csMultipleRequests);
	CMuttipleRequests::iterator it = pPrice->m_MultipleRequests.find(buffer);

	if(!dwStatus && it != pPrice->m_MultipleRequests.end())
	{
		{
			EgLib::CAutoLock lock(pPrice->m_csClosePrice);
			pPrice->m_mapClosePrices[buffer] = pRec->extended.close.price;
		}
		pPrice->PublicLastQuote(it->second->m_recParams, pRec->extended);
	}
	else if(bNotify)
	{
		//error
		CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
		pError->m_enRequestType = enRequestLastQuote;
		if(it != pPrice->m_MultipleRequests.end())
			const_cast<_QuoteUpdateParams&>(it->second->m_recParams).CopyTo(pError->m_vtRequest);			
		else
			pError->m_vtRequest = vtMissing;

		pError->m_bstrDescription = L"Could not get the last quote data";
		pError->m_Error = enNoDataAvailableForSymbol;
		pPrice->PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));		
	}
	return FALSE;
}

long CPrice::_MultipleDecInterest(VOID* pblk,	// pointer to param block
									ULONG			ulFunc,	// function
									VOID*			pKey,	// pointer to key
									INT				nKeyLen,// length of key struct
									DBA_RECORD*		pRec,	// pointer to return record
									INT				nRecLen,// length of return record
									ULONG			ulKey,	// key number
									DWORD			dwStatus,//Status of the DBA callback
									VOID*			pVoid)//ptr to Stream Filter struct
{
	
	
	#ifdef _DEBUG
		int fh;
		char fbuffer[16];

		fh = _open( "d:\\UnSubsLog.txt", _O_WRONLY );
		_lseek( fh, 0L, SEEK_END );
		_write(fh, _T("\n"), sizeof(_T("\n")) );
		_write(fh, _T("Status: "), sizeof(_T("Status: ")) );
		ZeroMemory(fbuffer, sizeof(fbuffer));
		ltoa((long)dwStatus,(char *)&fbuffer,10);
		_write(fh, fbuffer, strlen(fbuffer));
		_write(fh, _T("  "), sizeof(_T("  ")) );
	#endif
	
	char buffer[2048];
	ZeroMemory(buffer, sizeof(buffer));
	KeyToString((DBA_KEY *)pKey, buffer, 2047);
	
	#ifdef _DEBUG
		_write(fh, _T("Key: "), sizeof(_T("Key: ")) );
		_write(fh, buffer, strlen( buffer ));
		_write(fh, _T("  "), sizeof(_T("  ")) );
	#endif




	ULONG 			ulFunction  = ulFunc & DBAX_FUNCTION_MASK;
	MULTIGE_FILTER* pFilter = (MULTIGE_FILTER*)pVoid;
	//--------------------------------------
	// No more data to process, so all done 
	//--------------------------------------
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION)
	 {
		#ifdef _DEBUG
			_write(fh, _T("DBA_TERMINATE_STREAM_FUNCTION"), sizeof(_T("DBA_TERMINATE_STREAM_FUNCTION")) );
			_close(fh);
		#endif
		return TRUE;
	 }

	CPrice* pPrice = reinterpret_cast<CPrice*>(pFilter->pUser);
	if(pPrice)
	 {
		if (pPrice->m_bCancelLastQuote)
		 {
			#ifdef _DEBUG
				_write(fh, _T("CancelLastQuote"), sizeof(_T("CancelLastQuote")) );
				_close(fh);
			#endif

			return FALSE;
		 }

	
		EgLib::CAutoLock lock(pPrice->m_csMultipleSubscriptions);
		CMuttipleRequests::iterator it = pPrice->m_MultipleSubscriptions.find(buffer);

		if(!dwStatus && it != pPrice->m_MultipleSubscriptions.end())
		 {
	
			#ifdef _DEBUG
				_write(fh, _T("MultipleSubs"), sizeof(_T("MultipleSubs")) );
				_close(fh);
			#endif

			 /*
			{
				EgLib::CAutoLock lock(pPrice->m_csSubscript);
				pPrice->m_mapSubscript.insert(subscriptionmap::value_type(buffer, it->second->m_recParams->Type));
			}
			{
				EgLib::CAutoLock lock(pPrice->m_csClosePrice);
				pPrice->m_mapClosePrices[buffer] = pRec->extended.close.price;
			}
			*/

			return TRUE;
		 }

	 }


	#ifdef _DEBUG
		_close(fh);
	#endif
	return FALSE;



}

long  CPrice::_MultipleSubscriptions(VOID* pblk,	// pointer to param block
											ULONG			ulFunc,	// function
											VOID*			pKey,	// pointer to key
											INT				nKeyLen,// length of key struct
											DBA_RECORD*		pRec,	// pointer to return record
											INT				nRecLen,// length of return record
											ULONG			ulKey,	// key number
											DWORD			dwStatus,//Status of the DBA callback
											VOID*			pVoid)//ptr to Stream Filter struct
{
	ULONG 			ulFunction  = ulFunc & DBAX_FUNCTION_MASK;
	MULTIGE_FILTER* pFilter = (MULTIGE_FILTER*)pVoid;
	//--------------------------------------
	// No more data to process, so all done 
	//--------------------------------------
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION)
		return TRUE;

	try
	{
		CPrice* pPrice = reinterpret_cast<CPrice*>(pFilter->pUser);
		if(pPrice)
		{
			bool bNotify = pFilter->nUserInfo!=0;
			char buffer[2048];
			ZeroMemory(buffer, sizeof(buffer));
			KeyToString((DBA_KEY *)pKey, buffer, 2047);

			EgLib::CAutoLock lock(pPrice->m_csMultipleSubscriptions);
			CMuttipleRequests::iterator it = pPrice->m_MultipleSubscriptions.find(buffer);

//			pPrice->m_MultipleSubscriptions
//			pPrice->m_csMultipleSubscriptions 

			if(!dwStatus && it != pPrice->m_MultipleSubscriptions.end())
			{
				{
					EgLib::CAutoLock lock(pPrice->m_csSubscript);
					pPrice->m_mapSubscript.insert(subscriptionmap::value_type(buffer, it->second->m_recParams->Type));
				}
				{
					EgLib::CAutoLock lock(pPrice->m_csClosePrice);
					pPrice->m_mapClosePrices[buffer] = pRec->extended.close.price;
				}
				if(pPrice->m_bUnsubscribeAll)
					return TRUE;

				if(bNotify)
				{
					CResponseBasePtr pResponse = boost::shared_dynamic_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(it->second->m_recParams)));
					pPrice->PublicResponse(pResponse);
				}
				pPrice->PublicQuoteUpdate(it->second->m_recParams, pRec->extended);
				
				#ifdef _DEBUG
					WriteSubsTime();
					int fh;
					fh = _open( "d:\\SubsLog.txt", _O_WRONLY );
					_lseek( fh, 0L, SEEK_END );
					_write(fh, _T("Subscribed to: "),  sizeof(_T("Subscribed to: ")));
					_write(fh, buffer,  strlen(buffer));
					_write(fh, _T("\n"),  sizeof(_T("\n")));
					_close(fh);
				#endif


			}
			else
			{
				//error
				CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
				pError->m_enRequestType = enSubscribeQuote;

				if(it != pPrice->m_MultipleSubscriptions.end())
				{
					CComVariant vtReq;
					const_cast<_QuoteUpdateParams&>(it->second->m_recParams).CopyTo(pError->m_vtRequest);			
				}

				if(dwStatus == DBA_ERR_KEY_NOT_FOUND)
				{
					pError->m_bstrDescription = L"Could not subscribe symbol";
					pError->m_Error = enNoDataAvailableForSymbol;
				}
				else if(dwStatus == DBA_ERR_INTEREST)
				{
					pError->m_bstrDescription = L"HyperFeed server subscription list is full";
					pError->m_Error = enProviderInternalError;
				}
				else
				{
					_bstr_t bs =  "Could not subscribe symbol. Error: ";
					bs += EtGetMessage(DBA_ERROR,dwStatus);
					TCHAR buffer[0x100] = {0};				
					_ltot(dwStatus, buffer,10);
					bs += " (";
					bs += buffer;
					bs += ")";
					pError->m_bstrDescription = bs;
					pError->m_Error = enProviderInternalError;
				}
				pPrice->PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));
			}
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return FALSE;
}


QuoteStatusType CPrice::GetQuoteStatus(USHORT Indicator)
{
	switch(Indicator)
	{
	case QI_CLOSING:
	case QI_NEWS_DISS:
	case QI_ORDER_INFLUX:
	case QI_NEWS_PENDING:
	case QI_DUE_TO_RELATED:
	case QI_VIEW_OF_COMMON:
	case QI_ORDER_IMBALANCE:
	case QI_NO_OPEN_RESUME:
	case QI_INFO_PENDING:
	case QI_SUSPEND:
	case QI_DUE_TO_RELATED_NEWS_DISS:
	case QI_DUE_TO_RELATED_NEWS_PENDING:
	case QI_ADD_INFO:
	case QI_ADD_INFO_DUE_TO_RELATED:
	case QI_EQUIPMENT_CHANGEOVER:
	case QI_NOT_CURRENT:
	case QI_QUOTATION_CANCELED:
	case QI_NON_COMPLIENCE:
	case QI_NO_ACTIVE_MM:
	case QI_SEC_SUSPENSION:
	case QI_NEWS_DISS_RESUMPTION_TIME:
	case QI_OTHER:
	case QI_HALT_ETH:
		return enQuoteStatus_Halted;

	case QI_FAST_MARKET:
	case QI_LOCKED:
	case QI_CROSSED:
	case QI_CANCEL_BID_ASK:
	case QI_CANCEL_BID:
	case QI_CANCEL_ASK:
	case QI_BID_ONLY:
	case QI_ASK_ONLY:
	case QI_UNABLE_TO_QUOTE:
	case QI_REDEMPTION:
	case QI_DELETED:
		return enQuoteStatus_Unreliable;
	}
	return enQuoteStatus_OK;
}
