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
	m_bWasDisconnected = false;
	m_bWasConnectedOnce = false;

	m_bMultipleRequests      = false;
	m_bMultipleSubscription  = false;

	m_bUnsubscribeAll        = false;
	m_bCancelLastQuote       = false;
	m_hDataReady.Attach(::CreateEvent(NULL, FALSE,FALSE,NULL));
	m_spGlobalPerfCounter = CHFServer::GetGlobalPerfMonitor();
	if(m_spGlobalPerfCounter!=NULL)
		m_spGlobalPerfCounter->Clients = ++CHFServer::m_lClients;

	if(long nResult = m_hfServer.Initialize() != DBA_ERR_NO_ERROR)
		return nResult;
	m_DBAStrategy.Start();
	return CExtended::Attach();
}
long CPrice::Detach()
{
	if(m_spGlobalPerfCounter!=NULL)
	{
		m_spGlobalPerfCounter->Clients = --CHFServer::m_lClients;;
		m_spGlobalPerfCounter = NULL;
	}

	m_DBAStrategy.Stop();
	UnsubscribeAll();
//	m_hfServer.Unsubscribe(this);
	m_hfServer.Uninitialize();
	//_ASSERTE(m_mapSubscript.size() == 0);
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("Assert failed at detach - mapSubscrip is empty"));
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

	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Get price Enter") ,m_strUserName.c_str() );
	const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
	if(!Params)
	{
		
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Failed to get params"),m_strUserName.c_str());
		return nResult;
	}
		
	switch(Params->m_recParams->Type)
	{
	case enMStart:
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t enMStart"),m_strUserName.c_str());
			m_bCancelLastQuote = false;
			EgLib::CAutoLock lock(m_csMultipleRequests);
			m_MultipleRequests.clear();
			m_AllreadySubscribedInMultipleRequest.clear();
			m_bMultipleRequests = true;

		} break;
	case enMStop:
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t enMStop"),m_strUserName.c_str());
			if(m_AllreadySubscribedInMultipleRequest.size())
			{
				std :: deque<CSubscriptionInfoPtr>::iterator itSubscribed = m_AllreadySubscribedInMultipleRequest.begin();
				while(itSubscribed != m_AllreadySubscribedInMultipleRequest.end())
				{
					EgLib::CAutoLock lock(m_csMultipleRequests);
					CMuttipleRequests::iterator it = m_MultipleRequests.find((*itSubscribed)->GetHashedKey());
					if(it != m_MultipleRequests.end())
					{
						if( (*itSubscribed)->IsValid() )
						{
							CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
							if(pRecord)
							{
								memcpy(pRecord.get(), (*itSubscribed)->GetData(), sizeof(DBA_RECORD_3));
								PublicLastQuote(&it->second->m_recParams, pRecord );
							}
						}
						else
						{
							CErrorResponsePtr pError = (*itSubscribed)->GetErrorMessage();
							if(pError != NULL)
							{
								it->second->m_recParams.CopyTo(pError->m_vtRequest);								
								pError->m_enRequestType = enRequestLastQuote;
								PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
							}
						}
						it = m_MultipleRequests.erase(it);
					}

					(*itSubscribed)->Release();
					++itSubscribed;
				}
				m_AllreadySubscribedInMultipleRequest.clear();
			}

			if(m_MultipleRequests.size())
			{
				CMuttipleRequests::iterator it = m_MultipleRequests.begin();
				while(it != m_MultipleRequests.end())
				{
					if(it->second->m_recParams->Exchange!=NULL && *it->second->m_recParams->Exchange!=L' ')
					{
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enSubscribeQuote;

						CComVariant vtReq;
						const_cast<_QuoteUpdateParams&>(it->second->m_recParams).CopyTo(pError->m_vtRequest);			

						pError->m_bstrDescription = L"Unable to not get symbol information";
						pError->m_Error = enNoDataAvailableForSymbol;
						pError->m_enRequestType = enRequestLastQuote;
						PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
						it = m_MultipleRequests.erase(it);
					}
					else
						++it;
				}
			}

			if(!m_MultipleRequests.empty())
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

				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Requesting multiple"),m_strUserName.c_str());
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

				EgLib::CAutoLock lock(m_csMultipleRequests);
				m_MultipleRequests.clear();
			}
			m_bMultipleRequests = false;
		} break;
	default:
		{
			DBA_MAXKEY maxKey;
			::ZeroMemory(&maxKey, sizeof(maxKey));
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Default"),m_strUserName.c_str());
			_SetDbKey(Params->m_recParams, maxKey.dbaKey);
			CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString(&maxKey.dbaKey));

			CSubscriptionInfoPtr pGlobal = GetGlobalSubscribed(pHashedKey);

			if(m_bMultipleRequests)
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Multiple"),m_strUserName.c_str());
				if(pGlobal != NULL)
					m_AllreadySubscribedInMultipleRequest.push_back(pGlobal);
					m_bCancelLastQuote = false;
				if (Params->m_recParams->Type == enFUT)
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Requesting: %s"),m_strUserName.c_str(), pHashedKey->GetKeyString().c_str());
					EgLib::CAutoLock lock(m_csMultipleRequests);
					m_MultipleRequests[pHashedKey] = Params;
				}
				else
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Requesting: %s"),m_strUserName.c_str(), pHashedKey->GetKeyString().c_str());
					EgLib::CAutoLock lock(m_csMultipleRequests);
					m_MultipleRequests[pHashedKey] = Params;
				}
			}
			else
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Not Multiple"),m_strUserName.c_str());
				if(m_bCancelLastQuote && bInternal)
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Error"),m_strUserName.c_str());
					if(pGlobal)
						pGlobal->Release();

					return ERROR_SUCCESS;
				}
				else
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t No Error"),m_strUserName.c_str());
					m_bCancelLastQuote = false;
				}

				if(pGlobal)
				{
					CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
					if(pRecord)
					{
						memcpy(pRecord.get(), pGlobal->GetData(), sizeof(DBA_RECORD_3));
						PublicLastQuote(&Params->m_recParams, pRecord );
					}
					pGlobal->Release();
					return ERROR_SUCCESS;
				}
				if(Params->m_recParams->Exchange && _bstr_t(Params->m_recParams->Exchange).length() && _bstr_t(Params->m_recParams->Exchange) != _bstr_t(" "))
				{
					CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
					pError->m_enRequestType = enRequestLastQuote;
					const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			
					pError->m_bstrDescription = L"Could not get the last quote data";
					pError->m_Error = enNoDataAvailableForSymbol;
					PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Key not found"),m_strUserName.c_str());
					return ERROR_SUCCESS;
				}

		
				if (Params->m_recParams->Type == enFUT)
				{

					bool bGotUsual = false;
					bool bGotGlobex= false;
				
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Get EqualCall: symbol - %s, Exchange - %s"), m_strUserName.c_str(), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
					nResult = GetEqual();
					// try to get usually
					if(nResult == DBA_ERR_NO_ERROR)
					{		
						bGotUsual = true;
						CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));
						StoreClosePrice(pKey, m_dbaRec.extended.close.price);
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Publishing last"),m_strUserName.c_str());
					
						CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
						if(pRecord)
						{
							memcpy(pRecord.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
							PublicLastQuote(&Params->m_recParams, pRecord);
						}
					}
					//else // try to get Globex - specific // if(bFireError)
					//{
					/*

						wcsncat(Params->m_recParams->Symbol, L"G",1);
						_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("Get EqualCall: symbol - %s, Exchange - %s"), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
						nResult = GetEqual();
						// try to get usually
						if(nResult == DBA_ERR_NO_ERROR)
						{		
							bGotGlobex = true;
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("GetEqual Succeded"));
							char szSymbol[MAX_LENGTH + 1];
							ZeroMemory(szSymbol,sizeof(szSymbol));
							KeyToString(&m_dbaRec.extended.item, szSymbol, MAX_LENGTH);
							{
								EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("KeyToString: %s"), szSymbol);
								EgLib::CAutoLock lock(m_csClosePrice);
								m_mapClosePrices[szSymbol] =  m_dbaRec.extended.close.price;
							}
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("Publishing last"));
							PublicLastQuote(Params->m_recParams, m_dbaRec.extended);
						}
					//}
					*/

					//if(((!bGotUsual)&&(!bGotGlobex))&&(bFireError))
					if( !bGotUsual && bFireError)
					{	
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Error firing"),m_strUserName.c_str());
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enRequestLastQuote;
						const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			
						if(nResult == DBA_ERR_KEY_NOT_FOUND)
						{
							pError->m_bstrDescription = L"Could not get the last quote data";
							pError->m_Error = enNoDataAvailableForSymbol;
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("Key not found"));
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
							EgStd::CEgTracingClass::TraceStatic(enlogSystem, __FUNCTION__, _T("[%s]\t Error getting last quote: %s"),m_strUserName.c_str(), buffer );
						}
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Public Error"),m_strUserName.c_str());
						PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
					}


				} // - future
				else
				{
					_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
					
					
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Get EqualCall: symbol - %s, Exchange - %s"),m_strUserName.c_str(), m_dbaKey.dbaKey.symbol, m_dbaKey.dbaKey.exchangeCode);
					nResult = GetEqual();
					if(nResult == DBA_ERR_NO_ERROR)
					{		
						char szSymbol[MAX_LENGTH + 1];
						ZeroMemory(szSymbol,sizeof(szSymbol));

						CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));
						StoreClosePrice(pKey, m_dbaRec.extended.close.price);

						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Publishing last"),m_strUserName.c_str());
						CDBARecordPtr pRecord = CDBARecordPtr(new DBA_RECORD_3);
						if(pRecord)
						{
							memcpy(pRecord.get(), &m_dbaRec.extended, sizeof(DBA_RECORD_3));
							PublicLastQuote(&Params->m_recParams, pRecord);
						}
					}
					else if(bFireError)
					{	
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Error firing"),m_strUserName.c_str());
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enRequestLastQuote;
						const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			
						if(nResult == DBA_ERR_KEY_NOT_FOUND)
						{
							pError->m_bstrDescription = L"Could not get the last quote data";
							pError->m_Error = enNoDataAvailableForSymbol;
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Key not found"),m_strUserName.c_str());
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
							EgStd::CEgTracingClass::TraceStatic(enlogSystem, __FUNCTION__, _T("[%s]\t Error getting last quote: %s"),m_strUserName.c_str(), buffer );
						}
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Public Response"),m_strUserName.c_str());
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
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t SubscribeMultiple Records Enter"),m_strUserName.c_str());
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
			EgStd::CEgTracingClass::TraceStatic(enLogError, __FUNCTION__ , _T("[%s]\t SubscribeMultiple Error %d"),m_strUserName.c_str(), nResult);

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
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t SubscribeMultiple Records Exit"),m_strUserName.c_str());
	return bRet;
}

void CPrice::Subscribe(const CRequestBasePtr& pParams, bool bNotify, bool bInternal)
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Subscribe Enter..."));
	try
	{
		long nResult = 0;
		const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
		switch(Params->m_recParams->Type)
		{
		case enMStart:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t Subs enMStart"),m_strUserName.c_str());
				m_bUnsubscribeAll = false;
				EgLib::CAutoLock lock(m_csMultipleSubscriptions);
				m_MultipleSubscriptions.clear();
				m_AllreadySubscribedInMultipleRequest.clear();
				m_bMultipleSubscription = true;
			}break;
		case enMStop:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t Subs enMStop"),m_strUserName.c_str());

				if(!m_AllreadySubscribedInMultipleRequest.empty())
				{
					std :: deque<CSubscriptionInfoPtr>::iterator itSubscribed = m_AllreadySubscribedInMultipleRequest.begin();
					while(itSubscribed != m_AllreadySubscribedInMultipleRequest.end())
					{
						EgLib::CAutoLock lock(m_csMultipleRequests);
						CMuttipleRequests::iterator it = m_MultipleSubscriptions.find((*itSubscribed)->GetHashedKey());
						if(it != m_MultipleSubscriptions.end())
						{
							if((*itSubscribed)->IsValid())
							{

								CSubscriptionInfoPtr pSubs = AddSubscription((*itSubscribed)->GetHashedKey(), (*itSubscribed)->GetType());
								if(bNotify)
								{
									CResponseBasePtr pResponse = boost::shared_static_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(it->second->m_recParams)));
									PublicResponse(pResponse);
								}
								if(pSubs)
									PublicQuoteUpdate(&it->second->m_recParams, pSubs);
							}
							else
							{
								CErrorResponsePtr pError = (*itSubscribed)->GetErrorMessage();
								if(pError != NULL)
								{
									it->second->m_recParams.CopyTo(pError->m_vtRequest);								
									pError->m_enRequestType = enSubscribeQuote;
									PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
								}
							}
							it = m_MultipleSubscriptions.erase(it);
						}
						(*itSubscribed)->Release();
						++itSubscribed;
					}
					m_AllreadySubscribedInMultipleRequest.clear();
				}
				if(!m_MultipleSubscriptions.empty())
				{
					CMuttipleRequests::iterator it = m_MultipleSubscriptions.begin();
					while(it != m_MultipleSubscriptions.end())
					{
						if(it->second->m_recParams->Exchange!=NULL && *it->second->m_recParams->Exchange!=L' ')
						{
							CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
							pError->m_enRequestType = enSubscribeQuote;

							CComVariant vtReq;
							const_cast<_QuoteUpdateParams&>(it->second->m_recParams).CopyTo(pError->m_vtRequest);			

							pError->m_bstrDescription = L"Unable to not get symbol information";
							pError->m_Error = enNoDataAvailableForSymbol;
							pError->m_enRequestType = enSubscribeQuote;
							PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
							it = m_MultipleSubscriptions.erase(it);
						}
						else
							++it;
					}
				}

				m_bUnsubscribeAll = false;

				if(!m_MultipleSubscriptions.empty())
				{
					MULTIGE_FILTER		multiGEFilter;
					ZeroMemory(&multiGEFilter, sizeof(multiGEFilter));
					multiGEFilter.nRecLen			= sizeof(DBA_RECORD);
					multiGEFilter.count				= 0;
					multiGEFilter.flags				= 0;
					multiGEFilter.pUser             = this;
					multiGEFilter.nUserInfo         = bNotify;
					multiGEFilter.lpfnDbMultiGEProc	= _MultipleSubscriptions;

					m_pKeySubscriptionKey = boost::shared_array<DBA_MAXKEY>(new DBA_MAXKEY[m_MultipleSubscriptions.size()]);
					::ZeroMemory(m_pKeySubscriptionKey.get(), sizeof(DBA_MAXKEY)*m_MultipleSubscriptions.size());

					multiGEFilter.count=0;
					for(CMuttipleRequests::iterator iter = m_MultipleSubscriptions.begin(); iter != m_MultipleSubscriptions.end(); ++iter)
					{
						if(m_bUnsubscribeAll)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t enMStop, Subs - UnsubsAll"),m_strUserName.c_str());
							m_MultipleSubscriptions.clear();
							multiGEFilter.count=0;
							break;
						}

						iter->first->GetRawKey(&m_pKeySubscriptionKey[multiGEFilter.count].dbaKey);
						multiGEFilter.count++;
						
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t enMStop, multiGEFilter iterating: %s "),m_strUserName.c_str(), iter->first->GetKeyString().c_str());

						if(multiGEFilter.count > MAX_REQUESTS && !m_bUnsubscribeAll)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t enMStop, MaxRequest reached"),m_strUserName.c_str());
							_SubscribeMultipleRecords(&multiGEFilter, m_pKeySubscriptionKey.get(), bNotify);
							::ZeroMemory(m_pKeySubscriptionKey.get(), sizeof(DBA_MAXKEY)*m_MultipleSubscriptions.size());
							multiGEFilter.count = 0;
						}
						if(m_MultipleSubscriptions.empty())
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t enMStop, m_MultipleSubscriptions.empty..."),m_strUserName.c_str());						
							break;
						}
					}
					if(!m_bUnsubscribeAll  && multiGEFilter.count)
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t enMStop, Subscribing Multiple Records"),m_strUserName.c_str());
						_SubscribeMultipleRecords(&multiGEFilter, m_pKeySubscriptionKey.get(), bNotify);
					}
				}
				m_bMultipleSubscription = false;

				EgLib::CAutoLock lock(m_csMultipleSubscriptions);
				m_MultipleSubscriptions.clear();
			}break;
		default:
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t Subs default"),m_strUserName.c_str());
				DBA_MAXKEY maxKey;
				::ZeroMemory(&maxKey, sizeof(maxKey));

				_SetDbKey(Params->m_recParams, maxKey.dbaKey);
				CHahedKeyStringPtr pHashedKey = CHahedKeyStringPtr(new CHahedKeyString(&maxKey.dbaKey));
				CSubscriptionInfoPtr pGlobal = GetGlobalSubscribed(pHashedKey);

				if(m_bMultipleSubscription)
				{
					m_bUnsubscribeAll = false;

					EgLib::CAutoLock lock(m_csMultipleSubscriptions);
					if(pGlobal != NULL)
						m_AllreadySubscribedInMultipleRequest.push_back(pGlobal);

					m_MultipleSubscriptions[pHashedKey] = Params;
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Multiple, m_MultipleSubscriptions filling: %s"),m_strUserName.c_str(), pHashedKey->GetKeyString().c_str());
				}
				else
				{
					if(bInternal && m_bUnsubscribeAll)
					 {
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, UnsubsAll or Internal"),m_strUserName.c_str());
						return;
					 }
						
					else
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple"),m_strUserName.c_str());
						m_bUnsubscribeAll = false;
					}

					if(Params->m_recParams->Exchange && _bstr_t(Params->m_recParams->Exchange).length() && _bstr_t(Params->m_recParams->Exchange) != _bstr_t(" "))
					{
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enSubscribeQuote;
						const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			
						pError->m_bstrDescription = L"Unable to load the last quote data";
						pError->m_Error = enNoDataAvailableForSymbol;
						PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t Key not found"),m_strUserName.c_str());
						return ;
					}

					_SetDbKey(Params->m_recParams, m_dbaKey.dbaKey);
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Add Interest"),m_strUserName.c_str());
					long nResult = AddInterest(bNotify);
					if(nResult == DBA_ERR_NO_ERROR)
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Add Interest, NO_ERROR"),m_strUserName.c_str());

						CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaRec.extended.item));

						CSubscriptionInfoPtr pSubscription = AddSubscription(pKey, Params->m_recParams->Type);
						StoreClosePrice(pKey,  m_dbaRec.extended.close.price);

						if(bNotify)
						{
							CResponseBasePtr pResponse = boost::shared_static_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(Params->m_recParams)));
							PublicResponse(pResponse);
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Notify"),m_strUserName.c_str());
						}
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, Public Quote Update"),m_strUserName.c_str());
						if(pSubscription!=NULL)
						{
							pSubscription->AssignData(&m_dbaRec.extended);
							PublicQuoteUpdate(&Params->m_recParams, pSubscription);
						}
					}
					else
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, AddInterest, Error occured"),m_strUserName.c_str());
						CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
						pError->m_enRequestType = enSubscribeQuote;

						CComVariant vtReq;
						const_cast<_QuoteUpdateParams&>(Params->m_recParams).CopyTo(pError->m_vtRequest);			

						if(nResult == DBA_ERR_KEY_NOT_FOUND)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, AddInterest, DBA_ERR_KEY_NOT_FOUND"),m_strUserName.c_str());
							pError->m_bstrDescription = L"Unable to subscribe symbol";
							pError->m_Error = enNoDataAvailableForSymbol;
						}
						else if(nResult == DBA_ERR_INTEREST)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, AddInterest, DBA_ERR_INTEREST"),m_strUserName.c_str());
							pError->m_bstrDescription = L"HyperFeed server subscription list is full";
							pError->m_Error = enProviderInternalError;
						}
						else
						{
							
							_bstr_t bs =  "Unable to subscribe symbol. Error: ";
							bs += EtGetMessage(DBA_ERROR,nResult);
							TCHAR buffer[0x100] = {0};				
							_ltot(nResult,buffer,10);
							bs += " (";
							bs += buffer;
							bs += ")";
							pError->m_bstrDescription = bs;
							pError->m_Error = enProviderInternalError;
							
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t default, Not Multiple, AddInterest, Unknown Error with %s"),m_strUserName.c_str(), _T(buffer) );
						}
						
						PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
						
					}
				}
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Subscribe Exit"));
}

void CPrice::Unsubscribe(const CComVariant& params)
{
	try
	{
		ATLTRACE(_T("CPrice::Unsubscribe Enter\n"));
//		#ifdef _DEBUG
//			WriteUnsubsTime();
//			WriteUnsubsLineLN(_T("CPrice::Unsubscribe Enter\n"));
//		#endif
		if(V_VT(&params) == VT_ERROR)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t Unsubscribe All"),m_strUserName.c_str());
			UnsubscribeAll();
		}
		else
		{
			_QuoteUpdateParams Params(params);
			CHahedKeyStringPtr pKeyDelete = CHahedKeyStringPtr(new CHahedKeyString(&m_dbaKey.dbaKey));
			CVectorKeys keys;
			keys.push_back(pKeyDelete);
			DeleteSubscription(keys);
			if(!keys.empty())
			{
				EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t Unsubscribe %s %s %d"),m_strUserName.c_str(), CW2T(Params->Symbol), CW2T(Params->Exchange), Params->Type);
				_SetDbKey(Params, m_dbaKey.dbaKey);
				DecInterest(&m_dbaKey);
			}
		}
		ATLTRACE(_T("CPrice::Unsubscribe Exit\n"));
//		#ifdef _DEBUG
//			WriteUnsubsLineLN(_T("CPrice::Unsubscribe Exit\n"));
//		#endif
	}
_CATCH_UNHANDLED_EXCEPTION;

}

long CPrice::GetMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t GetMultiple Enter %s"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		lRet =  dba(&m_blk, DBA_MULTI_GET_EQUAL,	pKey, &nKeyLen,	pFilrer, &nRecLen, 0);
		if(lRet)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t GetMultiple Return %d"),m_strUserName.c_str(), lRet);
		//	return lRet;
		}
		else
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t GetMultiple Waiting infinite %s"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol) );
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("DBA_MULTI_GET_EQUAL Exit %s"), _T(pKey->dbaKey.symbol) );
	return lRet;
}

long CPrice::AddInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t AddInterstMultiple Enter %s"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		lRet =  dba(&m_blk, DBA_MULTI_GET_EQUAL| DBAX_INTEREST /*| DBA_ADD_INTEREST*/, pKey, &nKeyLen,	pFilrer, &nRecLen, 0);
		if(lRet)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogWarning, __FUNCTION__ , _T("[%s]\t AddInterstMultiple Return %d"),m_strUserName.c_str(), lRet);
			//return lRet;
		}
		else
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t AddInterstMultiple Infinite Wait %s"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
		}

	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t AddInterstMultiple Exit %s"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
	return lRet;
}

long CPrice::DecInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Enter %s on %i"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol), pFilrer->count);
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
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Return %d "),m_strUserName.c_str(), lRet);
		}
		else
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Infinite Wait %s"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
			ATLTRACE(_T("DecInterstMultiple_CP3\n"));
			long lRett = dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple End Wait with code %i"),m_strUserName.c_str(), lRett);

		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	ATLTRACE(_T("DecInterstMultiple_CP4\n"));
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t DecInterstMultiple Exit %s"),m_strUserName.c_str(), _T(pKey->dbaKey.symbol));
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
		DeleteSubscription(vecLocal);

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
					for(CVectorKeys::iterator iter = vecLocal.begin(); iter !=  vecLocal.end(); ++iter)
					{
						ATLTRACE("subscription map iterating\n");
						DBA_KEYS Keys;
						ZeroMemory(&Keys, sizeof(DBA_KEYS));
						(*iter)->GetRawKey(&Keys.dbaKey);
						DecInterest(&Keys);		
					}
					ATLTRACE("clear\n");
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
				for(CVectorKeys::iterator iter = vecLocal.begin(); iter !=  vecLocal.end(); ++iter)
				{
					(*iter)->GetRawKey(&m_dbaKey.dbaKey);
					ATLTRACE("DecInterst\n");
					DecInterest(&m_dbaKey);		
				}
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
	ATLTRACE(_T("CPrice::UnsubscribeAll Enter\n"));

	//	#ifdef _DEBUG
	//		WriteUnsubsLineLN(_T("CPrice::UnsubscribeAll Enter\n"));
	//	#endif
	m_bUnsubscribeAll = true;
	try
	{
		if(!m_Subscriptions.empty())
		{
			CVectorKeys  vecLocal;
			{
				CAutoLock lock(m_csSubscript);
				subscriptionSet ::const_iterator itrCpy  = m_Subscriptions.begin();
				subscriptionSet ::const_iterator itrCpyEnd  = m_Subscriptions.end();

//				vector<string> sTestVector;
//				sTestVector.reserve(m_mapSubscript.size());

				char buffer[128];
				while(itrCpy != itrCpyEnd)
				{
					ZeroMemory(buffer, sizeof(buffer));
					vecLocal.push_back((*itrCpy)->GetHashedKey());
//					sTestVector.insert(sTestVector.end(), itrCpy->first);
					++itrCpy;
				}
				DeleteSubscription(vecLocal);
			}
			UnsubscribeAll(vecLocal);
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
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t ProccessMessages Enter %s"),m_strUserName.c_str(), pExtRec->GetKey().c_str());
		_QuoteUpdateParams Params;

		if(!pExtRec->GetClosePrice())
			pExtRec->SetClosePrice(GetClosePrice(pExtRec->GetHashedKey()));

		{
			CAutoLock lc(pExtRec->GetSubscriptionLock());
			if(pExtRec->GetData())
			{
				DBA_KEYS* pDbaKey = (DBA_KEYS*)&pExtRec->GetData()->item;	 
				ParamFromKey(pDbaKey->dbaKey, Params);

				pExtRec->GetData()->close.price = pExtRec->GetClosePrice();
			}
		}

		// fix hyperfeed bug with incorrect type for index quotes...
		Params->Type = pExtRec->GetType();
		PublicQuoteUpdate(&Params, pExtRec);

		if(m_pPerfMon)
			m_pPerfMon->AddProcessedUpdate();
		if(m_spGlobalPerfCounter)
			m_spGlobalPerfCounter->AddProcessedUpdate();
	}
_CATCH_UNHANDLED_EXCEPTION;
EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("[%s]\t ProccessMessages Exit %s"),m_strUserName.c_str(), pExtRec->GetKey().c_str());
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
			EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("[%s]\t Was disconnected...."),m_strUserName.c_str());
			
			if (( m_bWasDisconnected) && (m_bWasConnectedOnce))
			{
				m_bWasConnectedOnce = true;
				EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("[%s]\t ReSubscribe after Reconnect..."),m_strUserName.c_str());
				m_bWasDisconnected = false;
				nRes = CPrice::Detach();
				if(nRes != DBA_ERR_NO_ERROR)
				{
					HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
					EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("[%s]\t Unable to detach after reconnect..."),m_strUserName.c_str());
				}
				nRes = CPrice::Attach();
				if(nRes != DBA_ERR_NO_ERROR)
				{
					HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
					EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("[%s]\t Unable to attach after reconnect..."),m_strUserName.c_str());
				}
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::RealtimeUpdate(CHahedKeyStringPtr pKey, DBA_RECORD_3* etiPtr)
{
	if(NULL != etiPtr || pKey != NULL)
	{
		CSubscriptionInfoPtr  pSubsctiption = GetGlobalSubscribed(pKey);
		if(pSubsctiption != NULL)
		{
			pSubsctiption->AssignData(etiPtr);
			pSubsctiption->Release();
		}
	}
}

void CPrice::ErrorMessage(long lErrorMessage)
{
	_bstr_t bs =  "Server fatal status. ETI Status: ";		
	bs += EtGetMessage(DBA_ERROR,m_hfServer.GetStatus());
	TCHAR buffer[0x100] = {0};				
	_ltot(lErrorMessage, buffer,10);
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

	EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("RealTime Message - Error %s"), (LPCSTR)bs);	
	PublicResponse(boost::shared_static_cast<CResponseBase>(pError));
}

void CPrice::RealtimeMessage(CHahedKeyStringPtr pKey, DBA_RECORD_3* etiPtr)
{
	try
	{
		if ( NULL == etiPtr || pKey == NULL)
		{
			_ASSERTE(false);
		}
		else
		{
			CSubscriptionInfoPtr  pSubsctiption = GetSubscribed(pKey);
			if(pSubsctiption!=NULL)
			{
				//Subscribed
				if(m_pPerfMon)
					m_pPerfMon->AddClientRealTimeUpdate();
				if(m_spGlobalPerfCounter)
					m_spGlobalPerfCounter->AddClientRealTimeUpdate();

				//pSubsctiption->AssignData(etiPtr);
				m_DBAStrategy.PushData(pSubsctiption);
				
				if(m_pPerfMon)
					m_pPerfMon->QuoteUpdatesQueueSnapshot = InterlockedIncrement(&m_lRTResponses);
				if(m_spGlobalPerfCounter)
					m_spGlobalPerfCounter->QuoteUpdatesQueueSnapshot = InterlockedIncrement(&CHFServer::m_lRTResponse);
			}
			else
			{
				int k=0;
				k++;
			}
		}
	}
_CATCH_UNHANDLED_EXCEPTION;
}

void CPrice::CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3& rec, _QuoteUpdateInfo& result)
{
	try
	{
		result.Init();
		double dClosePrice = BAD_DOUBLE_VALUE;
		double dLastPrice  = BAD_DOUBLE_VALUE;

		result->BidPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.bid.price));
		result->AskPrice = SetDoubleValue(Price2Dbl(rec.priceType, rec.ask.price));

		result->BidSize = InitByLong(rec.bid.size);
		result->AskSize = InitByLong(rec.ask.size);
		result->SeqNum = 0;

		result->Currency = SysAllocString(L"USD");
		result->LastPrice  = dLastPrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.lastSale.price));
		result->ClosePrice = dClosePrice= SetDoubleValue(Price2Dbl( rec.priceType, rec.close.price));

		if(!IsInvalidDoubleValue(dClosePrice) && !IsInvalidDoubleValue(dLastPrice))
			result->NetChange = dLastPrice - dClosePrice;
		else
			result->NetChange = BAD_DOUBLE_VALUE;

		result->UpdateDateTime = EgLib::vt_date::GetCurrentDate();

		result->OpenInterest = InitByLong(rec.openInterest);
		result->Volume = InitByLong(rec.volume);

		result->OpenPrice  = SetDoubleValue(Price2Dbl( rec.priceType, rec.open.price));

		result->HighPrice = SetDoubleValue(Price2Dbl( rec.priceType, rec.high.price));
		result->LowPrice  = SetDoubleValue(Price2Dbl(rec.priceType, rec.low.price));


		result->Status = GetQuoteStatus(rec.quoteIndicator);

		result->Skipped = 0;
		result->TotalRequests = 0;
		result->WaitTime = 0;

		if(enOPT == enType || enFOP == enType)
		{
			char buffer[2];
			buffer[1]= 0;
			buffer[0] = GetETSOptionExchange(*rec.bid.exchangeCode);
			_bstr_t bsBidExchange(buffer);

			buffer[0] = GetETSOptionExchange(*rec.ask.exchangeCode);
			_bstr_t bsAskExchange(buffer);

			result->AskExchange = bsAskExchange.copy();
			result->BidExchange = bsBidExchange.copy();
		}
		else
		{
			char buffer[2];
			buffer[1]= 0;
			buffer[0] = GetETSStockExchange(*rec.bid.exchangeCode);
			_bstr_t bsBidExchange(buffer);

			buffer[0] = GetETSStockExchange(*rec.ask.exchangeCode);
			_bstr_t bsAskExchange(buffer);

			result->AskExchange = bsAskExchange.copy();
			result->BidExchange = bsBidExchange.copy();
		}

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
	USES_CONVERSION;
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__, _T("[%s]\t _SetDbKey Enter for Symbol - %s, Exchange - %s, Type - %d "),m_strUserName.c_str(), W2T(Params->Symbol), W2T(Params->Exchange) , (double)Params->Type );
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
	
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("[%s]\t _SetDbKey Exit for Symbol - %s, Exchange - %s, Currency - %s, Type - %d "),m_strUserName.c_str(), dbaKey.symbol, dbaKey.exchangeCode, dbaKey.currencyCode, dbaKey.type );
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

void CPrice::CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3& rec, QuoteUpdateInfo* result)
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


	if(enOPT == enType || enFOP == enType)
	{
		char buffer[2];
		buffer[1]= 0;
		buffer[0] = GetETSOptionExchange(*rec.bid.exchangeCode);
		_bstr_t bsBidExchange(buffer);

		buffer[0] = GetETSOptionExchange(*rec.ask.exchangeCode);
		_bstr_t bsAskExchange(buffer);

		result->AskExchange = bsAskExchange.copy();
		result->BidExchange = bsBidExchange.copy();
	}
	else
	{
		char buffer[2];
		buffer[1]= 0;
		buffer[0] = GetETSStockExchange(*rec.bid.exchangeCode);
		_bstr_t bsBidExchange(buffer);

		buffer[0] = GetETSStockExchange(*rec.ask.exchangeCode);
		_bstr_t bsAskExchange(buffer);

		result->AskExchange = bsAskExchange.copy();
		result->BidExchange = bsBidExchange.copy();
	}

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

void CPrice::ParamFromKey(const DBA_KEY& key, QuoteUpdateParams* pParams)
{
	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)key.symbol, MAX_TICKER);

	switch(key.type) 
	{
	case IT_EQUITY:
		pParams->Type = enSTK;
		break;
	case IT_INDEX:
		pParams->Type = enIDX;
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
		
		*szExch = GetETSOptionExchange(*szExch);
		*Exchange = ::SysAllocString(CA2W(szExch));// _bstr_t(szExch).copy();
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
		pPrice->StoreClosePrice(pHashedKey, pRec->extended.close.price);
		if(pPrice->m_spGlobalPerfCounter!=NULL)
			pPrice->m_spGlobalPerfCounter->AddMultipleRequestResponce();

		if(pPrice->m_pPerfMon!=NULL)
			pPrice->m_pPerfMon->AddMultipleRequestResponce();

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
		CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
		pError->m_enRequestType = enRequestLastQuote;
		if(it != pPrice->m_MultipleRequests.end())
			const_cast<_QuoteUpdateParams&>(it->second->m_recParams).CopyTo(pError->m_vtRequest);			
		else
			pError->m_vtRequest = vtMissing;

		pError->m_bstrDescription = L"Unable to load the last quote data";
		pError->m_Error = enNoDataAvailableForSymbol;
		pPrice->PublicResponse(boost::shared_static_cast<CResponseBase>(pError));		
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
			pPrice->Trace(enlogDebug, __FUNCTION__ , _T("[%s]\t Terminate"), pPrice->m_strUserName.c_str());
			return TRUE;
		}

		if(pPrice->m_spGlobalPerfCounter!=NULL)
			pPrice->m_spGlobalPerfCounter->AddMultipleUnsubscriptionResponce();

		if(pPrice->m_pPerfMon!=NULL)
			pPrice->m_pPerfMon->AddMultipleUnsubscriptionResponce();

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
						itr = pPrice->m_SubsctiptionVector.erase(itr);
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
			pPrice->Trace(enlogInfo, __FUNCTION__ , _T("[%s]\t Quote Cancel  %s on %i"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str(), pPrice->m_SubsctiptionVector.size());
			return FALSE;
		}


		if(!dwStatus && !bFind)
		{

			//			#ifdef _DEBUG
			//				_write(fh, _T("MultipleSubs"), sizeof(_T("MultipleSubs")) );
			//				_close(fh);
			//			#endif
			pPrice->Trace(enlogWarning, __FUNCTION__ , _T("[%s]\t STOPPED Unsubscribed %s left %i"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str(), pPrice->m_SubsctiptionVector.size());
			return TRUE;
		}
		else
			pPrice->Trace(enlogWarning, __FUNCTION__ , _T("[%s]\t Unsubscribed %s left %i"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str(), pPrice->m_SubsctiptionVector.size());

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
				EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__, _T("[%s]\t Multimple Option Subscribe %s"),pPrice->m_strUserName.c_str(), pHashedKey->GetKeyString().c_str()); 
				CSubscriptionInfoPtr pSubs = pPrice->AddSubscription(pHashedKey, it->second->m_recParams->Type);
				pPrice->StoreClosePrice(pHashedKey, pRec->extended.close.price);

				if(pPrice->m_bUnsubscribeAll)
					return TRUE;

				if(bNotify)
				{
					CResponseBasePtr pResponse = boost::shared_static_cast<CResponseBase>(CSubscribedResponsePtr(new CSubscribedResponse(it->second->m_recParams)));
					pPrice->PublicResponse(pResponse);
				}

				if(pSubs)
				{
					pSubs->AssignData(&pRec->extended);
					pPrice->PublicQuoteUpdate(&it->second->m_recParams, pSubs);
				}
			}
			else
			{
				CSubscriptionInfoPtr pSubs;
				if(it != pPrice->m_MultipleSubscriptions.end())
					pSubs = pPrice->AddSubscription(pHashedKey, it->second->m_recParams->Type);

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
					pError->m_bstrDescription = L"Unable to subscribe. Symbol is not found";
					pError->m_Error = enNoDataAvailableForSymbol;
				}
				else if(dwStatus == DBA_ERR_INTEREST)
				{
					pError->m_bstrDescription = L"HyperFeed server subscription list is full";
					pError->m_Error = enProviderInternalError;
				}
				else
				{
					_bstr_t bs =  "Unable to subscribe symbol. Error: ";
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
				if(pSubs)
					pSubs->SetErrorMessage(pError);
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
	Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

	HRESULT dwResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(dwResult))
	{
		Trace(enLogError, __FUNCTION__ , _T("Error while initializing Com %i"), dwResult);
		
	}
	Trace(enlogDebug, __FUNCTION__ , _T("Leave"));
	return dwResult;

}
void  CPrice::OnStrategyUninitialize()
{
	CoUninitialize();
}

void CPrice::OnData(CSubscriptionInfoPtr& Data)
{
	InterlockedDecrement(&m_lRTResponses);
	InterlockedDecrement(&CHFServer::m_lRTResponse);
	ProcessMessage(Data);
	if(m_pPerfMon)
		m_pPerfMon->QuoteUpdatesQueueSnapshot = m_lRTResponses;
    if(m_spGlobalPerfCounter)
		m_spGlobalPerfCounter->QuoteUpdatesQueueSnapshot = CHFServer::m_lRTResponse;

}

