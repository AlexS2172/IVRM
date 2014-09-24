// Price.cpp : Implementation of CPrice
#include "stdafx.h"
#include "Price.h"
#include <io.h>
#include <fcntl.h>

CHFServer CPrice::m_hfServer;
#define   MAX_REQUESTS     1500
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPrice::~CPrice()
{
	UnsubscribeAll() ;
}
long CPrice::Attach()
{
	if(m_bAtteched)
		return ERROR_SUCCESS;

	m_bWasDisconnected = false;
	m_bWasConnectedOnce = false;

	m_bMultipleRequests      = false;
	m_bMultipleSubscription  = false;

	m_bUnsubscribeAll        = false;
	m_bCancelLastQuote       = false;
	m_hDataReady.Attach(::CreateEvent(NULL, FALSE,FALSE,NULL));

#ifdef __PERFMON
	m_spGlobalPerfCounter = CHFServer::GetGlobalPerfMonitor();
	if(m_spGlobalPerfCounter!=NULL)
		m_spGlobalPerfCounter->Clients = ++CHFServer::m_lClients;
#endif // __PERFMON

	if(long nResult = m_hfServer.Initialize() != DBA_ERR_NO_ERROR)
		return nResult;
	else
		m_bAtteched = true;

	InitializeRealtimeMessageProcessor();
	return CExtended::Attach();
}

 void CPrice::InitializeRealtimeMessageProcessor()
 {
	 m_DBAStrategy.Start();
 }
 void CPrice::UninitializeRealtimeMessageProcessor()
 {
	m_DBAStrategy.Stop();
 }
 void CPrice::PostIntoRealtimeMessageProcessor(CSubscriptionInfoPtr& pSubsctiption)
 {
	  m_DBAStrategy.PushData(pSubsctiption);
 }

long CPrice::Detach()
{
	if(!m_bAtteched)
		return ERROR_SUCCESS;

#ifdef __PERFMON
	if(m_spGlobalPerfCounter!=NULL)
	{
		m_spGlobalPerfCounter->Clients = --CHFServer::m_lClients;
		m_spGlobalPerfCounter = NULL;
	}
#endif // __PERFMON

	UnsubscribeAll();
	UninitializeRealtimeMessageProcessor();

	m_hfServer.Uninitialize();
	m_bAtteched = false;

	//_ASSERTE(m_mapSubscript.size() == 0);
	//EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__, _T("Assert failed at detach - mapSubscrip is empty"));
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
				PublicResponse(boost::shared_static_cast<CResponseBase>(pError));		
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

	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Get price Enter"), m_strUserName.c_str());
	const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
	if(!Params)
	{
		
		EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Failed to get params"), m_strUserName.c_str());
		return nResult;
	}
		
	switch(Params->m_recParams->Type)
	{
	case enMStart:
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t enMStart"), m_strUserName.c_str());
			m_bCancelLastQuote = false;
			EgLib::CAutoLock lock(m_csMultipleRequests);
			m_MultipleRequests.clear();
			m_bMultipleRequests = true;

		} break;
	case enMStop:
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t enMStop"), m_strUserName.c_str());
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
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Requesting multiple"), m_strUserName.c_str());
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

					iter->first->GetRawKey(&m_pLastQuoteKey[multiGEFilter.count].dbaKey);
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
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Default"), m_strUserName.c_str());
			if(m_bMultipleRequests)
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Multiple"), m_strUserName.c_str());
				m_bCancelLastQuote = false;
				DBA_MAXKEY maxKey;
				::ZeroMemory(&maxKey, sizeof(maxKey));
				if (Params->m_recParams->Type == enFUT)
				{
					_SetDbKey(Params->m_recParams, maxKey.dbaKey);
					CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString(&maxKey.dbaKey));
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Requesting: %s"), m_strUserName.c_str(), pHashedKey->GetKeyString().c_str());

					EgLib::CAutoLock lock(m_csMultipleRequests);
					m_MultipleRequests[pHashedKey] = Params;
				}
				else
				{
					_SetDbKey(Params->m_recParams, maxKey.dbaKey);
					CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString(&maxKey.dbaKey));
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Requesting: %s"), m_strUserName.c_str(), pHashedKey->GetKeyString().c_str());

					EgLib::CAutoLock lock(m_csMultipleRequests);
					m_MultipleRequests[pHashedKey] = Params;
				}
			}
			else
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Not Multiple"), m_strUserName.c_str());
				if(m_bCancelLastQuote && bInternal)
				{
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Error"), m_strUserName.c_str());
					return ERROR_SUCCESS;
				}
					
				else
				{
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t No Error"), m_strUserName.c_str());
					m_bCancelLastQuote = false;
				}

		
				if (Params->m_recParams->Type == enFUT || Params->m_recParams->Type == enFOP )
				{

					bool bGotUsual = false;
					bool bGotGlobex= false;
					_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
					
					
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Get EqualCall: symbol - %s, Exchange - %s"), m_strUserName.c_str(), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
					nResult = GetEqual();
					// try to get usually
					if(nResult == DBA_ERR_NO_ERROR)
					{		
						bGotUsual = true;
						//CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));
						//StoreClosePrice(pKey, m_dbaRec.extended.close.price);

						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Publishing last"), m_strUserName.c_str());
						CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
						if(pRecord)
						{
							memcpy(pRecord.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
							PublicLastQuote(&Params->m_recParams, pRecord);
						}
					}
					else // try to get Globex - specific // if(bFireError)
					{
						_QuoteUpdateParams globexParam;
						globexParam.Init();
						_bstr_t bsSymbol(Params->m_recParams->Symbol);
						bsSymbol+=L"G";

						globexParam->Symbol = bsSymbol.copy();
						globexParam->Type =Params->m_recParams->Type;
						globexParam->Exchange = Params->m_recParams->Exchange!=NULL?SysAllocString(Params->m_recParams->Exchange):0;

						//wcsncat(Params->m_recParams->Symbol, L"G",1);
						_SetDbKey(globexParam, m_dbaKey.dbaKey);
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("Get EqualCall: symbol - %s, Exchange - %s"), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
						nResult = GetEqual();
						// try to get usually
						if(nResult == DBA_ERR_NO_ERROR)
						{		
							bGotGlobex = true;
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("GetEqual Succeded"));
							//char szSymbol[MAX_LENGTH + 1];
							//ZeroMemory(szSymbol,sizeof(szSymbol));
							//KeyToString(&m_dbaRec.extended.item, szSymbol, MAX_LENGTH);
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("Publishing last"));
							CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
							if(pRecord)
							{
								memcpy(pRecord.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
								PublicLastQuote(&Params->m_recParams, pRecord);
							}
						}
					}
					

					//if(((!bGotUsual)&&(!bGotGlobex))&&(bFireError))
					if((!bGotUsual && !bGotGlobex) && bFireError)
					{	
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("Error firing"));
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enRequestLastQuote;
						const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			
						if(nResult == DBA_ERR_KEY_NOT_FOUND)
						{
							pError->m_bstrDescription = L"Unable to get the last quote data";
							pError->m_Error = enNoDataAvailableForSymbol;
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("Key not found"));
						}
						else
						{
							_bstr_t bs =  "Unable to get the last quote data. Error: ";
							bs += EtGetMessage(DBA_ERROR,nResult);
							TCHAR buffer[0x100] = {0};				
							_ltot(nResult,buffer,10);
							bs += " (";
							bs += buffer;
							bs += ")";
							pError->m_bstrDescription = bs;
							pError->m_Error = enProviderInternalError;
							EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__, _T("[%s]\t Error getting last quote: %s"), m_strUserName.c_str(), buffer );
						}
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Public Response"),m_strUserName.c_str());
						PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
					}


				} // - future
				else
				{
					_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
					
					
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Get EqualCall: symbol - %s, Exchange - %s"), m_strUserName.c_str(), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
					nResult = GetEqual();
					if(nResult == DBA_ERR_NO_ERROR)
					{		
						char szSymbol[MAX_LENGTH + 1];
						ZeroMemory(szSymbol,sizeof(szSymbol));

						//CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));
						//StoreClosePrice(pKey, m_dbaRec.extended.close.price);

						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Publishing last"),m_strUserName.c_str());
						CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
						if(pRecord)
						{
							memcpy(pRecord.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
							PublicLastQuote(&Params->m_recParams, pRecord);
						}
					}
					else if(bFireError)
					{	
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Error firing"),m_strUserName.c_str());
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enRequestLastQuote;
						const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			
						if(nResult == DBA_ERR_KEY_NOT_FOUND)
						{
							pError->m_bstrDescription = L"Unable to get the last quote data";
							pError->m_Error = enNoDataAvailableForSymbol;
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Key not found"),m_strUserName.c_str());
						}
						else
						{
							_bstr_t bs =  "Unable to get the last quote data. Error: ";
							bs += EtGetMessage(DBA_ERROR,nResult);
							TCHAR buffer[0x100] = {0};				
							_ltot(nResult,buffer,10);
							bs += " (";
							bs += buffer;
							bs += ")";
							pError->m_bstrDescription = bs;
							pError->m_Error = enProviderInternalError;
							EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__, _T("[%s]\t Error getting last quote: %s"),m_strUserName.c_str(), buffer );
						}
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Public Response"), m_strUserName.c_str());
						PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
					}
				} // Not a future
			}
		}break;
	}

	return nResult;
}

bool CPrice::_SubscribeMultipleRecords(MULTIGE_FILTER* pFilter, DBA_MAXKEY* pKey, bool bNotify)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("SubscribeMultiple Records Enter"));
//	#ifdef _DEBUG
//		WriteSubsTime();
//	#endif
	bool bRet = true;
	try
	{
		long nResult = AddInterstMultiple(pFilter, pKey);
		if(nResult && !m_bUnsubscribeAll)
		{
			bRet = false;
			m_bMultipleSubscription = false;
			EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("SubscribeMultiple Error %d"), nResult);

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
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("SubscribeMultiple Records Exit"));
	return bRet;
}

void CPrice::Subscribe(const CRequestBasePtr& pParams, bool bNotify, bool bInternal)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t Subscribe Enter..."), m_strUserName.c_str());
	try
	{
		long nResult = 0;
		const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
		switch(Params->m_recParams->Type)
		{
		case enMStart:
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t Subs enMStart"), m_strUserName.c_str());
				m_bUnsubscribeAll = false;
				EgLib::CAutoLock lock(m_csMultipleSubscriptions);
				m_MultipleSubscriptions.clear();
				m_bMultipleSubscription = true;

			}break;
		case enMStop:
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t Subs enMStop"), m_strUserName.c_str());
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
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t enMStop, Subs - UnsubsAll"), m_strUserName.c_str());
							m_MultipleSubscriptions.clear();
							multiGEFilter.count=0;
							break;
						}

						iter->first->GetRawKey(&m_pKeySubscriptionKey[multiGEFilter.count].dbaKey);
						multiGEFilter.count++;
						
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t enMStop, multiGEFilter iterating: %s "), m_strUserName.c_str(), iter->first->GetKeyString().c_str());

						if(multiGEFilter.count > MAX_REQUESTS && !m_bUnsubscribeAll)
						{
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("enMStop, MaxRequest reached"));
							_SubscribeMultipleRecords(&multiGEFilter, m_pKeySubscriptionKey.get(), bNotify);
							::ZeroMemory(m_pKeySubscriptionKey.get(), sizeof(DBA_MAXKEY)*m_MultipleSubscriptions.size());
							multiGEFilter.count = 0;
						}
						if(m_MultipleSubscriptions.empty())
						{
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t enMStop, m_MultipleSubscriptions.empty..."), m_strUserName.c_str());						
							break;
						}
					}
					if(!m_bUnsubscribeAll  && multiGEFilter.count)
					{
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t enMStop, Subscribing Multiple Records"), m_strUserName.c_str());
						_SubscribeMultipleRecords(&multiGEFilter, m_pKeySubscriptionKey.get(), bNotify);
					}
				}
				m_bMultipleSubscription = false;

				EgLib::CAutoLock lock(m_csMultipleSubscriptions);
				m_MultipleSubscriptions.clear();
			}break;
		default:
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t Subs default"), m_strUserName.c_str());
				if(m_bMultipleSubscription)
				{
					m_bUnsubscribeAll = false;
					DBA_MAXKEY maxKey;
					::ZeroMemory(&maxKey, sizeof(maxKey));

					_SetDbKey(Params->m_recParams, maxKey.dbaKey);
					CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString(&maxKey.dbaKey));
					{
						EgLib::CAutoLock lock(m_csMultipleSubscriptions);
						m_MultipleSubscriptions[pHashedKey] = Params;
					}
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Multiple, m_MultipleSubscriptions filling: %s"), m_strUserName.c_str(), pHashedKey->GetKeyString().c_str());
				}
				else
				{
					if(bInternal && m_bUnsubscribeAll)
					 {
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, UnsubsAll or Internal"), m_strUserName.c_str());
						return;
					 }
						
					else
					{
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple"), m_strUserName.c_str());
						m_bUnsubscribeAll = false;
					}

					if (Params->m_recParams->Type == enFUT || Params->m_recParams->Type == enFOP )
					{

						bool bGotUsual = false;
						bool bGotGlobex= false;
						_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);


						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t Get Future EqualCall: symbol - %s, Exchange - %s"), m_strUserName.c_str(), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
						long nResult = AddInterest(bNotify);
						// try to get usually
						if(nResult == DBA_ERR_NO_ERROR)
						{	
							bGotUsual = true;
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Add Interest, NO_ERROR"), m_strUserName.c_str());

							CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));
							CSubscriptionInfoPtr pSubscription = AddSubscription(pKey, Params->m_recParams->Type);

							//StoreClosePrice(pKey,  m_dbaRec.extended.close.price);

							if(bNotify)
							{
								CResponseBasePtr pResponse = boost::shared_dynamic_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(Params->m_recParams)));
								PublicResponse(pResponse);
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Notify"), m_strUserName.c_str());
							}
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Public Quote Update"), m_strUserName.c_str());
							if(pSubscription!=NULL)
							{

								pSubscription->SetClosePrice(m_dbaRec.extended.close.price);

								CDBARecordPtr pExtended = CDBARecordPtr(new DBA_RECORD_3);
								if(pExtended)
								{
									memcpy(pExtended.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
									pSubscription->AssignData(pExtended);
									PublicQuoteUpdate(&Params->m_recParams, pSubscription);
								}
								else
									EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t Unable to allocate memory"), m_strUserName.c_str());

							}
						}
						else // try to get Globex - specific // if(bFireError)
						{
							_QuoteUpdateParams globexParam;
							globexParam.Init();
							_bstr_t bsSymbol(Params->m_recParams->Symbol);
							bsSymbol+=L"G";

							globexParam->Symbol = bsSymbol.copy();
							globexParam->Type =Params->m_recParams->Type;
							globexParam->Exchange = Params->m_recParams->Exchange!=NULL?SysAllocString(Params->m_recParams->Exchange):0;

							//wcsncat(Params->m_recParams->Symbol, L"G",1);
							_SetDbKey(globexParam, m_dbaKey.dbaKey);
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("Get EqualCall: symbol - %s, Exchange - %s"), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
							nResult = AddInterest();
							// try to get usually
							if(nResult == DBA_ERR_NO_ERROR)
							{		
								bGotGlobex = true;
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Add Interest, NO_ERROR"), m_strUserName.c_str());

								CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));
								CSubscriptionInfoPtr pSubscription = AddSubscription(pKey, Params->m_recParams->Type);

								//StoreClosePrice(pKey,  m_dbaRec.extended.close.price);

								if(bNotify)
								{
									CResponseBasePtr pResponse = boost::shared_dynamic_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(Params->m_recParams)));
									PublicResponse(pResponse);
									EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Notify"), m_strUserName.c_str());
								}
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Public Quote Update"), m_strUserName.c_str());
								if(pSubscription!=NULL)
								{
									pSubscription->SetClosePrice(m_dbaRec.extended.close.price);

									CDBARecordPtr pExtended = CDBARecordPtr(new DBA_RECORD_3);
									if(pExtended)
									{
										memcpy(pExtended.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
										pSubscription->AssignData(pExtended);
										PublicQuoteUpdate(&Params->m_recParams, pSubscription);
									}
									else
										EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t Unable to allocate memory"), m_strUserName.c_str());

								}
							}
						}


						//if(((!bGotUsual)&&(!bGotGlobex))&&(bFireError))
						if(!bGotUsual && !bGotGlobex )
						{	
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, Error occured"));
							CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
							pError->m_enRequestType = enSubscribeQuote;

							CComVariant vtReq;
							const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			

							if(nResult == DBA_ERR_KEY_NOT_FOUND)
							{
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, DBA_ERR_KEY_NOT_FOUND"));
								pError->m_bstrDescription = L"Could not subscribe symbol";
								pError->m_Error = enNoDataAvailableForSymbol;
							}
							else if(nResult == DBA_ERR_INTEREST)
							{
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, DBA_ERR_INTEREST"));
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

								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, Unknown Error with %s"), _T(buffer) );
							}

							PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
							EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t %s"), m_strUserName.c_str(), (LPCSTR)pError->m_bstrDescription);
						}


					} // - future
					else
					{
						_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Add Interest"), m_strUserName.c_str());
						long nResult = AddInterest(bNotify);
						if(nResult == DBA_ERR_NO_ERROR)
						{
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Add Interest, NO_ERROR"), m_strUserName.c_str());

							CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));
							CSubscriptionInfoPtr pSubscription = AddSubscription(pKey, Params->m_recParams->Type);

							//StoreClosePrice(pKey,  m_dbaRec.extended.close.price);

							if(bNotify)
							{
								CResponseBasePtr pResponse = boost::shared_dynamic_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(Params->m_recParams)));
								PublicResponse(pResponse);
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Notify"), m_strUserName.c_str());
							}
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Public Quote Update"), m_strUserName.c_str());
							if(pSubscription!=NULL)
							{
								pSubscription->SetClosePrice(m_dbaRec.extended.close.price);

								CDBARecordPtr pExtended = CDBARecordPtr(new DBA_RECORD_3);
								if(pExtended)
								{
									memcpy(pExtended.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
									pSubscription->AssignData(pExtended);
									PublicQuoteUpdate(&Params->m_recParams, pSubscription);
								}
								else
									EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t Unable to allocate memory"), m_strUserName.c_str());

							}
						}
						else
						{
							EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, Error occured"));
							CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
							pError->m_enRequestType = enSubscribeQuote;

							CComVariant vtReq;
							const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			

							if(nResult == DBA_ERR_KEY_NOT_FOUND)
							{
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, DBA_ERR_KEY_NOT_FOUND"));
								pError->m_bstrDescription = L"Could not subscribe symbol";
								pError->m_Error = enNoDataAvailableForSymbol;
							}
							else if(nResult == DBA_ERR_INTEREST)
							{
								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, DBA_ERR_INTEREST"));
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

								EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("default, Not Multiple, AddInterest, Unknown Error with %s"), _T(buffer) );
							}

							PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
							EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t %s"), m_strUserName.c_str(), (LPCSTR)pError->m_bstrDescription);
						}
					}
				}
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Subscribe Exit"));
}

void CPrice::Unsubscribe(const _variant_t& vtParams)
{
	try
	{
		EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("[%s]\t Enter %s"), m_strUserName.c_str());
		ATLTRACE(_T("CPrice::Unsubscribe Enter\n"));
//		#ifdef _DEBUG
//			WriteUnsubsTime();
//			WriteUnsubsLineLN(_T("CPrice::Unsubscribe Enter\n"));
//		#endif
		if(!utils::HasValue(&vtParams))
		{
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("Unsubscribe All"));
			UnsubscribeAll();
		}
		else
		{
			if(!m_Subscriptions.empty())
			{
				_QuoteUpdateParams Params( vtParams );
				EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("Unsubscribe %s %s %d"),Params->Symbol!=NULL?(LPCSTR)_bstr_t(Params->Symbol):_T(""), Params->Exchange!=NULL?(LPCSTR)_bstr_t(Params->Exchange):_T(""), Params->Type);
				_SetDbKey(Params, m_dbaKey.dbaKey);
				if(DBA_ERR_NO_ERROR == DecInterest(&m_dbaKey))
				{
					CHahedKeyStringPtr pKeyDelete = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaKey.dbaKey));
					DeleteSubscription(pKeyDelete);
				}
			}
		}
		ATLTRACE(_T("CPrice::Unsubscribe Exit\n"));
		EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("[%s]\t Exit %s"), m_strUserName.c_str());
//		#ifdef _DEBUG
//			WriteUnsubsLineLN(_T("CPrice::Unsubscribe Exit\n"));
//		#endif
	}
	catch(_com_error& e)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Com Error %d %s"), e.Error(), (LPCSTR)e.Description() );
	}
	catch (...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Unknown Error"));
	}
}

long CPrice::GetMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t GetMultiple Enter %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		lRet =  dba(&m_blk, DBA_MULTI_GET_EQUAL,	pKey, &nKeyLen,	pFilrer, &nRecLen, 0);
		if(lRet)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t GetMultiple Return %d"), m_strUserName.c_str(), lRet);
		//	return lRet;
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t GetMultiple Waiting infinite %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol) );
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DBA_MULTI_GET_EQUAL Exit %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol) );
	return lRet;
}

long CPrice::AddInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t AddInterstMultiple Enter %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		lRet =  dba(&m_blk, DBA_MULTI_GET_EQUAL | DBAX_INTEREST/*| DBA_ADD_INTEREST*/, pKey, &nKeyLen,	pFilrer, &nRecLen, 0);
		if(lRet)
		{
			EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("[%s]\t AddInterstMultiple Return %d"), m_strUserName.c_str(), lRet);
			//return lRet;
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t AddInterstultiple Infinite Wait %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
		}

	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t AddInterstMultiple Exit %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
	return lRet;
}

long CPrice::DecInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{

	char szKey[MAX_TICKER*2+1];
	szKey[MAX_TICKER*2] = 0;
	CUtilities::KeyToString(&pKey->dbaKey, szKey, MAX_TICKER*2);

	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Enter %s on %i"), m_strUserName.c_str(), szKey, pFilrer->count);
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
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Return %d "), m_strUserName.c_str(), lRet);
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Infinite Wait %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
			ATLTRACE(_T("DecInterstMultiple_CP3\n"));
			long lRett = dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple End Wait with code %i"), m_strUserName.c_str(), lRett);

		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	ATLTRACE(_T("DecInterstMultiple_CP4\n"));
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Exit %s"), m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
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

long CPrice::DecInterest(DBA_KEYS* pKeys)
{
	
	long lRet = 0;
	try
	{
		lRet = dbax(&m_blk, DBA_DEC_INTEREST, pKeys, &m_dbaRec, 0);
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return lRet;

}

void CPrice::UnsubscribeAll(CVectorKeys& vecLocal)
{
	CAutoLock lock(m_cUnsubscribeAll);
	try
	{
		m_SubsctiptionVector.clear();

		MULTIGE_FILTER		multiGEFilter;
		ZeroMemory(&multiGEFilter, sizeof(multiGEFilter));
		multiGEFilter.nRecLen				= sizeof(DBA_RECORD);
		multiGEFilter.count				= 0;
		multiGEFilter.flags				= 0;
		multiGEFilter.pUser             = this;
		multiGEFilter.lpfnDbMultiGEProc	= _MultipleDecInterest; //_MultipleRequests;

		std::sort(vecLocal.begin(), vecLocal.end());
		m_SubsctiptionVector.insert(m_SubsctiptionVector.end(),vecLocal.begin(),vecLocal.end());

		m_pUnsubscribeKey = boost::shared_array<DBA_MAXKEY>(new DBA_MAXKEY[vecLocal.size()]);
		::ZeroMemory(m_pUnsubscribeKey.get(), sizeof(DBA_MAXKEY)*vecLocal.size());

		multiGEFilter.count=0;
		for(CVectorKeys::iterator iter = vecLocal.begin(); iter !=  vecLocal.end(); ++iter)
		{
			(*iter)->GetRawKey(&m_pUnsubscribeKey[multiGEFilter.count].dbaKey);
			multiGEFilter.count++;

			if(multiGEFilter.count > MAX_REQUESTS)
			{
				ATLTRACE("if(multiGEFilter.count > MAX_REQUESTS)\n");
				if(DecInterstMultiple(&multiGEFilter, m_pUnsubscribeKey.get()))
				{
					int iCount = vecLocal.size();
					for(CVectorKeys::iterator iter = vecLocal.begin(); iter !=  vecLocal.end(); ++iter)
					{
						ATLTRACE("subscription map iterating\n");
						EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DecInterst %s on %i"), m_strUserName.c_str(), (*iter)->GetKeyString().c_str(), iCount--);

						DBA_KEYS Keys;
						ZeroMemory(&Keys, sizeof(DBA_KEYS));
						(*iter)->GetRawKey(&Keys.dbaKey);
						DecInterest(&Keys);		
					}
					ATLTRACE("clear\n");
//					if(m_)
//					CHahedKeyStringPtr _hashedKey;
//					DeleteSubscription(_hashedKey);
					multiGEFilter.count = 0;
					break;
				}
				ATLTRACE("Zero m_pUnsubscribeKey\n");
				::ZeroMemory(m_pUnsubscribeKey.get(), sizeof(DBA_MAXKEY)*vecLocal.size());
				multiGEFilter.count = 0;
			}
		}
		if(multiGEFilter.count)
		{
			ATLTRACE("DecInterstMultiple\n");
			//				#ifdef _DEBUG
			//					WriteUnsubsLineLN(_T("CPrice::Unsubscribe if(multiGEFilter.count) DecInterestMultiple call"));
			//				#endif
			if(DecInterstMultiple(&multiGEFilter, m_pUnsubscribeKey.get()))
			{
				ATLTRACE(_T("Iterate subscription map\n"));
				int iCount = vecLocal.size();

				for(CVectorKeys::iterator iter = vecLocal.begin(); iter !=  vecLocal.end(); ++iter)
				{
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t DecInterst %s on %i"), m_strUserName.c_str(), (*iter)->GetKeyString(), iCount--);
					(*iter)->GetRawKey(&m_dbaKey.dbaKey);
					ATLTRACE("DecInterst\n");
					DecInterest(&m_dbaKey);		
				}
//				CHahedKeyStringPtr _hashedKey;
//				DeleteSubscription(_hashedKey);
				multiGEFilter.count=0;
			}
		}
		ATLTRACE("ClearSubscript\n");
		//			m_mapSubscript.clear();
	}
	_CATCH_UNHANDLED_EXCEPTION;
	ATLTRACE(_T("CPrice::UnsubscribeAll Exit\n"));
}

void CPrice::UnsubscribeAll()
{		 
	EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t Unsubscribe All"), m_strUserName.c_str()); 
	m_bUnsubscribeAll = true;

	m_hfServer.Unsubscribe(this);
	try
	{
		if(!m_Subscriptions.empty())
		{
			CVectorKeys  vecLocal;
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t %d Active Subscriptions"),m_strUserName.c_str(), m_Subscriptions.size()); 
			{

				CReaderAutoLock lock(m_csSubscript);
				subscriptionSet ::const_iterator itrCpy  = m_Subscriptions.begin();
				subscriptionSet ::const_iterator itrCpyEnd  = m_Subscriptions.end();
				while(itrCpy != itrCpyEnd)
				{
					vecLocal.push_back((*itrCpy)->GetHashedKey());
					++itrCpy;
				}
			}

			CHahedKeyStringPtr _hashedKey;
			DeleteSubscription(_hashedKey, false);
			UnsubscribeAll(vecLocal);
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__, _T("[%s]\t No Active Subscriptions"), m_strUserName.c_str()); 
			CHahedKeyStringPtr _hashedKey;
			DeleteSubscription(_hashedKey, false);
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	ATLTRACE(_T("CPrice::UnsubscribeAll Exit\n"));
}

void CPrice::ProcessMessage(CSubscriptionInfoPtr& pExtRec)
{
	if( pExtRec == NULL || pExtRec->GetData()==NULL )
		return;

	try
	{
		_QuoteUpdateParams Params;
		//EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("[%s]\t ProccessMessages Enter %s"), m_strUserName.c_str(), pExtRec->GetKey().c_str());
//		if(!pExtRec->GetClosePrice())
//			pExtRec->SetClosePrice(GetClosePrice(pExtRec->GetHashedKey()));

		CDBARecordPtr pRec = pExtRec->GetData();
		if(pRec != NULL)
		{
			DBA_KEYS* pDbaKey = (DBA_KEYS*)&pRec->item;	 
			ParamFromKey(pDbaKey->dbaKey, pExtRec->GetType() , Params);
			pRec->close.price = pExtRec->GetClosePrice();
		}
		// fix hyperfeed bug with incorrect type for index quotes...
		Params->Type = pExtRec->GetType();
		PublicQuoteUpdate(&Params, pExtRec);

#ifdef __PERFMON
		if(m_pPerfMon)
			m_pPerfMon->AddProcessedUpdate();
		if(m_spGlobalPerfCounter)
			m_spGlobalPerfCounter->AddProcessedUpdate();
#endif // __PERFMON
	}
_CATCH_UNHANDLED_EXCEPTION;
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
		PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
		long nRes;
		if ( enType == enConnectionWasDisconnected )
		{
			m_bWasDisconnected = true;
		}

		if (enType == enProviderConnected)
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Was disconnected.... Connected now"));
			
			if (( m_bWasDisconnected) && (m_bWasConnectedOnce))
			{
				m_bWasConnectedOnce = true;
				EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("ReSubscribe after Reconnect..."));
				m_bWasDisconnected = false;
				nRes = CPrice::Detach();
				if(nRes != DBA_ERR_NO_ERROR)
				{
					HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
					EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Couldn't detach after reconnect..."));
				}
				nRes = CPrice::Attach();
				if(nRes != DBA_ERR_NO_ERROR)
				{
					HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
					EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Couldn't attach after reconnect..."));
				}
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::ErrorMessage(long lErrorMessage)
{
	_bstr_t bs =  L"Server fatal status. ETI Status: ";		
	bs += EtGetMessage(DBA_ERROR,m_hfServer.GetStatus());
	bs += L" (";
	bs += _bstr_t(lErrorMessage);
	bs += L")";
	_QuoteUpdateParams empty;
	empty.Init();
	CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
	pError->m_enRequestType = enSubscribeQuote;
	empty.CopyTo(pError->m_vtRequest);			
	pError->m_bstrDescription = bs;
	pError->m_Error = enProviderInternalError;

	EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("[%s]\t RealTime Message - Error %s"), m_strUserName.c_str(), (LPCSTR)bs);	
	PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
}

void CPrice::RealtimeMessage(CHahedKeyStringPtr pKey, CDBARecordPtr etiPtr)
{
	int iPosition = 0;
	try
	{
		if ( etiPtr == NULL || pKey == NULL)
		{
			_ASSERTE(FALSE);
		}
		else
		{
			iPosition++;
			if(!m_bUnsubscribeAll)	
			{
				CSubscriptionInfoPtr  pSubsctiption;
				bool bProcessed = GetSubscribed(pKey, pSubsctiption, false );
				if(bProcessed)
				{
					InterlockedExchange(&m_lProcessDenied, 0L);

					if(pSubsctiption != NULL)
					{
						iPosition++;
						//Subscribed
#ifdef __PERFMON
						if(m_pPerfMon)
							m_pPerfMon->AddClientRealTimeUpdate();
						if(m_spGlobalPerfCounter)
							m_spGlobalPerfCounter->AddClientRealTimeUpdate();
#endif // __PERFMON

						pSubsctiption->AssignData(etiPtr);
						iPosition++;
						PostIntoRealtimeMessageProcessor(pSubsctiption); //OnData(pSubsctiption);
						iPosition++;
						//m_DBAStrategy.PushData(pSubsctiption);

#ifdef __PERFMON
						if(m_pPerfMon)
							m_pPerfMon->QuoteUpdatesQueueSnapshot = InterlockedIncrement(&m_lRTResponses);
						if(m_spGlobalPerfCounter)
							m_spGlobalPerfCounter->QuoteUpdatesQueueSnapshot = InterlockedIncrement(&CHFServer::m_lRTResponse);
#endif // __PERFMON
					}
				}
				else
				{
					if(InterlockedIncrement(&m_lProcessDenied)> 5000)
						EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ ,_T("[%s]\t Skipped. The component is write locked"), m_strUserName.c_str());	
				}
			}
			else
			{
				EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ ,_T("[%s]\t Skipped. Unsubscribe in progress"), m_strUserName.c_str());	
			}
		}
	}
	catch(...)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ ,_T("[%s]\t Unknown Exception: Position %d"),m_strUserName.c_str(), iPosition);	
	}
}

void CPrice::CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3* pRec, _QuoteUpdateInfo& result)
{
	try
	{
		result.Init();
		double dClosePrice = BAD_DOUBLE_VALUE;
		double dLastPrice  = BAD_DOUBLE_VALUE;

		result->BidPrice = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->bid.price));
		result->AskPrice = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->ask.price));

		result->BidSize = InitByLong(pRec->bid.size);
		result->AskSize = InitByLong(pRec->ask.size);
		result->SeqNum = 0;

		result->Currency = SysAllocString(L"USD");
		result->LastPrice  = dLastPrice = SetDoubleValue(Price2Dbl( pRec->priceType, pRec->lastSale.price));
		result->ClosePrice = dClosePrice= SetDoubleValue(Price2Dbl( pRec->priceType, pRec->close.price));

		if(!IsInvalidDoubleValue(dClosePrice) && !IsInvalidDoubleValue(dLastPrice))
			result->NetChange = dLastPrice - dClosePrice;
		else
			result->NetChange = BAD_DOUBLE_VALUE;

		result->UpdateDateTime = EgLib::vt_date::GetCurrentDate();

		result->OpenInterest = InitByLong(pRec->openInterest);
		if( !(pRec->lastSale.hour&0x80) && !(pRec->lastSale.minute&0x80))
			result->Volume = InitByLong(pRec->volume);
		else
		{
			vt_date dtNow =  vt_date::GetCurrentDate(true);
			if(dtNow.get_month() == DWORD(pRec->lastSale.hour&0x7f) && dtNow.get_day() == DWORD(pRec->lastSale.minute&0x7f))
				result->Volume = InitByLong(pRec->volume);
			else
				result->Volume = 0;
		}

		result->OpenPrice  = SetDoubleValue(Price2Dbl( pRec->priceType, pRec->open.price));

		result->HighPrice = SetDoubleValue(Price2Dbl( pRec->priceType, pRec->high.price));
		result->LowPrice  = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->low.price));
		result->PriceTick = pRec->tick;


		result->Status = GetQuoteStatus(pRec->quoteIndicator);

		result->Skipped = 0;
		result->TotalRequests = 0;
		result->WaitTime = 0;

		_bstr_t bsBidExchange(L"");
		_bstr_t bsAskExchange(L"");
		char buffer[2];
		ZeroMemory(buffer, sizeof(buffer));

		if(enOPT == enType || enFOP == enType)
		{
			*buffer = GetETSOptionExchange(*pRec->bid.exchangeCode);
			if(*buffer)
				bsBidExchange = buffer;

			*buffer = GetETSOptionExchange(*pRec->ask.exchangeCode);
			if(*buffer)
				bsAskExchange = buffer;
		}
		else
		{
			*buffer = GetETSStockExchange(*pRec->bid.exchangeCode);
			if(*buffer)
				bsBidExchange = buffer;

			*buffer = GetETSStockExchange(*pRec->ask.exchangeCode);
			if(*buffer)
				bsAskExchange = buffer;

		}
		result->AskExchange = bsAskExchange.copy();
		result->BidExchange = bsBidExchange.copy();

	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::CopyExtendedToResult(const DBA_RECORD_3* pRec, _QuoteUpdateResult& result)
{
	try
	{
		result.Init();

		result->BidPrice = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->bid.price));
		result->AskPrice = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->ask.price));

		result->BidSize = InitByLong(pRec->bid.size);
		result->AskSize = InitByLong(pRec->ask.size);

		result->Currency = SysAllocString(L"USD");
		result->UpdateDateTime = EgLib::vt_date::GetCurrentDate();

		result->OpenInterest = InitByLong(pRec->openInterest);
		result->Volume = InitByLong(pRec->volume);
	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::CopyExtendedToIvResult(const DBA_RECORD_3* pRec, _StockForIvResult& Result)
{
	Result->Bid = Price2Dbl(pRec->priceType, pRec->bid.price);
	Result->Ask = Price2Dbl(pRec->priceType, pRec->ask.price);
	Result->Open = Price2Dbl(pRec->priceType, pRec->open.price);
	Result->High = Price2Dbl(pRec->priceType, pRec->high.price);
	Result->Low = Price2Dbl(pRec->priceType, pRec->low.price);
	Result->Close = Price2Dbl(pRec->priceType, pRec->close.price);
	Result->Volume = pRec->volume;
}

void CPrice::_SetDbKey(const _QuoteUpdateParams& Params, DBA_KEY& dbaKey)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t _SetDbKey Enter for Symbol - %s, Exchange - %s, Type - %d "), m_strUserName.c_str() , (LPCSTR)_bstr_t(Params->Symbol),(LPCSTR)_bstr_t(Params->Exchange) , (double)Params->Type );
	::ZeroMemory(&dbaKey, sizeof(DBA_KEY));
	
	char cExchange = ' ';
	if(Params->Exchange)
	{
		_bstr_t bsExchange(Params->Exchange);
		const char* szExch = bsExchange;
		if(strlen(szExch) == 1)
			cExchange = *szExch;
	}
	CreateSpaced((char*)dbaKey.countryCode, "US", 2);
	CreateSpaced((char*)dbaKey.currencyCode, "USD", 3);
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
	
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__, _T("[%s]\t _SetDbKey Exit for Symbol - %s, Exchange - %s, Currency - %s, Type - %d "),m_strUserName.c_str() ,dbaKey.symbol, dbaKey.exchangeCode, dbaKey.currencyCode, dbaKey.type );
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

void CPrice::CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3* pRec, QuoteUpdateInfo* result)
{

	if(!pRec)
		return;

	result->BidPrice = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->bid.price));
	result->AskPrice = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->ask.price));

	result->BidSize = InitByLong(pRec->bid.size);
	result->AskSize = InitByLong(pRec->ask.size);
	result->SeqNum = 0;

	result->Currency = SysAllocString(L"USD");
	result->LastPrice = SetDoubleValue(Price2Dbl( pRec->priceType, pRec->lastSale.price));
	result->UpdateDateTime = EgLib::vt_date::GetCurrentDate();

	result->OpenInterest = InitByLong(pRec->openInterest);
	result->Volume = InitByLong(pRec->volume);

	result->OpenPrice  = SetDoubleValue(Price2Dbl( pRec->priceType, pRec->open.price));
	result->ClosePrice = SetDoubleValue(Price2Dbl( pRec->priceType, pRec->close.price));

	result->HighPrice = SetDoubleValue(Price2Dbl( pRec->priceType, pRec->high.price));
	result->LowPrice  = SetDoubleValue(Price2Dbl(pRec->priceType, pRec->low.price));
	result->PriceTick = pRec->tick;

	if(enOPT == enType || enFOP == enType)
	{
		char buffer[2];
		buffer[1]= 0;
		buffer[0] = GetETSOptionExchange(*pRec->bid.exchangeCode);
		_bstr_t bsBidExchange(buffer);

		buffer[0] = GetETSOptionExchange(*pRec->ask.exchangeCode);
		_bstr_t bsAskExchange(buffer);

		result->AskExchange = bsAskExchange.copy();
		result->BidExchange = bsBidExchange.copy();
	}
	else
	{
		char buffer[2];
		buffer[1]= 0;
		buffer[0] = GetETSStockExchange(*pRec->bid.exchangeCode);
		_bstr_t bsBidExchange(buffer);

		buffer[0] = GetETSStockExchange(*pRec->ask.exchangeCode);
		_bstr_t bsAskExchange(buffer);

		result->AskExchange = bsAskExchange.copy();
		result->BidExchange = bsBidExchange.copy();
	}

	if(!IsInvalidDoubleValue(result->ClosePrice) && !IsInvalidDoubleValue(result->LastPrice))
		result->NetChange = result->LastPrice - result->ClosePrice;
	else
		result->NetChange = BAD_DOUBLE_VALUE;
	result->Status = GetQuoteStatus(pRec->quoteIndicator);

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

void CPrice::ParamFromKey(const DBA_KEY& key, InstrumentTypeEnum enType, QuoteUpdateParams* pParams)
{
	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)key.symbol, MAX_TICKER);

	switch(key.type) 
	{
	case IT_EQUITY:
		pParams->Type = enType;
		break;
	case IT_INDEX:
		pParams->Type = enType;
		AddBucksInName(szName);
		break;
	case IT_EQUITY_OPTION:
		pParams->Type = enOPT;
		RemoveSpaceFromName(szName);
		break;
	case IT_FUTURE:
		pParams->Type = enFUT;
		break;
	case IT_FUTURE_OPTION:
		pParams->Type = enFOP;
		break;
	}

	if(pParams->Type != enFUT && pParams->Type != enFOP)
	{
		if(pParams->Type == enOPT)
			_GetOptionExchangeFromKey(key, &(pParams->Exchange));
		else
			_GetStockExchangeFromKey(key, &(pParams->Exchange));
	}
	else if(pParams->Type == enFUT)
	{
		_GetFutureExchangeFromName(szName, &(pParams->Exchange));
	}
	else if(pParams->Type == enFOP)
	{
		_GetFutureOptionExchangeFromName(szName, &(pParams->Exchange));
	}

	pParams->Symbol = ::SysAllocString(CA2W(szName));// _bstr_t(szName).copy();
}
void CPrice::_GetOptionExchangeFromKey(const DBA_KEY& key, BSTR* Exchange)
{
	if (key.type == IT_EQUITY_OPTION)
	{
		char szExch[3];
		//ZeroMemory(szExch,sizeof(szExch));
		CreateSz(szExch, (const char*)key.exchangeCode, 2);
		if(*szExch==0)
		{
			*szExch = 0;
			*Exchange = ::SysAllocString(L"");

		}
		else
		{
			*szExch = GetETSOptionExchange(*szExch);
			*Exchange = ::SysAllocString(CA2W(szExch));// _bstr_t(szExch).copy();
		}
	}
}
void CPrice::_GetStockExchangeFromKey(const DBA_KEY& key, BSTR* Exchange)
{
	if (key.type == IT_EQUITY || key.type == IT_INDEX )
	{
		char szExch[3];
		//ZeroMemory(szExch,sizeof(szExch));
		CreateSz(szExch, (const char*)key.exchangeCode, 2);
		
		*szExch = GetETSStockExchange(*szExch);
		*Exchange =::SysAllocString(CA2W(szExch)); // _bstr_t(szExch).copy();
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

	CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString((DBA_KEY *)pKey));

	EgLib::CAutoLock lock(pPrice->m_csMultipleRequests);
	CMuttipleRequests::iterator it = pPrice->m_MultipleRequests.find(pHashedKey);

	if(!dwStatus && it != pPrice->m_MultipleRequests.end())
	{
		//pPrice->StoreClosePrice(pHashedKey, pRec->extended.close.price);
#ifdef __PERFMON
		if(pPrice->m_spGlobalPerfCounter!=NULL)
			pPrice->m_spGlobalPerfCounter->AddMultipleRequestResponce();

		if(pPrice->m_pPerfMon!=NULL)
			pPrice->m_pPerfMon->AddMultipleRequestResponce();
#endif // __PERFMON

		CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
		if(pRecord)
		{
			memcpy(pRecord.get(), &pRec->extended, sizeof(DBA_RECORD_3));
			pPrice->PublicLastQuote(&it->second->m_recParams, pRecord);
		} 
	}
	else if(bNotify)
	{
		//error
		// Check if globex futures
		if(it->second->m_recParams->Type != enFUT || !pPrice->ResendQuoteRequest(it->second))
		{
			CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
			pError->m_enRequestType = enRequestLastQuote;
			if(it != pPrice->m_MultipleRequests.end())
				const_cast<_QuoteUpdateParams&>(it->second->m_recParams).CopyTo(pError->m_vtRequest);			
			else
				pError->m_vtRequest = vtMissing;

			pError->m_bstrDescription = L"Unable to get the last quote data";
			pError->m_Error = enNoDataAvailableForSymbol;
			pPrice->PublicResponse(boost::shared_static_cast<CResponseBase>(pError));		
		}
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
	MULTIGE_FILTER* pFilter = (MULTIGE_FILTER*)pVoid;
	CPrice* pPrice = reinterpret_cast<CPrice*>(pFilter->pUser);
	if(pPrice)
	{

		CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString((DBA_KEY *)pKey));
		ULONG 			ulFunction  = ulFunc & DBAX_FUNCTION_MASK;


		//--------------------------------------
		// No more data to process, so all done 
		//--------------------------------------
		if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION)
		{
			//		#ifdef _DEBUG
			//			_write(fh, _T("DBA_TERMINATE_STREAM_FUNCTION"), sizeof(_T("DBA_TERMINATE_STREAM_FUNCTION")) );
			//			_close(fh);
			//		#endif
			EgLib::CEgLibTraceManager::Trace(LogDebug, __FUNCTION__ , _T("Terminate"));
			return TRUE;
		}

#ifdef __PERFMON
		if(pPrice->m_spGlobalPerfCounter!=NULL)
			pPrice->m_spGlobalPerfCounter->AddMultipleUnsubscriptionResponce();

		if(pPrice->m_pPerfMon!=NULL)
			pPrice->m_pPerfMon->AddMultipleUnsubscriptionResponce();
#endif // __PERFMON

		bool bFind = false;
		// Most likely the key is first in the vector
		if(!pPrice->m_SubsctiptionVector.empty())
		{
			if(pHashedKey->GetKeyString() == (*pPrice->m_SubsctiptionVector.begin())->GetKeyString())
			{
				bFind = true; 
				pPrice->m_SubsctiptionVector.pop_front();
			}
			else
			{
				CVectorKeys::iterator itr = std::lower_bound(pPrice->m_SubsctiptionVector.begin(), pPrice->m_SubsctiptionVector.end(), pHashedKey);
				if(itr!=pPrice->m_SubsctiptionVector.end())
				{
					if(pHashedKey->GetKeyString() == (*itr)->GetKeyString())
					{
						bFind = true;                
						pPrice->m_SubsctiptionVector.erase(itr);
					}
				}
			}
		}

		if (pPrice->m_bCancelLastQuote)
		{
			//			#ifdef _DEBUG
			//				_write(fh, _T("CancelLastQuote"), sizeof(_T("CancelLastQuote")) );
			//				_close(fh);
			//			#endif
			EgLib::CEgLibTraceManager::Trace(LogSubsExt, __FUNCTION__ , _T("[%s]\t Quote Cancel  %s on %i"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str(), pPrice->m_SubsctiptionVector.size());
			return FALSE;
		}


		if(!dwStatus && !bFind)
		{

			//			#ifdef _DEBUG
			//				_write(fh, _T("MultipleSubs"), sizeof(_T("MultipleSubs")) );
			//				_close(fh);
			//			#endif
			EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("[%s]\t STOPPED Unsubscribed %s left %i"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str(), pPrice->m_SubsctiptionVector.size());
			return TRUE;
		}
		else
			EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("[%s]\t Unsubscribed %s left %i"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str(), pPrice->m_SubsctiptionVector.size());

	}
	
//	#ifdef _DEBUG
//		_close(fh);
//	#endif
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
			CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString((DBA_KEY *)pKey));

			EgLib::CAutoLock lock(pPrice->m_csMultipleSubscriptions);
			CMuttipleRequests::iterator it = pPrice->m_MultipleSubscriptions.find(pHashedKey);

//			pPrice->m_MultipleSubscriptions
//			pPrice->m_csMultipleSubscriptions 

			if(!dwStatus && it != pPrice->m_MultipleSubscriptions.end())
			{
				EgLib::CEgLibTraceManager::Trace(LogSubsExt, __FUNCTION__, _T("[%s]\t Multiple Option Subscribe %s"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str()); 
				CSubscriptionInfoPtr pSubs = pPrice->AddSubscription(pHashedKey, it->second->m_recParams->Type);
				//pPrice->StoreClosePrice(pHashedKey, );

				if(pPrice->m_bUnsubscribeAll)
					return TRUE;

				if(bNotify)
				{
					CResponseBasePtr pResponse = boost::shared_static_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(it->second->m_recParams)));
					pPrice->PublicResponse(pResponse);
				}

				if(pSubs)
				{
					CDBARecordPtr pDataRec = CDBARecordPtr(new DBA_RECORD_3);
					pSubs->SetClosePrice(pRec->extended.close.price);

					if(pDataRec)
					{
						memcpy(pDataRec.get(), &pRec->extended, sizeof(DBA_RECORD_3));
						pSubs->AssignData(pDataRec);
						pPrice->PublicQuoteUpdate(&it->second->m_recParams, pSubs);
					}
				}
			}
			else
			{
				if(it->second->m_recParams->Type != enFUT || !pPrice->ResendQuoteRequest(it->second))
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
					pPrice->PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
					EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("[%s]\t %s"), pPrice->m_strUserName.c_str(), (LPCSTR)pError->m_bstrDescription);
				}

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

HRESULT CPrice::OnStrategyInitialize()
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
void  CPrice::OnStrategyUninitialize()
{
	CoUninitialize();
}

void CPrice::OnData(CSubscriptionInfoPtr& Data)
{
#ifdef __PERFMON
	InterlockedDecrement(&m_lRTResponses);
	InterlockedDecrement(&CHFServer::m_lRTResponse);
#endif // __PERFMON
	ProcessMessage(Data);
#ifdef __PERFMON
	if(m_pPerfMon)
		m_pPerfMon->QuoteUpdatesQueueSnapshot = m_lRTResponses;
    if(m_spGlobalPerfCounter)
		m_spGlobalPerfCounter->QuoteUpdatesQueueSnapshot = CHFServer::m_lRTResponse;
#endif // __PERFMON

}


